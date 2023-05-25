/*
 * Kernel lib - support basic C++ runtime functions
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "kernel.h"

void kcmemset(void *dst, int c, unsigned int len)
{
    memset(dst, c, len);
}

void *kcmemcpy(void *dst, void *src, unsigned int len)
{
    memcpy(dst, src, len);
    return dst;
}

void *kcmemmove(void *dst, void *src, unsigned int len)
{
    memmove(dst, src, len);
    return dst;
}

int kcmemcmp(void *p1, void *p2, unsigned int len)
{
    return memcmp(p1, p2, len);
}

void *kcmalloc(unsigned int size)
{
    return kmalloc(size, GFP_ATOMIC);
}

void *kcrealloc(void *mem, unsigned int size)
{
    return krealloc(mem, size, GFP_ATOMIC);
}

void kcfree(void *mem)
{
    kfree(mem);
}

/** From : https://zhuanlan.zhihu.com/p/480934356
 */
/*
 * 把虚拟内存地址设置为可写
 */
int make_vm_rw(unsigned long address)
{
    unsigned int level;

    //查找虚拟地址所在的页表地址
    pte_t* pte = lookup_address(address, &level);

    if (pte->pte & ~_PAGE_RW)  //设置页表读写属性
        pte->pte |= _PAGE_RW;

    return 0;
}

/*
 * 把虚拟内存地址设置为只读
 */
int make_vm_ro(unsigned long address)
{
    unsigned int level;

    pte_t* pte = lookup_address(address, &level);
    pte->pte &= ~_PAGE_RW;  //设置只读属性

    return 0;
}