;		[ITOA]
.286
.model tiny

.code

org 100h

start:

mov BX, 8d	;BASE

		;[TRANSMIT STRING STORAGE ADRESS]
mov AX, DS
mov ES, AX

lea DI, int2str

mov AX, 177d	;[INTEGER ITSELF]
 
;call itoa

comment !
push AX
push BX
push DS
push DI !

call itoa2

		;[OUTPUT TEXT INTEGER]
mov AH, 09h

lea DX, int2str

int 21h

		;[EXIT 0]
mov AX, 4c00h
int 21h

;------------------------------------------------
;		[ITOA via stack]
;------------------------------------------------
;[params]:
;	1) integer to print
;	2) base (in which base should be transfered)
;	
;	3) segment adress
;	4) offser in segment
;------------------------------------------------
;[return]:
;	puts string in user's memory
;------------------------------------------------
itoa_stack		proc
						;[PROLOG]
			push BP
			mov BP, SP
						;[PARAMS]
			fourth = 2 + 2
			third = fourth + 2
			second = third + 2
			first = second + 2

			mov DI, [BP + fourth]
			mov ES, [BP + third]
			mov BX, [BP + second]
			mov AX, [BP + first]
			
			call itoa
						;[EPILOG]
			pop BP						
			ret 4 
			endp
;------------------------------------------------



;------------------------------------------------
;		[ITOA for 2^n]
;------------------------------------------------
;[equal to ITOA]
;------------------------------------------------
itoa2		proc
		
		pusha
		
		xor CX, CX		; two's degree counter
		mov BP, AX		; save origin int
		mov AX, BX		; for div
		
		xor DX, DX
	pokamest:
		mov BX, 2d	
		inc CX			; two's degree
		div BX

		cmp AX, 1d
		jne pokamest
			
		mov DX, DI
			
		rest:
			mov AX, BP

			shr BP, CL		; Clean CX bits from edge
			shl BP, CL

			sub AX, BP		; Get rest
			
			xchg SI, AX
			mov AL, ma_alpha[SI]
			
			stosb			; Put it in de string
		
			shr BP, CL
		
			cmp BP, 0
		
		jne rest
		
		mov AL, '$'
		mov ES:[DI], AL
					; Prepare for perevorot()	
		sub DI, DX
		mov CX, DI
		mov DI, DX
		
		call perevorot
	
		popa

		ret
		endp		
;------------------------------------------------



;------------------------------------------------
;		[ITOA]
;------------------------------------------------
;[params]:
;	AX - integer to print
;	BX - base (to)
;
;	ES -\ memory adress with
;	DI -/ enough memory to store stringed-integer
;------------------------------------------------
;[return]:
;	puts string in user's memory
; -----------------------------------------------
;[destroy]:
;	AX, BX, CX, DX, DI, SI, BP
;------------------------------------------------
itoa		proc

		nop
		nop
		nop

		IRP REG, <AX,BX,CX,DX,DI,SI,BP>
			push REG
		ENDM
		
		mov BP, DI	

whale:		
		xor DX, DX			
		div BX
		
		xchg SI, DX
		mov DL, ma_alpha[SI]
		mov ES:[DI], DL
		inc DI
		
		cmp AX, 0 
		jne whale
		
		mov CL, '$'
		mov ES:[DI], CL

		sub DI, BP		; Prepare CX argument for prerevorot()
		mov CX, DI
		
		mov DI, BP
		
		call perevorot
		
		IRP REG, <BP,SI,DI,DX,CX,BX,AX>
			pop REG
		ENDM

		ret

		nop
		nop
		nop

 		endp
;------------------------------------------------



;------------------------------------------------
;		[PEREVOROT]
;------------------------------------------------
;[params]:
;	CX - length of string
;
;	ES -\ adress of string
;	DI -/ that needs some perevorot
;------------------------------------------------
;[returns]:
;	reversed string stored in user's adress
;------------------------------------------------
;[destroy]:
;	BX, CX, DX, DI, BP
;------------------------------------------------
perevorot		proc
			
			nop
			nop
			nop

			IRP REG, <BX,CX,DX,DI,BP>
				push REG
			ENDM	
	
			mov BP, DI
			dec CX
			add BP, CX		; Now BP points to end of string

		poka:
			mov DL, ES:[DI]
			mov BL, ES:[BP]
			mov ES:[BP], DL
			mov ES:[DI], BL

			dec BP
			inc DI
			
			cmp BP, DI
			jg poka			
		
			IRP REG, <BP,DI,DX,CX,BX>
				pop REG
			ENDM
	
			ret

			nop
			nop
			nop
			
			endp
;------------------------------------------------

.data

ma_alpha db '0123456789ABCDEF'
int2str db 100 dup(0)		; User allocate memory
debug_machine db 'Four$'

end start