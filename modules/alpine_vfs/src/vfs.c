#include "vfs.h"
#include "../../core/echo.h"

#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/parser.h>
#include <linux/hardirq.h> // #include <linux/smp_lock.h>
#include <linux/buffer_head.h>
#include <linux/exportfs.h>
#include <linux/vfs.h>
#include <linux/random.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/quotaops.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

/** From : https://www.cnblogs.com/wangzahngjun/p/5365310.html
 */
#define ALPINE_MAGIC 0x20220523
static unsigned long ALPINE_SALT = 22355153;

static struct inode* alpine_make_inode(struct super_block* sb, int mode)
{
    struct inode* ret = new_inode(sb);
    if (ret) {
        struct timespec current_time;
        clock_gettime(CLOCK_REALTIME, &current_time);
        ret->i_mode = mode;
        ret->i_uid.val = ret->i_gid.val = 0;
        ret->i_blocks = 0;
        ret->i_atime = ret->i_mtime = ret->i_ctime = current_time;
    }
    return ret;
}

static int alpine_open(struct inode* inode, struct file* filp)
{
    filp->private_data = inode->i_private;
    return 0;
}

#define TMPSIZE 20
static ssize_t alpine_read_file(struct file* filp, char* buf,
    size_t count, loff_t* offset)
{
    atomic_t* counter = (atomic_t*)filp->private_data;
    int v, len;
    char tmp[TMPSIZE];
    v = atomic_read(counter);
    if (*offset > 0)
        v -= 1;
    else
        atomic_inc(counter);
    len = snprintf(tmp, TMPSIZE, "%d\n", v);
    if (*offset > len)
        return 0;
    if (count > len - *offset)
        count = len - *offset;
    if (copy_to_user(buf, tmp + *offset, count))
        return -EFAULT;
    *offset += count;
    return count;
}
static ssize_t alpine_write_file(struct file* filp, const char* buf,
    size_t count, loff_t* offset)
{
    atomic_t* counter = (atomic_t*)filp->private_data;
    char tmp[TMPSIZE];

    if (*offset != 0)
        return -EINVAL;

    if (count >= TMPSIZE)
        return -EINVAL;
    memset(tmp, 0, TMPSIZE);
    if (copy_from_user(tmp, buf, count))
        return -EFAULT;

    atomic_set(counter, simple_strtol(tmp, NULL, 10));
    return count;
}

static struct file_operations alpine_file_ops = {
        .open  = alpine_open,
        .read  = alpine_read_file,
        .write = alpine_write_file,
};

static struct dentry* alpine_create_file(struct super_block* sb,struct dentry* dir, const char* name,atomic_t* counter)
{
    struct dentry* dentry;
    struct inode* inode;
    struct qstr qname;

    qname.name = name;
    qname.len = strlen(name);
    qname.hash = full_name_hash(&ALPINE_SALT,name, qname.len);

    dentry = d_alloc(dir, &qname);
    if (!dentry)
        goto out;
    inode = alpine_make_inode(sb, S_IFREG | 0644);
    if (!inode)
        goto out_dput;
    inode->i_fop = &alpine_file_ops;
    inode->i_private = counter;
    d_add(dentry, inode);
    return dentry;
out_dput:
    dput(dentry);
out:
    return 0;
}

static struct dentry* alpine_create_dir(struct super_block* sb,
    struct dentry* parent, const char* name)
{
    struct dentry* dentry;
    struct inode* inode;
    struct qstr qname;
    qname.name = name;
    qname.len = strlen(name);
    qname.hash = full_name_hash(&ALPINE_SALT，name, qname.len);
    /**
     * dentry的主要作用是建立文件名和inode之间的关联。
     * 所以该结构体包括两个最主要的字段，d_inode和d_name。
     * 其中，d_name为文件名。qstr是内核对字符串的封装（可以理解为带有散列值的char*）。
     * d_inode是与该文件名对应的inode。
     */
    dentry = d_alloc(parent, &qname);
    if (!dentry)
        goto out;
    inode = alpine_make_inode(sb, S_IFDIR | 0644);
    if (!inode)
        goto out_dput;
    inode->i_op = &simple_dir_inode_operations;
    inode->i_fop = &simple_dir_operations;
    d_add(dentry, inode);
    return dentry;
out_dput:
    dput(dentry);
out:
    return 0;
}

static atomic_t counter, subcounter;
static void alpine_create_files(struct super_block* sb, struct dentry* root)
{
    struct dentry* subdir;

    atomic_set(&counter, 0);
    alpine_create_file(sb, root, "counter", &counter);

    atomic_set(&subcounter, 0);
    subdir = alpine_create_dir(sb, root, "subdir");
    if (subdir)
        alpine_create_file(sb, subdir, "subcounter", &subcounter);
}

static struct super_operations alpine_s_ops = {
        .statfs = simple_statfs,
        .drop_inode = generic_delete_inode,
};

static int alpine_fill_super(struct super_block* sb, void* data, int silent)
{
    struct inode* root;
    struct dentry* root_dentry;

    /**
     * 这两个是什么玩意儿？ 
     */
    //sb->s_blocksize = PAGE_CACHE_SIZE;
    //sb->s_blocksize_bits = PAGE_CACHE_SHIFT;
    sb->s_magic = ALPINE_MAGIC;
    sb->s_op = &alpine_s_ops;

    printk(KERN_INFO "alpine_fill_super is here\n");
    root = alpine_make_inode(sb, S_IFDIR | 0755);
    if (!root)
        goto out;
    root->i_op  = &simple_dir_inode_operations;
    root->i_fop = &simple_dir_operations;

    root_dentry = d_alloc_root(root);
    if (!root_dentry)
        goto out_iput;
    sb->s_root = root_dentry;

    alpine_create_files(sb, root_dentry);
    return 0;

out_iput:
    iput(root);
out:
    return -ENOMEM;
}
static int alpine_get_super(struct file_system_type* fst, int flags, const char* devname, void* data, struct vfsmount* mount)
{
    printk(KERN_INFO "mount from user\n");
    return get_sb_single(fst, flags, data, alpine_fill_super, mount);
}

static struct file_system_type alpine_vfs_type = {
        .owner   = THIS_MODULE,
        .name    = "alpine",
        .get_sb  = alpine_get_super,
        .kill_sb = kill_litter_super,
};

int vfs_init(void) {
    return register_filesystem(&alpine_vfs_type);
}

void vfs_exit(void){
    unregister_filesystem(&alpine_vfs_type);
}
