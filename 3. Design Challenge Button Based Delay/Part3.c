/*  
 * Part3.c
 *
 *  Created on: Feb 23, 2023
 *      Author: Matthew Currey
 *
 *      
 */

#define INITIAL_TIMER_VALUE 10000

#include <msp430.h>

unsigned long newtime = 0;             // Default blinking time value
unsigned int default_blink = 0;                  // Determine if LED will blink by default value or value of LED time pressed
                                            // 0 = timer not counting, 1 = timer counting
unsigned int rising_edge = 1;
unsigned int falling_edge = 0;


void gpioInit();
void timerInit();

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);

}


void gpioInit(){
    // Configure RED LED on P1.0 as Output
    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction

    // Configure Button on P2.3 as input with pullup resistor BLINK BUTTON
    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                         // P2.3 Low to High
    P2IE |= BIT3;                           // P2.3 interrupt enabled

    // Configure Button on P4.1 as input with pullup resistor RESET BUTTON
    P4OUT |= BIT1;                          // Configure P4.1 as pulled-up
    P4REN |= BIT1;                          // P4.1 pull-up register enable
    P4IES &= ~BIT1;                         // P4.1 Low to High
    P4IE |= BIT1;                           // P4.1 interrupt enabled
}

void timerInit(){
    // Setup Timer Compare IRQ
    TB0CCTL0 |= CCIE;                       // Enable TB0 CCR0 Overflow IRQ
    TB0CCR0 = 1;
    TB0CTL = TBSSEL_1 | MC_2 | ID_3;               // ACLK, continuous mode

    // Setup Timer Compare IRQ
    TB1CCTL0 |= CCIE;                       // Enable TB1 CCR0 Overflow IRQ
    TB1CCR0 = INITIAL_TIMER_VALUE;
    TB1CTL = TBSSEL_1 | MC_2 | ID_3;               // ACLK, continuous mode
}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                            // Clear P2.3 interrupt flag
    if (rising_edge)
    {
        rising_edge = 0;
        falling_edge = 1;
        default_blink = 1;
        newtime = 0;
        P2IES &= ~BIT3;                         // P2.3 low to high
        P1OUT &= ~BIT0; //Led is Cleared

    }
    else if (falling_edge)
    {
        rising_edge = 1;
        falling_edge = 0;  //Go to rising edge loop
        default_blink = 0;


        P2IES |= BIT3;                         // P2.3 high to low


    }

}

// Port 4 interrupt service routine
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;                         // Clear P4.1 interrupt flag
    newtime = INITIAL_TIMER_VALUE;      // Reset timer value to initialized value 10000
    default_blink = 0;
}

// Timer B0 interrupt service routine
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void)
{
    if (default_blink)  // If button pressed, continue to count
        newtime++;
                                            // to add to time of interrupt for LED blinking
    else
        newtime = newtime; // once button is released
    TB0CCR0 += 1;
}

// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    P1OUT ^= BIT0;                          //  Red LED toggle
    TB1CCR0 += newtime;                 // time between interrupts
}





