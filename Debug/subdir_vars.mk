################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../empty.cfg 

CMD_SRCS += \
../EK_TM4C123GXL.cmd 

C_SRCS += \
../controlLED.c \
../main.c \
../movement.c \
../sensing.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg/ 

C_DEPS += \
./controlLED.d \
./main.d \
./movement.d \
./sensing.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./controlLED.obj \
./main.obj \
./movement.obj \
./sensing.obj 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

OBJS__QUOTED += \
"controlLED.obj" \
"main.obj" \
"movement.obj" \
"sensing.obj" 

C_DEPS__QUOTED += \
"controlLED.d" \
"main.d" \
"movement.d" \
"sensing.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

C_SRCS__QUOTED += \
"../controlLED.c" \
"../main.c" \
"../movement.c" \
"../sensing.c" 


