################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include firmware/hal/CortexM3/GNU/subdir.mk
-include firmware/hal/CortexM3/subdir.mk
-include firmware/drivers_config/sys_config/subdir.mk
-include firmware/drivers/mss_uart/subdir.mk
-include firmware/drivers/mss_sys_services/subdir.mk
-include firmware/drivers/mss_nvm/subdir.mk
-include firmware/drivers/mss_hpdma/subdir.mk
-include firmware/drivers/mss_gpio/subdir.mk
-include firmware/CMSIS/startup_gcc/subdir.mk
-include firmware/CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
C11204.hex \

SECONDARY_SIZE += \
C11204.siz \


# All Target
all: C11204.elf secondary-outputs

# Tool invocations
C11204.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -Xlinker --gc-sections -Wl,-Map,"C11204.map" -o "C11204.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

C11204.hex: C11204.elf
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "C11204.elf"  "C11204.hex"
	@echo 'Finished building: $@'
	@echo ' '

C11204.siz: C11204.elf
	@echo 'Invoking: GNU ARM Cross Print Size'
	arm-none-eabi-size --format=berkeley "C11204.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) C11204.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
