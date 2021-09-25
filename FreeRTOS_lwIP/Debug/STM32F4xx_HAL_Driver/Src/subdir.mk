################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
../STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c 

OBJS += \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.o \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.o 

C_DEPS += \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.d \
./STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.d 


# Each subdirectory must supply rules for building sources it contributes
STM32F4xx_HAL_Driver/Src/%.o: ../STM32F4xx_HAL_Driver/Src/%.c
	arm-atollic-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -I../src -I../inc -I../inc/arch -I../inc/CMSIS -I../FreeRTOS/Source/include -I../FreeRTOS/Source/portable/GCC/ARM_CM4F -I../lwIP -I../lwIP/src -I../lwIP/src/include -I../lwIP/src/include/compat/stdc -I../lwIP/src/include/compat/posix -I../lwIP/src/include/compat/posix/arpa -I../lwIP/src/include/compat/posix/net -I../lwIP/src/include/compat/posix/sys -I../lwIP/src/include/netif -I../lwIP/src/include/netif/ppp -I../lwIP/src/include/netif/ppp/polarssl -I../lwIP/src/include/lwip -I../lwIP/src/include/lwip/prot -I../lwIP/src/include/lwip/priv -I../lwIP/src/include/lwip/apps -I../lwIP/ports/freertos/include -I../lwIP/ports/freertos/include/arch -I../STM32F4xx_HAL_Driver/Inc -I../STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"

