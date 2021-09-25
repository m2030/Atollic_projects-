################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwIP/src/apps/http/altcp_proxyconnect.c \
../lwIP/src/apps/http/fs.c \
../lwIP/src/apps/http/fsdata.c \
../lwIP/src/apps/http/http_client.c \
../lwIP/src/apps/http/httpd.c 

OBJS += \
./lwIP/src/apps/http/altcp_proxyconnect.o \
./lwIP/src/apps/http/fs.o \
./lwIP/src/apps/http/fsdata.o \
./lwIP/src/apps/http/http_client.o \
./lwIP/src/apps/http/httpd.o 

C_DEPS += \
./lwIP/src/apps/http/altcp_proxyconnect.d \
./lwIP/src/apps/http/fs.d \
./lwIP/src/apps/http/fsdata.d \
./lwIP/src/apps/http/http_client.d \
./lwIP/src/apps/http/httpd.d 


# Each subdirectory must supply rules for building sources it contributes
lwIP/src/apps/http/%.o: ../lwIP/src/apps/http/%.c
	arm-atollic-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -I../src -I../Libraries/CMSIS/Include -I../Libraries/Device/STM32F4xx/Include -I../Libraries/STM32F4xx_StdPeriph_Driver/inc -I../inc -I../FreeRTOS/Source/include -I../FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"

