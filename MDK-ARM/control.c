#include "control.h"	

#include "system.h"

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
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
�������ܣ�����PWM�f�ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=7100;    //===PWM������7200 ������7100
	  if(Moto<-Amplitude) Moto=-Amplitude;	
		if(Moto>Amplitude)  Moto=Amplitude;		
}


/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PIA (int Encoder,int Target)
{ 	
	 static float BiasA,PwmA,Last_biasA;
	 BiasA=Target-Encoder;                                  //����ƫ��
	 PwmA+=Velocity_KP*(BiasA-Last_biasA)+Velocity_KI*BiasA;   //����ʽPI������
	 Last_biasA=BiasA;	                                   //������һ��ƫ�� 
	 return PwmA;                                           //�������
}

int Incremental_PIB (int Encoder,int Target)
{ 	
	 static float BiasB,PwmB,Last_biasB;
	 BiasB=Target-Encoder;                                  //����ƫ��
	 PwmB+=Velocity_KP*(BiasB-Last_biasB)+Velocity_KI*BiasB;   //����ʽPI������
	 Last_biasB=BiasB;	                                   //������һ��ƫ�� 
	 return PwmB;                                           //�������
}

int Incremental_PIC (int Encoder,int Target)
{ 	
	 static float BiasC,PwmC,Last_biasC;
	 BiasC=Target-Encoder;                                  //����ƫ��
	 PwmC+=Velocity_KP*(BiasC-Last_biasC)+Velocity_KI*BiasC;   //����ʽPI������
	 Last_biasC=BiasC;	                                   //������һ��ƫ�� 
	 return PwmC;                                           //�������
}


void Setspeeds(double V_x,double V_y,double W){
	
	  double Va;
    double Vb;
    double Vc;
    double R1 = 0.12 ; // δ�����
    double R2 = 0.12 ; // δ�����
    double R3 = 0.12 ; // δ�����
    double a = PI / 6; // �ο�ϵ��ļн�
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

