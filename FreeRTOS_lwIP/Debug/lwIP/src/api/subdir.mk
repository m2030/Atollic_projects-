################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwIP/src/api/api_lib.c \
../lwIP/src/api/api_msg.c \
../lwIP/src/api/err.c \
../lwIP/src/api/if_api.c \
../lwIP/src/api/netbuf.c \
../lwIP/src/api/netdb.c \
../lwIP/src/api/netifapi.c \
../lwIP/src/api/sockets.c \
../lwIP/src/api/tcpip.c 

OBJS += \
./lwIP/src/api/api_lib.o \
./lwIP/src/api/api_msg.o \
./lwIP/src/api/err.o \
./lwIP/src/api/if_api.o \
./lwIP/src/api/netbuf.o \
./lwIP/src/api/netdb.o \
./lwIP/src/api/netifapi.o \
./lwIP/src/api/sockets.o \
./lwIP/src/api/tcpip.o 

C_DEPS += \
./lwIP/src/api/api_lib.d \
./lwIP/src/api/api_msg.d \
./lwIP/src/api/err.d \
./lwIP/src/api/if_api.d \
./lwIP/src/api/netbuf.d \
./lwIP/src/api/netdb.d \
./lwIP/src/api/netifapi.d \
./lwIP/src/api/sockets.d \
./lwIP/src/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
lwIP/src/api/%.o: ../lwIP/src/api/%.c
	arm-atollic-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -I../src -I../inc -I../inc/arch -I../inc/CMSIS -I../FreeRTOS/Source/include -I../FreeRTOS/Source/portable/GCC/ARM_CM4F -I../lwIP -I../lwIP/src -I../lwIP/src/include -I../lwIP/src/include/compat/stdc -I../lwIP/src/include/compat/posix -I../lwIP/src/include/compat/posix/arpa -I../lwIP/src/include/compat/posix/net -I../lwIP/src/include/compat/posix/sys -I../lwIP/src/include/netif -I../lwIP/src/include/netif/ppp -I../lwIP/src/include/netif/ppp/polarssl -I../lwIP/src/include/lwip -I../lwIP/src/include/lwip/prot -I../lwIP/src/include/lwip/priv -I../lwIP/src/include/lwip/apps -I../lwIP/ports/freertos/include -I../lwIP/ports/freertos/include/arch -I../STM32F4xx_HAL_Driver/Inc -I../STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"

