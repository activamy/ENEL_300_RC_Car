/*
 * motor.c
 *
 *  Created on: Mar 17, 2026
 *      Author: frede
 */

#include <motor2.h>

#include "motor2.h"

#include "motor2.h"

// ---------- INIT ----------
void Motor_Init(void)
{
    // Start PWM on all channels
    HAL_TIM_PWM_Start(&htim1, M1_IN1_CHANNEL);
    HAL_TIM_PWM_Start(&htim1, M1_IN2_CHANNEL);
    HAL_TIM_PWM_Start(&htim1, M2_IN1_CHANNEL);
    HAL_TIM_PWM_Start(&htim1, M2_IN2_CHANNEL);

    // Stop motors initially
    Motor_SetSpeed(MOTOR1, 0);
    Motor_SetSpeed(MOTOR2, 0);
}

// ---------- SET SPEED ----------
void Motor_SetSpeed(Motor_ID motor, int16_t speed)
{
    uint32_t pwm = (abs(speed) * __HAL_TIM_GET_AUTORELOAD(&htim1)) / 100;

    uint32_t in1, in2;

    if (motor == MOTOR1)
    {
        in1 = M1_IN1_CHANNEL;
        in2 = M1_IN2_CHANNEL;
    }
    else
    {
        in1 = M2_IN1_CHANNEL;
        in2 = M2_IN2_CHANNEL;
    }

    if (speed > 0)
    {
        // Forward
        __HAL_TIM_SET_COMPARE(&htim1, in1, pwm);
        __HAL_TIM_SET_COMPARE(&htim1, in2, 0);
    }
    else if (speed < 0)
    {
        // Reverse
        __HAL_TIM_SET_COMPARE(&htim1, in1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, in2, pwm);
    }
    else
    {
        // Stop
        __HAL_TIM_SET_COMPARE(&htim1, in1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, in2, 0);
    }
}


void Motor_Drive(Direction direction, int16_t speed){
	if (direction == STRAIGHT){
		Motor_SetSpeed(MOTOR1, speed);
		Motor_SetSpeed(MOTOR2, speed);
	}
	else if (direction == LEFT){
			Motor_SetSpeed(MOTOR1, -speed);
			Motor_SetSpeed(MOTOR2, speed);
	}
	else if (direction == RIGHT){
				Motor_SetSpeed(MOTOR1, speed);
				Motor_SetSpeed(MOTOR2, -speed);
	}
}






//extern TIM_HandleTypeDef htim1;
//
//// attepmt
//void Motor_Init()
//{
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
//    // STOP state (coast)
//    HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_F_PIN, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_B_PIN, GPIO_PIN_RESET);
//}
//
//int time = 1000;
//
//void Set_Motor_Speed(int16_t speed, Motor_Driver driver, Direction dir)
//{
//    uint32_t pulse_value = (abs(speed) * __HAL_TIM_GET_AUTORELOAD(&htim1)) / 100;
//
//    if (speed > 0)
//    {
//        // Forward
//        HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_F_PIN, GPIO_PIN_SET);
//        HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_B_PIN, GPIO_PIN_RESET);
//
//        __HAL_TIM_SET_COMPARE(&htim1, MOTOR1_FCHANNEL, pulse_value);
//    }
//    else if (speed < 0)
//    {
//        // Reverse
//        HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_F_PIN, GPIO_PIN_RESET);
//        HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_B_PIN, GPIO_PIN_SET);
//
//        __HAL_TIM_SET_COMPARE(&htim1, MOTOR1_FCHANNEL, pulse_value);
//    }
//    else
//    {
//        // Stop (coast)
//        HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_F_PIN, GPIO_PIN_RESET);
//        HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_B_PIN, GPIO_PIN_RESET);
//
//        __HAL_TIM_SET_COMPARE(&htim1, MOTOR1_FCHANNEL, 0);
//    }
//}














