################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwIP/src/netif/bridgeif.c \
../lwIP/src/netif/bridgeif_fdb.c \
../lwIP/src/netif/ethernet.c \
../lwIP/src/netif/lowpan6.c \
../lwIP/src/netif/lowpan6_ble.c \
../lwIP/src/netif/lowpan6_common.c \
../lwIP/src/netif/slipif.c \
../lwIP/src/netif/zepif.c 

OBJS += \
./lwIP/src/netif/bridgeif.o \
./lwIP/src/netif/bridgeif_fdb.o \
./lwIP/src/netif/ethernet.o \
./lwIP/src/netif/lowpan6.o \
./lwIP/src/netif/lowpan6_ble.o \
./lwIP/src/netif/lowpan6_common.o \
./lwIP/src/netif/slipif.o \
./lwIP/src/netif/zepif.o 

C_DEPS += \
./lwIP/src/netif/bridgeif.d \
./lwIP/src/netif/bridgeif_fdb.d \
./lwIP/src/netif/ethernet.d \
./lwIP/src/netif/lowpan6.d \
./lwIP/src/netif/lowpan6_ble.d \
./lwIP/src/netif/lowpan6_common.d \
./lwIP/src/netif/slipif.d \
./lwIP/src/netif/zepif.d 


# Each subdirectory must supply rules for building sources it contributes
lwIP/src/netif/%.o: ../lwIP/src/netif/%.c
	arm-atollic-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -I../src -I../inc -I../inc/arch -I../inc/CMSIS -I../FreeRTOS/Source/include -I../FreeRTOS/Source/portable/GCC/ARM_CM4F -I../lwIP -I../lwIP/src -I../lwIP/src/include -I../lwIP/src/include/compat/stdc -I../lwIP/src/include/compat/posix -I../lwIP/src/include/compat/posix/arpa -I../lwIP/src/include/compat/posix/net -I../lwIP/src/include/compat/posix/sys -I../lwIP/src/include/netif -I../lwIP/src/include/netif/ppp -I../lwIP/src/include/netif/ppp/polarssl -I../lwIP/src/include/lwip -I../lwIP/src/include/lwip/prot -I../lwIP/src/include/lwip/priv -I../lwIP/src/include/lwip/apps -I../lwIP/ports/freertos/include -I../lwIP/ports/freertos/include/arch -I../STM32F4xx_HAL_Driver/Inc -I../STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"

