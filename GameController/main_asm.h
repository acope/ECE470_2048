#ifndef _MAIN_ASM_H
#define _MAIN_ASM_H

#ifdef __cplusplus
    extern "C" { /* our assembly functions have C calling convention */
#endif

void asm_main(void);
  /* interface to my assembly main function */
void PLL_init(void);
void led_enable(void);
void leds_on(int);
void leds_off(void);
void led_disable(void);
void led_on(int);
void led_off(int);
void seg7_enable(void);
void seg7_on(int, int);
void seg7_disable(void);
void seg7s_off(void);
void ms_delay(int);
void seg7dec(int,int);
void SW_enable(void);
short SW2_down(void);
short SW2_up(void);
short SW3_down(void);
short SW3_up(void);
short SW4_down(void);
short SW4_up(void);
short SW5_down(void);
short SW5_up(void);
char SW1_dip(void);
void  keypad_enable(void);
int keyscan(void);
char getkey(void);
void  wait_keyup(void);
void  instr8(char);
void  data8(char);
void  lcd_init(void);
void  clear_lcd(void);
void  hex2lcd(char);
char  hex2asc(char);
void  type_lcd(char*);
void  write_int_lcd(int);       
void  write_long_lcd(long);       
void  set_lcd_addr(char);
void  SCI0_init(int);
char  inchar0(void);
void  outchar0(unsigned char);
void  SCI1_init(int);
char  inchar1(void);
void  outchar1(unsigned char);
long  number(char*);
void  ad0_enable(void);
int   ad0conv(char);
void  ad1_enable(void);
int   ad1conv(char);
void  servo54_init(void);
void  servo76_init(void);
void  set_servo54(int);
void  set_servo76(int);
void  motor0_init(void);
void  motor1_init(void);
void  motor2_init(void);
void  motor3_init(void);
void  motor4_init(void);
void  motor5_init(void);
void  motor6_init(void);
void  motor7_init(void);
void  motor0(int);
void  motor1(int);
void  motor2(int);
void  motor3(int);
void  motor4(int);
void  motor5(int);
void  motor6(int);
void  motor7(int);
void  SPI0_init(void);
void  SPI1_init(void);
void  SPI2_init(void);
char  send_SPI0(char);
char  send_SPI1(char);
char  send_SPI2(char);
void  SS0_HI(void);
void  SS1_HI(void);
void  SS2_HI(void);
void  SS0_LO(void);
void  SS1_LO(void);
void  SS2_LO(void);
void  RTI_init(void);
void  clear_RTI_flag(void);
void  RTI_disable(void);
void  SCI0_int_init(int); 
char  read_SCI0_Rx(void);
void  ptrain6_init(void);
void  ptrain6(int,int);
void  sound_init(void);
void  sound_on(void);
void  sound_off(void);
void  tone(int);
void  HILO1_init(void);
void  HILOtimes1(void);
int   get_HI_time1(void);
int   get_LO_time1(void);
void  fill_weights(unsigned char*,unsigned char*,int,unsigned char);
void fire_rules(unsigned char*,unsigned char*,unsigned char*,int);
unsigned char calc_output(unsigned char*,unsigned char*,int);

#ifdef __cplusplus
    }
#endif

#endif /* _MAIN_ASM_H */
