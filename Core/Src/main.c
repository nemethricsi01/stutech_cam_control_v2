/* USER CODE BEGIN Header */
					/**
						******************************************************************************
						* @file					 : main.c
						* @brief					: Main program body
						******************************************************************************
						* @attention
						*
						* <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
						* All rights reserved.</center></h2>
						*
						* This software component is licensed by ST under BSD 3-Clause license,
						* the "License"; You may not use this file except in compliance with the
						* License. You may obtain a copy of the License at:
						*												opensource.org/licenses/BSD-3-Clause
						*
						******************************************************************************
						*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
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
TIM_HandleTypeDef htim17;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
					const uint8_t pwron[6] =	{0x81,0x01,0x04,0x00,0x02,0xFF};//camera power on
					const uint8_t pwroff[6] = {0x81,0x01,0x04,0x00,0x03,0xFF};//camera power off
					const uint8_t cam1preset[7] = {0x81,0x01,0x04,0x3f,0x02,0x00,0xFF};//camera mem recall 01
					const uint8_t cam2preset[7] = {0x81,0x01,0x04,0x3f,0x02,0x01,0xFF};//camera mem recall 02
					const uint8_t cam3preset[7] = {0x81,0x01,0x04,0x3f,0x02,0x02,0xFF};//camera mem recall 03
					const uint8_t cam4preset[7] = {0x81,0x01,0x04,0x3f,0x02,0x03,0xFF};//camera mem recall 04
					const uint8_t autotrackon[7] = {0x81,0x01,0x0B,0x00,0x00,0x02,0xFF};//auto tracking on
					const uint8_t autotrackoff[7] = {0x81,0x01,0x0B,0x00,0x00,0x03,0xFF};//auto tracking off
					const uint8_t powerinquiry[5] = {0x81,0x09,0x04,0x00,0xFF};//camera mem recall 04
					int actgomb[5];
					int lastgomb[5];
					int prelltime = 10;
					int prelltimer = 0;
					uint8_t gomb0prell;
					uint8_t gomb3prell;
					uint8_t uartbuff[20];
					uint8_t uartbuf;
					uint8_t uartcount= 0;
					uint8_t ackflag,cpltflag,poweronflag,poweroffflag = 0;
					uint8_t villtimer = 0;
					int gomb0hosszutime,gomb4hosszutime = 0;
					int pwrtime = 0;
					uint8_t firston = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM17_Init(void);
/* USER CODE BEGIN PFP */
					void readgomb(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
					void readgomb(void)
					{
						actgomb[0] = HAL_GPIO_ReadPin(BUTT_1_GPIO_Port,BUTT_1_Pin);
						actgomb[1] = HAL_GPIO_ReadPin(BUTT_2_GPIO_Port,BUTT_2_Pin);
						actgomb[2] = HAL_GPIO_ReadPin(BUTT_3_GPIO_Port,BUTT_3_Pin);
						actgomb[3] = HAL_GPIO_ReadPin(BUTT_4_GPIO_Port,BUTT_4_Pin);
						actgomb[4] = HAL_GPIO_ReadPin(BUTT_GEN_GPIO_Port,BUTT_GEN_Pin);
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
  MX_USART1_UART_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
						HAL_TIM_Base_Start_IT(&htim17);
						HAL_UART_Receive_IT(&huart1,&uartbuf,1);
						readgomb();
						lastgomb[0] = actgomb[0];
						lastgomb[1] = actgomb[1];
						lastgomb[2] = actgomb[2];
						lastgomb[3] = actgomb[3];
						lastgomb[4] = actgomb[4];

						//bekapcs villogas
						HAL_GPIO_WritePin(PWR_LED_GPIO_Port,PWR_LED_Pin,GPIO_PIN_SET);
						HAL_Delay(200);
						HAL_GPIO_WritePin(PWR_LED_GPIO_Port,PWR_LED_Pin,GPIO_PIN_RESET);
						HAL_Delay(200);
						HAL_GPIO_WritePin(PWR_LED_GPIO_Port,PWR_LED_Pin,GPIO_PIN_SET);
						HAL_Delay(200);
						HAL_GPIO_WritePin(PWR_LED_GPIO_Port,PWR_LED_Pin,GPIO_PIN_RESET);
						HAL_Delay(200);
						HAL_GPIO_WritePin(PWR_LED_GPIO_Port,PWR_LED_Pin,GPIO_PIN_SET);
						HAL_Delay(200);
						HAL_GPIO_WritePin(PWR_LED_GPIO_Port,PWR_LED_Pin,GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
						while (1)
						{
							if((poweronflag == 1)&&(villtimer == 0))
							{
								HAL_GPIO_WritePin(PWR_LED_GPIO_Port,PWR_LED_Pin,GPIO_PIN_SET);
								if(firston == 1)
								{
									HAL_UART_Transmit(&huart1,autotrackon,7,100);
									firston = 0;
								}

							}
							if((poweroffflag == 1)&&(villtimer == 0))
							{
							HAL_GPIO_WritePin(PWR_LED_GPIO_Port,PWR_LED_Pin,GPIO_PIN_RESET);
							firston = 1;
							}


							readgomb();
							if((lastgomb[0] != actgomb[0])||
								 (lastgomb[1] != actgomb[1])||
								 (lastgomb[2] != actgomb[2])||
								 (lastgomb[3] != actgomb[3])||
								 (lastgomb[4] != actgomb[4]))
									{
										prelltimer = prelltime;
										while(prelltimer >0)
										{
											readgomb();
											if(lastgomb[0] != actgomb[0])
											{
												prelltimer = prelltime;
												lastgomb[0] = actgomb[0];
											}
											if(lastgomb[1] != actgomb[1])
											{
												prelltimer = prelltime;
												lastgomb[1] = actgomb[1];
											}
											if(lastgomb[2] != actgomb[2])
											{
												prelltimer = prelltime;
												lastgomb[2] = actgomb[2];
											}
											if(lastgomb[3] != actgomb[3])
											{
												prelltimer = prelltime;
												lastgomb[3] = actgomb[3];
											}
											if(lastgomb[4] != actgomb[4])
											{
												prelltimer = prelltime;
												lastgomb[4] = actgomb[4];
											}

										}
											if(lastgomb[0] == 0)//gomb1
											{
												HAL_UART_Transmit(&huart1,autotrackoff,7,100);
												HAL_UART_Transmit(&huart1,cam2preset,7,100);
												HAL_UART_Transmit(&huart1,powerinquiry,5,100);
												villtimer = 200;
											}
											if(lastgomb[1] == 0)//gomb2
											{
												HAL_UART_Transmit(&huart1,cam3preset,7,100);
												HAL_UART_Transmit(&huart1,powerinquiry,5,100);
												villtimer = 200;
											}
											if(lastgomb[2] == 0)//gomb3
											{
												HAL_UART_Transmit(&huart1,cam4preset,7,100);
												HAL_UART_Transmit(&huart1,powerinquiry,5,100);
												villtimer = 200;
											}
											if(lastgomb[3] == 0)//gomb4
											{
												HAL_UART_Transmit(&huart1,autotrackon,7,100);
												HAL_UART_Transmit(&huart1,powerinquiry,5,100);
												villtimer = 200;
											}
											if(lastgomb[4] == 0)//powergomb
											{

											}
									}

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM17_Init(void)
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 10;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 4469;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM17_Init 2 */

  /* USER CODE END TIM17_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PWR_LED_GPIO_Port, PWR_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BUTT_2_Pin BUTT_3_Pin BUTT_4_Pin BUTT_GEN_Pin */
  GPIO_InitStruct.Pin = BUTT_2_Pin|BUTT_3_Pin|BUTT_4_Pin|BUTT_GEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTT_1_Pin */
  GPIO_InitStruct.Pin = BUTT_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTT_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PWR_LED_Pin */
  GPIO_InitStruct.Pin = PWR_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PWR_LED_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
					void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
					{

					if(prelltimer >0)
					{
						prelltimer--;
					}
					if(villtimer >0)
					{
						villtimer--;
						if(poweronflag == 1)
						{
							HAL_GPIO_WritePin(PWR_LED_GPIO_Port,PWR_LED_Pin,GPIO_PIN_RESET);//led off
							if(villtimer == 0)
							{
								HAL_GPIO_WritePin(PWR_LED_GPIO_Port,PWR_LED_Pin,GPIO_PIN_SET);//led on
							}
						}
						if(poweroffflag == 1)
						{
							HAL_GPIO_WritePin(PWR_LED_GPIO_Port,PWR_LED_Pin,GPIO_PIN_SET);//led on
							if(villtimer == 0)
							{
								HAL_GPIO_WritePin(PWR_LED_GPIO_Port,PWR_LED_Pin,GPIO_PIN_RESET);//led off
							}
						}
					}
					if(actgomb[0] == 0)
					{
						gomb0hosszutime++;
						if(gomb0hosszutime > 3000)
						{
							HAL_UART_Transmit(&huart1,pwron,6,100);
							pwrtime = 100;
							poweronflag = 1;
							poweroffflag = 0;
							gomb0hosszutime = 0;
							HAL_UART_Transmit(&huart1,powerinquiry,5,100);
						}
						else if(actgomb[0] == 1)
						{
							gomb0hosszutime = 0;
						}
					}

					if(actgomb[0] == 1)
					{
						if(gomb0hosszutime >0)
						{
							gomb0prell++;
						}
						if(gomb0prell >50)
						{
							gomb0hosszutime = 0;
							gomb0prell = 0;
						}

					}
					if(actgomb[3] == 0)
					{
						gomb4hosszutime++;
						if(gomb4hosszutime > 3000)
						{
							HAL_UART_Transmit(&huart1,pwroff,6,100);
							pwrtime = 300;
							poweronflag = 0;
							poweroffflag = 1;
							gomb4hosszutime = 0;
							HAL_UART_Transmit(&huart1,powerinquiry,5,100);
						}
						else if(actgomb[3] == 1)
						{
							gomb4hosszutime = 0;
						}
					}
					if(actgomb[3] == 1)
					{
							if(gomb4hosszutime >0)
						{
							gomb3prell++;
						}
						if(gomb3prell >50)
						{
							gomb4hosszutime = 0;
							gomb3prell = 0;
						}
					}

					if(pwrtime >0)
					{
						pwrtime--;
						if((pwrtime == 0)||(pwrtime == 99)||(pwrtime == 150)||(pwrtime == 250))
						{
							HAL_UART_Transmit(&huart1,powerinquiry,5,100);
						}
					}

					}

					void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
					{
						uartbuff[uartcount] = uartbuf;
						uartcount++;
						if(uartcount > 5)
						{
							uartcount = 0;
							memset(uartbuff,0,10);
						}

						if((uartbuff[0] == 0x90)&&(uartbuff[1] == 0x41)&&(uartbuff[2] == 0xFF))
						{
							villtimer = 200;
							uartcount = 0;
							ackflag = 1;
							memset(uartbuff,0,10);
						}else if((uartbuff[0] == 0x90)&&(uartbuff[1] == 0x51)&&(uartbuff[2] == 0xFF))
						{
							villtimer = 200;
							uartcount = 0;
							cpltflag = 1;
							memset(uartbuff,0,10);
						}
						else if((uartbuff[0] == 0x90)&&(uartbuff[1] == 0x50)&&(uartbuff[2] == 0x02)&&(uartbuff[3] == 0xFF))//pwr on
						{
							villtimer = 200;
							uartcount = 0;
							poweronflag = 1;
							poweroffflag = 0;
							memset(uartbuff,0,10);
						}else if((uartbuff[0] == 0x90)&&(uartbuff[1] == 0x50)&&(uartbuff[2] == 0x03)&&(uartbuff[3] == 0xFF))//pwr off
						{
							villtimer = 200;
							uartcount = 0;
							poweroffflag = 1;
							poweronflag = 0;
							memset(uartbuff,0,10);
						}
						HAL_UART_Receive_IT(&huart1,&uartbuf,1);
					}
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
						/* User can add his own implementation to report the file name and line number,
							 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
