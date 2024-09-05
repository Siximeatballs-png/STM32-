#include "Buzzer.h"
#include "control.h"

void baojing_Init(void)
{
	Buzzer_Init();
	if(Pitch>=50)
	{
		Buzzer_ON(); 
	}
	else if(Pitch<=-60)
	{
		Buzzer_ON();
	}
	else
	{
		Buzzer_OFF();
	}
	
}
