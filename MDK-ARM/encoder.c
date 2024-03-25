#include "encoder.h"
#include "tim.h"
int Read_Velocity(uint8_t TIMX)
{
   int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 3:  Encoder_TIM= (short)TIM3 -> CNT;  
		          TIM3 -> CNT=0;
		          break;
		 case 8:  Encoder_TIM= (short)TIM8 -> CNT;  
		          TIM8 -> CNT=0;
		          break;	
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;  
		          TIM4 -> CNT=0;
		          break;	
		 default: Encoder_TIM=0;
	 }
		return Encoder_TIM;
}

int Read_Position(uint8_t TIMX)
{
   int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 3:  Encoder_TIM= (short)TIM3 -> CNT;  
		          //TIM3 -> CNT=0;
		          break;
		 case 8:  Encoder_TIM= (short)TIM8 -> CNT;  
		          //TIM8 -> CNT=0;
		          break;	
		 case 4:  Encoder_TIM= (short)TIM4 -> CNT;  
		          //TIM4 -> CNT=0;
		          break;	
		 default: Encoder_TIM=0;
	 }
		return Encoder_TIM;
}


