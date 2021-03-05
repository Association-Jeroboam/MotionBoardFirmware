#!/usr/bin/env python3
from controller import Supervisor, Motor, Keyboard

# Robot mechanical constants
MAX_MOTOR_VELOCITY = 50 # [rpm]
WHEEL_RADIUS = 0.025 # [m]
WHEEL_BASE = 0.165   # [m]
MAX_LINEAR_SPEED = 1 # [m.s-1]
MAX_ANGULAR_SPEED = 1.5708 # [rad.s-1]
LINEAR_ACCEL = 0.5   # [m.s-2]
ANGULAR_ACCEL = LINEAR_ACCEL / WHEEL_BASE # [rad.s-2] 

# time in [ms] of a simulation step
TIME_STEP = 32

# create the Robot instance.
supervisor = Supervisor()

# Init keyboard
keyboard = supervisor.getKeyboard()
keyboard.enable(TIME_STEP)

# Init motors
leftMotor = supervisor.getDevice('left wheel motor')
rightMotor = supervisor.getDevice('right wheel motor')
leftMotor.setPosition(float('inf'))
rightMotor.setPosition(float('inf'))
leftMotor.setVelocity(0.0)
rightMotor.setVelocity(0.0)

linearVelocity = 0
angularVelocity = 0

def check_keyboard_inputs():
    keys = [keyboard.getKey()]

    while keys[-1] != -1:
        keys.append(keyboard.getKey())

    if len(keys) > 1:
        keys = keys[:-1]

    for key in keys:
        handle_key(key)

def handle_key(key):
    global linearVelocity, angularVelocity

    if key == -1:
        linearVelocity, angularVelocity = 0, 0
        return

    if key == Keyboard.UP or key == 90:
        linearVelocity = max(-MAX_LINEAR_SPEED, min(linearVelocity + LINEAR_ACCEL * (TIME_STEP/1000), MAX_LINEAR_SPEED))
    elif key == Keyboard.DOWN or key == 83:
        linearVelocity = max(-MAX_LINEAR_SPEED, min(linearVelocity - LINEAR_ACCEL * (TIME_STEP/1000), MAX_LINEAR_SPEED))
    elif key == Keyboard.LEFT or key == 81:
        angularVelocity = max(-MAX_ANGULAR_SPEED, min(angularVelocity + ANGULAR_ACCEL * (TIME_STEP/1000), MAX_ANGULAR_SPEED))
        angularVelocity = MAX_ANGULAR_SPEED
    elif key == Keyboard.RIGHT or key == 68:
        angularVelocity = max(-MAX_ANGULAR_SPEED, min(angularVelocity - ANGULAR_ACCEL * (TIME_STEP/1000), MAX_ANGULAR_SPEED))
        angularVelocity = -MAX_ANGULAR_SPEED

def update_motors():
    global linearVelocity, angularVelocity
    leftVelocity = (linearVelocity - angularVelocity * WHEEL_BASE) / WHEEL_RADIUS
    rightVelocity = (linearVelocity + angularVelocity * WHEEL_BASE) / WHEEL_RADIUS 
    leftMotor.setVelocity(max(-MAX_MOTOR_VELOCITY, min(leftVelocity, MAX_MOTOR_VELOCITY)))
    rightMotor.setVelocity(max(-MAX_MOTOR_VELOCITY, min(rightVelocity, MAX_MOTOR_VELOCITY)))

while supervisor.step(TIME_STEP) != -1:
    check_keyboard_inputs()
    update_motors()
