#ifndef __PID_H
#define __PID_H

typedef struct
{
	double Expect;//期望值
	double Currn;//目前值
	double Err_now;//目前误差
	double Err_last;//定义上一个偏差值
	double Err_Cumulative;//累计误差
	double Kp;//比例系数
	double Ki;//比例系数
	double Kd;//微分系数
}PID_TypDef;
void PID_Init(PID_TypDef *PID);
void PID_Set(PID_TypDef *PID,double Kp,double Ki,double Kd);
int PID_run(PID_TypDef* PID,double currn,double exp);
#endif
