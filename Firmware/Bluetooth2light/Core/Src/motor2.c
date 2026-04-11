/*
 * motor.c
 *
 *  Created on: Mar 17, 2026
 *      Author: frede
 */

#include <motor2.h>

extern TIM_HandleTypeDef MOTOR_TIMER;

void Motor_Init(){
	TIM1->CCR1=300;
	HAL_TIM_PWM_Start (&MOTOR_TIMER, MOTOR1_FCHANNEL);
	HAL_TIM_PWM_Start (&MOTOR_TIMER, MOTOR1_BCHANNEL);

	//set all gpio to high for metal detector
	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_F_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR1_B_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_F_PIN, 1);
	HAL_GPIO_WritePin(MOTOR_PORT, MOTOR2_B_PIN, 1);
}

int time = 1000;

void Set_Motor_Speed(int16_t speed ,  Motor_Driver driver, Direction dir) {
    // Speed range: -100 to 100
    uint32_t pulse_value = (abs(speed) * __HAL_TIM_GET_AUTORELOAD(&MOTOR_TIMER)) / 100;
    MOTOR_TIMER_INSTANCE->CCR1=499;

//	if (driver == PCB){ // pcb driver
		if (speed > 0) {
			if (dir == STRAIGHT){
			// Forward: Pulse IN1, keep IN2 Low
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, 0);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, 0);
			}
			else if (dir == LEFT){
						// Forward: Pulse IN1, keep IN2 Low
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value/2);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, 0);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, 0);
			}
			else if (dir == RIGHT){
						// Forward: Pulse IN1, keep IN2 Low
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, 0);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, pulse_value/2);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, 0);
			}
		}
		else if (speed < 0) {
			// Reverse: Keep IN1 Low, pulse IN2
			if (dir == STRAIGHT){
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, 0);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, 0);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, pulse_value);
			}
			else if (dir == LEFT){
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, 0);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value/2);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, 0);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, pulse_value);
			}
			else if (dir == RIGHT){
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, 0);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, 0);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, pulse_value/2);
			}
		}
		else {
			// Stop: Both
			if (dir == STRAIGHT){
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, pulse_value);
			}
			else if (dir == LEFT){
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, 0);
			}
			else if (dir == RIGHT){
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_FCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR1_BCHANNEL, 0);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_FCHANNEL, pulse_value);
			__HAL_TIM_SET_COMPARE(&MOTOR_TIMER, MOTOR2_BCHANNEL, pulse_value);
			}
		}
	}
		// Low (Low-Power Sleep Mode after 1ms)


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


