;**************************************************************
;* LBE_Dragon12_Plus_Rev4                                     *
;* Assembly language routines for C function calls            *
;* Learning By Example Using C                                *
;* -- Programming the Dragon12-Plus Using CodeWarrior         *
;* by Richard E. Haskell                                      *
;* Copyright 2008                                             *
;**************************************************************


; export symbols
            XDEF asm_main, PLL_init, led_enable, led_disable
            XDEF leds_on, leds_off, led_on, led_off
            XDEF seg7_enable, seg7_disable, seg7_on, seg7s_off   
            XDEF ms_delay, seg7dec
            XDEF SW_enable, SW2_down,SW2_up,SW3_down,SW3_up
            XDEF SW4_down,SW4_up,SW5_down,SW5_up, SW1_dip
            XDEF keypad_enable, keyscan,getkey,wait_keyup
            XDEF data8,lcd_init,clear_lcd, set_lcd_addr
            XDEF hex2lcd,hex2asc,type_lcd, write_int_lcd, write_long_lcd       
            XDEF SCI0_init, inchar0, outchar0 
            XDEF SCI1_init, inchar1, outchar1
            XDEF number 
            XDEF ad0_enable, ad0conv, ad1_enable, ad1conv
            XDEF servo54_init,servo76_init
            XDEF set_servo54, set_servo76
            XDEF motor0_init, motor1_init, motor2_init,motor3_init
            XDEF motor4_init, motor5_init, motor6_init,motor7_init
            XDEF motor0, motor1, motor2, motor3 
            XDEF motor4, motor5, motor6, motor7 
            XDEF SPI0_init, SPI1_init, SPI2_init
            XDEF send_SPI0, send_SPI1, send_SPI2
            XDEF SS0_HI, SS1_HI, SS2_HI
            XDEF SS0_LO, SS1_LO, SS2_LO
            XDEF RTI_init, clear_RTI_flag, RTI_disable
            XDEF SCI0_int_init, read_SCI0_Rx             
            XDEF ptrain6_init, ptrain6
            XDEF sound_init, sound_on, sound_off, tone
            XDEF HILO1_init, HILOtimes1
            XDEF get_HI_time1, get_LO_time1
            XDEF fill_weights, fire_rules, calc_output


; include derivative specific macros
            INCLUDE 'mc9s12dg256.inc'

; variable/data section
MY_EXTENDED_RAM: SECTION
ONE_MS:		equ	4000	; 4000 x 250ns = 1 ms at 24 MHz bus speed
FIVE_MS:	equ	20000
TEN_MS:		equ	40000
FIFTY_US:	equ	200
DB2		    equ     4       ; 00000100
DB3:		  equ	8       ; 00001000
REG_SEL:	equ	DB3	; 0=reg, 1=data
NOT_REG_SEL:	equ	$F7     ; 11110111
ENABLE:		equ     DB2
NOT_ENABLE: 	equ     $FB     ; 11111011
LCD:		  equ	PTM
LCD_RS:		equ	PTM
LCD_EN:		equ	PTM
REGBLK:		equ	$0
bas10:    equ	10

temp:     rmb 1
pmimg:		rmb	1
temp1:		rmb	1
bas:      rmb 2
dnum:	    rmb	4
buff:	    rmb	12
pad:	    rmb	1
TC1old:   rmb 2
LO_time1  rmb 2
HI_time1  rmb 2
LCDimg:		equ	pmimg
LCD_RSimg:	equ	pmimg
LCD_ENimg:	equ	pmimg

; code section
MyCode:     SECTION

MASK:     DC.B    1,2,4,8,16,32,64,128

SEG7TBL:  DC.B    $3F,$06,$5B,$4f
					DC.B    $66,$6D,$7D,$07
					DC.B    $7F,$6F,$77,$7C
					DC.B    $39,$5E,$79,$71
					
