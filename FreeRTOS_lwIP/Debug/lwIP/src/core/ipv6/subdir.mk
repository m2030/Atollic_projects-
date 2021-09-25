################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwIP/src/core/ipv6/dhcp6.c \
../lwIP/src/core/ipv6/ethip6.c \
../lwIP/src/core/ipv6/icmp6.c \
../lwIP/src/core/ipv6/inet6.c \
../lwIP/src/core/ipv6/ip6.c \
../lwIP/src/core/ipv6/ip6_addr.c \
../lwIP/src/core/ipv6/ip6_frag.c \
../lwIP/src/core/ipv6/mld6.c \
../lwIP/src/core/ipv6/nd6.c 

OBJS += \
./lwIP/src/core/ipv6/dhcp6.o \
./lwIP/src/core/ipv6/ethip6.o \
./lwIP/src/core/ipv6/icmp6.o \
./lwIP/src/core/ipv6/inet6.o \
./lwIP/src/core/ipv6/ip6.o \
./lwIP/src/core/ipv6/ip6_addr.o \
./lwIP/src/core/ipv6/ip6_frag.o \
./lwIP/src/core/ipv6/mld6.o \
./lwIP/src/core/ipv6/nd6.o 

C_DEPS += \
./lwIP/src/core/ipv6/dhcp6.d \
./lwIP/src/core/ipv6/ethip6.d \
./lwIP/src/core/ipv6/icmp6.d \
./lwIP/src/core/ipv6/inet6.d \
./lwIP/src/core/ipv6/ip6.d \
./lwIP/src/core/ipv6/ip6_addr.d \
./lwIP/src/core/ipv6/ip6_frag.d \
./lwIP/src/core/ipv6/mld6.d \
./lwIP/src/core/ipv6/nd6.d 


# Each subdirectory must supply rules for building sources it contributes
lwIP/src/core/ipv6/%.o: ../lwIP/src/core/ipv6/%.c
	arm-atollic-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -I../src -I../inc -I../inc/arch -I../inc/CMSIS -I../FreeRTOS/Source/include -I../FreeRTOS/Source/portable/GCC/ARM_CM4F -I../lwIP -I../lwIP/src -I../lwIP/src/include -I../lwIP/src/include/compat/stdc -I../lwIP/src/include/compat/posix -I../lwIP/src/include/compat/posix/arpa -I../lwIP/src/include/compat/posix/net -I../lwIP/src/include/compat/posix/sys -I../lwIP/src/include/netif -I../lwIP/src/include/netif/ppp -I../lwIP/src/include/netif/ppp/polarssl -I../lwIP/src/include/lwip -I../lwIP/src/include/lwip/prot -I../lwIP/src/include/lwip/priv -I../lwIP/src/include/lwip/apps -I../lwIP/ports/freertos/include -I../lwIP/ports/freertos/include/arch -I../STM32F4xx_HAL_Driver/Inc -I../STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"

