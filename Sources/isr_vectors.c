extern void near irqISR(void); // irqISR () is in a different file
extern void near rtiISR(void); // rtiISR() is in a different file
// put the function name of your interrupt handler into this constant array
// at the ISR location that you want.
#pragma CODE_SEG_NEAR_SEG NON_BANKED /* interrupt section for this module. placement will be in NON_
BANKED area. */
_interrupt void UnimplementedISR(void)
{
for(;;); // do nothing, simply return
}
// added redirected ISR vectors when BootLoader is enabled.
// the application cannot have a reset vector (resides in BootLoader).
#pragma CODE_SEG DEFAULT /* change code section to default. */
typedef void (*near tIsrFunc)(void);
const tIsrFunc _vect[] @0xFF80 = {
/* Interrupt table */
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // PWM emergency shutdown
UnimplementedISR, // port P
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // CAN1 transmit
UnimplementedISR, // CAN1 receive
UnimplementedISR, // CAN1 errors
UnimplementedISR, // CAN1 wake-up
UnimplementedISR, // CAN0 transmit
UnimplementedISR, // CAN0 receive
UnimplementedISR, // CAN0 errors
UnimplementedISR, // CAN0 wake-up
UnimplementedISR, // flash
UnimplementedISR, // EEPROM
UnimplementedISR, // reserved
UnimplementedISR, // reserved
UnimplementedISR, // IIC bus
UnimplementedISR, // BDLC
UnimplementedISR, // CRG self clock mode
UnimplementedISR, // CRG PLL clock
UnimplementedISR, // pulse accumulator B overflow
UnimplementedISR, // modulus down counter underflow
UnimplementedISR, // portH
UnimplementedISR, // portJ
UnimplementedISR, // ATD1
UnimplementedISR, // ATD0
UnimplementedISR, // SCI1
UnimplementedISR, // SCI0
UnimplementedISR, // SPI0
UnimplementedISR, // pulse accumulator input edge
UnimplementedISR, // pulse accumulator A overflow
UnimplementedISR, // enhanced capture timer overflow
UnimplementedISR, // enhanced capture timer Ch7
UnimplementedISR, // enhanced capture timer Ch6
UnimplementedISR, // enhanced capture timer Ch5
UnimplementedISR, // enhanced capture timer Ch4
UnimplementedISR, // enhanced capture timer Ch3
UnimplementedISR, // enhanced capture timer Ch2
UnimplementedISR, // enhanced capture timer Ch1
UnimplementedISR, // enhanced capture timer Ch0
rtiISR, // real time interrupt
irqISR, // IRQ
UnimplementedISR, // XIRQ
UnimplementedISR, // swi
UnimplementedISR, // unimplemented instruction trap
UnimplementedISR, // COP failure reset
UnimplementedISR // clock monitor fail reset
/*_startup, by default in library*/ // reset vector
};