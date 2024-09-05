#include "stm32f10x.h"                  // Device header
#include "control.h"
#include "delay.h"
float Med_Angle=0;		//机械中值。---在这里修改你的机械中值即可。
float Target_Speed=0;	//期望速度（俯仰）。---二次开发接口，用于控制小车前进后退及其速度。
float Turn_Speed=0;		//期望速度（偏航）

float 
	Vertical_Kp=250,//直立环KP、KD
	Vertical_Kd=0.6;
float 
	Velocity_Kp=0.65,//2,//速度环KP、KI
	Velocity_Ki=0.00325;//0.01;
float 
	Turn_Kd=0.6,//转向环KP、KD
	Turn_Kp=20;
extern int length;
#define SPEED_Y 15 //俯仰(前后)最大设定速度
#define SPEED_Z 80//偏航(左右)最大设定速度 
int Vertical_out,Velocity_out,Turn_out;//直立环&速度环&转向环 的输出变量

int Vertical(float Med,float Angle,float gyro_Y);//函数声明
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z,int RC); 

void qianjin_Init (void)
{
	int PWM_out;
	
	if(EXTI_GetITStatus(EXTI_Line5)!=0)//一级判定
	{
		if(PBin(5)==0)//二级判定
		{
			EXTI_ClearITPendingBit(EXTI_Line5);//清除中断标志位
			
			//1、采集编码器数据&MPU6050角度信息。
			Encoder_Left=-Read_Speed(2);//电机是相对安装，刚好相差180度，为了编码器输出极性一致，就需要对其中一个取反。
			Encoder_Right=Read_Speed(4);
			
			mpu_dmp_get_data(&Roll,&Pitch,&Yaw);			//角度
			MPU_Get_Gyroscope(&gyroy,&gyrox,&gyroz);	//陀螺仪
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//加速度

			//1.5、二开部分
			/*********************************************************************************************/
			/*前后*/
			if((Fore==0)&&(Back==0))
				Target_Speed=0;
//				if(length<50)
//					Target_Speed++;
//				else
//					if(Target_Speed!=0)
//						Target_Speed--;//未接受到前进后退指令-->速度清零，稳在原地
			
			
			//2、将数据压入闭环控制中，计算出控制输出量。
			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);	//速度环
			Vertical_out=Vertical(Velocity_out+Med_Angle,Pitch,gyroy);			//直立环
			Turn_out=Turn(gyroz,Turn_Speed);																//转向环
			
			PWM_out=Vertical_out;//最终输出
			//3、把控制输出量加载到电机上，完成最终的的控制。
			MOTO1=PWM_out-Turn_out;//左电机
			MOTO2=PWM_out+Turn_out;//右电机
			Limit(&MOTO1,&MOTO2);	 //PWM限幅			
			Load(MOTO1,MOTO2);		 //加载到电机上。
			
			Stop(&Med_Angle,&Pitch);//安全检测
			
		}
	}
   	delay_s(5);
	MPU_IIC_Stop();	
}
	