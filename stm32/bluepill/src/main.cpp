#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_gpio.h"

#include <TFT_eSPI.h>
#include <stdio.h>

#include "stm32_id.h"

#define LED_GPIO_PORT             GPIOC
#define LED_GPIO_PIN              GPIO_PIN_13  


const uint16_t AMOUNT_OF_DATA_1 = 1;

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library


void LED_Init(void);
  void SystemClock_Config(void);

extern "C" {
  // HAL callbacks and interrupt handlers
  void Error_Handler(void);
  void SysTick_Handler(void);
  void NMI_Handler(void);
  void HardFault_Handler(void);
  void MemManage_Handler(void);
  void BusFault_Handler(void);
  void UsageFault_Handler(void);
  void SVC_Handler(void);
  void DebugMon_Handler(void);
  void PendSV_Handler(void);
}

int main(void)
{
  STM32_UniqueId_Parsed myParsedID;

  HAL_Init();
  SystemClock_Config();
  LED_Init();

  getParsedSTM32UniqueID(&myParsedID);

  tft.init();

  tft.fillScreen(TFT_BLACK);
  // tft.drawRect(0, 0, tft.width(), tft.height(), TFT_RED);
  tft.drawSmoothRoundRect(0, 0, 8, 6, tft.width(), tft.height(), TFT_RED, TFT_BLACK);
  tft.setRotation(1);

  //tft.fillScreen(TFT_DARKGREY);
  //tft.setTextFont(2);

  //tft.fillRectHGradient(0, 0, 160, 50, TFT_MAGENTA, TFT_BLUE);
  //tft.fillRectVGradient(0, 50, 160, 50, TFT_ORANGE, TFT_RED);

  tft.setTextColor(TFT_VIOLET, TFT_BLACK);

  //tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 6);
  //tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 6);

  ///tft.drawString(myParsedID.wafer_xy_coordinates, 20, 5, 4);
  ///tft.drawString(myParsedID.lot_number_ascii, 20, 30, 4);
  ///tft.drawNumber(myParsedID.wafer_number, 20, 60, 4);

/*   uint32_t uid_w0 = HAL_GetUIDw0(); // Corresponds to UID[31:0]
  uint32_t uid_w1 = HAL_GetUIDw1(); // Corresponds to UID[63:32]
  uint32_t uid_w2 = HAL_GetUIDw2(); // Corresponds to UID[95:64]
  char s_uid_w0[9]; // 8 hex chars + null terminator
  char s_uid_w1[9];
  char s_uid_w2[9];
  snprintf(s_uid_w0, sizeof(s_uid_w0), "%08lX", uid_w0);
  snprintf(s_uid_w1, sizeof(s_uid_w1), "%08lX", uid_w1);
  snprintf(s_uid_w2, sizeof(s_uid_w2), "%08lX", uid_w2);
  tft.drawString(s_uid_w0, 10, 10, 2); // x, y, font
  tft.drawString(s_uid_w1, 10, 30, 2);
  tft.drawString(s_uid_w2, 10, 50, 2); */
  //tft.drawString("abcdefghijklmnopqrstuvw", 0, 48, 6);
  //int xpos = 0;
  //xpos += tft.drawString("xyz{|}~", 0, 64, 2);
  //tft.drawChar(127, xpos, 64, 2);

  //tft.drawSmoothArc(80, 40, 25, 20, 0, 200, TFT_YELLOW, TFT_BLACK, true);

  tft.setCursor(0, 0, 2);
  tft.println("Hello World!");
  
  // Set the font colour to be yellow with no background, set to font 7
  tft.setTextColor(TFT_YELLOW); tft.setTextFont(2);
  tft.println(1234.56);
  
  // Set the font colour to be red with black background, set to font 4
  // tft.setTextColor(TFT_RED,TFT_BLACK);    tft.setTextFont(4);
  // tft.println((long)3735928559, HEX); // Should print DEADBEEF

  // // Set the font colour to be green with black background, set to font 2
  // tft.setTextColor(TFT_GREEN,TFT_BLACK);
  // tft.setTextFont(2);
  // tft.println("Groop");

  // Test some print formatting functions
  float fnumber = 123.45;
   // Set the font colour to be blue with no background, set to font 2
  tft.setTextColor(TFT_BLUE);    tft.setTextFont(2);
  tft.print("Float = "); tft.println(fnumber);           // Print floating point number
  tft.print("Binary = "); tft.println((int)fnumber, BIN); // Print as integer value in binary
  tft.print("Hexadecimal = "); tft.println((int)fnumber, HEX); // Print as integer number in Hexadecimal



  while (true)
  {
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
    HAL_Delay(500);
  }
}

// --- System Clock Configuration ---
//extern "C" 
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Configure HSE (External High-Speed Oscillator) 8MHz
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

void LED_Init()
{
  /* Configure LED pin */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE(); // For LED
  GPIO_InitStruct.Pin = LED_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET); // LED OFF initially
}

extern "C" void Error_Handler(void)
{  __disable_irq();
    while (1) {}
}

// --- Standard HAL Callbacks (can be empty if not used) ---
extern "C" void SysTick_Handler(void)
{
  HAL_IncTick();
}

extern "C" void NMI_Handler(void)        {}
extern "C" void HardFault_Handler(void)  { while (1) {} }
extern "C" void MemManage_Handler(void)  { while (1) {} }
extern "C" void BusFault_Handler(void)   { while (1) {} }
extern "C" void UsageFault_Handler(void) { while (1) {} }
extern "C" void SVC_Handler(void)        {}
extern "C" void DebugMon_Handler(void)   {}
extern "C" void PendSV_Handler(void)     {}
