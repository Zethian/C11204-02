################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../firmware/drivers/mss_gpio/mss_gpio.c 

OBJS += \
./firmware/drivers/mss_gpio/mss_gpio.o 

C_DEPS += \
./firmware/drivers/mss_gpio/mss_gpio.d 


# Each subdirectory must supply rules for building sources it contributes
firmware/drivers/mss_gpio/%.o: ../firmware/drivers/mss_gpio/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


