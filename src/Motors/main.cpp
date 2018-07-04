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

/*
 * 
 */
int main(int argc, char** argv) {
//    i2c_init();
    float pwmValue = 0;
    float count = 10;
    Motors *motorModule = new Motors();
    while(true) {
        motorModule->motor1PWM(pwmValue, 1);
        pwmValue += count;
        if(pwmValue >= 50){
            count = -10;
        }
        if(pwmValue <= -50){
            count = 10;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

//    i2c_end();
    return 0;
}

