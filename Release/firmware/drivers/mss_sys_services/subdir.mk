################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../firmware/drivers/mss_sys_services/mss_comblk.c \
../firmware/drivers/mss_sys_services/mss_sys_services.c 

OBJS += \
./firmware/drivers/mss_sys_services/mss_comblk.o \
./firmware/drivers/mss_sys_services/mss_sys_services.o 

C_DEPS += \
./firmware/drivers/mss_sys_services/mss_comblk.d \
./firmware/drivers/mss_sys_services/mss_sys_services.d 


# Each subdirectory must supply rules for building sources it contributes
firmware/drivers/mss_sys_services/%.o: ../firmware/drivers/mss_sys_services/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


