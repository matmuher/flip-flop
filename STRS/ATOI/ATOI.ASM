;		[ATOI]

.model tiny

.code

org 100h

start:

lea SI, test_lime

call atoi

add DX, '0'

		;[PRINT CHAR]
mov AH, 02h
int 21h

		;[EXIT]
mov AX, 4c00h
int 21h



;------------------------------------------------
;		[ATOI]
;------------------------------------------------
;[params]:
;	DS -\ addres of 
;	SI -/ striged integer
;------------------------------------------------
;[returns]:
;	DX - integer
;------------------------------------------------
;[destroy]:
;	AX, BX, CX, SI
;------------------------------------------------
atoi		proc
		
		IRP REG, <AX, BX, CX, SI>
			push REG
		ENDM

		CLD
		
		MAX_LIME = 100
		mov CX, MAX_LIME
		mov BL, 10d

		xor DX, DX

		atoing:
			xor AX, AX		;[LOAD CHAR]
			lodsb
			
			sub AL, '0'
			add DX, AX
						;[END if $]
			mov BH, DS:[SI]
			cmp BH, '$'
			je this_ist_die_end

			xchg AX, DX
			mul BL
			xchg DX, AX

		LOOP atoing 

		this_ist_die_end:	
		
		IRP REG, <SI, CX, BX, AX>
			pop REG
		ENDM

		ret		
		endp
;------------------------------------------------



.data

test_lime db '9$'

end start