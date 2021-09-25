################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwIP/src/apps/snmp/snmp_asn1.c \
../lwIP/src/apps/snmp/snmp_core.c \
../lwIP/src/apps/snmp/snmp_mib2.c \
../lwIP/src/apps/snmp/snmp_mib2_icmp.c \
../lwIP/src/apps/snmp/snmp_mib2_interfaces.c \
../lwIP/src/apps/snmp/snmp_mib2_ip.c \
../lwIP/src/apps/snmp/snmp_mib2_snmp.c \
../lwIP/src/apps/snmp/snmp_mib2_system.c \
../lwIP/src/apps/snmp/snmp_mib2_tcp.c \
../lwIP/src/apps/snmp/snmp_mib2_udp.c \
../lwIP/src/apps/snmp/snmp_msg.c \
../lwIP/src/apps/snmp/snmp_netconn.c \
../lwIP/src/apps/snmp/snmp_pbuf_stream.c \
../lwIP/src/apps/snmp/snmp_raw.c \
../lwIP/src/apps/snmp/snmp_scalar.c \
../lwIP/src/apps/snmp/snmp_snmpv2_framework.c \
../lwIP/src/apps/snmp/snmp_snmpv2_usm.c \
../lwIP/src/apps/snmp/snmp_table.c \
../lwIP/src/apps/snmp/snmp_threadsync.c \
../lwIP/src/apps/snmp/snmp_traps.c \
../lwIP/src/apps/snmp/snmpv3.c \
../lwIP/src/apps/snmp/snmpv3_mbedtls.c 

OBJS += \
./lwIP/src/apps/snmp/snmp_asn1.o \
./lwIP/src/apps/snmp/snmp_core.o \
./lwIP/src/apps/snmp/snmp_mib2.o \
./lwIP/src/apps/snmp/snmp_mib2_icmp.o \
./lwIP/src/apps/snmp/snmp_mib2_interfaces.o \
./lwIP/src/apps/snmp/snmp_mib2_ip.o \
./lwIP/src/apps/snmp/snmp_mib2_snmp.o \
./lwIP/src/apps/snmp/snmp_mib2_system.o \
./lwIP/src/apps/snmp/snmp_mib2_tcp.o \
./lwIP/src/apps/snmp/snmp_mib2_udp.o \
./lwIP/src/apps/snmp/snmp_msg.o \
./lwIP/src/apps/snmp/snmp_netconn.o \
./lwIP/src/apps/snmp/snmp_pbuf_stream.o \
./lwIP/src/apps/snmp/snmp_raw.o \
./lwIP/src/apps/snmp/snmp_scalar.o \
./lwIP/src/apps/snmp/snmp_snmpv2_framework.o \
./lwIP/src/apps/snmp/snmp_snmpv2_usm.o \
./lwIP/src/apps/snmp/snmp_table.o \
./lwIP/src/apps/snmp/snmp_threadsync.o \
./lwIP/src/apps/snmp/snmp_traps.o \
./lwIP/src/apps/snmp/snmpv3.o \
./lwIP/src/apps/snmp/snmpv3_mbedtls.o 

C_DEPS += \
./lwIP/src/apps/snmp/snmp_asn1.d \
./lwIP/src/apps/snmp/snmp_core.d \
./lwIP/src/apps/snmp/snmp_mib2.d \
./lwIP/src/apps/snmp/snmp_mib2_icmp.d \
./lwIP/src/apps/snmp/snmp_mib2_interfaces.d \
./lwIP/src/apps/snmp/snmp_mib2_ip.d \
./lwIP/src/apps/snmp/snmp_mib2_snmp.d \
./lwIP/src/apps/snmp/snmp_mib2_system.d \
./lwIP/src/apps/snmp/snmp_mib2_tcp.d \
./lwIP/src/apps/snmp/snmp_mib2_udp.d \
./lwIP/src/apps/snmp/snmp_msg.d \
./lwIP/src/apps/snmp/snmp_netconn.d \
./lwIP/src/apps/snmp/snmp_pbuf_stream.d \
./lwIP/src/apps/snmp/snmp_raw.d \
./lwIP/src/apps/snmp/snmp_scalar.d \
./lwIP/src/apps/snmp/snmp_snmpv2_framework.d \
./lwIP/src/apps/snmp/snmp_snmpv2_usm.d \
./lwIP/src/apps/snmp/snmp_table.d \
./lwIP/src/apps/snmp/snmp_threadsync.d \
./lwIP/src/apps/snmp/snmp_traps.d \
./lwIP/src/apps/snmp/snmpv3.d \
./lwIP/src/apps/snmp/snmpv3_mbedtls.d 


# Each subdirectory must supply rules for building sources it contributes
lwIP/src/apps/snmp/%.o: ../lwIP/src/apps/snmp/%.c
	arm-atollic-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -I../src -I../Libraries/CMSIS/Include -I../Libraries/Device/STM32F4xx/Include -I../Libraries/STM32F4xx_StdPeriph_Driver/inc -I../inc -I../FreeRTOS/Source/include -I../FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"

