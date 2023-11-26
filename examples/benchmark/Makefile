#
# Makefile for application
#

WASI_SDK_PATH ?= /home/nskernel/wasi/wasi-sdk-20.0
CROSS_COMPILE	?= 

AOTCC       := /home/nskernel/wasm-micro-runtime/wamr-compiler/build/wamrc
WASMOPT		:= /home/nskernel/binaryen/bin/wasm-opt -O4 --enable-bulk-memory

PCD_ROOT	:= ../../../../../../

AS			:= $(CROSS_COMPILE)gcc -x assembler-with-cpp
CC			:= $(WASI_SDK_PATH)/bin/clang
CXX			:= $(CROSS_COMPILE)g++
LD			:= $(CROSS_COMPILE)ld
AR			:= $(CROSS_COMPILE)ar
OC			:= $(CROSS_COMPILE)objcopy
OD			:= $(CROSS_COMPILE)objdump
RM			:= rm -fr

ASFLAGS		:= -g -ggdb -Wall -O3
CFLAGS		:= $(TARGET) -mbulk-memory -O3 -D_WASI_EMULATED_PROCESS_CLOCKS -DWASM_ENV --sysroot=$(WASI_SDK_PATH)/share/wasi-sysroot/ 
CXXFLAGS	:= -g -ggdb -Wall -O3
LDFLAGS		:= $(TARGET) -mbulk-memory -Wl,-lc -Wl,--allow-undefined -Wl,--export=malloc -Wl,--export=free -Wl,--export=main -Wl,--export=__wasm_call_ctors -Wl,--export=__heap_base,--export=__data_end -z stack-size=3145728 -Wl,--initial-memory=141688832
OCFLAGS		:= -v -O binary
ODFLAGS		:=
MCFLAGS		:=

LIBDIRS		:= -L ../../src
LIBS 		:= -lm

INCDIRS		:= -I . -I ../../src
SRCDIRS		:= .

SFILES		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CPPFILES	:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

SDEPS		:= $(patsubst %, %, $(SFILES:.S=.o.d))
CDEPS		:= $(patsubst %, %, $(CFILES:.c=.o.d))
CPPDEPS		:= $(patsubst %, %, $(CPPFILES:.cpp=.o.d))
DEPS		:= $(SDEPS) $(CDEPS) $(CPPDEPS)

SOBJS		:= $(patsubst %, %, $(SFILES:.S=.o))
COBJS		:= $(patsubst %, %, $(CFILES:.c=.o))
CPPOBJS		:= $(patsubst %, %, $(CPPFILES:.cpp=.o)) 
OBJS		:= $(SOBJS) $(COBJS) $(CPPOBJS)

OBJDIRS		:= $(patsubst %, %, $(SRCDIRS))
NAME		:= benchmark
VPATH		:= $(OBJDIRS)

.PHONY: all aot clean

all : $(NAME) aot

aot : $(NAME).aot

$(NAME).aot : $(NAME)
	@echo [AOT] AOT Compiling $@
	@$(AOTCC) -sgx --size-level=1 -o $@ $<
	@echo [HASH]  $@
	@sha256sum $@ | cut -d " " -f 1 | xxd -r -p > ../../../app.hash

$(NAME) : $(OBJS)
	@echo [LD] Linking $@
	@$(CC) $(LDFLAGS) $^ -o $@ -static
#@echo [OPT] Optimsing $@
#@$(WASMOPT) -o $@ $@

$(SOBJS) : %.o : %.S
	@echo [AS] $<
	@$(AS) $(ASFLAGS) -MD -MP -MF $@.d $(INCDIRS) -c $< -o $@

$(COBJS) : %.o : %.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) -MD -MP -MF $@.d $(INCDIRS) -c $< -o $@

$(CPPOBJS) : %.o : %.cpp
	@echo [CXX] $<
	@$(CXX) $(CXXFLAGS) -MD -MP -MF $@.d $(INCDIRS) -c $< -o $@

clean:
	@$(RM) $(DEPS) $(OBJS) $(NAME)

sinclude $(DEPS)
