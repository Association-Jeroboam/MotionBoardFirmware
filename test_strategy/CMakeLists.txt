cmake_minimum_required(VERSION 3.16)

project(test_strategy)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/Control.cpp
    src/MotorControl.cpp
    src/Motor.cpp
    ../common/Strategy/Strategy.cpp
    ../common/Strategy/MatchState/GetBuoyState/GoToBuoyState.cpp
    ../common/PID.cpp
    ../common/RobotPose.cpp
    ../common/Goal.cpp
    ../webots/controllers/eurobot/Logging.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ../common)
target_include_directories(${PROJECT_NAME} PRIVATE ../libs/eHSM/include)
target_include_directories(${PROJECT_NAME} PRIVATE ./src)
