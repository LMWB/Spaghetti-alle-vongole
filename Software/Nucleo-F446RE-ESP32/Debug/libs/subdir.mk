################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/lukas/work/git/LMWB/noRTOS/Software/libs/multifunc.c \
/home/lukas/work/git/LMWB/noRTOS/Software/libs/noRTOS.c \
/home/lukas/work/git/LMWB/noRTOS/Software/libs/utils.c 

OBJS += \
./libs/multifunc.o \
./libs/noRTOS.o \
./libs/utils.o 

C_DEPS += \
./libs/multifunc.d \
./libs/noRTOS.d \
./libs/utils.d 


# Each subdirectory must supply rules for building sources it contributes
libs/multifunc.o: /home/lukas/work/git/LMWB/noRTOS/Software/libs/multifunc.c libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../../libs -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
libs/noRTOS.o: /home/lukas/work/git/LMWB/noRTOS/Software/libs/noRTOS.c libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../../libs -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
libs/utils.o: /home/lukas/work/git/LMWB/noRTOS/Software/libs/utils.c libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../../libs -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libs

clean-libs:
	-$(RM) ./libs/multifunc.cyclo ./libs/multifunc.d ./libs/multifunc.o ./libs/multifunc.su ./libs/noRTOS.cyclo ./libs/noRTOS.d ./libs/noRTOS.o ./libs/noRTOS.su ./libs/utils.cyclo ./libs/utils.d ./libs/utils.o ./libs/utils.su

.PHONY: clean-libs

