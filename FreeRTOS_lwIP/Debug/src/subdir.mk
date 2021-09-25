################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/startup_stm32f40xx.s 

C_SRCS += \
../src/app_ethernet.c \
../src/enc28j60.c \
../src/main.c \
../src/stm32f4xx_hal_msp.c \
../src/stm32f4xx_it.c \
../src/system_stm32f4xx.c \
../src/tcp_echoserver.c \
../src/tiny_printf.c 

OBJS += \
./src/app_ethernet.o \
./src/enc28j60.o \
./src/main.o \
./src/startup_stm32f40xx.o \
./src/stm32f4xx_hal_msp.o \
./src/stm32f4xx_it.o \
./src/system_stm32f4xx.o \
./src/tcp_echoserver.o \
./src/tiny_printf.o 

C_DEPS += \
./src/app_ethernet.d \
./src/enc28j60.d \
./src/main.d \
./src/stm32f4xx_hal_msp.d \
./src/stm32f4xx_it.d \
./src/system_stm32f4xx.d \
./src/tcp_echoserver.d \
./src/tiny_printf.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	arm-atollic-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -I../src -I../inc -I../inc/arch -I../inc/CMSIS -I../FreeRTOS/Source/include -I../FreeRTOS/Source/portable/GCC/ARM_CM4F -I../lwIP -I../lwIP/src -I../lwIP/src/include -I../lwIP/src/include/compat/stdc -I../lwIP/src/include/compat/posix -I../lwIP/src/include/compat/posix/arpa -I../lwIP/src/include/compat/posix/net -I../lwIP/src/include/compat/posix/sys -I../lwIP/src/include/netif -I../lwIP/src/include/netif/ppp -I../lwIP/src/include/netif/ppp/polarssl -I../lwIP/src/include/lwip -I../lwIP/src/include/lwip/prot -I../lwIP/src/include/lwip/priv -I../lwIP/src/include/lwip/apps -I../lwIP/ports/freertos/include -I../lwIP/ports/freertos/include/arch -I../STM32F4xx_HAL_Driver/Inc -I../STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"
src/%.o: ../src/%.s
	arm-atollic-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -I../src -I../Libraries/CMSIS/Include -I../Libraries/Device/STM32F4xx/Include -I../Libraries/STM32F4xx_StdPeriph_Driver/inc -I../lwIP/ports/freertos/include/arch -I../lwIP/src/include/compat/stdc -I../lwIP/src/include/compat/posix -I../lwIP/src/include/compat/posix/arpa -I../lwIP/src/include/compat/posix/net -I../lwIP/src/include/compat/posix/sys -I../lwIP/src/include/netif/ppp/polarssl -I../lwIP/src/include/netif/ppp -I../lwIP/src/include/netif -I/FreeRTOS_lwIP/lwIP/src/include/lwip -I../lwIP/src/include/lwip/prot -I../lwIP/src/include/lwip/priv -I../lwIP/src/include/lwip/apps -I../lwIP/src -I../lwIP/src/netif/ppp -I../lwIP -I/FreeRTOS_lwIP/lwIP/src/include -I../inc/arch -I/FreeRTOS_lwIP/lwIP/ports/freertos/include/arch -I../lwIP/ports/freertos -I../STM32F4xx_HAL_Driver/Inc -I../STM32F4xx_HAL_Driver/Inc/Legacy -I../inc/CMSIS -g -Wa,--warn -x assembler-with-cpp -specs=nano.specs -o "$@" "$<"

