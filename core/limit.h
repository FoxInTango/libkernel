#ifndef _LIMIT_H_foxintango
#define _LIMIT_H_foxintango

typedef struct _cpu_limit{}cpu_limit;
typedef struct _mem_limit{}mem_limit;
typedef struct _net_limit{}net_limit;

typedef struct _limit{
    cpu_limit cpu;
    mem_limit mem;
    net_limit net;
}limit;
#endif // ! _LIMIT_H_foxintango
