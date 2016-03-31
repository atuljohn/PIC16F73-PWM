/*
 * File:   pwm.c
 * Author: AtulJohn
 *
 * Created on February 18, 2016, 9:49 AM
 */


#include <xc.h>
#include <pic16f87.h>
#define _XTAL_FREQ 4000000
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config CP = OFF         // FLASH Program Memory Code Protection bit (Code protection off)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)

// CCP1 module is used here to generate the required PWM
// Timer2 module is used to generate the PWM
// This PWM has 10bit resolution


SetPWMDutyCycle(unsigned int DutyCycle) 
{
	CCPR1L   = DutyCycle>>2;        	// Put MSB 8 bits in CCPR1L
	CCP1CON &= ~(0x30);                	// Make bit4 and 5 zero
	CCP1CON |= (0x30&(DutyCycle<<4));   // Assign Last 2 LSBs to CCP1CON
}

InitPWM()
{
	TRISC2  = 0;            // Make CCP1 pin as output
    TRISB   = 0xFF;         // Make RB0 pin as input for increasing the duty cycle.
                            // Make RB1 pin as input for reducing the duty cycle.
	CCP1CON = 0x0C;         // Configure CCP1 module in PWM mode

	PR2   = 0xFF;           // Configure the Timer2 period
	T2CON = 0x01;           // Set Prescaler to be 4, hence PWM frequency is set to 4.88KHz.

	SetPWMDutyCycle(0);     // Intialize the PWM to 0 duty cycle

	T2CON |= 0x04;          // Enable the Timer2, hence enable the PWM.
}

main(void) {
      
    InitPWM();
    int dutycycle = 512;
    SetPWMDutyCycle(dutycycle);
    while(1)
    {
        if (!RB0)
        {
            __delay_ms(10);
            dutycycle++;
            SetPWMDutyCycle(dutycycle);
        }
            
        if (!RB1)
        {
            __delay_ms(10);
            dutycycle--;
            SetPWMDutyCycle(dutycycle);
        }
        
        if (dutycycle < 0 || dutycycle > 1024)
        {
            dutycycle = 512;
            SetPWMDutyCycle(dutycycle);
        }
    }
}
