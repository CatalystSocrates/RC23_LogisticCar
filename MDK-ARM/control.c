#include "control.h"	

#include "system.h"

/**************************************************************************
º¯Êı¹¦ÄÜ£º¸³Öµ¸øPWM¼Ä´æÆ÷
Èë¿Ú²ÎÊı£ºPWM
·µ»Ø  Öµ£ºÎŞ
**************************************************************************/
void Set_Pwma(int moto)
{
    	if(moto>0)			
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); 
 	      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); 
			}
			else 	          
			{
			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); 
 	      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
			}
			PWMA=abs(moto);
}
void Set_Pwmb(int moto)
{
    	if(moto>0)			
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET); 
 	      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
			}
			else 	          
			{
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); 
 	      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
			}
			PWMB=abs(moto);
}
void Set_Pwmc(int moto)
{
    	if(moto>0)			
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); 
 	      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
			}
			else 	          
			{
			  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET); 
 	      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			PWMC=abs(moto);
}

/**************************************************************************
º¯Êı¹¦ÄÜ£ºÏŞÖÆPWM¸f³Öµ 
Èë¿Ú²ÎÊı£ºÎŞ
·µ»Ø  Öµ£ºÎŞ
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=7100;    //===PWMÂú·ùÊÇ7200 ÏŞÖÆÔÚ7100
	  if(Moto<-Amplitude) Moto=-Amplitude;	
		if(Moto>Amplitude)  Moto=Amplitude;		
}


/**************************************************************************
º¯Êı¹¦ÄÜ£ºÔöÁ¿PI¿ØÖÆÆ÷
Èë¿Ú²ÎÊı£º±àÂëÆ÷²âÁ¿Öµ£¬Ä¿±êËÙ¶È
·µ»Ø  Öµ£ºµç»úPWM
¸ù¾İÔöÁ¿Ê½ÀëÉ¢PID¹«Ê½ 
pwm+=Kp[e£¨k£©-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)´ú±í±¾´ÎÆ«²î 
e(k-1)´ú±íÉÏÒ»´ÎµÄÆ«²î  ÒÔ´ËÀàÍÆ 
pwm´ú±íÔöÁ¿Êä³ö
ÔÚÎÒÃÇµÄËÙ¶È¿ØÖÆ±Õ»·ÏµÍ³ÀïÃæ£¬Ö»Ê¹ÓÃPI¿ØÖÆ
pwm+=Kp[e£¨k£©-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PIA (int Encoder,int Target)
{ 	
	 static float BiasA,PwmA,Last_biasA;
	 BiasA=Target-Encoder;                                  //¼ÆËãÆ«²î
	 PwmA+=Velocity_KP*(BiasA-Last_biasA)+Velocity_KI*BiasA;   //ÔöÁ¿Ê½PI¿ØÖÆÆ÷
	 Last_biasA=BiasA;	                                   //±£´æÉÏÒ»´ÎÆ«²î 
	 return PwmA;                                           //ÔöÁ¿Êä³ö
}

int Incremental_PIB (int Encoder,int Target)
{ 	
	 static float BiasB,PwmB,Last_biasB;
	 BiasB=Target-Encoder;                                  //¼ÆËãÆ«²î
	 PwmB+=Velocity_KP*(BiasB-Last_biasB)+Velocity_KI*BiasB;   //ÔöÁ¿Ê½PI¿ØÖÆÆ÷
	 Last_biasB=BiasB;	                                   //±£´æÉÏÒ»´ÎÆ«²î 
	 return PwmB;                                           //ÔöÁ¿Êä³ö
}

int Incremental_PIC (int Encoder,int Target)
{ 	
	 static float BiasC,PwmC,Last_biasC;
	 BiasC=Target-Encoder;                                  //¼ÆËãÆ«²î
	 PwmC+=Velocity_KP*(BiasC-Last_biasC)+Velocity_KI*BiasC;   //ÔöÁ¿Ê½PI¿ØÖÆÆ÷
	 Last_biasC=BiasC;	                                   //±£´æÉÏÒ»´ÎÆ«²î 
	 return PwmC;                                           //ÔöÁ¿Êä³ö
}


void Setspeeds(double V_x,double V_y,double W){
	
	  double Va;
    double Vb;
    double Vc;
    double R1 = 0.12 ; // Î´¼ÓÎó²î
    double R2 = 0.12 ; // Î´¼ÓÎó²î
    double R3 = 0.12 ; // Î´¼ÓÎó²î
    double a = PI / 6; // ²Î¿¼Ïµ¼äµÄ¼Ğ½Ç
	  double angle_error1=0;
	  double angle_error2=0;
	  double angle_error3=0;

    Va = V_x * cos(a + angle_error1) + V_y * sin(a + angle_error1) + W * R1;
    Vb = -V_x * cos(a + (PI/3) + angle_error2) - V_y * sin(a + (PI/3) + angle_error2) + W * R2;
    Vc = -V_x * sin(a + (PI/6) + angle_error3) + V_y * cos(a + (PI/6) + angle_error3) + W * R3;
	
	  Set_Pwma(Incremental_PIA (EncoderA,Va));
	  Set_Pwmb(Incremental_PIB (EncoderB,Vb));
   	Set_Pwmc(Incremental_PIC (EncoderC,Vc));

	  
}

