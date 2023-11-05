#
# Makefile for application
#

WASI_SDK_PATH ?= /home/nskernel/wasi/wasi-sdk-20.0
CROSS_COMPILE	?= 

AS			:= $(CROSS_COMPILE)gcc -x assembler-with-cpp
CC			:= $(WASI_SDK_PATH)/bin/clang
CXX			:= $(CROSS_COMPILE)g++
LD			:= $(CROSS_COMPILE)ld
AR			:= $(CROSS_COMPILE)ar
OC			:= $(CROSS_COMPILE)objcopy
OD			:= $(CROSS_COMPILE)objdump
RM			:= rm -fr

ASFLAGS		:= -g -ggdb -Wall -O3
CFLAGS		:= $(TARGET) -D_WASI_EMULATED_PROCESS_CLOCKS --sysroot=$(WASI_SDK_PATH)/share/wasi-sysroot/ 
CXXFLAGS	:= -g -ggdb -Wall -O3
LDFLAGS		:= $(TARGET) -Wl,-lc -Wl,--allow-undefined -Wl,--export=main -Wl,--export=__wasm_call_ctors -Wl,--export=__heap_base,--export=__data_end
OCFLAGS		:= -v -O binary
ODFLAGS		:=
MCFLAGS		:=

LIBDIRS		:= -L ../../src
LIBS 		:= -lonnx -lm

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

LIBOBJS     := $(foreach dir, ../../src ../../src/default, $(wildcard $(dir)/*.o))

.PHONY: all clean

all : $(NAME)

$(NAME) : $(OBJS) $(LIBOBJS) 
	@echo [LD] Linking $@
	@$(CC) $(LDFLAGS) $(LIBDIRS) $^ -o $@ -static

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
