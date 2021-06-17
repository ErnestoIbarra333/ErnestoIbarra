;
; DA1.asm
; 
; Created: 6/15/2021 7:17:38 PM
; Author : Ernesto Ibarra
;
/////////////// A).
.org 0
.def numAH = R16
.def numAL = R17
	LDI numAH, 0x12
	LDI numAL, 0x34
	STS 0x402, numAH
	STS 0x403, numAL

/////////////// B).
.org 0x998 ;[0x1330] 
MYDATA: .db 0x56,0x78

/////////////// C).
	LDI ZL, LOW(2*MYDATA)
	LDI ZH, HIGH(2*MYDATA)
	LPM R4, Z+ //load values into R4 and R5
	LPM R5, Z
	ADD numAL, R5 // 0x1234 + 0x5678 = 0x68AC
	ADC numAH, R4 // store values in R16 and R17
	LDI YH, HIGH(0x0000)
	LDI YL, LOW(0x0000)
	CALL STORE_IN_EEPROM //stores R16(68) in starting EEEPROM starting location
	MOV numAH, numAL
	INC YL
	CALL STORE_IN_EEPROM // stores R17(AC) in the next EEPROM location
	NOP

	END: JMP END
/////////////// D).
.ORG 0x1000 ; 0x100 << 1 = 0x2000 which is startng location wanted
MYDATA1: .dw 0x9100,0x9101,0x9102,0x9103,0x9104,0x9105,0x9106,0x9107,0x9108,0x9109,0x9110
// I tried getting it working with the X pointer but I could not get "SPM" working, also it keeps telling me it is only allowed
// with the Z pointer so I'm not sure what to do here besides doing it the same way we did it in part B. hope this is okay for part D
// thank you.

STORE_IN_EEPROM:
	SBIC EECR, EEPE
	RJMP STORE_IN_EEPROM
	OUT EEARH, YH
	OUT EEARL, YL
	OUT EEDR, numAH
	SBI EECR, EEMPE
	SBI EECR, EEPE
	RET




