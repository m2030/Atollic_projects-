################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwIP/src/netif/ppp/auth.c \
../lwIP/src/netif/ppp/ccp.c \
../lwIP/src/netif/ppp/chap-md5.c \
../lwIP/src/netif/ppp/chap-new.c \
../lwIP/src/netif/ppp/chap_ms.c \
../lwIP/src/netif/ppp/demand.c \
../lwIP/src/netif/ppp/eap.c \
../lwIP/src/netif/ppp/ecp.c \
../lwIP/src/netif/ppp/eui64.c \
../lwIP/src/netif/ppp/fsm.c \
../lwIP/src/netif/ppp/ipcp.c \
../lwIP/src/netif/ppp/ipv6cp.c \
../lwIP/src/netif/ppp/lcp.c \
../lwIP/src/netif/ppp/magic.c \
../lwIP/src/netif/ppp/mppe.c \
../lwIP/src/netif/ppp/multilink.c \
../lwIP/src/netif/ppp/ppp.c \
../lwIP/src/netif/ppp/pppapi.c \
../lwIP/src/netif/ppp/pppcrypt.c \
../lwIP/src/netif/ppp/pppoe.c \
../lwIP/src/netif/ppp/pppol2tp.c \
../lwIP/src/netif/ppp/pppos.c \
../lwIP/src/netif/ppp/upap.c \
../lwIP/src/netif/ppp/utils.c \
../lwIP/src/netif/ppp/vj.c 

OBJS += \
./lwIP/src/netif/ppp/auth.o \
./lwIP/src/netif/ppp/ccp.o \
./lwIP/src/netif/ppp/chap-md5.o \
./lwIP/src/netif/ppp/chap-new.o \
./lwIP/src/netif/ppp/chap_ms.o \
./lwIP/src/netif/ppp/demand.o \
./lwIP/src/netif/ppp/eap.o \
./lwIP/src/netif/ppp/ecp.o \
./lwIP/src/netif/ppp/eui64.o \
./lwIP/src/netif/ppp/fsm.o \
./lwIP/src/netif/ppp/ipcp.o \
./lwIP/src/netif/ppp/ipv6cp.o \
./lwIP/src/netif/ppp/lcp.o \
./lwIP/src/netif/ppp/magic.o \
./lwIP/src/netif/ppp/mppe.o \
./lwIP/src/netif/ppp/multilink.o \
./lwIP/src/netif/ppp/ppp.o \
./lwIP/src/netif/ppp/pppapi.o \
./lwIP/src/netif/ppp/pppcrypt.o \
./lwIP/src/netif/ppp/pppoe.o \
./lwIP/src/netif/ppp/pppol2tp.o \
./lwIP/src/netif/ppp/pppos.o \
./lwIP/src/netif/ppp/upap.o \
./lwIP/src/netif/ppp/utils.o \
./lwIP/src/netif/ppp/vj.o 

C_DEPS += \
./lwIP/src/netif/ppp/auth.d \
./lwIP/src/netif/ppp/ccp.d \
./lwIP/src/netif/ppp/chap-md5.d \
./lwIP/src/netif/ppp/chap-new.d \
./lwIP/src/netif/ppp/chap_ms.d \
./lwIP/src/netif/ppp/demand.d \
./lwIP/src/netif/ppp/eap.d \
./lwIP/src/netif/ppp/ecp.d \
./lwIP/src/netif/ppp/eui64.d \
./lwIP/src/netif/ppp/fsm.d \
./lwIP/src/netif/ppp/ipcp.d \
./lwIP/src/netif/ppp/ipv6cp.d \
./lwIP/src/netif/ppp/lcp.d \
./lwIP/src/netif/ppp/magic.d \
./lwIP/src/netif/ppp/mppe.d \
./lwIP/src/netif/ppp/multilink.d \
./lwIP/src/netif/ppp/ppp.d \
./lwIP/src/netif/ppp/pppapi.d \
./lwIP/src/netif/ppp/pppcrypt.d \
./lwIP/src/netif/ppp/pppoe.d \
./lwIP/src/netif/ppp/pppol2tp.d \
./lwIP/src/netif/ppp/pppos.d \
./lwIP/src/netif/ppp/upap.d \
./lwIP/src/netif/ppp/utils.d \
./lwIP/src/netif/ppp/vj.d 


# Each subdirectory must supply rules for building sources it contributes
lwIP/src/netif/ppp/%.o: ../lwIP/src/netif/ppp/%.c
	arm-atollic-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -I../src -I../inc -I../inc/arch -I../inc/CMSIS -I../FreeRTOS/Source/include -I../FreeRTOS/Source/portable/GCC/ARM_CM4F -I../lwIP -I../lwIP/src -I../lwIP/src/include -I../lwIP/src/include/compat/stdc -I../lwIP/src/include/compat/posix -I../lwIP/src/include/compat/posix/arpa -I../lwIP/src/include/compat/posix/net -I../lwIP/src/include/compat/posix/sys -I../lwIP/src/include/netif -I../lwIP/src/include/netif/ppp -I../lwIP/src/include/netif/ppp/polarssl -I../lwIP/src/include/lwip -I../lwIP/src/include/lwip/prot -I../lwIP/src/include/lwip/priv -I../lwIP/src/include/lwip/apps -I../lwIP/ports/freertos/include -I../lwIP/ports/freertos/include/arch -I../STM32F4xx_HAL_Driver/Inc -I../STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"

