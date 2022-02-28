;		[STRLEN]

.model tiny

.code

org 100h

start:

main:
	mov BX, DS
	mov ES, BX

	lea DI, test_str

	push ES
	push DI
	call stack_strlen
	
;	call regs_strlen

;	mov DX, DI
;	add DX, '0'	

	mov AH, 02h
	int 21h

	xor AH, AH
	int 16h

	mov AX, 4c00h
	int 21h


;		[f:STACK_STRLEN]
stack_strlen		proc
				; [PROLOG]
			push BP			; save previous BP
			mov bp, sp
			sub sp, 4		; storage for 2 params
			

			mov DI, [BP + 2 + 2 * 1]
			mov AX, [BP + 2 + 2 * 2]
			mov ES, AX
			
			call regs_strlen

				; [EPILOG]
			add SP, 4		; free params' storage						
			pop BP
			ret

			endp



;		[f:REGS_STRLEN]
;params:
;	ES - string segment
;	DI - string index
;	
;return:
;	DI - length
;destroy:
;	AL
regs_strlen		proc
		
		mov AL, '$'
		mov SI, DI 	; save current DI

		REPNE scasb 	; DI shifts to '$' 
		sub DI, SI	; count length
		dec DI
				
		ret
		endp

.data

test_str db 'MeowMeow$'
end start

