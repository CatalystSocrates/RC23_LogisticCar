#ifndef __CONTROL_H
#define __CONTROL_H
#endif
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265
#define ZHONGZHI 3085
extern	int Balance_Pwm,Velocity_Pwm;
void Set_Pwma(int moto);  //��ֵ��PWM�Ĵ���
void Set_Pwmb(int moto);  //��ֵ��PWM�Ĵ���
void Set_Pwmc(int moto);  //��ֵ��PWM�Ĵ���
void Key(void);  //�����޸�����״̬ 
void Xianfu_Pwm(void);  //����PWM��ֵ
int myabs(int a);   //ȡ����ֵ
int Incremental_PI (int Encoder,int Target);
int Position_PID (int position,int target);
int Incremental_PIA (int Encoder,int Target);
int Incremental_PIB (int Encoder,int Target);
int Incremental_PIC (int Encoder,int Target);
void Setspeeds(double V_x,double V_y,double W);
