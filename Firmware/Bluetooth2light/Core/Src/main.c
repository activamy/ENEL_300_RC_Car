/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor2.h"
#include "hcsr04.h"
#include <stdio.h>
#include "lcd_i2c.h"
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
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
//uint8_t rxData;
//volatile uint8_t data_receive = 0;
//volatile uint8_t data;



uint8_t rx_data;
volatile uint8_t received_data;

volatile uint8_t command = 'S';
uint32_t cmd_time = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
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
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart3, &rx_data, 1);



  Motor_Init();
  HCSR04_Init();

   uint16_t LCD_ADDR = (0x27 << 1);/* Standard addresses are 0x27 or 0x3F. HAL requires the address to be shifted left by 1 bit. */

   HAL_StatusTypeDef statusl;

   statusl = HAL_I2C_IsDeviceReady(&hi2c1,LCD_ADDR, 3, 10);

      if (statusl == HAL_OK) {
          // SUCCESS: The STM32 found the LCD
    	  LCD_Init(&hi2c1);
          printf("System Online");
      } else {
          // FAILURE: The STM32 cannot find the device
          // Trigger an error LED or serial print
          Error_Handler();
          printf("LCD Not Found");
      }

  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("  HC-SR04 Live  ");
  LCD_SetCursor(0, 1);
  LCD_Print("  Initializing  ");
  HAL_Delay(1500);
//
  char buf[17];
  float    distance_cm;
  HCSR04_Status status;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



  while (1)
  {
	  if(received_data) {
		  received_data = 0;
		  dataToMotor(command);
//		  switch(command) {
//
//		  case 'F':
//			  // motor_forward
//			  break;
//		  case 'L':
//			  // motor_left
//			  break;
//		  case 'R':
//			  // motor_right
//			  break;
//		  case 'B':
//			  // motor_backward
//			  break;
//		  case 'S':
//			  //stop
//			  break;
//		  }
	  }

	  if ((HAL_GetTick() - cmd_time) > 300) {
			  command = 'S';
	  }
    /* USER CODE END WHILE */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */



	  status = HCSR04_Read(&distance_cm);
	  LCD_SetCursor(0, 0);
	  LCD_Print("  Distance (cm) ");
	  if (status == HCSR04_OK)
	  {
//		  filter(dist_arr, distance_cm);
		  printf("Distance: %.1f cm\r\n", distance_cm);
		  LCD_SetCursor(0, 1);
		  snprintf(buf, sizeof(buf), "    %6.1f cm   ", distance_cm);
		  LCD_Print(buf);
	  }
	  else
	  {
		  printf("\r\n");
	  }
//
//	  HAL_Delay(20);   /* Measure ~5 times per second */
//	  Motor_SetSpeed(MOTOR1, 70);  // forward 50%
//	  Motor_SetSpeed(MOTOR2, 70);   //THIS IS NOT ??% OF 6V ITS ??% OF VM+0.7
//
//	  Motor_Drive(STRAIGHT, 25);
//	  HAL_Delay(2000);
//	  Motor_Drive(RIGHT, 25);
//	  HAL_Delay(2000);
//	  Motor_Drive(LEFT, 25);
//	  HAL_Delay(2000);
//
//	  Motor_SetSpeed(MOTOR1, -50); // reverse
//	  Motor_SetSpeed(MOTOR2, -50);
//
//	  HAL_Delay(2000);
//
//	  Motor_SetSpeed(MOTOR1, 0);
//	  Motor_SetSpeed(MOTOR2, 0);
//
//	  HAL_Delay(2000);




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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 83;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 499;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 83;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFF;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|LD2_Pin|GreenLight_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 LD2_Pin GreenLight_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|LD2_Pin|GreenLight_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
uint8_t speed;
void dataToMotor(uint8_t data){

	if (data == 'V'){
		if (speed == 50){
		speed = 70;
		}
		else{
			speed = 50;
		}
	}

	switch(data) {
//		case 'O':
//			HAL_GPIO_WritePin(GreenLight_GPIO_Port, GreenLight_Pin, GPIO_PIN_SET);
//			break;
//
//		case 'F':
//			HAL_GPIO_WritePin(GreenLight_GPIO_Port, GreenLight_Pin, GPIO_PIN_RESET);
//			break;

		case 'F':
			Motor_Drive(STRAIGHT, speed);
			break;

		case 'B':
			Motor_Drive(STRAIGHT, -speed);
			break;
//
//		case 'K': // forwards and left
//			Motor_Drive(LEFT, 25);
//			break;
//
//		case 'Q': // forwards and right
//			Motor_Drive(RIGHT, 25);
//			break;
//
//		case 'J': // back and left
//			Motor_Drive(LEFT, -25);
//			break;
//
//		case 'P': // back and right
//			Motor_Drive(RIGHT, -25);
//			break;

		case 'L': // left
			Motor_Drive(LEFT, speed);
			break;

		case 'R': // right
			Motor_Drive(RIGHT, speed);
			break;
		case 'S':
			Motor_Drive(STRAIGHT, 0);
			break;
		}
}

void HAL_UART_RxCpltCallback (UART_HandleTypeDef *huart)
{
	if (huart -> Instance == USART3) {
		HAL_UART_Transmit(&huart2, &rx_data, 1, 10);
		received_data = 1;

		cmd_time = HAL_GetTick();
		HAL_UART_Receive_IT(&huart3, &rx_data, 1);
		command = rx_data;

		// ensure connection
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
}
//
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart -> Instance == UART4)
//	{
//		// Forward received byte to PC via USB (USART2)
//		HAL_UART_Transmit(&huart2, &rxData, 1, 10);
//
//		data_receive = 1;
//		data = rxData;
//		HAL_UART_Receive_IT(&huart3, &rxData, 1); // what does this do again????
//
//		if (data_receive) {
//				  data_receive = 0;
//
//				  switch(data) {
//				    case 'O':
//			  			HAL_GPIO_WritePin(GreenLight_GPIO_Port, GreenLight_Pin, GPIO_PIN_SET);
//			  			break;
//
//			  		case 'F':
//			  			HAL_GPIO_WritePin(GreenLight_GPIO_Port, GreenLight_Pin, GPIO_PIN_RESET);
//			  			break;
//
//			  		case 'U':
//			  			Motor_Drive(STRAIGHT, 50);
//			  			break;
//
//			  		case 'D':
//			  			Motor_Drive(STRAIGHT, -50);
//			  			break;
//
//			  		case 'K': // forwards and left
//			  			Motor_Drive(LEFT, 25);
//			  			break;
//
//			  		case 'Q': // forwards and right
//			  			Motor_Drive(RIGHT, 25);
//			  			break;
//
//			  		case 'J': // back and left
//			  			Motor_Drive(LEFT, -25);
//			  			break;
//
//			  		case 'P': // back and right
//			  			Motor_Drive(RIGHT, -25);
//			  			break;
//
//			  		case 'L': // left
//			  			Motor_Drive(LEFT, 50);
//			  			break;
//
//			  		case 'R': // right
//			  			Motor_Drive(RIGHT, 50);
//			  			break;
//			  		case 'S':
//			  			Motor_Drive(STRAIGHT, 0);
//			  			break;
//			  		}
//			  }
//	}
//
//}
//


////	   Connection checker
//	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//	{
//	    if(huart->Instance == UART4)
//	    {
//	    	HAL_GPIO_WritePin(GreenLight_GPIO_Port, GreenLight_Pin, GPIO_PIN_SET);
//
//	        HAL_UART_Receive_IT(&huart4, &rxData, 1);
//	    }
//	}
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
