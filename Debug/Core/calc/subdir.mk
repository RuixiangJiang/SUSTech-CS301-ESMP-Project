################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/calc/binaryExpression.c \
../Core/calc/calc.c \
../Core/calc/calcExpression.c \
../Core/calc/solveEquation.c \
../Core/calc/stack.c 

OBJS += \
./Core/calc/binaryExpression.o \
./Core/calc/calc.o \
./Core/calc/calcExpression.o \
./Core/calc/solveEquation.o \
./Core/calc/stack.o 

C_DEPS += \
./Core/calc/binaryExpression.d \
./Core/calc/calc.d \
./Core/calc/calcExpression.d \
./Core/calc/solveEquation.d \
./Core/calc/stack.d 


# Each subdirectory must supply rules for building sources it contributes
Core/calc/%.o Core/calc/%.su Core/calc/%.cyclo: ../Core/calc/%.c Core/calc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-calc

clean-Core-2f-calc:
	-$(RM) ./Core/calc/binaryExpression.cyclo ./Core/calc/binaryExpression.d ./Core/calc/binaryExpression.o ./Core/calc/binaryExpression.su ./Core/calc/calc.cyclo ./Core/calc/calc.d ./Core/calc/calc.o ./Core/calc/calc.su ./Core/calc/calcExpression.cyclo ./Core/calc/calcExpression.d ./Core/calc/calcExpression.o ./Core/calc/calcExpression.su ./Core/calc/solveEquation.cyclo ./Core/calc/solveEquation.d ./Core/calc/solveEquation.o ./Core/calc/solveEquation.su ./Core/calc/stack.cyclo ./Core/calc/stack.d ./Core/calc/stack.o ./Core/calc/stack.su

.PHONY: clean-Core-2f-calc

