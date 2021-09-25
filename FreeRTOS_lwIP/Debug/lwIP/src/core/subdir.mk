################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwIP/src/core/altcp.c \
../lwIP/src/core/altcp_alloc.c \
../lwIP/src/core/altcp_tcp.c \
../lwIP/src/core/def.c \
../lwIP/src/core/dns.c \
../lwIP/src/core/ethernetif.c \
../lwIP/src/core/inet_chksum.c \
../lwIP/src/core/init.c \
../lwIP/src/core/ip.c \
../lwIP/src/core/mem.c \
../lwIP/src/core/memp.c \
../lwIP/src/core/netif.c \
../lwIP/src/core/pbuf.c \
../lwIP/src/core/raw.c \
../lwIP/src/core/stats.c \
../lwIP/src/core/sys.c \
../lwIP/src/core/tcp.c \
../lwIP/src/core/tcp_in.c \
../lwIP/src/core/tcp_out.c \
../lwIP/src/core/timeouts.c \
../lwIP/src/core/udp.c 

OBJS += \
./lwIP/src/core/altcp.o \
./lwIP/src/core/altcp_alloc.o \
./lwIP/src/core/altcp_tcp.o \
./lwIP/src/core/def.o \
./lwIP/src/core/dns.o \
./lwIP/src/core/ethernetif.o \
./lwIP/src/core/inet_chksum.o \
./lwIP/src/core/init.o \
./lwIP/src/core/ip.o \
./lwIP/src/core/mem.o \
./lwIP/src/core/memp.o \
./lwIP/src/core/netif.o \
./lwIP/src/core/pbuf.o \
./lwIP/src/core/raw.o \
./lwIP/src/core/stats.o \
./lwIP/src/core/sys.o \
./lwIP/src/core/tcp.o \
./lwIP/src/core/tcp_in.o \
./lwIP/src/core/tcp_out.o \
./lwIP/src/core/timeouts.o \
./lwIP/src/core/udp.o 

C_DEPS += \
./lwIP/src/core/altcp.d \
./lwIP/src/core/altcp_alloc.d \
./lwIP/src/core/altcp_tcp.d \
./lwIP/src/core/def.d \
./lwIP/src/core/dns.d \
./lwIP/src/core/ethernetif.d \
./lwIP/src/core/inet_chksum.d \
./lwIP/src/core/init.d \
./lwIP/src/core/ip.d \
./lwIP/src/core/mem.d \
./lwIP/src/core/memp.d \
./lwIP/src/core/netif.d \
./lwIP/src/core/pbuf.d \
./lwIP/src/core/raw.d \
./lwIP/src/core/stats.d \
./lwIP/src/core/sys.d \
./lwIP/src/core/tcp.d \
./lwIP/src/core/tcp_in.d \
./lwIP/src/core/tcp_out.d \
./lwIP/src/core/timeouts.d \
./lwIP/src/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
lwIP/src/core/%.o: ../lwIP/src/core/%.c
	arm-atollic-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -I../src -I../inc -I../inc/arch -I../inc/CMSIS -I../FreeRTOS/Source/include -I../FreeRTOS/Source/portable/GCC/ARM_CM4F -I../lwIP -I../lwIP/src -I../lwIP/src/include -I../lwIP/src/include/compat/stdc -I../lwIP/src/include/compat/posix -I../lwIP/src/include/compat/posix/arpa -I../lwIP/src/include/compat/posix/net -I../lwIP/src/include/compat/posix/sys -I../lwIP/src/include/netif -I../lwIP/src/include/netif/ppp -I../lwIP/src/include/netif/ppp/polarssl -I../lwIP/src/include/lwip -I../lwIP/src/include/lwip/prot -I../lwIP/src/include/lwip/priv -I../lwIP/src/include/lwip/apps -I../lwIP/ports/freertos/include -I../lwIP/ports/freertos/include/arch -I../STM32F4xx_HAL_Driver/Inc -I../STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"

