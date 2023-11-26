#
# Makefile for library
#

CROSS_COMPILE	?= 

AS			:= $(CROSS_COMPILE)gcc -x assembler-with-cpp
CC			:= $(CROSS_COMPILE)gcc
CXX			:= $(CROSS_COMPILE)g++
LD			:= $(CROSS_COMPILE)ld
AR			:= $(CROSS_COMPILE)ar
OC			:= $(CROSS_COMPILE)objcopy
OD			:= $(CROSS_COMPILE)objdump
RM			:= rm -fr

PCD_ROOT	:= ../../../../../

ASFLAGS		:= -g -ggdb -Wall -O3
CFLAGS		:= -g -ggdb -Wall -O3 -DPCD_CONFIG_RUNTIME_WAMR -nostdinc -Wno-format -fno-builtin-printf
CXXFLAGS	:= -g -ggdb -Wall -O3
INCDIRS		:= -I . -I$(SGX_SDK)/include -I$(SGX_SDK)/include/tlibc -I$(PCD_ROOT)/include -I$(PCD_ROOT)/runtime/wamr/wasm-micro-runtime/core/iwasm/include -I$(PCD_ROOT)/runtime/wamr/wasm-micro-runtime/core/iwasm/interpreter -I$(PCD_ROOT)/runtime/wamr/wasm-micro-runtime/core/shared/utils -I$(PCD_ROOT)/runtime/wamr/wasm-micro-runtime/core/shared/platform/linux-sgx
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
	@$(AR) -rcs $@ $(OBJS)

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
