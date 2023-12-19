################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/24cxx.c \
../Core/Src/24l01.c \
../Core/Src/binaryExpression.c \
../Core/Src/bmp.c \
../Core/Src/calc.c \
../Core/Src/calcExpression.c \
../Core/Src/ctiic.c \
../Core/Src/delay.c \
../Core/Src/diskio.c \
../Core/Src/exfuns.c \
../Core/Src/fattester.c \
../Core/Src/ff.c \
../Core/Src/fontupd.c \
../Core/Src/ft5206.c \
../Core/Src/gif.c \
../Core/Src/gpio.c \
../Core/Src/gt9147.c \
../Core/Src/key.c \
../Core/Src/lcd.c \
../Core/Src/led.c \
../Core/Src/main.c \
../Core/Src/malloc.c \
../Core/Src/mmc_sd.c \
../Core/Src/mycc936.c \
../Core/Src/myiic.c \
../Core/Src/ott2001a.c \
../Core/Src/piclib.c \
../Core/Src/remote.c \
../Core/Src/rtc.c \
../Core/Src/solveEquation.c \
../Core/Src/spi.c \
../Core/Src/stack.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/sys.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/tetris.c \
../Core/Src/text.c \
../Core/Src/tim.c \
../Core/Src/tjpgd.c \
../Core/Src/touch.c \
../Core/Src/usart.c \
../Core/Src/utils.c \
../Core/Src/w25qxx.c 

OBJS += \
./Core/Src/24cxx.o \
./Core/Src/24l01.o \
./Core/Src/binaryExpression.o \
./Core/Src/bmp.o \
./Core/Src/calc.o \
./Core/Src/calcExpression.o \
./Core/Src/ctiic.o \
./Core/Src/delay.o \
./Core/Src/diskio.o \
./Core/Src/exfuns.o \
./Core/Src/fattester.o \
./Core/Src/ff.o \
./Core/Src/fontupd.o \
./Core/Src/ft5206.o \
./Core/Src/gif.o \
./Core/Src/gpio.o \
./Core/Src/gt9147.o \
./Core/Src/key.o \
./Core/Src/lcd.o \
./Core/Src/led.o \
./Core/Src/main.o \
./Core/Src/malloc.o \
./Core/Src/mmc_sd.o \
./Core/Src/mycc936.o \
./Core/Src/myiic.o \
./Core/Src/ott2001a.o \
./Core/Src/piclib.o \
./Core/Src/remote.o \
./Core/Src/rtc.o \
./Core/Src/solveEquation.o \
./Core/Src/spi.o \
./Core/Src/stack.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/sys.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/tetris.o \
./Core/Src/text.o \
./Core/Src/tim.o \
./Core/Src/tjpgd.o \
./Core/Src/touch.o \
./Core/Src/usart.o \
./Core/Src/utils.o \
./Core/Src/w25qxx.o 

