#include <stdbool.h>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_spi.h"

// --- Global SPI Handle ---
SPI_HandleTypeDef hspi1;

#define LED_GPIO_PORT             GPIOC
#define LED_GPIO_PIN              GPIO_PIN_13

#define DISPLAY_RESET_GPIO_PORT   GPIOB
#define DISPLAY_RESET_GPIO_PIN    GPIO_PIN_6

#define DISPLAY_CS_GPIO_PORT      GPIOB
#define DISPLAY_CS_GPIO_PIN       GPIO_PIN_7

#define DISPLAY_DC_GPIO_PORT      GPIOB
#define DISPLAY_DC_GPIO_PIN       GPIO_PIN_4

#define DISPLAY_CMD_MODE()    HAL_GPIO_WritePin(DISPLAY_DC_GPIO_PORT, DISPLAY_DC_GPIO_PIN, GPIO_PIN_RESET) // DC low for Command
#define DISPLAY_DATA_MODE()   HAL_GPIO_WritePin(DISPLAY_DC_GPIO_PORT, DISPLAY_DC_GPIO_PIN, GPIO_PIN_SET)   // DC high for Data

#define DISPLAY_SELECT_CHIP() HAL_GPIO_WritePin(DISPLAY_CS_GPIO_PORT, DISPLAY_CS_GPIO_PIN, GPIO_PIN_RESET) // CS low to Select
#define DISPLAY_DESELECT_CHIP() HAL_GPIO_WritePin(DISPLAY_CS_GPIO_PORT, DISPLAY_CS_GPIO_PIN, GPIO_PIN_SET)   // CS high to Deselect

// --- Common ST7735 Commands and Data/Command macros ---
#define ST7735_NOP           0x00 // No Operation
#define ST7735_SWRESET       0x01 // Software Reset
#define ST7735_SLPOUT        0x11 // Sleep Out
#define ST7735_DISPON        0x29 // Display On
#define ST7735_RAMWR         0x2C // RAM Write
#define ST7735_CASET         0x2A // Column Address Set
#define ST7735_RASET         0x2B // Row Address Set
#define ST7735_MADCTL        0x36 // Memory Access Control (for orientation)
#define ST7735_COLMOD        0x3A // Color Mode (pixel format)

// --- Color Definitions (RGB565 format) ---
// RGB565: 5 bits Red, 6 bits Green, 5 bits Blue
#define ST7735_COLOR_RED        0xF800 // Full Red      (11111 000000 00000)
#define ST7735_COLOR_GREEN      0x07E0 // Full Green    (00000 111111 00000)
#define ST7735_COLOR_BLUE       0x001F // Full Blue     (00000 000000 11111)
#define ST7735_COLOR_WHITE      0xFFFF // White
#define ST7735_COLOR_BLACK      0x0000 // Black

#define ST7735_COLOR_YELLOW     0xFFE0 // Red + Green   (11111 111111 00000)
#define ST7735_COLOR_CYAN       0x07FF // Green + Blue  (00000 111111 11111)
#define ST7735_COLOR_MAGENTA    0xF81F // Red + Blue    (11111 000000 11111)
#define ST7735_COLOR_ORANGE     0xFD20 // A mix of red and some green

#define ST7735_COLOR_PURPLE     0x8010 // Darker Red + Darker Blue
#define ST7735_COLOR_PINK       0xFC1F // Lighter Magenta

#define ST7735_COLOR_GREY       0x8410 // A medium grey
#define ST7735_COLOR_LIGHTGREY  0xC618 // Lighter grey
#define ST7735_COLOR_DARKGREY   0x4208 // Darker grey

#define ST7735_COLOR_BROWN      0xA145 // A shade of brown
#define ST7735_COLOR_MAROON     0x7800 // Dark Red
#define ST7735_COLOR_NAVY       0x000F // Dark Blue
#define ST7735_COLOR_LIME       0x07C0 // Brighter Green
#define ST7735_COLOR_TEAL       0x0410 // Greenish Blue
#define ST7735_COLOR_OLIVE      0x7BE0 // Dark Yellow/Green
#define ST7735_COLOR_INDIGO     0x4810 // Darker Blue/Purple

// --- Display Dimensions (Adjust for your specific ST7735 display, e.g., 128x160 or 128x128) ---
#define ST7735_WIDTH          160   // i want a landscape orientation
#define ST7735_HEIGHT         80
#define ST7735_OFFSET_X       0   // Common for 160x80 panels
#define ST7735_OFFSET_Y       24    

