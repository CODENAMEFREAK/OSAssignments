################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../FreakWrapper/freakPrintSyscall.asm 

O_SRCS += \
../FreakWrapper/freakprint.o 

OBJS += \
./FreakWrapper/freakPrintSyscall.o 


# Each subdirectory must supply rules for building sources it contributes
FreakWrapper/%.o: ../FreakWrapper/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


