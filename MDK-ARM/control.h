#ifndef __CONTROL_H
#define __CONTROL_H
#endif
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265
#define ZHONGZHI 3085
extern	int Balance_Pwm,Velocity_Pwm;
void Set_Pwma(int moto);  //赋值给PWM寄存器
void Set_Pwmb(int moto);  //赋值给PWM寄存器
void Set_Pwmc(int moto);  //赋值给PWM寄存器
void Key(void);  //按键修改运行状态 
void Xianfu_Pwm(void);  //限制PWM幅值
int myabs(int a);   //取绝对值
int Incremental_PI (int Encoder,int Target);
int Position_PID (int position,int target);
int Incremental_PIA (int Encoder,int Target);
int Incremental_PIB (int Encoder,int Target);
int Incremental_PIC (int Encoder,int Target);
void Setspeeds(double V_x,double V_y,double W);
