#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PWMA   TIM1->CCR1
#define PWMB   TIM1->CCR2
#define PWMC   TIM1->CCR3
#define PWMD   TIM1->CCR4
#define PWME   TIM5->CCR1
#define PWMF   TIM5->CCR2
#define PWMG   TIM5->CCR3
#define PWMH   TIM5->CCR4
#define PWMI   TIM8->CCR3
#define PWMJ   TIM8->CCR4
#define PWMK   TIM4->CCR3
#define PWML   TIM4->CCR4

#define AIN1 PBout(0);
#define AIN2 PBout(1);
#define BIN1 PCout(0);
#define BIN2 PCout(1);
#define CIN1 PCout(2);
#define CIN2 PCout(3);
#define DIN1 PCout(4);
#define DIN2 PCout(5);

#include "show.h"
#include "encoder.h"
#include "DataScop_DP.h"
#include "control.h"
#include "st7735.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "KF.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <stdint.h>
#include "TCS34725.h"
#include <math.h>

#include "main.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stm32f1xx_it.h"

extern float pitch,roll,yaw; 		    
extern short aacx,aacy,aacz;			
extern short gyrox,gyroy,gyroz;		
extern float temp;
extern float Aacx,Aacy,Aacz,Gyrox,Gyroy;

extern COLOR_HSL hsl;
extern COLOR_RGBC rgb;
extern uint32_t EncoderA,EncoderB,EncoderC,Moto; 
extern int Target_Velocity;
extern float Velocity_KP,Velocity_KI,Velocity_KD; //PID系数