const uint16_t AMOUNT_OF_DATA_1 = 1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
void Error_Handler(void);

void Display_WriteCommand(uint8_t cmd);
void Display_WriteData(uint8_t data);
void Display_WriteMultiData(uint8_t *data, uint16_t len);
void Display_Init(void);
void Display_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void Display_FillScreen(uint16_t color);

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();   // Initialize GPIOs (including LED and SPI control if needed)
  MX_SPI1_Init();   // Initialize alternative pins SPI1
  Display_Init();         // Initialize the ST7735 display itself

  // Data to transmit
  uint8_t tx_data = 0xA5;

  // --- Define an array of colors to cycle through ---
  const uint16_t color_sequence[] = {
    ST7735_COLOR_RED,
    ST7735_COLOR_GREEN,
    ST7735_COLOR_BLUE,
    ST7735_COLOR_WHITE,
    ST7735_COLOR_BLACK,
    ST7735_COLOR_YELLOW,
    ST7735_COLOR_CYAN,
    ST7735_COLOR_MAGENTA,
    ST7735_COLOR_ORANGE,
    ST7735_COLOR_PURPLE,
    ST7735_COLOR_PINK,
    ST7735_COLOR_GREY,
    ST7735_COLOR_LIGHTGREY,
    ST7735_COLOR_DARKGREY,
    ST7735_COLOR_BROWN,
    ST7735_COLOR_MAROON,
    ST7735_COLOR_NAVY,
    ST7735_COLOR_LIME,
    ST7735_COLOR_TEAL,
    ST7735_COLOR_OLIVE,
    ST7735_COLOR_INDIGO
};
  // Calculate the number of colors in the array
  const uint32_t num_colors = sizeof(color_sequence) / sizeof(color_sequence[0]);
  uint32_t current_color_index = 0; // Index to keep track of the current color


  while (true)
  {

    // Toggle LED to show the program is running
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);


    //HAL_GPIO_WritePin(DISPLAY_CS_GPIO_PORT, DISPLAY_CS_GPIO_PIN, GPIO_PIN_RESET);
    //HAL_SPI_Transmit(&hspi1, &tx_data, AMOUNT_OF_DATA_1, HAL_MAX_DELAY);                // HAL_MAX_DELAY: Timeout value (infinite wait)
    //HAL_GPIO_WritePin(DISPLAY_CS_GPIO_PORT, DISPLAY_CS_GPIO_PIN, GPIO_PIN_SET);
    // Display_Init();
    // HAL_Delay(10);

    Display_FillScreen(color_sequence[current_color_index]);
    current_color_index++;
    if (current_color_index >= num_colors)
    {
      current_color_index = 0;
    }

    //HAL_Delay(100);
  }
}

// --- System Clock Configuration ---
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Configure HSE (External High-Speed Oscillator)
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1; // HSE / 1 = 8MHz (assuming 8MHz crystal)
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;             // Multiply HSE (8MHz) by 6 to get 48MHz

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  // Configure CPU, AHB and APB clocks
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // System clock source is PLL (48MHz)
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        // HCLK = 48MHz / 1 = 48MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;         // PCLK1 = 48MHz / 2 = 24MHz (Max 36MHz for F103)
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;         // PCLK2 = 48MHz / 1 = 48MHz (Max 72MHz for F103)

  // Flash Latency: 1 wait state for 48 MHz
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  // Re-initialize SysTick for HAL_Delay
  HAL_InitTick(15U); // Default priority
}

// --- GPIO Initialization ---
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE(); // For LED
  __HAL_RCC_GPIOB_CLK_ENABLE(); // For SPI (PB3, PB5)
  //__HAL_RCC_GPIOA_CLK_ENABLE(); 

  /* Configure LED pin Output Level (assuming active low for common PC13 LEDs) */


  /* Configure LED pin */
  GPIO_InitStruct.Pin = LED_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET); // LED OFF initially

  /* Configure a general purpose output for software NSS (Chip Select) */
  /* This pin (PB0) is not connected to the SPI peripheral directly
     when NSS is software-controlled, but it's good practice to configure
     it as an output if you plan to manually control CS.
     For this simple example, we don't control it in the loop,
     but it's part of a proper SPI setup. */
  GPIO_InitStruct.Pin =  DISPLAY_CS_GPIO_PIN | DISPLAY_RESET_GPIO_PIN | DISPLAY_DC_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DISPLAY_CS_GPIO_PORT, &GPIO_InitStruct);

  DISPLAY_DESELECT_CHIP();  // Keep CS high (inactive)
  HAL_GPIO_WritePin(DISPLAY_RESET_GPIO_PORT, DISPLAY_RESET_GPIO_PIN, GPIO_PIN_SET);
  DISPLAY_CMD_MODE();
}