//void Motor_Init(){
//	TIM1->CCR1=300;
//	HAL_TIM_PWM_Start (&MOTOR_TIMER, MOTOR1_FCHANNEL);
//	HAL_TIM_PWM_Start (&MOTOR_TIMER, MOTOR1_BCHANNEL);
//
//	//set all gpio to high for metal detector
//	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_F_PIN, 1);
//	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_B_PIN, 1);
//	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_F_PIN, 1);
//	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_B_PIN, 1);
//}
//
//int time = 1000;
//
//void Set_Motor_Speed(int16_t speed ,  Motor_Driver driver, Direction dir) {
//    // Speed range: -100 to 100
//    uint32_t pulse_value = (abs(speed) * __HAL_TIM_GET_AUTORELOAD(&MOTOR_TIMER)) / 100;
//    MOTOR_TIMER_INSTANCE->CCR1=499;
//
////	if (driver == PCB){ // pcb driver
//		if (speed > 0) {
//			if (dir == STRAIGHT){
//			// Forward: Pulse IN1, keep IN2 Low
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, 0);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, 0);
//			}
//			else if (dir == LEFT){
//						// Forward: Pulse IN1, keep IN2 Low
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value/2);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, 0);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, 0);
//			}
//			else if (dir == RIGHT){
//						// Forward: Pulse IN1, keep IN2 Low
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, 0);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, pulse_value/2);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, 0);
//			}
//		}
//		else if (speed < 0) {
//			// Reverse: Keep IN1 Low, pulse IN2
//			if (dir == STRAIGHT){
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, 0);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, 0);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, pulse_value);
//			}
//			else if (dir == LEFT){
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, 0);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value/2);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, 0);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, pulse_value);
//			}
//			else if (dir == RIGHT){
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, 0);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, 0);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, pulse_value/2);
//			}
//		}
//		else {
//			// Stop: Both
//			if (dir == STRAIGHT){
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, pulse_value);
//			}
//			else if (dir == LEFT){
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, 0);
//			}
//			else if (dir == RIGHT){
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, 0);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, pulse_value);
//			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, pulse_value);
//			}
//		}
//	}
//		// Low (Low-Power Sleep Mode after 1ms)








//	else{ // red driver

//		__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
//		__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value);
//			if (speed > 0) {
//				// Forward: Pulse IN1, keep IN2 Low
////				__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
////				__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, 0);
//				if (dir == STRAIGHT){
//					__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
//					__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value);
//				}else if (dir == RIGHT){
//					__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value/3);
//					__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value);
//				}else{
//					__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
//					__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value/3);
//				}
//				HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_F_PIN, 1);
//				HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_B_PIN, 0);
//				HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_F_PIN, 1);
//				HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_B_PIN, 0);
////				HAL_Delay(time);
//
//			}
//			else if (speed < 0) {
//				// Reverse: Keep IN1 Low, pulse IN2
//				if (dir == STRAIGHT){
//					__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
//					__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value);
//				}else if (dir == LEFT){
//					__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value/2);
//					__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value);
//				}else{
//					__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
//					__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value/2);
//				}
//				HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_F_PIN, 0);
//				HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_B_PIN, 1);
//				HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_F_PIN, 0);
//				HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_B_PIN, 1);
////				HAL_Delay(time);
//
//			}
//			else {
//				// Stop: Both
//				__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, 400);
//				__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, 400);
//				if (dir == STRAIGHT){
//					HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_F_PIN, 1);
//					HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_B_PIN, 1);
//					HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_F_PIN, 1);
//					HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_B_PIN, 1);
//				}else if (dir == LEFT){
//					HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_F_PIN, 0);
//					HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_B_PIN, 1);
//					HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_F_PIN, 1);
//					HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_B_PIN, 0);
//				}else{
//
//					HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_F_PIN, 1);
//					HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_B_PIN, 0);
//					HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_F_PIN, 0);
//					HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_B_PIN, 1);
//				}
////				HAL_Delay(time);
//			}
//			// ADD STOP BOTH LOW??(Low-Power Sleep Mode after 1ms)


