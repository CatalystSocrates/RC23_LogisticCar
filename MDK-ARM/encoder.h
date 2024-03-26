#ifndef __ENCODER_H
#define __ENCODER_H

typedef unsigned          char uint8_t;

int Read_Position(uint8_t TIMX);               //读取位置信息
int Read_Velocity(uint8_t TIMX);               //单位时间读取编码器计数

#endif