// --- SPI1 Initialization ---
static void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES; // Standard 2-line mode
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;   // CPOL = 0 (Clock idle low)
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;      // CPHA = 0 (Data sampled on leading edge)
  hspi1.Init.NSS = SPI_NSS_SOFT;             // Software Chip Select (controlled manually or by user application)
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; // 48MHz / 256 = 187.5 kHz
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7; // Default value, ignored when CRCCalculation is disabled

  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}

// --- SPI MSP Initialization (Handles low-level peripheral and GPIO setup) ---
// This function is automatically called by HAL_SPI_Init()
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hspi->Instance == SPI1)
  {
    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable GPIOB clock for PB3, PB5
    __HAL_RCC_AFIO_CLK_ENABLE();  // CRITICAL: Enable AFIO clock for pin remapping
    __HAL_AFIO_REMAP_SWJ_NOJTAG();  // IMPORTANT this command be after   __HAL_AFIO_REMAP_SWJ_NOJTAG();

    /* Enable SPI1 Remap for PB3 (SCK), PB4 (MISO - not used in this example), PB5 (MOSI) */
    __HAL_AFIO_REMAP_SPI1_ENABLE();

    /**SPI1 GPIO Configuration (Remapped)
    PB3     ------> SPI1_SCK
    PB5     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_5; // PB3 (SCK) and PB5 (MOSI)
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;       // Alternate Function Push Pull
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* SPI1 interrupt Init (optional for transmit-only, but good practice)*/
    HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0); // Set to highest priority (no preemption, no subpriority)
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
  }
}


// Function to send a command to ST7735
void Display_WriteCommand(uint8_t cmd)
{
  DISPLAY_CMD_MODE(); // DC low for command
  DISPLAY_SELECT_CHIP(); // CS low to select
  HAL_SPI_Transmit(&hspi1, &cmd, AMOUNT_OF_DATA_1, HAL_MAX_DELAY); // Use hspi1 now
  DISPLAY_DESELECT_CHIP(); // CS high to deselect
}

// Function to send data to ST7735
void Display_WriteData(uint8_t data)
{
  DISPLAY_DATA_MODE(); // DC high for data
  DISPLAY_SELECT_CHIP(); // CS low to select
  HAL_SPI_Transmit(&hspi1, &data, AMOUNT_OF_DATA_1, HAL_MAX_DELAY); // Use hspi1 now
  DISPLAY_DESELECT_CHIP(); // CS high to deselect
}

// Function to send multiple bytes of data
void Display_WriteMultiData(uint8_t *data, uint16_t len)
{
  DISPLAY_DATA_MODE(); // DC high for data
  DISPLAY_SELECT_CHIP(); // CS low to select
  HAL_SPI_Transmit(&hspi1, data, len, HAL_MAX_DELAY); // Use hspi1 now
  DISPLAY_DESELECT_CHIP(); // CS high to deselect
}

// Set drawing window on the display
void Display_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{

    x0 += ST7735_OFFSET_X;
    x1 += ST7735_OFFSET_X;
    y0 += ST7735_OFFSET_Y;
    y1 += ST7735_OFFSET_Y;
    
    // Column address set (CASET)
    Display_WriteCommand(ST7735_CASET);
    uint8_t data_c[4];
    data_c[0] = (x0 >> 8) & 0xFF; // X start high byte
    data_c[1] = x0 & 0xFF;       // X start low byte
    data_c[2] = (x1 >> 8) & 0xFF; // X end high byte
    data_c[3] = x1 & 0xFF;       // X end low byte
    Display_WriteMultiData(data_c, 4);

    // Row address set (RASET)
    Display_WriteCommand(ST7735_RASET);
    uint8_t data_r[4];
    data_r[0] = (y0 >> 8) & 0xFF; // Y start high byte
    data_r[1] = y0 & 0xFF;       // Y start low byte
    data_r[2] = (y1 >> 8) & 0xFF; // Y end high byte
    data_r[3] = y1 & 0xFF;       // Y end low byte
    Display_WriteMultiData(data_r, 4);

    Display_WriteCommand(ST7735_RAMWR); // RAM Write
}

