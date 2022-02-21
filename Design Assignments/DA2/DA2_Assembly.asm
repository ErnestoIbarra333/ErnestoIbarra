.ORG 0
	jmp main
.ORG 0x02
	JMP EXO_ISR
main:
	LDI R20, HIGH(RAMEND) // initializing the stack
	OUT SPH, R20
	LDI R20, LOW(RAMEND)
	OUT SPL, R20

	SBI DDRB,2 // makes portb.2 an output
	SBI DDRB,3 // makes portb.3 an output
	SBI PORTB, 2 // set LED off
	SBI PORTB, 3 // set LED off

	CBI DDRC,2 // makes portc.2 an input
	SBI PORTC, 2 // pulls up resistor pulled up
// Here is part 3 of the assignment
	LDI R20, 0x2
	STS EICRA,R20 //mkae it falling edge triggered
	SBI PORTD, 2 //enable pull up
	LDI R20, 1<<INT0
	OUT EIMSK, R20 // enable interrupt 0
	SEI // enable interrupts

// Here is Part 2 to turn on portb.2 for 1.25s after pressing a switch in portc.2
loop: 
	ldi R22,5
	SBIC PINC, 2    ; skip next instr if PINC is low
	jmp	LED_ON        ; jump when PINC is high
	CBI PORTB, 2    ; set LED on
	call delay_counter
	jmp  loop

LED_ON: 
	SBI PORTB, 2    ; set LED off
	jmp loop

// Here is Part 1 for a 0.25s Delay
delay_func: 
	ldi R19,21     ; R19 = 21
delay0:  
	ldi R23,255    ; R23 = 255
delay1:  
	ldi R21,248    ; R21 = 255
delay2:  
	dec R21        ; Decrease R21 value
	brne delay2     ; if (R23 != 0) goto delay2 label
	dec R23        ; Decrease R23 value
	brne delay1     ; if (R23 != 0) goto delay1 label
	dec R19        ; Decrease R19 value
	brne delay0     ; if (R19 != 0) goto delay0 label
	nop

delay_counter: // this function will execute the 0.25s delay 5 times for the desired 1.25s
	dec R22
	brne delay_func
	ret

EXO_ISR: 
	CBI PORTB, 3 // turn on port3
	ldi R22,2 // here we load 2 into r22 so it makes a 0.5sec delay
	call delay_counter // here we call the delay function
	SBI PORTB, 3 // here we turn off led 3
	RETI // we return.