keycodes:		 
	          dc.b	$7D	; 0
	          dc.b	$EE	; 1
	          dc.b	$ED	; 2
	          dc.b	$EB	; 3
	          dc.b	$DE	; 4
	          dc.b	$DD	; 5
	          dc.b	$DB	; 6
	          dc.b	$BE	; 7
	          dc.b	$BD	; 8
	          dc.b	$BB	; 9
	          dc.b	$E7	; A
	          dc.b	$D7	; B
	          dc.b	$B7	; C
	          dc.b	$77	; D
	          dc.b	$7E	; E/*
	          dc.b	$7B	; F/#

asm_main:

PLL_init:
          movb    #$02,SYNR         ;PLLOSC = 48 MHz
          movb    #$00,REFDV
          clr     CLKSEL
          movb    #$F1,PLLCTL
pll1:     brclr   CRGFLG,#$08,pll1  ;wait for PLL to lock
          movb    #$80,CLKSEL       ;select PLLCLK
          rts

;         LEDs          
led_enable:
            movb  #$FF,DDRB   ;	 DataDirB -->all outputs
            movb  #$FF,DDRJ   ;	 DataDirJ -->all outputs
            bclr  PTJ,$02     ;  enable leds PJ1 = 0
            clr   PORTB       ;  Turn-off all LEDS
            rts

leds_on:
            stab  PORTB       ; turn on selected led
            rts

leds_off:
            clr  PORTB        ; turn off all leds
            rts

led_disable:
            movb  #$FF,DDRJ   ;	 DataDirJ -->all outputs
            bset  PTJ,$02     ;  enable leds PJ1 = 0
            rts
            

;         7-Segment Displays
seg7_enable:
            movb  #$FF,DDRB   ;	 DataDirB -->all outputs
            movb  #$FF,DDRP   ;	 DataDirP -->all outputs
            bclr  PTP,$0F     ;  enable 7-seg digits PTP[0:3] = 0000
            clr   PORTB       ;  Turn-off all 7-seg digits
            rts

seg7_disable:
            movb  #$FF,DDRP   ;	 DataDirP -->all outputs
            bset  PTP,$0F     ;  disable 7-seg digits PTP[0:3] = 1111
            rts

;       display selected segments on one digit
; void seg7_on(int segs, int digit#);
; digit# is in D
; segs is at 2,sp  (hex value to store in Port B)
seg7_on:
          ldy     #MASK
          aby
          ldaa    0,y         ;A = mask
          coma
          staa    PTP         ;enable digit digit#
          ldd     2,sp        ;B = segs
          stab    PORTB
          rts

seg7s_off:
            clr  PORTB        ; turn off all 7-segment displays
            rts

            
;       ms_delay                              
;       input: D = no. of milliseconds to delay
;       clock = 24 MHz
ms_delay:
          pshx
          pshy
          tfr     D,Y
md1:      ldx     #5999   ; N = (24000 - 5)/4
md2:      dex             ; 1 ccycle
          bne     md2     ; 3 ccycle
          dey             
          bne     md1     ; Y ms
          puly
          pulx
          rts

;       led_on( b# )
;       set bit number b# of PORTB to 1
led_on:     
	        ldy     #MASK            
	        aby
	        ldaa    0,Y                    
	        oraa    PORTB             ;OR mask with PORTB 
	        staa    PORTB             ;store back in PORTB
	        rts

;       led_off( b# )
;       clear bit number b# of PORTB to 0
led_off:     
	        ldy     #MASK            
	        aby   
	        ldaa    0,Y                    
	        coma                    ;complement mask
	        anda    PORTB             ;AND mask with PORTB 
	        staa    PORTB             ;store back in PORTB
	        rts

;       7-segment decoder
; void seg7dec(int digit, int digit#);
; digit# is in D
; digit is at 2,sp  (index into SEG7TBL)
seg7dec:
          ldy     #MASK
          aby
          ldaa    0,y         ;A = mask
          coma
          staa    PTP         ;enable digit digit#
          ldd     2,sp        ;B = digit
          ldy     #SEG7TBL
          aby                 ;y -> 7-seg code
          ldaa    0,y
          staa    PORTB
          rts
                    
; Pushbutton and DIP switches
SW_enable:
            clr  DDRH   ;Port H inputs
            rts

SW1_dip:
            ldab  PTH   ;Read Port H 
            rts
                        
SW2_down:         ;return true is SW2 is down
            clra
            clrb
            brset  PTH, #$08,SW2end1             
            ldd   #$FFFF
SW2end1:    rts


SW2_up:           ;return true is SW2 is up
            clra
            clrb
            brclr  PTH, #$08,SW2end2             
            ldd   #$FFFF
SW2end2:    rts


SW3_down:         ;return true is SW3 is down
            clra
            clrb
            brset  PTH, #$04,SW2end1             
            ldd   #$FFFF
SW3end1:    rts


SW3_up:           ;return true is SW3 is up
            clra
            clrb
            brclr  PTH, #$04,SW2end2             
            ldd   #$FFFF
SW3end2:    rts


SW4_down:         ;return true is SW4 is down
            clra
            clrb
            brset  PTH, #$02,SW4end1             
            ldd   #$FFFF
SW4end1:    rts


SW4_up:           ;return true is SW4 is up
            clra
            clrb
            brclr  PTH, #$02,SW4end2             
            ldd   #$FFFF
SW4end2:    rts


SW5_down:         ;return true is SW5 is down
            clra
            clrb
            brset  PTH, #$01,SW5end1             
            ldd   #$FFFF
SW5end1:    rts


SW5_up:           ;return true is SW5 is up
            clra
            clrb
            brclr  PTH, #$01,SW5end2             
            ldd   #$FFFF
SW5end2:    rts




; Enable keypad
keypad_enable:
            movb  #$0f,DDRA   ;A7:A4 inputs, A3:A0 outputs
            bset  PUCR, $01  ;enable pullup resistors
            rts

;	scan all keys
;	B = key pressed
;	if B = #$10, no key pressed
keyscan:
          	clrb			        ; B = index
	          ldx	  #keycodes
ks1	        ldaa	b,x
	          staa	PORTA		    ;write next code
	          anda  #$F0        ;save high nibble
	          staa  temp
   	        ldaa  #10			    ;wait to settle down
ks2         deca
	          bne   ks2
	          ldaa	PORTA		    ;read it back
	          anda  #$F0        ; check high nibble
	          cmpa	temp		    ;if key pressed,
	          beq	  ks3		        ; B = key
	          incb			        ;else, inx index
	          cmpb	#$10		    ; and scan all keys
	          bne	  ks1		        ;if no key pressed
ks3	        clra								; B = #$10
	          rts			          ;B = key value

;	wait to press a key
;	B = key value
keypad:
	          bsr	  keyscan	      ;scan keypad
	          cmpb	#$10		    ;until key pressed
	          beq	  keypad
	          rts

;	debounced key input
;	B = key value
getkey:
gk1	        bsr	  keypad		    ;wait for key
	          pshb				      ;save key value
	          ldd	  #10
	          jsr	  ms_delay		  ;delay ~10 ms
	          bsr	  keypad		    ;wait for key
	          pula				      ;get 1st value
	          sba				        ;if not same as 2nd
	          bne	  gk1		      ;repeat
	          rts				        ;D = key value

;	wait to lift finger from key (with debounce)
wait_keyup:
	          bsr	  keyscan		    ;scan keypad
	          cmpb	#$10			  ;while key is pressed
	          bne	  wait_keyup
	          ldd	  #10
	          jsr	  ms_delay		  ;delay ~10 ms
	          bsr	  keyscan		    ;scan keypad
	          cmpb	#$10			  ;if key is pressed
	          bne	  wait_keyup		;repeat
	          rts

;  Initialize LCD
lcd_init:
	      ldaa	#$ff
	      staa	DDRK		              ; port K = output
       	ldx	  #init_codes 	        ; point to init. codes.
	      pshb            	          ; output instruction command.
       	ldab   	1,x+                ; no. of codes
lcdi1:	ldaa   	1,x+                ; get next code
       	jsr    	write_instr_nibble 	; initiate write pulse.
       	pshd
       	ldd     #5
       	jsr     ms_delay		;delay 5 ms
       	puld
       	decb                    ; in reset sequence to simplify coding
       	bne    	lcdi1
       	pulb
       	rts

; Initialization codes for 4-bit mode      	
; uses only data in high nibble
init_codes: 
        fcb	12		; number of high nibbles
	      fcb	$30		; 1st reset code, must delay 4.1ms after sending
        fcb	$30		; 2nd reste code, must delay 100us after sending
        ;  following 10 nibbles must  delay 40us each after sending
	      fcb $30   ; 3rd reset code,
	      fcb	$20		; 4th reste code,
        fcb	$20   ; 4 bit mode, 2 line, 5X7 dot
	      fcb	$80   ; 4 bit mode, 2 line, 5X7 dot
        fcb	$00		; cursor increment, disable display shift
	      fcb	$60		; cursor increment, disable display shift
        fcb	$00		; display on, cursor off, no blinking
	      fcb	$C0		; display on, cursor off, no blinking
	      fcb	$00		; clear display memory, set cursor to home pos
	      fcb	$10		; clear display memory, set cursor to home pos

; write instruction upper nibble
write_instr_nibble:
        anda    #$F0
        lsra
        lsra            ; nibble in PK2-PK5
        oraa    #$02    ; E = 1 in PK1; RS = 0 in PK0
        staa    PORTK
        ldy     #10
win     dey
        bne     win
        anda    #$FC    ; E = 0 in PK1; RS = 0 in PK0
        staa    PORTK
        rts

; write data upper nibble
write_data_nibble:
        anda    #$F0
        lsra
        lsra            ; nibble in PK2-PK5
        oraa    #$03    ; E = 1 in PK1; RS = 1 in PK0
        staa    PORTK
        ldy     #10
wdn     dey
        bne     wdn
        anda    #$FD    ; E = 0 in PK1; RS = 1 in PK0
        staa    PORTK
        rts

; write instruction byte
write_instr_byte:
        psha
        jsr     write_instr_nibble
        pula
        asla
        asla
        asla
        asla
        jsr     write_instr_nibble
        rts

;write data byte
write_data_byte:
        psha
        jsr     write_data_nibble
        pula
        asla
        asla
        asla
        asla
        jsr     write_data_nibble
        rts
        
;   write instruction byte B to LCD
instr8:
            tba
;            jsr   sel_inst
            jsr   write_instr_byte
            ldd     #10
            jsr     ms_delay
            rts

;   write data byte B to LCD
data8:
            tba
;            jsr   sel_data
            jsr   write_data_byte
            ldd     #10
            jsr     ms_delay
            rts

;   set address to B
set_lcd_addr:
            orab    #$80
            tba
            jsr     write_instr_byte
            ldd     #10
            jsr     ms_delay
            rts

;   clear LCD
clear_lcd:
            ldaa    #$01
            jsr     write_instr_byte
            ldd     #10
            jsr     ms_delay
            rts
                                    
;	display hex value in B on LCD 
hex2lcd:  
	          bsr	hex2asc		  ;convert to ascii
	          jsr	data8		    ;display it
	          rts

;       Hex to ascii subroutine
;       input: B = hex value
;       output: B = ascii value of lower nibble of input
hex2asc:
     	      andb    #$0f    	;mask upper nibble
            cmpb    #$9     	;if B > 9
            bls     ha1     
          	addb    #$37    	; add $37
           	rts             	  ;else
ha1     	  addb    #$30    	; add $30
            rts

;	display asciiz string on LCD
;	D -> asciiz string 
type_lcd:
            pshx              ;save X
            tfr     D,X       ;X -> asciiz string
next_char   ldaa	  1,X+		  ;get next char
	          beq	    done		  ;if null, quit
	          jsr	    write_data_byte	;else display it
            ldd     #10
            jsr     ms_delay
	          bra	    next_char	;and repeat
done	      pulx              ;restore X
            rts

;   write an integer to the LCD display
;   write_int_lcd(int);
write_int_lcd:
            pshd              ;save D
            bsr     blank_pad ;fill pad with blanks
            puld              ;get D
            std     dnum+2
            clr     dnum
            clr     dnum+1
            ldx     #pad
            jsr     sharps
            ldx     #pad-5 
wl1	        ldab	  1,x+
	          jsr	    data8		;display the ascii string
	          cpx	    #pad
	          blo	    wl1
	          rts

;   write an integer to the LCD display
;   write_long_lcd(long);
write_long_lcd:
            pshd              ;save D
            pshx              ;save X
            bsr     blank_pad ;fill pad with blanks
            pulx              ;get X
            puld              ;get D
            std     dnum+2
            stx     dnum
            ldx     #pad
            jsr     sharps
            ldx     #pad-10 
wll1	      ldab	  1,x+
	          jsr	    data8		;display the ascii string
	          cpx	    #pad
	          blo	    wll1
	          rts

;   blank pad
blank_pad:
            ldx     #buff
            ldab    #13
            ldaa    #$20    ;ascii blank
bp1:        staa    1,x+
            decb
            bne     bp1
            rts            
            
SCI0_init: 												
            CLR SCI0CR1
            MOVB #$0C, SCI0CR2
            TFR D,X
            LDY #$0016    ; 24 mhz
            LDD #$E360
            EDIV
            STY SCI0BDH
            RTS
            
inchar0:
            LDAA  SCI0SR1
            ANDA  #$20
            BEQ   inchar0
            LDAB  SCI0DRL
            RTS
            
outchar0:
            TST   SCI0SR1
            BPL   outchar0
            STAB  SCI0DRL
            RTS

SCI1_init: 												
            CLR SCI1CR1
            MOVB #$0C, SCI1CR2
            TFR D,X
            LDY #$0016    ; 24 mhz
            LDD #$E360
            EDIV
            STY SCI1BDH
            RTS
            
inchar1:
            LDAA  SCI1SR1
            ANDA  #$20
            BEQ   inchar1
            LDAB  SCI1DRL
            RTS
            
outchar1:
            TST   SCI1SR1
            BPL   outchar1
            STAB  SCI1DRL
            RTS
                                                                        

;	double division  32 / 16 = 32  16 rem
;	numH:numL / denom = quotH:qoutL  remL
;	Y:D / X = Y:D  rem X
;	use EDIV twice  Y:D / X = Y   rem D
ddiv:
	          pshd		        ;save numL
	          tfr	  y,d	      ;d = numH
	          ldy	  #0	      ;0:numH / denom
	          ediv		        ;y = quotH, d = remH
	          bcc	  dd1	      ;if div by 0
	          puld	
	          ldd	  #$FFFF	  ;quot = $FFFFFFFF
	          tfr	  d,y
	          tfr	  d,x	      ;rem = $FFFF
	          rts
dd1	        sty	  2,-sp	    ;save quotH on stack
	          tfr	  d,y	      ;y = remH
	          ldd	  2,sp	    ;d = numL
	          ediv		        ;remH:numL/denom  Y = quotL  D = remL
	          tfr	  d,x	      ;x = remL
	          tfr	  y,d	      ;d = quotL
	          puly		        ;y = quotH
	          leas	2,sp	    ;fix stack
	          rts

;   Binary number to ASCII string conversion
;	  x -> ascii buffer
sharp:
	          pshd			      ;save regs
	          pshy			      
	          pshx			      ;save ptr
	          ldy	  dnum
	          ldd	  dnum+2
	          ldx	  #bas10
	          jsr	  ddiv		  ;dnum/base rem in X
	          sty	  dnum		  ; => dnum
	          std	  dnum+2
	          tfr	  x,d		    ;b = rem
	          cmpb	#9		    ;if rem > 9
	          bls	  shp1
	          addb	#7		    ; add 7
shp1	      addb	#$30		  ;conv to ascii
	          pulx			      ;restore ptr
	          stab	1,-x		  ;store digit
	          puly			      ;restore regs
	          puld
	          rts

;	input: x -> pad (ascii buffer)
;	output: x -> first char in ascii string
sharps:
	          bsr	  sharp		  ;do next digit
	          ldd	  dnum		  ;repeat until
          	bne	  sharps		; quot = 0
	          ldd	  dnum+2
	          bne	  sharps
	          rts


;	input: A = ascii code of char
;	output: if carry=0 A=valid hex value of char
;	        if carry=1 A=invalid char in current base
digit:
	          pshb
	          psha
	          suba	#$30		  ;ascii codes < 30
	          blo	  dgt2		  ; are invalid
	          cmpa	#9		    ;char between
	          bls	  dgt1		  ; 9 and A
	          cmpa	#17		    ; are invalid
	          blo	  dgt2		  ;fill gap
	          suba	#7		    ; between 9&A
dgt1	      cmpa	bas+1		;digit must be
	          bhs	  dgt2		  ; < base
	          andcc	#$FE		  ;clear carry (valid)
	          pulb			      ;pop old A
          	pulb			      ;restore B
	          rts
dgt2	      pula			      ;restore A
	          pulb			      ;restore B
	          orcc	#$01		  ;set carry (invalid)
	          rts
      
;	input:  D -> ascii number string buffer
; output: X:D is long int number
number:
	          pshy
	          tfr   d,y       ;y -> kbuf
	          ldd   #bas10
	          std   bas       ;base = 10
	          ldx	  #bas	    ;x -> base
	          ldd	  #0
	          std	  2,x	      ;clear dnum
	          std	  4,x
num1	      ldaa	1,y+	    ;get next digit
	          jsr	  digit	    ;conv to value
	          bcs	  num2
	          jsr	  dumul	    ;mult dnum by base
	          adda	5,x	      ;add digit value
	          staa	5,x
	          ldaa	4,x
	          adca	#0
	          staa	4,x
	          ldaa	3,x
	          adca	#0
          	staa	3,x
	          ldaa	2,x
	          adca	#0
	          staa	2,x
	          bra	  num1	    ;do until invalid digit
num2	      ldx   dnum      
            ldd   dnum+2    ; X:D = dnum
            puly
    	      rts
      
;	32 x 16 = 32 unsigned multiply
;	A:B x C = pH:pL
;	A x C = ACH:ACL  (drop ACH)
;	B x C = BCH:BCL
;	pL= BCL
;	pH = ALC + BCH
; C	rmb	2
; A	rmb	2
; B	rmb	2

dumul:
            psha            ;save A
            pshy						;save Y
	          ldd	  0,x		    ;D = C
	          ldy	  2,x		    ;Y = A
	          emul			      ;Y = ACH, D = ACL
	          std	  2,x		    ;save ACL
	          ldd	  0,x		    ;D = C
	          ldy	  4,x		    ;Y = B
	          emul			      ;Y = BCH, D = BCL
	          std	  4,x		    ;save pL = BCL
	          tfr	  y,d		    ;D = BCH
	          addd	2,x		    ;D = BCH+ACL = pH
	          std	  2,x		    ;save pH
	          puly            ;restore Y
	          pula						;restore A
	          rts
      
; A/D converter PADD0-PADD7
;   ad0_enable();
ad0_enable:
	          ldaa	#$0B		    ;10-bit resolution  /24 clock
	          staa	ATD0CTL4
	          ldaa	#$c0		    ;set ADPU & AFFC
	          staa	ATD0CTL2
	          rts
      
;   int adconv(char ch#)
ad0conv:
	          andb	#$07		    ;ch. 0 - 7 
          	orab	#$80		    ;right just  SCAN=0  MULT=0
	          stab	ATD0CTL5 
ad01	      ldaa	ATD0STAT0	  ;wait for conv
	          anda	#$80
	          beq 	ad01
	          bsr	  avg40
	          rts
            
avg40:
	          pshx			        ;save reg
	          ldx	  #ATD0DR0H
	          ldd	  2,x+		    ;adr0
	          addd	2,x+		    ;+adr1
	          addd	2,x+		    ;+adr2
	          addd	2,x+		    ;+adr3
	          lsrd
	          lsrd			        ;divide by 4
	          pulx			        ;restore reg
	          rts

; A/D converter PAD8-PAD15
;   ad1_enable();
ad1_enable:
	          ldaa	#$0B		    ;10-bit resolution  /24 clock
	          staa	ATD1CTL4
	          ldaa	#$c0		    ;set ADPU & AFFC
	          staa	ATD1CTL2
	          rts
      
;   int adconv(char ch#)
ad1conv:
	          andb	#$07		    ;ch. 0 - 7 
          	orab	#$80		    ;right just  SCAN=0  MULT=0
	          stab	ATD1CTL5 
ad11	      ldaa	ATD1STAT0	  ;wait for conv
	          anda	#$80
	          beq 	ad11
	          bsr	  avg41
	          rts
            
avg41:
	          pshx			        ;save reg
	          ldx	  #ATD1DR0H
	          ldd	  2,x+		    ;adr0
	          addd	2,x+		    ;+adr1
	          addd	2,x+		    ;+adr2
	          addd	2,x+		    ;+adr3
	          lsrd
	          lsrd			        ;divide by 4
	          pulx			        ;restore reg
	          rts

; PWM -- motors  10 ms period   8-bit mode
;       inputs  0 - 255
;       motors input duty cycles between 0 - 100%

; motor0_init();	 Initialize pwm 0 - pin PP0
motor0_init:
            bset  PWME,   #$01      ;enable PWM0
            bset  PWMPOL, #$01      ;start high
            bset  PWMCLK, #$01      ;use clock SA
            movb  #$22,   PWMPRCLK  ;/4  24/4 = 6 MHz
            movb  #$75,   PWMSCLA   ;SA = 6MHz/2*117 = 25.64KHz
            clr   PWMCAE            ;left align
            bclr  PWMCTL, #$10      ;8-bit channels 0 and 1
            movb  #255,   PWMPER0   ;period =~ 10ms
            movb  #128,   PWMDTY0   ;initial duty cycle = 50%
            rts

; motor1_init();	 Initialize pwm 1 - pin PP1
motor1_init:
            bset  PWME,   #$02      ;enable PWM1
            bset  PWMPOL, #$02      ;start high
            bset  PWMCLK, #$02      ;use clock SA
            movb  #$22,   PWMPRCLK  ;/4  24/4 = 6 MHz
            movb  #$75,   PWMSCLA   ;SA = 6MHz/2*117 = 25.64KHz
            clr   PWMCAE            ;left align
            bclr  PWMCTL, #$10      ;8-bit channels 0 and 1
            movb  #255,   PWMPER1   ;period =~ 10ms
            movb  #128,   PWMDTY1   ;initial duty cycle = 50%
            rts

; motor2_init();	 Initialize pwm 2 - pin PP2
motor2_init:
            bset  PWME,   #$04      ;enable PWM2
            bset  PWMPOL, #$04      ;start high
            bset  PWMCLK, #$04      ;use clock SB
            movb  #$22,   PWMPRCLK  ;/4  24/4 = 6 MHz
            movb  #$75,   PWMSCLB   ;SB = 6MHz/2*117 = 25.64KHz
            clr   PWMCAE            ;left align
            bclr  PWMCTL, #$20      ;8-bit channels 2 and 3
            movb  #255,   PWMPER2   ;period =~ 10ms
            movb  #128,   PWMDTY2   ;initial duty cycle = 50%
            rts

; motor3_init();	 Initialize pwm 3 - pin PP3
motor3_init:
            bset  PWME,   #$08      ;enable PWM0
            bset  PWMPOL, #$08      ;start high
            bset  PWMCLK, #$08      ;use clock SB
            movb  #$22,   PWMPRCLK  ;/4  24/4 = 6 MHz
            movb  #$75,   PWMSCLB   ;SB = 6MHz/2*117 = 25.64KHz
            clr   PWMCAE            ;left align
            bclr  PWMCTL, #$20      ;8-bit channels 2 and 3
            movb  #255,   PWMPER3   ;period =~ 10ms
            movb  #128,   PWMDTY3   ;initial duty cycle = 50%
            rts

; motor4_init();	 Initialize pwm 4 - pin PP4
motor4_init:
            bset  PWME,   #$10      ;enable PWM4
            bset  PWMPOL, #$10      ;start high
            bset  PWMCLK, #$10      ;use clock SA
            movb  #$22,   PWMPRCLK  ;/4  24/4 = 6 MHz
            movb  #$75,   PWMSCLA   ;SA = 6MHz/2*117 = 25.64KHz
            clr   PWMCAE            ;left align
            bclr  PWMCTL, #$40      ;8-bit channels 4 and 5
            movb  #255,   PWMPER4   ;period =~ 10ms
            movb  #128,   PWMDTY4   ;initial duty cycle = 50%
            rts

; motor5_init();	 Initialize pwm 5 - pin PP5
motor5_init:
            bset  PWME,   #$20      ;enable PWM5
            bset  PWMPOL, #$20      ;start high
            bset  PWMCLK, #$20      ;use clock SA
            movb  #$22,   PWMPRCLK  ;/4  24/4 = 6 MHz
            movb  #$75,   PWMSCLA   ;SA = 6MHz/2*117 = 25.64KHz
            clr   PWMCAE            ;left align
            bclr  PWMCTL, #$40      ;8-bit channels 4 and 5
            movb  #255,   PWMPER5   ;period =~ 10ms
            movb  #128,   PWMDTY5   ;initial duty cycle = 50%
            rts

; motor6_init();	 Initialize pwm 6 - pin PP6
motor6_init:
            bset  PWME,   #$40      ;enable PWM6
            bset  PWMPOL, #$40      ;start high
            bset  PWMCLK, #$40      ;use clock SB
            movb  #$22,   PWMPRCLK  ;/4  24/4 = 6 MHz
            movb  #$75,   PWMSCLB   ;SB = 6MHz/2*117 = 25.64KHz
            clr   PWMCAE            ;left align
            bclr  PWMCTL, #$80      ;8-bit channels 6 and 7
            movb  #255,   PWMPER6   ;period =~ 10ms
            movb  #128,   PWMDTY6   ;initial duty cycle = 50%
            rts

; motor7_init();	 Initialize pwm 7 - pin PP7
motor7_init:
            bset  PWME,   #$80      ;enable PWM7
            bset  PWMPOL, #$80      ;start high
            bset  PWMCLK, #$80      ;use clock SB
            movb  #$22,   PWMPRCLK  ;/4  24/4 = 6 MHz
            movb  #$75,   PWMSCLB   ;SB = 6MHz/2*117 = 25.64KHz
            clr   PWMCAE            ;left align
            bclr  PWMCTL, #$80      ;8-bit channels 6 and 7
            movb  #255,   PWMPER7   ;period =~ 10ms
            movb  #128,   PWMDTY7   ;initial duty cycle = 50%
            rts

                        
; MOTORS -- Input: duty cycle between 0 and 100%
; motor0(int duty);
motor0:
            stab   PWMDTY0						;set new pulse width
            rts
            
; motor1(int duty);
motor1:
            stab   PWMDTY1						;set new pulse width
            rts
            
; motor2(int duty);
motor2:
            stab   PWMDTY2						;set new pulse width
            rts
            
; motor3(int duty);
motor3:
            stab   PWMDTY3						;set new pulse width
            rts
            
; motor4(int duty);
motor4:
            stab   PWMDTY4						;set new pulse width
            rts
            
; motor5(int duty);
motor5:
            stab   PWMDTY5						;set new pulse width
            rts
            
; motor6(int duty);
motor6:
            stab   PWMDTY6						;set new pulse width
            rts
            
; motor7(int duty);
motor7:
            stab   PWMDTY7						;set new pulse width
            rts
            


; PWM -- servos  20 ms period   16-bit mode
;        pulse width 1.1 ms - 1.5 ms - 1.9 ms
;        for duty inputs  -3300    4500      +5700

; servo54_init();	 Initialize pwm 5 - pin PP5
servo54_init:
            bset  PWME,     #$20    ;enable PWM5
            bset  PWMPOL,   #$20    ;start high
            bclr  PWMCLK,   #$20    ;use clock A
            movb  #$33,   PWMPRCLK  ;/8  24/8 = 3 MHz
            clr   PWMCAE            ;left align
            bset  PWMCTL,   #$40    ;concatenate 4 and 5
            movw  #60000,   PWMPER4 ;period = 20ms
            movw  #4500,    PWMDTY4 ;initial pulse width = 1.52ms
            rts
            
; set duty cycle
; set_servo54(int width);
set_servo54:
            std   PWMDTY4           ;set new pulse width
            rts

; servo76_init();	 Initialize pwm 7 - pin PP7
servo76_init:
            bset  PWME,     #$80    ;enable PWM7
            bset  PWMPOL,   #$80    ;start high
            bclr  PWMCLK,   #$80    ;use clock B
            movb  #$33,   PWMPRCLK  ;/8  24/8 = 3 MHz
            clr   PWMCAE            ;left align
            bset  PWMCTL,   #$80    ;concatenate 6 and 7
            movw  #60000,   PWMPER6 ;period = 20ms
            movw  #4500,    PWMDTY6 ;initial pulse width = 1.5ms
            rts
            
; set duty cycle
; set_servo76(int width);
set_servo76:
            std   PWMDTY6           ;set new pulse width
            rts

; SPI ports
; Initialize SPI with baud rate of 250 KHz
; void SPI0_init();
SPI0_init:
            movb  #$53, SPI0BR      ;divide 24 MHz clock by 96
            movb  #$50, SPI0CR1     ;master, CPHA=0, CPOL=0
            clr   SPI0CR2           ;disable MODF
            bset  DDRS, #$80        ;SS0 an output port
            bclr  PTS,  #$80        ;SS0 = 0
            rts
            
; void SPI1_init();
SPI1_init:
            movb  #$53, SPI1BR      ;divide 24 MHz clock by 96
            movb  #$50, SPI1CR1     ;master, CPHA=0, CPOL=0
            clr   SPI1CR2           ;disable MODF
            bset  DDRP, #$08        ;SS1 an output port
            bclr  PTP,  #$08        ;SS1 = 0
            rts
            
; void SPI2_init();
SPI2_init:
            movb  #$53, SPI2BR      ;divide 24 MHz clock by 96
            movb  #$50, SPI2CR1     ;master, CPHA=0, CPOL=0
            clr   SPI2CR2           ;disable MODF
            bset  DDRP, #$40        ;SS2 an output port
            bclr  PTP,  #$40        ;SS2 = 0
            rts
            
; send byte out SPI port and receive byte in
; char send_SPI0(char);
send_SPI0:
snd0        brclr SPI0SR,#$20,snd0   ;wait for SPTEF=1
            stab  SPI0DR             ;start sending byte
snd01       brclr SPI0SR,#$80,snd01  ;wait until sent
            ldab  SPI0DR             ;get received byte
            rts            

; char send_SPI1(char);
send_SPI1:
snd1        brclr SPI1SR,#$20,snd1   ;wait for SPTEF=1
            stab  SPI1DR             ;start sending byte
snd11       brclr SPI1SR,#$80,snd11  ;wait until sent
            ldab  SPI1DR             ;get received byte
            rts            

; char send_SPI2(char);
send_SPI2:
snd2        brclr SPI2SR,#$20,snd2   ;wait for SPTEF=1
            stab  SPI2DR             ;start sending byte
snd21       brclr SPI2SR,#$80,snd21  ;wait until sent
            ldab  SPI2DR             ;get received byte
            rts            

; set SS high
; SS0_HI();
SS0_HI:
            bset  PTS, #$80         ;set bit 7 of port S
            rts
            
; SS1_HI();
SS1_HI:
            bset  PTP, #$08         ;set bit 3 of port P
            rts
            
; SS2_HI();
SS2_HI:
            bset  PTP, #$40         ;set bit 6 of port P
            rts
            
; set SS low
; SS0_LO();
SS0_LO:
            bclr  PTS, #$80         ;clear bit 7 of port S
            rts
            
; SS1_LO();
SS1_LO:
            bclr  PTP, #$08         ;clear bit 3 of port P
            rts
            
; SS2_LO();
SS2_LO:
            bclr  PTP, #$40         ;clear bit 6 of port P
            rts
            
;   Real-time interrupt
;   RTI_init();
RTI_init:
  	        sei			          ;disable interrupts
		        ldaa	#$54
		        staa	RTICTL	    ;set rti to 10.24 ms
		        ldaa	#$80
		        staa	CRGINT	    ;enable rti
		        cli			          ;enable interrupts
		        rts

;   clear_RTI_flag();
clear_RTI_flag:
		        ldaa	#$80
		        staa	CRGFLG	    ;clear rti flag
		        rts

;   disable RTI
;   RTI_disable():
RTI_disable:
            clr   CRGINT      ;disable rti
            rts
                        
;   SCI0 receive interrupt setup 9600 baud            
;   void SCI0_int_init(int)
SCI0_int_init: 												
            CLR   SCI0CR1
            MOVB  #$2C, SCI0CR2			;enable TE, RE, RX int
            TFR   D,X
            LDY   #$0016              ; 24 mhz
            LDD   #$E360
            EDIV
            STY   SCI0BDH
            CLI                     ;enable interrupts
            RTS


;   Read Rx byte
;   char read_SCI0_Rx()
read_SCI0_Rx:
            LDAA  SCI0SR1						; clears RDRF flag
            LDAB  SCI0DRL						; return data
            RTS

;  ptrain6_init()
ptrain6_init:
            movb  #$FF,DDRT   ;outputs
            movb  #$C0,TIOS   ;select output compares 6 & 7
            movb  #$04,TSCR2  ;div by 16: 24MHz/16 = 1.5 MHz
            movb  #$80,TSCR1  ;enable timer
            ldd   TCNT
            std   TC6 
            std   TC7					;init cnt in TC6 & TC7
            bset  OC7M,#$40   ;pulse train out PT6
            bset  OC7D,#$40   ;PT6 goes high on TC7 match
            bset  TCTL1,#$20  ;PT6 low on TC6  match
            bclr  TCTL1,#$10
            bset  TIE,#$40    ;enable TC6 interrupts
            cli               ;enable interrupts
            rts
              
; void ptrain(int period, int pwidth);
; pwidth is in D
; period is at 2,sp
; return address is at 0,sp
ptrain6:
            pshd              ;save pwidth
            ldd   4,sp        ;D = period
            addd  TC7
            std   TC7         ;TC7new =TC7old + period
            addd  0,sp        ;add pwidth 
            std   TC6         ;TC6new =TC7new + pwidth
            puld              ;restore D
            movb  #$C0,TFLG1  ;clear both C7F and C6F
            rts
            
;  sound_init()
sound_init:
            movb  #$A0,TIOS   ;select output compares 5 & 7
            movb  #$04,TSCR2  ;div by 16: 24MHz/16 = 1.5 MHz
            movb  #$80,TSCR1  ;enable timer
            ldd   TCNT
            std   TC5 
            std   TC7					;init cnt in TC5 & TC7
            bset  OC7M,#$20   ;pulse train out PT5
            bset  OC7D,#$20   ;PT5 goes high on TC7 match
            bset  TCTL1,#$08  ;PT5 low on TC5  match
            bclr  TCTL1,#$04
            rts

;  sound_on()
sound_on:
            movb  #$80,TSCR1  ;enable timer
            bset  TIE,#$20    ;enable TC5 interrupts
            cli               ;enable interrupts
            rts

;  sound_off()
sound_off:
            sei
            clr   TSCR1       ;disable timer
            bclr  TIE,#$20    ;disable TC5 interrupts
            rts
            
; void tone(int pitch);
; pitch is in D
tone:
            pshd              ;save pitch
            addd  TC5
            std   TC7         ;TC7new =TC6old + pitch
            puld              ;get pitch
            addd  TC7         ;add pitch 
            std   TC5         ;TC6new =TC7new + pitch
            movb  #$A0,TFLG1  ;clear both C7F and C5F
            rts
            
; calc HI-LO times of pulse train on Ch 1 
;   and store results in HI_time1 and LO_time1            
; void HILO1_init(void);
HILO1_init:
            bclr  TIOS,#$02   ;select input capture 1
            movb  #$04,TSCR2  ;div by 16: 24MHz/16 = 1.5 MHz
            movb  #$80,TSCR1  ;enable timer
            ldd   TCNT
            std   TC1 				;init cnt in TC1
            bset  TCTL4,#$0C  ;interrupt on both edges of Ch 1
						movb  #$02,TFLG1  ;clear any old flag on Ch 1
            bset  TIE,#$02    ;enable TC1 interrupts
            cli               ;enable interrupts
						rts

; calc HI-LO times of pulse train on Ch 1 
;   and store results in HI_time1 and LO_time1            
; void getHILOtimes1(void);
HILOtimes1:
						brclr PTT,$02,HL1 ;if PTT1 is hi, rising edge
						ldd   TC1
						pshd              ;save TC1
						subd  TC1old      ;LO_time'
            std   LO_time1;   ;store LO_time1
            puld              ;get TC1
            std   TC1old;     ;TC1old = TC1
            bra   HL2
HL1:        ldd   TC1					;else, falling edge
						pshd              ;save TC1
						subd  TC1old      ;HI_time'
            std   HI_time1;   ;save HI_time1
            puld              ;get TC1
            std   TC1old;     ;TC1old = TC1
HL2:        bset  TFLG1, #$02 ;clear int flag            
            rts

; int get_HI_time1(void);
get_HI_time1:
            ldd   HI_time1
            rts
     
; int get_LO_time1(void);
get_LO_time1:
            ldd   LO_time1
            rts
            
; Fuzzy Control routines
; void fill_weights(unsigned char* weight, unsigned char* membx, int num_mem_fncs, char x)
fill_weights:
            pshb              ; save x
            ldd   3,sp        ; B = num_mem_funcs
            ldx   5,sp        ; X -> membx
            ldy   7,sp        ; Y -> weight
            pula              ; A = x
fw1:        mem               ; fuzzy membership grade
            dbne  B,fw1
            rts
            
; void fire_rules(unsigned char* inout_array, unsigned char* rules, unsigned char* out, int numout)
fire_rules:
            ldy   2,sp        ;Y -> out array
fr0         clr   1,Y+        ;clear out array
            dbne  B,fr0       ;B = numout
            ldx   4,sp        ;X -> rules
            ldy   6,sp        ;Y -> inout array
            ldaa  #$FF        ;must set A = $FF
            rev               ;rule evaluation
            rts
                          
; unsigned char calc_output(unsigned char* out, unsigned char* cent, int numout)
calc_output:                  ;B = numout
            ldx   2,sp        ;X -> cent array
            ldy   4,sp        ;Y -> out array
            wav
            ediv              ;Y = quotient
            tfr   Y,D         ;D = quot A = 0, B = output
            rts
                                                  
                        
                                       
            end
                 					