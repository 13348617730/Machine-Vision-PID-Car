#include "pid.h"
void PID_Init(PID_TypDef *PID)//PID初始化
{
	PID->Expect = 0;	//期望值
	PID->Currn = 0;		//当前值
	PID->Err_now = 0.0;
	PID->Err_last = 0.0;//定义上一个偏差值
	PID->Err_Cumulative = 0.0;//累计误差
	PID->Kp = 0;
	PID->Ki = 0;
	PID->Kd = 0;
}

//设置各个PID系数

void PID_Set(PID_TypDef *PID,double Kp,double Ki,double Kd)
{
	PID->Kp = Kp;
	PID->Ki = Ki;
	PID->Kd = Kd;
}
//PID运行结果
int PID_run(PID_TypDef* PID,double currn,double exp)
{
	double output = 0;//自定义输入量
	PID->Currn = currn;//传入目前值
	PID->Expect = exp;//传入期望值
	PID->Err_now =PID->Currn - PID->Expect ;//期望值与目前值的差值
	output =(PID->Kp*PID->Err_now + PID->Ki*PID->Err_Cumulative + PID->Kd*(PID->Err_now - PID->Err_last));
	PID->Err_Cumulative +=PID->Err_now;
	PID->Err_last = PID->Err_now;
	PID->Err_Cumulative = PID->Err_Cumulative > 600? 600:PID->Err_Cumulative;
	PID->Err_Cumulative = PID->Err_Cumulative < -600? -600:PID->Err_Cumulative;
	return output;//返回输出量

}