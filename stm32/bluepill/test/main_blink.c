//// file: main.c
#include "main.h"

#include <stdio.h>
#include <string.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI1_Init(void);

UART_HandleTypeDef huart1;
SPI_HandleTypeDef hspi1; // SPI handle for SPI1 (NOT SPI2 anymore)

// GPIO definitions for ST7735 control pins
// Ensure these match your actual board wiring!
#define ST7735_CS_GPIO_Port   GPIOB
#define ST7735_CS_Pin         GPIO_PIN_7 // PB7 for Chip Select

#define ST7735_DC_GPIO_Port   GPIOB
#define ST7735_DC_Pin         GPIO_PIN_4 // PB4 for Data/Command (NEW ASSIGNMENT)

#define ST7735_RST_GPIO_Port  GPIOB
#define ST7735_RST_Pin        GPIO_PIN_6 // PB6 for Reset (NEW ASSIGNMENT)

// Common ST7735 Commands and Data/Command macros
#define ST7735_NOP           0x00 // No Operation
#define ST7735_SWRESET       0x01 // Software Reset
#define ST7735_SLPOUT        0x11 // Sleep Out
#define ST7735_DISPON        0x29 // Display On
#define ST7735_RAMWR         0x2C // RAM Write
#define ST7735_CASET         0x2A // Column Address Set
#define ST7735_RASET         0x2B // Row Address Set

#define ST7735_CMD_MODE()    HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_RESET) // DC low for Command
#define ST7735_DATA_MODE()   HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET)   // DC high for Data

#define ST7735_SELECT_CHIP() HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET) // CS low to Select
#define ST7735_DESELECT_CHIP() HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET)   // CS high to Deselect

void ST7735_WriteCommand(uint8_t cmd);
void ST7735_WriteData(uint8_t data);
void ST7735_WriteMultiData(uint8_t *data, uint16_t len);
void ST7735_Init(void);

int main(void)
{
  char MSG[35] = {'\0'};

  HAL_Init();
  SystemClock_Config();
  MX_USART1_UART_Init();

  MX_GPIO_Init();
  MX_SPI1_Init();        // Initialize SPI1 (NOT SPI2 anymore)
  ST7735_Init();         // Initialize the ST7735 display itself

  LED_G1_ON();
  LED_G2_ON();
  while (1)
  {
    LED_G1_TG();
    LED_G2_TG();
    ST7735_WriteCommand(ST7735_DISPON);
    sprintf(MSG, "Hello Dudes! Tracing %lu\r\n", HAL_GetTick());
    HAL_UART_Transmit(&huart1, (uint8_t*)MSG, strlen(MSG), 100);
    //printf("Hello from STM32! Tick: %lu ms\r\n", HAL_GetTick());
    //fflush(stdout);

    //HAL_Delay(500);
  }
}


void MX_USART1_UART_Init(void) {
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200; // Or your desired baud rate
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK) {
    Error_Handler(); // Your error handling function
  }
}


void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance == USART1)
  {
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX  (Output)
    PA10    ------> USART1_RX  (Input)
    */
    // PA9 as TX
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Alternate Function Push Pull
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // PA10 as RX
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // Input mode for RX
    GPIO_InitStruct.Pull = GPIO_NOPULL;    // No internal pull-up/pull-down (or GPIO_PULLUP)
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  * Configure the internal and external oscillators
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1; // HSE / 1 = 8MHz (assuming 8MHz external crystal)
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;             // Multiply HSE (8MHz) by 6 to get 48MHz

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler(); // Handle initialization error
  }

  /** Initializes the CPU, AHB and APB busses clocks
  * Configure the clock dividers for SYSCLK, HCLK, PCLK1, PCLK2
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // System clock source is PLL (48MHz)
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        // HCLK = 48MHz / 1 = 48MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;         // PCLK1 = 48MHz / 2 = 24MHz (Max 36MHz for F103)
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;         // PCLK2 = 48MHz / 1 = 48MHz (Max 72MHz for F103)

  // Flash Latency: For 48 MHz, 1 wait state (FLASH_LATENCY_1) is sufficient.
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler(); // Handle clock configuration error
  }

  // Re-initialize SysTick for the new clock frequency. This is crucial for HAL_Delay().
  HAL_InitTick(TICK_INT_PRIORITY);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */
 
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
 
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_G2_Port, LED_G2_Pin, GPIO_PIN_RESET);
 
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_G1_Port, LED_G1_Pin, GPIO_PIN_RESET);
  // Configure Output Level for ST7735 control pins (default high for inactive)

  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);   // CS high (inactive)
  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);   // DC high (data, or just default)
  HAL_GPIO_WritePin(ST7735_RST_GPIO_Port, ST7735_RST_Pin, GPIO_PIN_SET); // RST high (inactive)

 
  /*Configure GPIO pin : LED_G2_Pin */
  GPIO_InitStruct.Pin = LED_G2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_G2_Port, &GPIO_InitStruct);
 
  /*Configure GPIO pin : LED_G1_Pin */
  GPIO_InitStruct.Pin = LED_G1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_G1_Port, &GPIO_InitStruct);

  // Configure GPIO pins for ST7735 control signals (CS, DC, RST)
  GPIO_InitStruct.Pin = ST7735_CS_Pin | ST7735_DC_Pin | ST7735_RST_Pin; // Include all three
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;      // Output Push-Pull
  GPIO_InitStruct.Pull = GPIO_NOPULL;              // No Pull-up/down
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;    // High speed for control
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES; // <--- CHANGE THIS: Half-duplex (single line for Tx/Rx)
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32; // 48MHz / 32 = 1.5MHz
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hspi->Instance == SPI1)
  {
    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    // Enable SPI1 Remap for PB3, PB4, PB5
    __HAL_AFIO_REMAP_SPI1_ENABLE();

    // Configure SCK (PB3) and the Bidirectional Data Line (PB5)
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_5; // PB3 (SCK) and PB5 (MOSI/MISO)
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;       // Alternate Function Push Pull for bidirectional
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // PB4 (MISO alternative) is NOT configured here as it's for DC.
  }
}


