#include "pid.h"
void PID_Init(PID_TypDef *PID)//PID��ʼ��
{
	PID->Expect = 0;	//����ֵ
	PID->Currn = 0;		//��ǰֵ
	PID->Err_now = 0.0;
	PID->Err_last = 0.0;//������һ��ƫ��ֵ
	PID->Err_Cumulative = 0.0;//�ۼ����
	PID->Kp = 0;
	PID->Ki = 0;
	PID->Kd = 0;
}

//���ø���PIDϵ��

void PID_Set(PID_TypDef *PID,double Kp,double Ki,double Kd)
{
	PID->Kp = Kp;
	PID->Ki = Ki;
	PID->Kd = Kd;
}
//PID���н��
int PID_run(PID_TypDef* PID,double currn,double exp)
{
	double output = 0;//�Զ���������
	PID->Currn = currn;//����Ŀǰֵ
	PID->Expect = exp;//��������ֵ
	PID->Err_now =PID->Currn - PID->Expect ;//����ֵ��Ŀǰֵ�Ĳ�ֵ
	output =(PID->Kp*PID->Err_now + PID->Ki*PID->Err_Cumulative + PID->Kd*(PID->Err_now - PID->Err_last));
	PID->Err_Cumulative +=PID->Err_now;
	PID->Err_last = PID->Err_now;
	PID->Err_Cumulative = PID->Err_Cumulative > 600? 600:PID->Err_Cumulative;
	PID->Err_Cumulative = PID->Err_Cumulative < -600? -600:PID->Err_Cumulative;
	return output;//���������

}