################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/Libraries.c 

OBJS += \
./Libraries/Libraries.o 

C_DEPS += \
./Libraries/Libraries.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/%.o: ../Libraries/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


