;
; DA1.asm
; 
; Created: 2/05/2022 
; Author : Ernesto Ibarra
;
/////////////// 1).
.org 0
.def numAH = R16
.def numAL = R17
	LDI numAH, 0x12 // we store 0x12 into numAH
	LDI numAL, 0x34 // we store 0x34 into numAL
	STS 0x402, numAH // now we store it in SRAM location 0x402
	STS 0x403, numAL

/////////////// 2).
.def numBH = R18
.def numBL = R19
	LDI numBH, 0x56 // we store 0x12 into numAH
	LDI numBL, 0x78 // we store 0x12 into numAH
	STS 0x410, numBH // now we store it in SRAM location 0x410
	STS 0x411, numBL

/////////////// 3).
	ADD numAH, numBH // 0x1234 + 0x5678 = 0x68AC
	ADC numAL, numBL //store values in R16 and R17
	LDI YH, HIGH(0x0000) // here we initialize Y to EEPROM starting location
	LDI YL, LOW(0x0000)
	CALL STORE_IN_EEPROM //stores R16(68) in starting EEEPROM starting location
	MOV numAH, numAL
	INC YL
	CALL STORE_IN_EEPROM // stores R17(AC) in the next EEPROM location
	NOP

/////////////// 4).
.EQU STARTADDS = 0x500 

.def tmp = R20 // tmp variable to hold values 
.def count = R22 // count for the loop
.def sumcount = R23 // count to add the numbers 
.def sumH = R24 // here we will store the values 
.def sumL = R25
	LDI R21, HIGH(RAMEND) // here we initialize the stack 
	OUT SPH, R21 // we are using R21
	LDI R21, LOW(RAMEND)
	OUT SPL, R21

	LDI ZL, LOW(2*MYDATA1) // Here we let Z point to our Data
	LDI ZH, HIGH(2*MYDATA1)
	LDI XL, LOW(STARTADDS) // X will point to our address to store it in
	LDI XH, HIGH(STARTADDS)
	ldi count, 20
	ldi sumcount, 10

LOOP1: // Here we will store 10 in program memory then retrieve them and store them in SRAM using X pointer
	lpm tmp, Z+ // here we load Z into tmp
	PUSH tmp //PUSH it so we can later add all the numbers easily
	ST  X+, tmp // loads into SRAM location 0x500
	DEC count 
	brne LOOP1
LOOP2: // Here we will add all the 10 16 bit numbers and store them in SRAM starting location 0x406
	POP numAH //High byte 
	POP numAL //Low byte  
	ADD sumH, numAH // Here sumH and sumL are keeping stored all the addition
	ADC sumL, numAL
	DEC sumcount 
	brne LOOP2 
	STS 0x406, sumH // after we have added 0x0910+0x0911+0x0912 .... and we get a final value of 5ACD
	STS 0x407, sumL

END: JMP END // program ends

.ORG 0x1000 
MYDATA1: .dw 0x0910,0x0911,0x0912,0x0913,0x0914,0x0915,0x0916,0x0917,0x0918,0x0919

STORE_IN_EEPROM: // Store function for EEPROM
	SBIC EECR, EEPE
	RJMP STORE_IN_EEPROM
	OUT EEARH, YH
	OUT EEARL, YL
	OUT EEDR, numAH
	SBI EECR, EEMPE
	SBI EECR, EEPE
	RET




