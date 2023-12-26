################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/src/clcd/CLCD.c 

OBJS += \
./App/src/clcd/CLCD.o 

C_DEPS += \
./App/src/clcd/CLCD.d 


# Each subdirectory must supply rules for building sources it contributes
App/src/clcd/%.o App/src/clcd/%.su App/src/clcd/%.cyclo: ../App/src/clcd/%.c App/src/clcd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/workspaces/stm32_ws/stm32f429zi_fw/App" -I"D:/workspaces/stm32_ws/stm32f429zi_fw/App/src" -I"D:/workspaces/stm32_ws/stm32f429zi_fw/App/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-src-2f-clcd

clean-App-2f-src-2f-clcd:
	-$(RM) ./App/src/clcd/CLCD.cyclo ./App/src/clcd/CLCD.d ./App/src/clcd/CLCD.o ./App/src/clcd/CLCD.su

.PHONY: clean-App-2f-src-2f-clcd

