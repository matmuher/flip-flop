;		[STRNCPY]

.model tiny

.code

org 100h

start:

mov BX, DS
mov ES, BX

lea SI, lime_test
lea DI, lime_receiver

MAX_LIME = 100
mov CX, MAX_LIME

call strncpy

lea DX, lime_receiver

		;[PRINT]
mov AH, 09h
int 21h

		;[EXIT 0]
mov AX, 4c00h
int 21h

;------------------------------------------------
;		[STRNCPY]
;------------------------------------------------
;[params]:
;	DS -\ source
;	SI -/ string
;
;	ES -\ destination
;	DI -/ string
;
;	CX - number of chars to copy
;------------------------------------------------
;[returns]:
;	CX-copied chars in destination string
;------------------------------------------------
;[destroy]:
;	AL, DI, SI
;------------------------------------------------
strncpy		proc
		
		copying:
		
			lodsb
			stosb

			cmp AL, '$'
	
		LOOPNE copying
		
		ret	
		endp
;------------------------------------------------


.data

lime_test db 'Its just another lemon tree!$'
lime_receiver db MAX_LIME DUP(0)
end start