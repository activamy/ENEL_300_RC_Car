/*
 * motor.h
 *
 *  Created on: Mar 17, 2026
 *      Author: frede
 */
//

#ifndef INC_MOTOR2_H_
#define INC_MOTOR2_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdlib.h>

// Timer used for PWM
extern TIM_HandleTypeDef htim1;

// Motor selection
typedef enum {
    MOTOR1 = 0,
    MOTOR2 = 1
} Motor_ID;

// Pin mapping (IN1/IN2 per motor)
// Motor 1 = PA8, PA9  (TIM1 CH1, CH2)
// Motor 2 = PA10, PA11 (TIM1 CH3, CH4)
#define M1_IN1_CHANNEL TIM_CHANNEL_1
#define M1_IN2_CHANNEL TIM_CHANNEL_2

#define M2_IN1_CHANNEL TIM_CHANNEL_3
#define M2_IN2_CHANNEL TIM_CHANNEL_4

// Function prototypes
void Motor_Init(void);
void Motor_SetSpeed(Motor_ID motor, int16_t speed);

#endif

//#ifndef INC_MOTOR2_H_
//#define INC_MOTOR2_H_
//
//#include "stm32f4xx_hal.h"
//#include <stdint.h>
//#include "stdlib.h"
//
//#define MOTOR_TIMER          htim1        /* HAL timer handle variable name */
//#define MOTOR_TIMER_INSTANCE TIM1        /* Matching TIMx peripheral        */
//#define MOTOR1_FCHANNEL		TIM_CHANNEL_3 //(Left - near c8)
//#define MOTOR1_BCHANNEL		TIM_CHANNEL_4
//#define MOTOR2_FCHANNEL		TIM_CHANNEL_1 //(Right - near c15)
//#define MOTOR2_BCHANNEL		TIM_CHANNEL_2
//
//#define MOTOR_PORT GPIOA
//
//#define MOTOR1_F_PIN GPIO_PIN_8
//#define MOTOR1_B_PIN GPIO_PIN_9
//#define MOTOR2_F_PIN GPIO_PIN_10
//#define MOTOR2_B_PIN GPIO_PIN_11
//
//
//
//typedef enum {
//    RED =  0,
//    PCB = 1,
//} Motor_Driver;
//typedef enum {
//    LEFT =  0,
//    RIGHT = 1,
//	STRAIGHT = 2,
//} Direction;
//
//void Motor_Init();
//void Set_Motor_Speed(int16_t speed ,  Motor_Driver driver, Direction dir) ;
//
//#endif /* INC_MOTOR2_H_ */
