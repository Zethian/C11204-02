################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../firmware/CMSIS/system_m2sxxx.c 

OBJS += \
./firmware/CMSIS/system_m2sxxx.o 

C_DEPS += \
./firmware/CMSIS/system_m2sxxx.d 


# Each subdirectory must supply rules for building sources it contributes
firmware/CMSIS/%.o: ../firmware/CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


