################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Wf4464/Src/radio_comm.c \
../Core/Wf4464/Src/radio_hal.c \
../Core/Wf4464/Src/wf4463_app.c \
../Core/Wf4464/Src/wf446x_api_lib.c 

OBJS += \
./Core/Wf4464/Src/radio_comm.o \
./Core/Wf4464/Src/radio_hal.o \
./Core/Wf4464/Src/wf4463_app.o \
./Core/Wf4464/Src/wf446x_api_lib.o 

C_DEPS += \
./Core/Wf4464/Src/radio_comm.d \
./Core/Wf4464/Src/radio_hal.d \
./Core/Wf4464/Src/wf4463_app.d \
./Core/Wf4464/Src/wf446x_api_lib.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Wf4464/Src/%.o Core/Wf4464/Src/%.su Core/Wf4464/Src/%.cyclo: ../Core/Wf4464/Src/%.c Core/Wf4464/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/19344/Desktop/STM32F103C8T6_WF_DEMO/STM32F103C8T6_WF_DEMO/Core/Wf4464/Inc" -I"C:/Users/19344/Desktop/STM32F103C8T6_WF_DEMO/STM32F103C8T6_WF_DEMO/Core/Wf4464/Src" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Wf4464-2f-Src

clean-Core-2f-Wf4464-2f-Src:
	-$(RM) ./Core/Wf4464/Src/radio_comm.cyclo ./Core/Wf4464/Src/radio_comm.d ./Core/Wf4464/Src/radio_comm.o ./Core/Wf4464/Src/radio_comm.su ./Core/Wf4464/Src/radio_hal.cyclo ./Core/Wf4464/Src/radio_hal.d ./Core/Wf4464/Src/radio_hal.o ./Core/Wf4464/Src/radio_hal.su ./Core/Wf4464/Src/wf4463_app.cyclo ./Core/Wf4464/Src/wf4463_app.d ./Core/Wf4464/Src/wf4463_app.o ./Core/Wf4464/Src/wf4463_app.su ./Core/Wf4464/Src/wf446x_api_lib.cyclo ./Core/Wf4464/Src/wf446x_api_lib.d ./Core/Wf4464/Src/wf446x_api_lib.o ./Core/Wf4464/Src/wf446x_api_lib.su

.PHONY: clean-Core-2f-Wf4464-2f-Src

