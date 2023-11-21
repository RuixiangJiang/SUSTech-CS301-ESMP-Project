################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/calc/binaryExpression.c \
../Core/Src/calc/calc.c \
../Core/Src/calc/calcExpression.c \
../Core/Src/calc/solveEquation.c \
../Core/Src/calc/stack.c 

OBJS += \
./Core/Src/calc/binaryExpression.o \
./Core/Src/calc/calc.o \
./Core/Src/calc/calcExpression.o \
./Core/Src/calc/solveEquation.o \
./Core/Src/calc/stack.o 

C_DEPS += \
./Core/Src/calc/binaryExpression.d \
./Core/Src/calc/calc.d \
./Core/Src/calc/calcExpression.d \
./Core/Src/calc/solveEquation.d \
./Core/Src/calc/stack.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/calc/%.o Core/Src/calc/%.su Core/Src/calc/%.cyclo: ../Core/Src/calc/%.c Core/Src/calc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-calc

clean-Core-2f-Src-2f-calc:
	-$(RM) ./Core/Src/calc/binaryExpression.cyclo ./Core/Src/calc/binaryExpression.d ./Core/Src/calc/binaryExpression.o ./Core/Src/calc/binaryExpression.su ./Core/Src/calc/calc.cyclo ./Core/Src/calc/calc.d ./Core/Src/calc/calc.o ./Core/Src/calc/calc.su ./Core/Src/calc/calcExpression.cyclo ./Core/Src/calc/calcExpression.d ./Core/Src/calc/calcExpression.o ./Core/Src/calc/calcExpression.su ./Core/Src/calc/solveEquation.cyclo ./Core/Src/calc/solveEquation.d ./Core/Src/calc/solveEquation.o ./Core/Src/calc/solveEquation.su ./Core/Src/calc/stack.cyclo ./Core/Src/calc/stack.d ./Core/Src/calc/stack.o ./Core/Src/calc/stack.su

.PHONY: clean-Core-2f-Src-2f-calc