// Fill the entire screen with a specific color
void Display_FillScreen(uint16_t color)
{
    // Set the drawing window to the full display size
    Display_SetAddressWindow(0, 0, ST7735_WIDTH - 1, ST7735_HEIGHT - 1);

    // Prepare the 16-bit color data (high byte, then low byte)
    uint8_t color_bytes[2];
    color_bytes[0] = (color >> 8) & 0xFF; // High byte
    color_bytes[1] = color & 0xFF;        // Low byte

    // Transmit color data for every pixel
    // Each pixel is 2 bytes (16-bit color)
    for (uint32_t i = 0; i < (uint32_t)ST7735_WIDTH * ST7735_HEIGHT; i++)
    {
        Display_WriteMultiData(color_bytes, 2); // Send 2 bytes for each pixel
    }
}


// Basic ST7735 Initialization Sequence (simplified)
void Display_Init(void)
{
  // 1. Hardware Reset
  HAL_GPIO_WritePin(DISPLAY_RESET_GPIO_PORT, DISPLAY_RESET_GPIO_PIN, GPIO_PIN_RESET); // RST low
  HAL_Delay(100); // Hold low for a bit
  HAL_GPIO_WritePin(DISPLAY_RESET_GPIO_PORT, DISPLAY_RESET_GPIO_PIN, GPIO_PIN_SET);   // RST high
  HAL_Delay(100); // Wait for reset to complete

  // 2. Software Reset (Optional, but good practice)
  Display_WriteCommand(ST7735_SWRESET);
  HAL_Delay(150); // Wait for software reset to complete

  // 3. Sleep Out
  Display_WriteCommand(ST7735_SLPOUT);
  HAL_Delay(150); // Wait for oscillator to stabilize

  // 4. Color Mode (set to 16-bit color)
  Display_WriteCommand(ST7735_COLMOD);
  Display_WriteData(0x05); // 0x05 for RGB565 (16-bit)

  // 5. Memory Access Control (MADCTL - often needed for orientation)
  // For basic operation, 0x00 is common. Adjust for rotation (e.g., 0x40, 0x80, 0xC0)
  Display_WriteCommand(ST7735_MADCTL);
  Display_WriteData(0xA0); // 90 def rotation by Swap X/Y (can be adjusted for 90/180/270 degrees)

  // 6. Power Control (example) - often several commands
  // This varies greatly between ST7735 models. These are common placeholders.
  Display_WriteCommand(0xB1); // Normal (Full) Power Control
  Display_WriteData(0x01);
  Display_WriteData(0x2C);
  Display_WriteData(0x2D);

  Display_WriteCommand(0xB2); // Idle Mode Power Control
  Display_WriteData(0x01);
  Display_WriteData(0x2C);
  Display_WriteData(0x2D);

  Display_WriteCommand(0xB3); // Partial Mode Power Control
  Display_WriteData(0x01);
  Display_WriteData(0x2C);
  Display_WriteData(0x2D);

  Display_WriteCommand(0xB4); // Display Inversion Control
  Display_WriteData(0x03);

  // 7. Gamma Settings (Optional, but improves color accuracy)
  Display_WriteCommand(0xE0); // Positive Gamma Correction
  Display_WriteMultiData((uint8_t[]){0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10}, 16);

  Display_WriteCommand(0xE1); // Negative Gamma Correction
  Display_WriteMultiData((uint8_t[]){0x03, 0x1D, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10}, 16);


  // 8. Display On
  Display_WriteCommand(ST7735_DISPON);
  HAL_Delay(150);
}

void Error_Handler(void)
{  __disable_irq();
    while (1) {}
}

// --- Standard HAL Callbacks (can be empty if not used) ---
void SysTick_Handler(void)
{
  HAL_IncTick();
}

void NMI_Handler(void)       {}
void HardFault_Handler(void)  { while (1) {} }
void MemManage_Handler(void)  { while (1) {} }
void BusFault_Handler(void)   { while (1) {} }
void UsageFault_Handler(void) { while (1) {} }
void SVC_Handler(void)        {}
void DebugMon_Handler(void)   {}
void PendSV_Handler(void)     {}

// If using SPI interrupts for Rx/Tx Cplt/Error, you'd implement SPI1_IRQHandler
// void SPI1_IRQHandler(void)
// {
//   HAL_SPI_IRQHandler(&hspi1);
// }
