#ifndef _LIB_KERNEL_OPERATION_H_foxintango
#define _LIB_KERNEL_OPERATION_H_foxintango

#include "limit.h"
#include "permission.h"

typedef enum _operation_target_type {
    operation_target_type_uid,
    operation_target_type_gid,
    operation_target_type_pid,
    operation_target_type_tid
}operation_target_type;

typedef struct _operation_target{
    operation_target_type::operation_target_type_gid gid;
    operation_target_type::operation_target_type_uid uid;
    operation_target_type::operation_target_type_pid pid;
    operation_target_type::operation_target_type_tid tid;
}operation_target;

typedef struct _kernel_operation {
    kernel_operation* next;
    operation_target target;
    limit l;
    permission p;

}kernel_operation;
#endif // !_OPERATION_H_foxintango
