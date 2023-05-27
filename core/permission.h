#ifndef _PERMISSION_H_foxintango
#define _PERMISSION_H_foxintango

typedef struct _filesystem_permission{}filesystem_permission;
typedef struct _network_permission{}network_permission;

typedef struct _permission{
    filesystem_permission filesystem;
    network_permission    network;
}permission;


#endif