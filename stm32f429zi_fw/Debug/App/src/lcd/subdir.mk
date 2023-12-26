################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/src/lcd/st7735.c 

OBJS += \
./App/src/lcd/st7735.o 

C_DEPS += \
./App/src/lcd/st7735.d 


# Each subdirectory must supply rules for building sources it contributes
App/src/lcd/%.o App/src/lcd/%.su App/src/lcd/%.cyclo: ../App/src/lcd/%.c App/src/lcd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/workspaces/stm32_ws/stm32f429zi_fw/App" -I"D:/workspaces/stm32_ws/stm32f429zi_fw/App/src" -I"D:/workspaces/stm32_ws/stm32f429zi_fw/App/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-src-2f-lcd

clean-App-2f-src-2f-lcd:
	-$(RM) ./App/src/lcd/st7735.cyclo ./App/src/lcd/st7735.d ./App/src/lcd/st7735.o ./App/src/lcd/st7735.su

.PHONY: clean-App-2f-src-2f-lcd

