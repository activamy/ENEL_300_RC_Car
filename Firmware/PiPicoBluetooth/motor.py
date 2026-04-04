from machine import Pin, PWM
from time import sleep

# === L298N Motor Driver ===
# Motor A
motor_a_in1 = Pin(6, Pin.OUT)
motor_a_in2 = Pin(7, Pin.OUT)
motor_a_en = PWM(Pin(8))
motor_a_en.freq(1000)
motor_a_correction = 1.0 # Adjust so both motors have same speed

# Motor B
motor_b_in3 = Pin(4, Pin.OUT)
motor_b_in4 = Pin(3, Pin.OUT)
motor_b_en = PWM(Pin(2))
motor_b_en.freq(1000)
motor_b_correction = 1.0 # Adjust so both motors have same speed during each round even as battery slowly drains

# Function to control Motor A
def motor_a(direction = "stop", speed = 0):
    adjusted_speed = int(speed * motor_a_correction)  # Apply correction
    if direction == "forward":
        motor_a_in1.value(0)
        motor_a_in2.value(1)
    elif direction == "backward":
        motor_a_in1.value(1)
        motor_a_in2.value(0)
    else:  # Stop
        motor_a_in1.value(0)
        motor_a_in2.value(0)
    motor_a_en.duty_u16(int(adjusted_speed * 65535 / 100))  # Speed: 0-100%

# Function to control Motor B
def motor_b(direction = "stop", speed = 0):
    adjusted_speed = int(speed * motor_b_correction)  # Apply correction
    if direction == "forward":
        motor_b_in3.value(1)
        motor_b_in4.value(0)
    elif direction == "backward":
        motor_b_in3.value(0)
        motor_b_in4.value(1)
    else:  # Stop
        motor_b_in3.value(0)
        motor_b_in4.value(0)
    motor_b_en.duty_u16(int(adjusted_speed * 65535 / 100))  # Speed: 0-100%

########### CHANGE SPEED TO MATCH WHAT MOTOR CAN DO

# Motor Movement
def move_forward():
    motor_a("forward", 75)
    motor_b("forward", 75)
    
def move_backward():
    motor_a("backward", 75)
    motor_b("backward", 75)
    
def move_left():
    motor_a("forward", 35)
    motor_b("forward", 75)
    
def move_right():
    motor_b("forward", 35)
    motor_a("forward", 75)

def stop():
    motor_a(stop, 0)
    motor_b(stop, 0)