#include "control.h"	

#include "system.h"

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
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
函数功能：限制PWM竑持� 
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=7100;    //===PWM满幅是7200 限制在7100
	  if(Moto<-Amplitude) Moto=-Amplitude;	
		if(Moto>Amplitude)  Moto=Amplitude;		
}


/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PIA (int Encoder,int Target)
{ 	
	 static float BiasA,PwmA,Last_biasA;
	 BiasA=Target-Encoder;                                  //计算偏差
	 PwmA+=Velocity_KP*(BiasA-Last_biasA)+Velocity_KI*BiasA;   //增量式PI控制器
	 Last_biasA=BiasA;	                                   //保存上一次偏差 
	 return PwmA;                                           //增量输出
}

int Incremental_PIB (int Encoder,int Target)
{ 	
	 static float BiasB,PwmB,Last_biasB;
	 BiasB=Target-Encoder;                                  //计算偏差
	 PwmB+=Velocity_KP*(BiasB-Last_biasB)+Velocity_KI*BiasB;   //增量式PI控制器
	 Last_biasB=BiasB;	                                   //保存上一次偏差 
	 return PwmB;                                           //增量输出
}

int Incremental_PIC (int Encoder,int Target)
{ 	
	 static float BiasC,PwmC,Last_biasC;
	 BiasC=Target-Encoder;                                  //计算偏差
	 PwmC+=Velocity_KP*(BiasC-Last_biasC)+Velocity_KI*BiasC;   //增量式PI控制器
	 Last_biasC=BiasC;	                                   //保存上一次偏差 
	 return PwmC;                                           //增量输出
}


void Setspeeds(double V_x,double V_y,double W){
	
	  double Va;
    double Vb;
    double Vc;
    double R1 = 0.12 ; // 未加误差
    double R2 = 0.12 ; // 未加误差
    double R3 = 0.12 ; // 未加误差
    double a = PI / 6; // 参考系间的夹角
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

