################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include lwIP/src/netif/ppp/polarssl/subdir.mk
-include lwIP/src/netif/ppp/subdir.mk
-include lwIP/src/netif/subdir.mk
-include lwIP/src/core/ipv6/subdir.mk
-include lwIP/src/core/ipv4/subdir.mk
-include lwIP/src/core/subdir.mk
-include lwIP/src/api/subdir.mk
-include lwIP/ports/freertos/include/subdir.mk
-include STM32F4xx_HAL_Driver/Src/subdir.mk
-include FreeRTOS/Source/portable/MemMang/subdir.mk
-include FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include FreeRTOS/Source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECOUTPUTBUILDVAR += \
EXECUTABLES \


# All Target
all: FreeRTOS_lwIP.elf secondary-outputs

# Tool invocations
FreeRTOS_lwIP.elf: $(OBJS) $(USER_OBJS)
	arm-atollic-eabi-gcc -o "FreeRTOS_lwIP.elf" $(OBJS) $(USER_OBJS) $(LIBS) -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -T"../stm32f4_flash.ld" -specs=nosys.specs -static -Wl,-cref,-u,Reset_Handler "-Wl,-Map=FreeRTOS_lwIP.map" -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x80 -Wl,--start-group -lc -lm -Wl,--end-group -specs=nano.specs

EXECUTABLES: FreeRTOS_lwIP.elf $(EXECUTABLES)
	"C:\Program Files (x86)\Atollic\TrueSTUDIO for STM32 9.3.0\ide\jre\bin\java" -jar "C:\Program Files (x86)\Atollic\TrueSTUDIO for STM32 9.3.0\Tools\arm-atollic-reports.jar" convert-hex sizeinfo list FreeRTOS_lwIP.elf

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(SECOUTPUTBUILDVAR)$(OBJS)$(C_DEPS) FreeRTOS_lwIP.elf
	-@echo ' '

secondary-outputs: $(SECOUTPUTBUILDVAR)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
