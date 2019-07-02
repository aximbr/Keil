This small project to test TIMER capability on STM32F407ZG
T1 - Internal clock at 16Mhz, one shot mode, load TIMER1 with prescaler and value to reach 1 sec 
and turn on LED on PF9

T2 - Internal clock at 16 MHz, periodic mode, load TIMER1 with prescaler and value to reach 1 sec 
and togle LED on PF9

T3 - Internal clock at 16 MHz, periodic mode, load TIMER1 with prescaler and value to reach 1 sec 
and togle LED on PF9 when Timeout Interrupt occurs

T4 - PWM experiment. Period is 1 sec, the pulse time is change from 0% to 100% of period, when the
count matchs the compare value an interrupt is set. When counter reachs zero another interrupt is
set.
