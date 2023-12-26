################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/src/adc.c \
../App/src/btn_gpio.c \
../App/src/button.c \
../App/src/cli.c \
../App/src/gpio.c \
../App/src/led.c \
../App/src/pwm.c \
../App/src/spi.c \
../App/src/uart.c \
../App/src/utils.c 

OBJS += \
./App/src/adc.o \
./App/src/btn_gpio.o \
./App/src/button.o \
./App/src/cli.o \
./App/src/gpio.o \
./App/src/led.o \
./App/src/pwm.o \
./App/src/spi.o \
./App/src/uart.o \
./App/src/utils.o 

C_DEPS += \
./App/src/adc.d \
./App/src/btn_gpio.d \
./App/src/button.d \
./App/src/cli.d \
./App/src/gpio.d \
./App/src/led.d \
./App/src/pwm.d \
./App/src/spi.d \
./App/src/uart.d \
./App/src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
App/src/%.o App/src/%.su App/src/%.cyclo: ../App/src/%.c App/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/workspaces/stm32_ws/stm32f429zi_fw/App" -I"D:/workspaces/stm32_ws/stm32f429zi_fw/App/src" -I"D:/workspaces/stm32_ws/stm32f429zi_fw/App/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-src

clean-App-2f-src:
	-$(RM) ./App/src/adc.cyclo ./App/src/adc.d ./App/src/adc.o ./App/src/adc.su ./App/src/btn_gpio.cyclo ./App/src/btn_gpio.d ./App/src/btn_gpio.o ./App/src/btn_gpio.su ./App/src/button.cyclo ./App/src/button.d ./App/src/button.o ./App/src/button.su ./App/src/cli.cyclo ./App/src/cli.d ./App/src/cli.o ./App/src/cli.su ./App/src/gpio.cyclo ./App/src/gpio.d ./App/src/gpio.o ./App/src/gpio.su ./App/src/led.cyclo ./App/src/led.d ./App/src/led.o ./App/src/led.su ./App/src/pwm.cyclo ./App/src/pwm.d ./App/src/pwm.o ./App/src/pwm.su ./App/src/spi.cyclo ./App/src/spi.d ./App/src/spi.o ./App/src/spi.su ./App/src/uart.cyclo ./App/src/uart.d ./App/src/uart.o ./App/src/uart.su ./App/src/utils.cyclo ./App/src/utils.d ./App/src/utils.o ./App/src/utils.su

.PHONY: clean-App-2f-src

