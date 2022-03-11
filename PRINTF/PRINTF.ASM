;		[PRINTF]

.model tiny

.code

org 100h


;--------------------------------------
start:

push offset test_str
push '|'
push offset ma_str

call printf

call exit
;--------------------------------------


;--------------------------------------
exit		proc
	
		mov AX, 4c00h
		int 21h

		endp
;--------------------------------------


;--------------------------------------
;		[PRINTF] (cdecl)
;--------------------------------------
;[descript]:
;
;	Prints string concerning specifiers
;--------------------------------------
;[params]:
;
;	1 - formatted string adress in DS
;
;	... - format params	
;--------------------------------------
;
;	Returns number of written chars
;	or negative in error case
;--------------------------------------
printf		proc
				;[PROLOG]
		push BP
		mov BP, SP
		
		xor DI, DI
		mov DI, 4

		mov SI, BP[DI]
		add DI, 2
		
	str_scan:			;[WHILE]
		cmp byte ptr DS:[SI], '$'
		je str_end 
		
		cmp byte ptr DS:[SI], '%'		
		je frmt_proc
		
		jmp common_char
	
	
	frmt_proc:
		inc SI

		cmp byte ptr DS:[SI], 's'
		je str_proc
		
		cmp byte ptr DS:[SI], 'c'
		je chr_proc
	
		
	common_char:
				;[PRINT CHAR]
		lodsb
		xchg DL, AL
		mov AH, 02h
		int 21h

		jmp str_scan	


	chr_proc:
		mov byte ptr DL, BP[DI]
		add DI, 2	; Next argument
	
		mov AH, 02h
		int 21h
		
		inc SI		; Next str symbol
		jmp str_scan
		
	
	str_proc: 		; String must be stored in DS
		mov DX, BP[DI]
		add DI, 2
		
		mov AH, 09h
		int 21h		
		
		inc SI
		jmp str_scan
	
				
	str_end:
				;[EPILOG]
		pop BP
		ret

		endp
;--------------------------------------


;--------------------------------------
.data

test_str db 'Goodbye war?$'
ma_str db 'Hello world! %c %s$'

end start