// Function to send a command to ST7735
void ST7735_WriteCommand(uint8_t cmd)
{
  ST7735_CMD_MODE(); // DC low for command
  ST7735_SELECT_CHIP(); // CS low to select
  HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY); // Use hspi1 now
  ST7735_DESELECT_CHIP(); // CS high to deselect
}

// Function to send data to ST7735
void ST7735_WriteData(uint8_t data)
{
  ST7735_DATA_MODE(); // DC high for data
  ST7735_SELECT_CHIP(); // CS low to select
  HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY); // Use hspi1 now
  ST7735_DESELECT_CHIP(); // CS high to deselect
}

// Function to send multiple bytes of data
void ST7735_WriteMultiData(uint8_t *data, uint16_t len)
{
  ST7735_DATA_MODE(); // DC high for data
  ST7735_SELECT_CHIP(); // CS low to select
  HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY); // Use hspi1 now
  ST7735_DESELECT_CHIP(); // CS high to deselect
}

// Basic ST7735 Initialization Sequence (simplified) - No changes needed here
void ST7735_Init(void)
{
  // 1. Hardware Reset
  HAL_GPIO_WritePin(ST7735_RST_GPIO_Port, ST7735_RST_Pin, GPIO_PIN_RESET); // RST low
  HAL_Delay(100); // Hold low for a bit
  HAL_GPIO_WritePin(ST7735_RST_GPIO_Port, ST7735_RST_Pin, GPIO_PIN_SET);   // RST high
  HAL_Delay(100); // Wait for reset to complete

  // 2. Software Reset (Optional, but good practice)
  ST7735_WriteCommand(ST7735_SWRESET);
  HAL_Delay(150); // Wait for software reset to complete

  // 3. Sleep Out
  ST7735_WriteCommand(ST7735_SLPOUT);
  HAL_Delay(150); // Wait for oscillator to stabilize

  // 4. Power Control (example)
  ST7735_WriteCommand(0xB1); // Normal (Full) Power Control
  ST7735_WriteData(0x01);
  ST7735_WriteData(0x2C);
  ST7735_WriteData(0x2D);

  // 5. Display On
  ST7735_WriteCommand(ST7735_DISPON);
  HAL_Delay(150);

  // You will need to add the full ST7735 initialization sequence here.
  // It's typically a long list of commands and data to set up colors,
  // orientation, gamma, etc. Search for "ST7735 initialization sequence"
  // specific to your display size/color depth.
}

// @brief  Retargets the C library's printf function to the USART1.
// @param  ch: character to be transmitted.
// @retval character transmitted.
// ---
PUTCHAR_PROTOTYPE
{
  // Transmit the character over UART1
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF); // 0xFFFF is a long timeout
  return ch;
}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}
 
#ifdef  USE_FULL_ASSERT
 
void assert_failed(uint8_t *file, uint32_t line)
{
 
}
#endif /* USE_FULL_ASSERT */

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1) {}
}


void MemManage_Handler(void)
{
  while (1) {}
}

void BusFault_Handler(void)
{
  while (1) {}
}

void UsageFault_Handler(void)
{
  while (1) {}
}

void SVC_Handler(void)
{
}


void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}
  