/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void Delay_uS(uint16_t i)
{
	static uint32_t j=0, ij=0;
	for (ij=0;ij<i;ij++)
		for (j=0;j<1;j++);
}

void shiftOut(uint8_t command)
{
	bool MSBFIRST = true;
	uint16_t dataPin = GPIO_PIN_6;
	uint16_t clockPin = GPIO_PIN_5;
	for (int i = 0; i < 8; i++)
	{
		bool output = false;
		if (MSBFIRST)
		{
			output = command & 0b10000000;
			command = command << 1;
		}
		else
		{
			output = command & 0b00000001;
			command = command >> 1;
		}
		HAL_GPIO_WritePin(GPIOA, dataPin, output);
		HAL_GPIO_WritePin(GPIOA, clockPin, SET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOA, clockPin, RESET);
		HAL_Delay(1);
	}
}

void writeSevenSegment(char character, bool decimal)
{
	static const char supportedChars[34] = {
		'0',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',
		'a',
		'b',
		'c',
		'd',
		'e',
		'f',
		'g',
		'h',
		'i',
		'j',
		'l',
		'n',
		'o',
		'p',
		'q',
		'r',
		's',
		't',
		'u',
		'y',
		'z',
		'_',
		'-',
		' '
	};

	static const uint8_t displayLEDs[34] = {
		0b10111101,
		0b00010100,
		0b00111011,
		0b10101011,
		0b10000111,
		0b10101110,
		0b10111110,
		0b10001001,
		0b10111111,
		0b10101111,
		0b10011111,
		0b10110110,
		0b00110010,
		0b10110011,
		0b00111110,
		0b00011110,
		0b10101111,
		0b10010111,
		0b10000001,
		0b10110001,
		0b00110100,
		0b10010010,
		0b10110010,
		0b00011111,
		0b10001111,
		0b00010010,
		0b10101110,
		0b00110110,
		0b10110000,
		0b10100111,
		0b00111011,
		0b00100000,
		0b00000010,
		0b00000000
	};

	int charnum = sizeof(supportedChars)/sizeof(supportedChars[0]);
	bool success = false;

	for (int i = 0; i < charnum; i++)
	{
		if (character == supportedChars[i])
		{
			success = true;
			uint8_t returnchar = displayLEDs[i];
			if (decimal)
			{
				returnchar |= 0b01000000;
			}

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, RESET);
			shiftOut(returnchar);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, SET);
			i = charnum;
		}
	}

	if (!success)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, RESET);
		shiftOut(0b11111111);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, SET);
	}


}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  uint8_t startCount = 1;
	  writeSevenSegment(startCount + '0', true);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, SET);
	  for (int i = 0; i < 200; i++)
	  {
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, SET);
		  HAL_Delay(10);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, RESET);
		  HAL_Delay(10);
	  }
	  writeSevenSegment(startCount + '0', false);

	  startCount++;
	  HAL_Delay(1000);

	  writeSevenSegment(startCount + '0', true);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, RESET);
	  for (int i = 0; i < 200; i++)
	  {
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, SET);
		  HAL_Delay(1);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, RESET);
		  HAL_Delay(1);
	  }
	  writeSevenSegment(startCount + '0', false);

	  startCount++;
	  HAL_Delay(1000);

	  writeSevenSegment(startCount + '0', true);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, SET);
	  for (int i = 0; i < 200; i++)
	  {
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, SET);
		  HAL_Delay(1);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, RESET);
	  }
	  writeSevenSegment(startCount + '0', false);

	  startCount++;
	  HAL_Delay(1000);

	  writeSevenSegment(startCount + '0', true);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, RESET);
	  for (int i = 0; i < 200; i++)
	  {
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, SET);
		  HAL_Delay(1);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, RESET);
	  }
	  writeSevenSegment(startCount + '0', false);

	  HAL_Delay(1000);



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5|GPIO_PIN_6 
                          |GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA5 PA6 
                           PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5|GPIO_PIN_6 
                          |GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
