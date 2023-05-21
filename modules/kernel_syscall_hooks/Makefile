#
# From : https://olegkutkov.me/2019/11/10/cpp-in-linux-kernel/
# Linux kernel C++ module makefile
# Oleg Kutkov, 2019
#

MODULE_NAME := kernel_syscall_hooks
KERNEL      := /lib/modules/$(shell uname -r)/build
FLAGS       := -Wall -I /home/sparrow/alpine/inc
KMOD_DIR    := $(shell pwd)

ifdef SUPER_INCLUDE_PATH
    
endif

MODULE_OBJECTS := ./src/module.o ./src/kernel.o ./src/echo.o ./src/support.cpp.o ./src/hook.cpp.o

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
$(MODULE_NAME).ko:
    @echo "SUPER PATH"
    @echo ${SUPER_INCLUDE_PATH} 
    @echo building module
    make -C $(KERNEL) M=$(KMOD_DIR) modules -larguments -lmodel -lcpp -L /home/sparrow/alpine/lib # https://blog.csdn.net/wang20122013/article/details/120718939

cxx-prefix := " $(HOSTCXX) [M]  "

%.cpp.o: %.cpp
	@echo $(cxx-prefix)$@
	@$(HOSTCXX) $(cxxflags) -c $< -o $@
	@echo -n > $$(dirname $@)/.$$(basename $@).cmd

.PHONY: clean
clean:
	@echo clean
	make -C $(KERNEL) M=$(KMOD_DIR) clean
