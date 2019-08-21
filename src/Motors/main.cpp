/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: yovany
 *
 * Created on June 20, 2018, 9:46 PM
 */

#include <thread>
#include <chrono>
#include <cstdlib>
#include "Motors.h"

using namespace std;
Motors *motorModule;

void speedTest();
void pwmTest();

/*
 * 
 */
int main(int argc, char** argv) {
    motorModule = new Motors();
    // speedTest();
    pwmTest();
    return 0;
}

void pwmTest() {
    float pwmValue = 0;
    float count = 10;
    while(true) {
        motorModule->motor1PWM(pwmValue, 1);
        pwmValue += count;
        if(pwmValue >= 50) {
            count = -10;
        }
        if(pwmValue <= -50) {
            count = 10;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

void speedTest() {
    int16_t speed = 0;
    int16_t count = 20;
    while(true) {
        motorModule->motor1Speed(speed, 1);
        speed += count;
        if(speed >= 200) {
            count = -20;
        }
        if(speed <= -200) {
            count = 20;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

