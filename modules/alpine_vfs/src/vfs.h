#ifndef _HOOK_H_
#define _HOOK_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _alpine_vfs_node{
}alpine_vfs_node;

int  alpine_vfs_init(void);
void alpine_vfs_exit(void);

#ifdef __cplusplus
}
#endif

#endif
