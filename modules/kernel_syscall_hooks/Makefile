#
# From : https://olegkutkov.me/2019/11/10/cpp-in-linux-kernel/
# Linux kernel C++ module makefile
# Oleg Kutkov, 2019
#

# 参考 saosaole.com.cn:~/make 

# ****************libcpp****************
LIBCPP_ROOT = ${SUPER_LIBRARY_ROOT}/libcpp
LIBCPP_DIR_BESIDES  = \(
LIBCPP_DIR_BESIDES += -path    ${LIBCPP_ROOT}/lib
LIBCPP_DIR_BESIDES += -o -path ${LIBCPP_ROOT}/obj
LIBCPP_DIR_BESIDES += \)
LIBCPP_DIRS   = $(shell find $(LIBCPP_ROOT) $(LIBCPP_DIR_BESIDES) -prune -o -type d -print)

LIBCPP_SOURCES_CC  += $(foreach dir,$(LIBCPP_DIRS),$(wildcard $(dir)/*.c))
LIBCPP_OBJECTS_CC  += $(patsubst %.c,obj/%.o,$(notdir ${LIBCPP_SOURCES_CC}))

LIBCPP_SOURCES_PP  += $(foreach dir,$(LIBCPP_DIRS),$(wildcard $(dir)/*.cpp))
LIBCPP_OBJECTS_PP  += $(patsubst %.cpp,obj/%.o,$(notdir ${LIBCPP_SOURCES_PP}))

# ****************core****************
CORE_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))/../core
CORE_DIR_BESIDES  = \(
CORE_DIR_BESIDES += -path    ${CORE_ROOT}/lib
CORE_DIR_BESIDES += -o -path ${CORE_ROOT}/obj
CORE_DIR_BESIDES += \)
CORE_DIRS   = $(shell find $(CORE_ROOT) $(CORE_DIR_BESIDES) -prune -o -type d -print)

CORE_SOURCES_CC  += $(foreach dir,$(CORE_DIRS),$(wildcard $(dir)/*.c))
CORE_OBJECTS_CC  += $(patsubst %.c,obj/%.o,$(notdir ${MODULE_SOURCES_CC}))

CORE_SOURCES_PP  += $(foreach dir,$(CORE_DIRS),$(wildcard $(dir)/*.cpp))
CORE_OBJECTS_PP  += $(patsubst %.cpp,obj/%.o,$(notdir ${MODULE_SOURCES_PP}))

# ****************module****************
MODULE_ROOT = ./
MODULE_DIR_BESIDES  = \(
MODULE_DIR_BESIDES += -path    ${MODULE_ROOT}/lib
MODULE_DIR_BESIDES += -o -path ${MODULE_ROOT}/obj
MODULE_DIR_BESIDES += \)
MODULE_DIRS   = $(shell find $(MODULE_ROOT) $(MODULE_DIR_BESIDES) -prune -o -type d -print)

MODULE_SOURCES_CC  += $(foreach dir,$(MODULE_DIRS),$(wildcard $(dir)/*.c))
MODULE_OBJECTS_CC  += $(patsubst %.c,obj/%.o,$(notdir ${MODULE_SOURCES_CC}))

MODULE_SOURCES_PP  += $(foreach dir,$(MODULE_DIRS),$(wildcard $(dir)/*.cpp))
MODULE_OBJECTS_PP  += $(patsubst %.cpp,obj/%.o,$(notdir ${MODULE_SOURCES_PP}))

MODULE_NAME := kernel_syscall_hooks
KERNEL      := /lib/modules/$(shell uname -r)/build
FLAGS       := -Wall -I /home/sparrow/alpine/inc
KMOD_DIR    := $(shell pwd) 
MODULE_OBJECTS := ./src/module.o ./src/hook.cpp.o #./src/kernel.o ./src/echo.o ./src/support.cpp.o

ccflags-y += $(FLAGS)

# Apply C flags to the cpp compiler and disable cpp features that can't be supported in the kernel module
cxx-selected-flags = $(shell echo $(KBUILD_CFLAGS) \
            | sed s/-D\"KBUILD.\"//g \
            | sed s/-Werror=strict-prototypes//g \
            | sed s/-Werror=implicit-function-declaration//g \
            | sed s/-Werror=implicit-int//g \
            | sed s/-Wdeclaration-after-statement//g \
            | sed s/-Wno-pointer-sign//g \
            | sed s/-Werror=incompatible-pointer-types//g \
            | sed s/-Werror=designated-init//g \
            | sed s/-std=gnu90//g )

cxxflags = $(FLAGS) \
            $(cxx-selected-flags) \
            -fno-builtin \
            -nostdlib \
            -fno-rtti \
            -fno-exceptions \
            -std=c++0x


obj-m += $(MODULE_NAME).o

$(MODULE_NAME)-y := $(MODULE_OBJECTS)

.PHONY: $(MODULE_NAME).ko
$(MODULE_NAME).ko:${CORE_OBJECTS_CC} ${CORE_OBJECTS_PP}
	@echo ************************SUPER************************
	@echo 上层头文件路径:     ${SUPER_INCLUDE_PATH}
	@echo 上层库文件路径:     ${SUPER_LIBRARY_PATH}
	@echo 上层库工程根目录:   ${SUPER_LIBRARY_ROOT}
	@echo LIBCPP C源码:       ${LIBCPP_SOURCES_CC}
	@echo LIBCPP 目标文件:    ${LIBCPP_OBJECTS_CC}
	@echo LIBCPP C++源码:     ${LIBCPP_SOURCES_PP}
	@echo LIBCPP C++目标文件: ${LIBCPP_OBJECTS_PP}
	@echo ************************SUPER************************
	@echo ${MODULE_SOURCES_CC}
	@echo ${MODULE_OBJECTS_CC}
	@echo ${MODULE_SOURCES_PP}
	@echo ${MODULE_OBJECTS_PP}
	@echo ************************building module************************
	make -C $(KERNEL) M=$(KMOD_DIR) modules # https://blog.csdn.net/wang20122013/article/details/120718939

cxx-prefix := " $(HOSTCXX) [M]  "

#%.cpp.o: %.cpp
#	@echo $(cxx-prefix)$@
#	@$(HOSTCXX) $(cxxflags) -c $< -o $@
#	@echo -n > $$(dirname $@)/.$$(basename $@).cmd

${CORE_OBJECTS_CC}:%.o:%.c
	@$(HOSTCXX) $(cxxflags) -c $< -o ./obj/$(notdir $@) # $(patsubst %.c,obj/%.o,$(notdir ${CORE_SOURCES_PP}))
${CORE_OBJECTS_PP}:%.o:%.cpp
	@$(HOSTCXX) $(cxxflags) -c $< -o ./obj/$(notdir $@)
.PHONY: clean
clean:
	@echo clean
	make -C $(KERNEL) M=$(KMOD_DIR) clean
