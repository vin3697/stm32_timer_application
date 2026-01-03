################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/initialization/init_c_file.c 

OBJS += \
./Core/initialization/init_c_file.o 

C_DEPS += \
./Core/initialization/init_c_file.d 


# Each subdirectory must supply rules for building sources it contributes
Core/initialization/%.o Core/initialization/%.su Core/initialization/%.cyclo: ../Core/initialization/%.c Core/initialization/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Core/initialization -I../Core/algorithm -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-initialization

clean-Core-2f-initialization:
	-$(RM) ./Core/initialization/init_c_file.cyclo ./Core/initialization/init_c_file.d ./Core/initialization/init_c_file.o ./Core/initialization/init_c_file.su

.PHONY: clean-Core-2f-initialization

