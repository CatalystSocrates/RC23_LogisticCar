#include "system.h"
float pitch,roll,yaw; 		    
short aacx,aacy,aacz;			
short gyrox,gyroy,gyroz;		
float temp;
float Aacx,Aacy,Aacz,Gyrox,Gyroy;

COLOR_HSL hsl;
COLOR_RGBC rgb;
uint32_t EncoderA,EncoderB,EncoderC,Moto; 
int Target_Velocity=500;
float Velocity_KP=0.38,Velocity_KI=0.41,Velocity_KD=0.007; //PIDÏµÊý
