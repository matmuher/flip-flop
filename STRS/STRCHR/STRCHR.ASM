;		[STRCHR]

.model tiny

.code

org 100h

start:
		;[PREPARE FOR STRCHR]
mov AL, 'm'
mov BX, DS
mov ES, BX
lea DI, test_lime

MAX_LIME_LEN = 100
mov CX, MAX_LIME_LEN

call strchr

comment !

mov DX, DI
		;[PRINT]
mov AH, 09h
int 21h !

		;[EXIT 0]
mov AX, 4c00h
int 21h

;------------------------------------------------
;		[STRCHR]
;------------------------------------------------
;[params]:
;	AL - char to search
;
;	ES -\ addres of string
;	DI -/ where search should be implemented
;
;	CX - max number of characters in string
;------------------------------------------------
;[returns]:
;	ZF = 0, if char wasn't found
;	ZF = 1, otherwise
;
;	ES -\ addres of needed
;	DI -/ char
;------------------------------------------------
;[destroy]:
;	CX, DI
;------------------------------------------------
strchr		proc
		
		CLD		; Straight direction

		REPNE scasb

		jne no_ch	; No char was found

		ret	
		
		no_ch:
			comment !
			lea DX, no_char
			mov AH, 09h
			int 21h !
		
			ret	
		endp
;------------------------------------------------



.data
no_char db 'No char in de string!$'
test_lime db 'And i say to myself: what a wonderful life...!$'
end start