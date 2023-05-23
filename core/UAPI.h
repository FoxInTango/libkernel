#ifndef _LIB_KERNEL_UAPI_H_foxintango
#define _LIB_KERNEL_UAPI_H_foxintango

#include "operation.h"

int set_limit();
int set_pid_permission();
int set_tid_permission();
int set_gid_permission();
int set_uid_permission();

int push_operations(const unsigned int& count,const kernel_operation** operations);

#endif // !_OPERATION_H_foxintango