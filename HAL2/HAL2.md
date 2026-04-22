# HAL2 Drivers
## Important links

> Landing page

> Getting started with STM32Cube ecosystem: [https://stm32cubedocs-dev.st.com/stm32cube-docs/stm32cube-getting-started/1.1.1/en/index.html](https://stm32cubedocs-dev.st.com/stm32cube-docs/stm32cube-getting-started/1.1.1/en/index.html)



> STM32Cube Embedded SW online documentation: [https://dev.st.com/stm32cube-docs/embedded-software/2.0.0/en/index.html](https://dev.st.com/stm32cube-docs/embedded-software/2.0.0/en/index.html)

> STM32 HAL/LL Drivers Documentation: [https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/2.0.0/en/index.html](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/2.0.0/en/index.html)

> Configure your STM32Cube SW package: [STM32 Package Creator](https://dev.st.com/stm32pc/)

> Download specific example: [STM32 Example Library](https://dev.st.com/stm32-example-library/)

> GitHub repository for STM32Cube embedded SW [STMicroelectronics GitHub](https://github.com/stmicroelectronics) ([STM32CubeC5](https://github.com/STMicroelectronics/STM32CubeC5))

## Rationale behind HAL2

Standard Periph. Lib. (**SPL**) [2007] ----> **HAL**(1) [2014] ----> **HAL2** [2026]

It is the evolution of the HAL1-based drivers (the same programming model and API style).

Resolve major known issues of HAL"1" which means:
- Improve performance & footprint
- Reinforce readability & conformity to C standards​ -> Higher quality
- Increase portability across STM32 lines
- Enhance integration with RTOSes
- Enhanced documentation
- Now CMSIS startup is in C – allover by ARM
- HAL2 use natively LL inside function​

## HAL2 use natively LL inside function
[more details here](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/2.0.0/en/docs/overview/hal_ll_layered_drivers.html#hal-and-ll-layered-drivers)

HAL2 services now exclusively call LL functions instead of direct register access, improving driver maintenance and upgrades (the LL was developed after the introduction of the HAL). Although some exceptions exists like CAN FD driver which doesn't use LL driver.

<img src="imgs/HAL2Block.png" width="500"/>

HAL1 vs HAL2 - GPIO Initialization example:

![](imgs/HAL1vsHal2.png)
HAL**1** - **LL** - GPIO Initialization example:
```cpp
__STATIC_INLINE void LL_GPIO_SetAFPin_0_7(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Alternate)
{
  MODIFY_REG(GPIOx->AFR[0], (GPIO_AFRL_AFSEL0 << (POSITION_VAL(Pin) * GPIO_AFRL_AFSEL1_Pos)),
             (Alternate << (POSITION_VAL(Pin) * GPIO_AFRL_AFSEL1_Pos)));
}
__STATIC_INLINE void LL_GPIO_SetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
{
  MODIFY_REG(GPIOx->MODER, (GPIO_MODER_MODE0 << (POSITION_VAL(Pin) * GPIO_MODER_MODE1_Pos)),
             (Mode << (POSITION_VAL(Pin) * GPIO_MODER_MODE1_Pos)));
}
```

HAL**2** - **LL** - GPIO Initialization example: 
```cpp
__STATIC_INLINE void LL_GPIO_SetAFPin_0_7(GPIO_TypeDef *gpiox, uint32_t pin, uint32_t alternate)
{
  STM32_ATOMIC_MODIFY_REG_32(gpiox->AFR[0], (GPIO_AFRL_AFSEL0 << (STM32_POSITION_VAL(pin) * 4U)),
                             (alternate << (STM32_POSITION_VAL(pin) * 4U)));
}
__STATIC_INLINE void LL_GPIO_SetPinSpeed(GPIO_TypeDef *gpiox, uint32_t pin, uint32_t  speed)
{
  STM32_ATOMIC_MODIFY_REG_32(gpiox->OSPEEDR, (GPIO_OSPEEDR_OSPEED0 << (STM32_POSITION_VAL(pin) * 2U)),
                             (speed << (STM32_POSITION_VAL(pin) * 2U)));
}

```
## Init vs SetConfig
[more details here](https://dev.st.com/stm32cube-docs/hal1-to-hal2-migration/1.0.0/en/docs/markup/drivers_documentation/breaking_concepts/breaking_concepts_concept_A.html)
- HAL_..._Init() - initializes handler structure and link instance - big difference in HAL1 - no more the same function.
- HAL_..._SetConfig() - configures peripheral registers
- separated configuration structures to save memory, faster. Not one heavy structure, not all features needed (save extra RAM footprint).
  - global configuration (e.g., UART)
  - additional sub-block configuration (e.g., TIM requires a global configuration and may also require at least one channel configuration if used in output or input compare mode) see [here](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/2.0.0/en/docs/overview/hal_data_structure_and_types.html#hal-configuration-structures)

HAL1 MX_USART1_UART_Init() example:
```cpp
UART_HandleTypeDef huart1;  // global handler

static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;   // <---- USART peripheral instance
  huart1.Init.BaudRate = 115200;
  ...
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

// GPIO configuration in separate _msp file
```
HAL2 mx_usart1_uart_init() example:
```cpp
static hal_uart_handle_t hUSART1;  // static handler

hal_uart_handle_t *mx_usart1_uart_init(void)
{
  hal_uart_config_t uart_config;

  /* Basic configuration */
  if (HAL_UART_Init(&hUSART1, HAL_UART1) != HAL_OK)  // <---- Initialize handler structure (and link USART peripheral instance)
  {
    return NULL;
  }

  HAL_RCC_USART1_EnableClock();

  if (HAL_RCC_USART1_SetKernelClkSource(HAL_RCC_USART1_CLK_SRC_PCLK2) != HAL_OK)
  {
    return NULL;
  }

  uart_config.baud_rate = 115200;
  uart_config.clock_prescaler = HAL_UART_PRESCALER_DIV1;
  ...
  uart_config.one_bit_sampling = HAL_UART_ONE_BIT_SAMPLE_DISABLE;

  if (HAL_UART_SetConfig(&hUSART1, &uart_config) != HAL_OK)  // <---- Set config - configure peripheral registers
  {
    return NULL;
  }

// GPIO config in this function body...
...

}
```

## Better code handling and debuging
[more details here](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/2.0.0/en/docs/overview/hal_data_structure_and_types.html#hal-parameter-types)

thanks to enumeration types:
- HAL1 uses macros for configuration values.
- HAL2 uses enumeration types instead.

(This applies for HAL layer only, LL remains using defines)

![](imgs/HAL1macroHAL2enum.png)

## *xxxx_MspInit()* or *xxxx_hal_msp.c* not used any more
Weak MSP initialization function are not used anymore and xxxx_hal_msp.c file is not generated.

All the initialization including GPIO, clocks... is in the **dedicated mx_ .c file**

The mx_ .c file also contains interrupt handler.

![](imgs/sources.png)

![](imgs/msp.png)

## No user sections in the code
This detail is related to code generation.
MX2 never touch files outside **generated/** folder.

## User data feature
[more details here](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/2.0.0/en/docs/overview/hal_drivers_apis.html#setting-and-getting-user-data)

Once the feature is enabled, it allows to associate user object (variable, structure,...) with peripheral handle using *HAL_PPP_SetUserData(...)* and *HAL_PPP_GetUserData(...)* functions.

![](imgs/prjSettings.png)

![](imgs/commonSettings.png)

![](imgs/UartSettings.png)
## No global handles
Each peripheral handle is static, defined in the dedicated mx_ .c file. The handle can be accessed using getter function.

example for mx_usart1.c:
```cpp
/* Handle for UART */
static hal_uart_handle_t hUSART1;
...
hal_uart_handle_t *mx_usart1_uart_gethandle(void)
{
  return &hUSART1;
}
```
## Improved footprint
HAL vs HAL2 drivers comparision

Three project generated by CubeMX/CubeMX2 (STM32C0, STM32H5, STM32C5):
- USART1 asynchronous mode
- TIM1 CH1 PWM

- Comparing only HAL drivers, not application sources.
- Linker file adjusted to have the same amount for flash and SRAM memory for better comparison in build output.

![](imgs/HAL2_comparison_buildAnalyzer.png)

More details:

![](imgs/HAL2_comparison.png)