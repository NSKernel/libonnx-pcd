#
# Makefile for library
#

WASI_SDK_PATH ?= /home/nskernel/wasi/wasi-sdk-20.0
CROSS_COMPILE	?= 

AS			:= $(CROSS_COMPILE)gcc -x assembler-with-cpp
CC			:= $(WASI_SDK_PATH)/bin/clang
CXX			:= $(WASI_SDK_PATH)/bin/clang++
LD			:= $(CROSS_COMPILE)ld
AR			:= $(CROSS_COMPILE)ar
OC			:= $(CROSS_COMPILE)objcopy
OD			:= $(CROSS_COMPILE)objdump
RM			:= rm -fr

ASFLAGS		:= -g -ggdb -Wall -O3
CFLAGS		:= $(TARGET) -mbulk-memory -O3 --sysroot=$(WASI_SDK_PATH)/share/wasi-sysroot/ -D_WASI_EMULATED_PROCESS_CLOCKS
CXXFLAGS	:= $(TARGET) --sysroot=$(WASI_SDK_PATH)/share/wasi-sysroot/ -D_WASI_EMULATED_PROCESS_CLOCKS
INCDIRS		:= -I .
SRCDIRS		:= . default

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

NAME		:= libonnx.a

.PHONY: all clean

all : $(NAME)

$(NAME) : $(OBJS)
	@echo [AR] Archiving $@
	@echo Bypassed for WebAssembly	

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
