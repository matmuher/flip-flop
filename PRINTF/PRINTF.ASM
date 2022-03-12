;		[PRINTF]
.186
.model tiny

.code

org 100h


;--------------------------------------
start:

call prp_jmp_table

push 3082d
push offset test_str
push 31d
push ','
push offset ma_str

call printf

add SP, 4 * 2

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
;	Prints string concerning specifiers:
;	
;	%h - hexadecimal
;	%d - decimals
;	%o - octal
;	%b - binary
;
;	%c - char
;	%s - string ($-ended)
;
;	%% - percent
;--------------------------------------
;[params]:
;
;	1 - formatted string adress in DS
;
;	... - format params	
;--------------------------------------
;
;	CX - number of written chars
;	or
;	FFFF - if format error occured
;--------------------------------------
printf		proc

				;[PROLOG]
		push BP
		mov BP, SP
		
		IRP REG, <AX, BX, DX, DI, SI>
			push REG
		ENDM
		
		xor DI, DI
		mov DI, 4

		mov SI, BP[DI]
		add DI, 2
		
		xor CX, CX


	str_scan:
		cmp byte ptr DS:[SI], '$'
		je str_end 
		
		cmp byte ptr DS:[SI], '%'		
		je frmt_proc
		
		jmp common_char
		
					
	str_end:
		IRP REG, <SI, DI, DX, BX, AX>
			pop REG
		ENDM
				;[EPILOG]
		pop BP
		ret
	
	
	frmt_proc:
		inc SI

		xor BX, BX
		mov BL, DS:[SI]
		shl BL, 1d
	
		jmp word ptr jmp_table[BX]		
	

	common_char:
		lodsb
		xchg DL, AL
		mov AH, 02h
		int 21h
		
		inc CX		

		jmp str_scan	


	chr_proc:
		mov byte ptr DL, BP[DI]
		add DI, 2	; Next argument
	
		mov AH, 02h
		int 21h
		
		inc CX
		
		inc SI		; Next str symbol
		jmp str_scan
		
	
	str_proc: 		; String must be stored in DS
		mov DX, BP[DI]
		add DI, 2
		
		mov AH, 09h
		int 21h		
			
		inc CX
		
		inc SI
		jmp str_scan


	dec_proc:
		push BP[DI]
		add DI, 2

		push 10d
		
		jmp int_proc


	hex_proc:
		push BP[DI]
		add DI, 2
		
		push 16d
		
		jmp int_proc	


	oct_proc:
		push BP[DI]
		add DI, 2
		
		push 8d
		
		jmp int_proc

		
	bin_proc:
		push BP[DI]
		add DI, 2
		
		push 2d
		
		jmp int_proc

	
	int_proc:
		push DS
		push offset integer
				
		call itoa_stack
		
		mov DX, offset integer
		mov AH, 09h
		int 21h
		
		inc CX
		
		inc SI
		jmp str_scan


	prc_proc:
		mov DL, '%'
		mov AH, 02h
		int 21h
		
		inc CX
		
		inc SI
		jmp str_scan


	alert_unknwn_frmt:
		mov DX, offset alert_unknwn_frmt_str
		mov AH, 09h
		int 21h
				
		xor CX, CX
		dec CX
		jmp str_end		
	
		endp

alert_unknwn_frmt_str db '%[ERROR:Unknown format]$'
;--------------------------------------


;--------------------------------------
write_jmp_table		proc

			shl BL, 1d
			mov jmp_table[BX], AX
		
			xor BX, BX		

			ret		
			endp
;--------------------------------------
SET_JMP	MACRO CHAR, JMP_FUNC

		mov AX, offset JMP_FUNC
		mov BL, CHAR
		call write_jmp_table

		ENDM
;--------------------------------------
;		[PRP_JMP_TABLE]
;--------------------------------------
;[descript]:
;
;	Prepares jmp_table for printf
;--------------------------------------
prp_jmp_table	proc

		xor BX, BX
		
		SET_JMP 'c', chr_proc

		SET_JMP 's', str_proc
		
		SET_JMP 'd', dec_proc
		
		SET_JMP 'o', oct_proc
		
		SET_JMP 'b', bin_proc
		
		SET_JMP 'x', hex_proc
		
		SET_JMP '%', prc_proc
		
		ret
		endp
;--------------------------------------


;------------------------------------------------
;		[ITOA via stack]
;------------------------------------------------
;[params]:
;	1) integer to print
;	2) base (in which base should be transfered)
;	
;	3) segment adress
;	4) offset in segment
;------------------------------------------------
;[return]:
;	puts string in user's memory
;------------------------------------------------
itoa_stack		proc
						;[PROLOG]
			push BP
			mov BP, SP
					
			IRP REG, <DI, ES, BX, AX>
				push REG
			ENDM
						;[PARAMS]
			
			mov DI, [BP + 4]
			mov ES, [BP + 6]
			mov BX, [BP + 8]
			mov AX, [BP + 10]
			
			call itoa

			IRP REG, <AX, BX, ES, DI>
				pop REG	
			ENDM
						;[EPILOG]
			pop BP						
			ret 8
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


;--------------------------------------
;		[PEREVOROT]
;--------------------------------------
;[params]:
;	CX - length of string
;
;	ES -\ adress of string
;	DI -/ that needs some perevorot
;--------------------------------------
;[returns]:
;	reversed string stored in user's adress
;--------------------------------------
;[destroy]:
;	BX, CX, DX, DI, BP
;--------------------------------------
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
;--------------------------------------


;--------------------------------------
.data
ma_alpha db '0123456789ABCDEF'
integer db 17 DUP(0)
jmp_table dw ('z') DUP(offset alert_unknwn_frmt)

test_str db 'milk packets with fat $'
ma_str db 'GgWP%g$'

end start