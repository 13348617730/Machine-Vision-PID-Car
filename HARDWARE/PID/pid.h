#ifndef __PID_H
#define __PID_H

typedef struct
{
	double Expect;//����ֵ
	double Currn;//Ŀǰֵ
	double Err_now;//Ŀǰ���
	double Err_last;//������һ��ƫ��ֵ
	double Err_Cumulative;//�ۼ����
	double Kp;//����ϵ��
	double Ki;//����ϵ��
	double Kd;//΢��ϵ��
}PID_TypDef;
void PID_Init(PID_TypDef *PID);
void PID_Set(PID_TypDef *PID,double Kp,double Ki,double Kd);
int PID_run(PID_TypDef* PID,double currn,double exp);
#endif
