#include "stm32f10x.h"                  // Device header

void Buzzer_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//设置为推挽输出
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStruct);
		GPIO_SetBits (GPIOA,GPIO_Pin_3);
}

void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
}
void Buzzer_OFF(void)
{
	GPIO_SetBits (GPIOA,GPIO_Pin_3);
}
void Buzzer_TURN(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_3)==0)
	{
		GPIO_SetBits (GPIOA,GPIO_Pin_3);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	}
}
