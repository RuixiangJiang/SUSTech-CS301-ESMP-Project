# THIS FILE IS AUTOMATICALLY GENERATED. DO NOT EDIT.
# BASED ON c:\Users\Jrx\STM32CubeIDE\workspace_1.13.2\Project

function(add_st_target_properties TARGET_NAME)

target_compile_definitions(
    ${TARGET_NAME} PRIVATE
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:ASM>>:DEBUG>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:DEBUG>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:USE_HAL_DRIVER>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:STM32F103xE>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:USE_HAL_DRIVER>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:STM32F103xE>"
)

target_include_directories(
    ${TARGET_NAME} PRIVATE
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Core\\Inc>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\STM32F1xx_HAL_Driver\\Inc>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\STM32F1xx_HAL_Driver\\Inc\\Legacy>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\CMSIS\\Device\\ST\\STM32F1xx\\Include>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\CMSIS\\Include>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/FATFS\\Target>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/FATFS\\App>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Middlewares\\Third_Party\\FatFs\\src>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Middlewares\\Third_Party\\FatFs\\src\\drivers>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Core\\Inc>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\STM32F1xx_HAL_Driver\\Inc>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\STM32F1xx_HAL_Driver\\Inc\\Legacy>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\CMSIS\\Device\\ST\\STM32F1xx\\Include>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Drivers\\CMSIS\\Include>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/FATFS\\Target>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/FATFS\\App>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Middlewares\\Third_Party\\FatFs\\src>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:${PROJECT_SOURCE_DIR}/Middlewares\\Third_Party\\FatFs\\src\\drivers>"
)

target_compile_options(
    ${TARGET_NAME} PRIVATE
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:ASM>>:-g3>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g3>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g3>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:ASM>>:-g0>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:-g0>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:CXX>>:-g0>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-Os>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:-Os>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:CXX>>:-Os>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:>"
    "$<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:C>>:>"
    "$<$<AND:$<NOT:$<CONFIG:Debug>>,$<COMPILE_LANGUAGE:CXX>>:>"
    "$<$<CONFIG:Debug>:-mcpu=cortex-m3>"
    "$<$<NOT:$<CONFIG:Debug>>:-mcpu=cortex-m3>"
)

target_link_libraries(
    ${TARGET_NAME} PRIVATE
)

target_link_directories(
    ${TARGET_NAME} PRIVATE
)

target_link_options(
    ${TARGET_NAME} PRIVATE
    "$<$<CONFIG:Debug>:-mcpu=cortex-m3>"
    "$<$<NOT:$<CONFIG:Debug>>:-mcpu=cortex-m3>"
    -T
    "$<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/STM32F103RCTX_FLASH.ld>"
    "$<$<NOT:$<CONFIG:Debug>>:${PROJECT_SOURCE_DIR}/STM32F103RCTX_FLASH.ld>"
)

target_sources(
    ${TARGET_NAME} PRIVATE
    "Core\\Src\\24cxx.c"
    "Core\\Src\\binaryExpression.c"
    "Core\\Src\\calc.c"
    "Core\\Src\\calcExpression.c"
    "Core\\Src\\ctiic.c"
    "Core\\Src\\delay.c"
    "Core\\Src\\dma.c"
    "Core\\Src\\ft5206.c"
    "Core\\Src\\gpio.c"
    "Core\\Src\\gt9147.c"
    "Core\\Src\\lcd.c"
    "Core\\Src\\main.c"
    "Core\\Src\\myiic.c"
    "Core\\Src\\ott2001a.c"
    "Core\\Src\\rtc.c"
    "Core\\Src\\sdio.c"
    "Core\\Src\\solveEquation.c"
    "Core\\Src\\stack.c"
    "Core\\Src\\stm32f1xx_hal_msp.c"
    "Core\\Src\\stm32f1xx_it.c"
    "Core\\Src\\sys.c"
    "Core\\Src\\syscalls.c"
    "Core\\Src\\sysmem.c"
    "Core\\Src\\system_stm32f1xx.c"
    "Core\\Src\\tetris.c"
    "Core\\Src\\tim.c"
    "Core\\Src\\touch.c"
    "Core\\Src\\usart.c"
    "Core\\Src\\utils.c"
    "Core\\Startup\\startup_stm32f103rctx.s"
    "FATFS\\App\\fatfs.c"
    "FATFS\\Target\\bsp_driver_sd.c"
    "FATFS\\Target\\fatfs_platform.c"
    "Middlewares\\Third_Party\\FatFs\\src\\diskio.c"
    "Middlewares\\Third_Party\\FatFs\\src\\drivers\\sd_diskio.c"
    "Middlewares\\Third_Party\\FatFs\\src\\ff_gen_drv.c"
    "Middlewares\\Third_Party\\FatFs\\src\\ff.c"
    "Middlewares\\Third_Party\\FatFs\\src\\option\\syscall.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_cortex.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_dma.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_exti.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_flash_ex.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_flash.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_gpio_ex.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_gpio.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_pwr.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_rcc_ex.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_rcc.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_rtc_ex.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_rtc.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_sd.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_tim_ex.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_tim.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal_uart.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_hal.c"
    "Drivers\\STM32F1xx_HAL_Driver\\Src\\stm32f1xx_ll_sdmmc.c"
    "calc\\binaryExpression.c"
    "calc\\calc.c"
    "calc\\calcExpression.c"
    "calc\\solveEquation.c"
    "calc\\stack.c"
)

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET_NAME}>
)

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex
    $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.hex
)

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
    $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.bin
)

endfunction()