C_DEPS += \
./Core/Src/24cxx.d \
./Core/Src/24l01.d \
./Core/Src/binaryExpression.d \
./Core/Src/bmp.d \
./Core/Src/calc.d \
./Core/Src/calcExpression.d \
./Core/Src/ctiic.d \
./Core/Src/delay.d \
./Core/Src/diskio.d \
./Core/Src/exfuns.d \
./Core/Src/fattester.d \
./Core/Src/ff.d \
./Core/Src/fontupd.d \
./Core/Src/ft5206.d \
./Core/Src/gif.d \
./Core/Src/gpio.d \
./Core/Src/gt9147.d \
./Core/Src/key.d \
./Core/Src/lcd.d \
./Core/Src/led.d \
./Core/Src/main.d \
./Core/Src/malloc.d \
./Core/Src/mmc_sd.d \
./Core/Src/mycc936.d \
./Core/Src/myiic.d \
./Core/Src/ott2001a.d \
./Core/Src/piclib.d \
./Core/Src/remote.d \
./Core/Src/rtc.d \
./Core/Src/solveEquation.d \
./Core/Src/spi.d \
./Core/Src/stack.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/sys.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/tetris.d \
./Core/Src/text.d \
./Core/Src/tim.d \
./Core/Src/tjpgd.d \
./Core/Src/touch.d \
./Core/Src/usart.d \
./Core/Src/utils.d \
./Core/Src/w25qxx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/24cxx.cyclo ./Core/Src/24cxx.d ./Core/Src/24cxx.o ./Core/Src/24cxx.su ./Core/Src/24l01.cyclo ./Core/Src/24l01.d ./Core/Src/24l01.o ./Core/Src/24l01.su ./Core/Src/binaryExpression.cyclo ./Core/Src/binaryExpression.d ./Core/Src/binaryExpression.o ./Core/Src/binaryExpression.su ./Core/Src/bmp.cyclo ./Core/Src/bmp.d ./Core/Src/bmp.o ./Core/Src/bmp.su ./Core/Src/calc.cyclo ./Core/Src/calc.d ./Core/Src/calc.o ./Core/Src/calc.su ./Core/Src/calcExpression.cyclo ./Core/Src/calcExpression.d ./Core/Src/calcExpression.o ./Core/Src/calcExpression.su ./Core/Src/ctiic.cyclo ./Core/Src/ctiic.d ./Core/Src/ctiic.o ./Core/Src/ctiic.su ./Core/Src/delay.cyclo ./Core/Src/delay.d ./Core/Src/delay.o ./Core/Src/delay.su ./Core/Src/diskio.cyclo ./Core/Src/diskio.d ./Core/Src/diskio.o ./Core/Src/diskio.su ./Core/Src/exfuns.cyclo ./Core/Src/exfuns.d ./Core/Src/exfuns.o ./Core/Src/exfuns.su ./Core/Src/fattester.cyclo ./Core/Src/fattester.d ./Core/Src/fattester.o ./Core/Src/fattester.su ./Core/Src/ff.cyclo ./Core/Src/ff.d ./Core/Src/ff.o ./Core/Src/ff.su ./Core/Src/fontupd.cyclo ./Core/Src/fontupd.d ./Core/Src/fontupd.o ./Core/Src/fontupd.su ./Core/Src/ft5206.cyclo ./Core/Src/ft5206.d ./Core/Src/ft5206.o ./Core/Src/ft5206.su ./Core/Src/gif.cyclo ./Core/Src/gif.d ./Core/Src/gif.o ./Core/Src/gif.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/gt9147.cyclo ./Core/Src/gt9147.d ./Core/Src/gt9147.o ./Core/Src/gt9147.su ./Core/Src/key.cyclo ./Core/Src/key.d ./Core/Src/key.o ./Core/Src/key.su ./Core/Src/lcd.cyclo ./Core/Src/lcd.d ./Core/Src/lcd.o ./Core/Src/lcd.su ./Core/Src/led.cyclo ./Core/Src/led.d ./Core/Src/led.o ./Core/Src/led.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/malloc.cyclo ./Core/Src/malloc.d ./Core/Src/malloc.o ./Core/Src/malloc.su ./Core/Src/mmc_sd.cyclo ./Core/Src/mmc_sd.d ./Core/Src/mmc_sd.o ./Core/Src/mmc_sd.su ./Core/Src/mycc936.cyclo ./Core/Src/mycc936.d ./Core/Src/mycc936.o ./Core/Src/mycc936.su ./Core/Src/myiic.cyclo ./Core/Src/myiic.d ./Core/Src/myiic.o ./Core/Src/myiic.su ./Core/Src/ott2001a.cyclo ./Core/Src/ott2001a.d ./Core/Src/ott2001a.o ./Core/Src/ott2001a.su ./Core/Src/piclib.cyclo ./Core/Src/piclib.d ./Core/Src/piclib.o ./Core/Src/piclib.su ./Core/Src/remote.cyclo ./Core/Src/remote.d ./Core/Src/remote.o ./Core/Src/remote.su ./Core/Src/rtc.cyclo ./Core/Src/rtc.d ./Core/Src/rtc.o ./Core/Src/rtc.su ./Core/Src/solveEquation.cyclo ./Core/Src/solveEquation.d ./Core/Src/solveEquation.o ./Core/Src/solveEquation.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stack.cyclo ./Core/Src/stack.d ./Core/Src/stack.o ./Core/Src/stack.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/sys.cyclo ./Core/Src/sys.d ./Core/Src/sys.o ./Core/Src/sys.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/tetris.cyclo ./Core/Src/tetris.d ./Core/Src/tetris.o ./Core/Src/tetris.su ./Core/Src/text.cyclo ./Core/Src/text.d ./Core/Src/text.o ./Core/Src/text.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/tjpgd.cyclo ./Core/Src/tjpgd.d ./Core/Src/tjpgd.o ./Core/Src/tjpgd.su ./Core/Src/touch.cyclo ./Core/Src/touch.d ./Core/Src/touch.o ./Core/Src/touch.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/utils.cyclo ./Core/Src/utils.d ./Core/Src/utils.o ./Core/Src/utils.su ./Core/Src/w25qxx.cyclo ./Core/Src/w25qxx.d ./Core/Src/w25qxx.o ./Core/Src/w25qxx.su

.PHONY: clean-Core-2f-Src

