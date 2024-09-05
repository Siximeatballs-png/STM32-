#include "stm32f10x.h"                  // Device header
#include "control.h"
#include "delay.h"
float Med_Angle=0;		//��е��ֵ��---�������޸���Ļ�е��ֵ���ɡ�
float Target_Speed=0;	//�����ٶȣ���������---���ο����ӿڣ����ڿ���С��ǰ�����˼����ٶȡ�
float Turn_Speed=0;		//�����ٶȣ�ƫ����

float 
	Vertical_Kp=250,//ֱ����KP��KD
	Vertical_Kd=0.6;
float 
	Velocity_Kp=0.65,//2,//�ٶȻ�KP��KI
	Velocity_Ki=0.00325;//0.01;
float 
	Turn_Kd=0.6,//ת��KP��KD
	Turn_Kp=20;
extern int length;
#define SPEED_Y 15 //����(ǰ��)����趨�ٶ�
#define SPEED_Z 80//ƫ��(����)����趨�ٶ� 
int Vertical_out,Velocity_out,Turn_out;//ֱ����&�ٶȻ�&ת�� ���������

int Vertical(float Med,float Angle,float gyro_Y);//��������
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z,int RC); 

void qianjin_Init (void)
{
	int PWM_out;
	
	if(EXTI_GetITStatus(EXTI_Line5)!=0)//һ���ж�
	{
		if(PBin(5)==0)//�����ж�
		{
			EXTI_ClearITPendingBit(EXTI_Line5);//����жϱ�־λ
			
			//1���ɼ�����������&MPU6050�Ƕ���Ϣ��
			Encoder_Left=-Read_Speed(2);//�������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ����
			Encoder_Right=Read_Speed(4);
			
			mpu_dmp_get_data(&Roll,&Pitch,&Yaw);			//�Ƕ�
			MPU_Get_Gyroscope(&gyroy,&gyrox,&gyroz);	//������
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//���ٶ�

			//1.5����������
			/*********************************************************************************************/
			/*ǰ��*/
			if((Fore==0)&&(Back==0))
				Target_Speed=0;
//				if(length<50)
//					Target_Speed++;
//				else
//					if(Target_Speed!=0)
//						Target_Speed--;//δ���ܵ�ǰ������ָ��-->�ٶ����㣬����ԭ��
			
			
			//2��������ѹ��ջ������У�����������������
			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);	//�ٶȻ�
			Vertical_out=Vertical(Velocity_out+Med_Angle,Pitch,gyroy);			//ֱ����
			Turn_out=Turn(gyroz,Turn_Speed);																//ת��
			
			PWM_out=Vertical_out;//�������
			//3���ѿ�����������ص�����ϣ�������յĵĿ��ơ�
			MOTO1=PWM_out-Turn_out;//����
			MOTO2=PWM_out+Turn_out;//�ҵ��
			Limit(&MOTO1,&MOTO2);	 //PWM�޷�			
			Load(MOTO1,MOTO2);		 //���ص�����ϡ�
			
			Stop(&Med_Angle,&Pitch);//��ȫ���
			
		}
	}
   	delay_s(5);
	MPU_IIC_Stop();	
}
	