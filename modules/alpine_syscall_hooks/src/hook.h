#ifndef _HOOK_H_
#define _HOOK_H_

#ifdef __cplusplus
extern "C" {
#endif

int alpine_network_hook_startup(void);
void alpine_network_hook_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif
