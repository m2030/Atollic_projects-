################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwIP/src/apps/lwiperf/lwiperf.c 

OBJS += \
./lwIP/src/apps/lwiperf/lwiperf.o 

C_DEPS += \
./lwIP/src/apps/lwiperf/lwiperf.d 


# Each subdirectory must supply rules for building sources it contributes
lwIP/src/apps/lwiperf/%.o: ../lwIP/src/apps/lwiperf/%.c
	arm-atollic-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -I../src -I../Libraries/CMSIS/Include -I../Libraries/Device/STM32F4xx/Include -I../Libraries/STM32F4xx_StdPeriph_Driver/inc -I../inc -I../FreeRTOS/Source/include -I../FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"

