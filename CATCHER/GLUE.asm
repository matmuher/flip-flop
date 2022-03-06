;		[UNHOLY CATCHER]
.186
.model tiny

.code

org 100h

;--------------------------------------
start:

		;[HOTKEY STATUS]
			
	HOTKEY = 41d
	DEPRES = 0
	PRESS1 = 1
	PRESS2 = 2


	call Recature08h
	call Recapture09h
			
	call TSR
	
	mov AX, 4c00h
	int 21h
;--------------------------------------
		
		
;--------------------------------------
DrawRegs	proc
			
			
			text_field = total_shift + 82d * 2d
			
			new_line = 80d * 2d
			num_len = 10d ; register content length
			
			regs_style = 1010011100000000b ; red back, white chars
			
			regs_num = 8d
			mov CX, regs_num
			
			mov AX, regs_style
			
			mov BX, VIDEOSEG
			mov ES, BX
			
			mov DI, text_field
			
			mov SI, offset REGISTERS
			
			
			reg_labels:
			
				mov AL, CS:[SI]
				inc SI
				
				mov ES:[DI], AX
				add DI, 2d
				
				mov AL, CS:[SI]
				inc SI
				
				mov ES:[DI], AX
				add DI, 2d
				
				
				add DI, new_line - 2d * 2d
				
			loop reg_labels
				
			
			pop ret_adr
			
			IRP REG,<ES,BP,SI,DI,DX,CX,BX,AX>
				pop REG
			ENDM
		
			IRP REG,<AX,BX,CX,DX,DI,SI,BP,ES>
				push REG
			ENDM
			
			push ret_adr
			
			mov video_begunok, text_field + 2d * 3
			
			IRP REG,<AX,BX,CX,DX,DI,SI,BP,ES>
		
					;[ITOA REGS]
				
				push REG
				push 16d
				push CS
				push offset reg_str
				
				call itoa_stack
			
					;[PRINT REGISTERS]
					
				push regs_style ; red back, white chars
				push num_len
				push video_begunok
				push offset reg_str
				push VIDEOSEG
				
				call PrntStr_stack
				
				add video_begunok, new_line
			ENDM
			
			
			
			ret
			endp
			
			video_begunok dw 0
			ret_adr dw 0
			REGISTERS db 'AXBXCXDXDISIBPES'
			reg_str db num_len DUP(0)
;------------------------------------------------		
				
		
;--------------------------------------
;		[DrawFrame]
;--------------------------------------
;[params]: you have no choice)
;	AH - color
;--------------------------------------
;[destroy]: 
;	AX, BX, CX, DX, DI, SI, BP, ES
;--------------------------------------
DrawFrame	proc

		IRP REG,<AX,BX,CX,DX,DI,SI,BP,ES>
			push REG
		ENDM
		
		VIDEOSEG = 0b800h
	
		; set frame sizes
		len = 12d  
		HEIGHT = 12d
 
		; set frame position on screen
		line_shift = 9d
		column_shift = 60d

		weedth = 80d * 2h ; one line shift in VIDEOSEG
		line = weedth - len * 2h ; transition to a new line for frame element
		total_shift = line_shift * weedth + column_shift * 2h
		
		mov BX, VIDEOSEG
		mov ES, BX
		
		xor DI, DI
		mov DI, total_shift
		mov CX, len
		
				;[PRINT ROOF]
		mov SI, offset ROOF
		call DrawLine
		add DI, line
		
				;[PRINT WALL]
		mov DX, HEIGHT

		ma_wall:	

			mov CX, len
			mov SI, offset WALL

			call DrawLine
			add DI, line

			dec DX
			CMP DX, 0

		JNZ ma_wall

				;[PRINT FLOOR]
		mov CX, len
		mov SI, offset FLOOR
	
		call DrawLine

		IRP REG,<ES,BP,SI,DI,DX,CX,BX,AX>
			pop REG
		ENDM

		ret
		endp
;--------------------------------------		
		
		
;------------------------------------------------
;		[SaveScreen]
;------------------------------------------------		
SaveScreen	proc
			
			resolution = len * (HEIGHT + 2)
			
			IRP REG,<AX,BX,CX,DI,SI,ES>
				push REG
			ENDM
			
			
			xor DX, DX
			
			mov BX, VIDEOSEG
			mov ES, BX
			
			mov DI, total_shift
			
			ss_rows:
				
				xor CX, CX
				ss_columns:
				
					mov AX, DX
					mov BL, len * 2d
					MUL BL
					
					add AX, CX
					
					xchg AX, BX
					mov word ptr AX,  ES:[DI]
					mov word ptr cs:SI[BX], AX
					add DI, 2d
					add CX, 2d
					
				cmp CX, len * 2
				jne ss_columns
				
				add DI, new_line - len * 2d
				inc DX
				
			cmp DX, (HEIGHT + 2)
			jne ss_rows
			
			IRP REG,<ES,SI,DI,CX,BX,AX>
				pop REG
			ENDM
			
			ret
			endp
			
		filler = 0100000100100100b
		press_buffer dw resolution DUP(filler)
		tick_buffer dw resolution DUP(filler)
		frame_buffer dw resolution DUP(filler)
;------------------------------------------------


;------------------------------------------------
;		[PutBuffer]
;------------------------------------------------
PutBuffer	proc

			IRP REG,<AX,BX,CX,DI,SI,ES>
				push REG
			ENDM
			
			
			xor DX, DX
			
			mov BX, VIDEOSEG
			mov ES, BX
			
			mov DI, total_shift
			
			rows:
				
				xor CX, CX
				columns:
				
					mov AX, DX
					mov BL, len * 2d
					MUL BL
					
					add AX, CX
					
					
					xchg AX, BX

					mov word ptr AX, cs:SI[BX]
					mov word ptr ES:[DI], AX
					add DI, 2d
					add CX, 2d
					
				cmp CX, len * 2
				jne columns
				
				add DI, new_line - len * 2d
				inc DX
				
			cmp DX, (HEIGHT + 2)
			jne rows
			
			
			IRP REG,<ES,SI,DI,CX,BX,AX>
				pop REG
			ENDM
			
			ret
			endp
;------------------------------------------------
		
		
;------------------------------------------------
;		[PUTDIFF] : a - b => c
;	SI - a ; origin
;	DI - b ; changed
;	BP - c ; difference
;------------------------------------------------
PutDiff		proc
			xor BX, BX
				
			buffers_diff:
			
				mov word ptr AX, CS:DI[BX]
				
				cmp word ptr CS:SI[BX], AX
				
				je NoChange
				
					push SI
					push BP
					
					pop SI
					
					mov word ptr CS:SI[BX], AX
					
					pop SI
					
				NoChange:
				
				add BX, 2d
				
				cmp BX, resolution * 2d	
				
			jne buffers_diff
				
			ret
			endp

;------------------------------------------------
;		[CmpBuffers]
;[params]:
;	SI \ buffers to cmp
;	DI /
;------------------------------------------------		
CheckChanges	proc
				
			xor BX, BX
				
			buffers_cmp:
			
				mov AX, CS:SI[BX]
				
				cmp CS:DI[BX], AX
				jne ReDraw
			
				add BX, 2d
				cmp BX, resolution * 2
				
				jne buffers_cmp
				
				ret
				
			ReDraw:
				
				lea SI, frame_buffer
				lea DI, tick_buffer
				lea BP, press_buffer
				call PutDiff
				
				ret 
				endp
			
		
;--------------------------------------
TSR			proc

				;[Terminate and stay resident]
				
			mov AX, 3100h
			mov DX, offset proga_end
			and DX, 1111b	; Check % 16
			cmp DX, 0		; if > 0 => +1 paragraph
			ja one_more_paragraph
			
			mov DX, offset proga_end
			shr DX, 4
			int 21h

		one_more_paragraph:

			mov DX, offset proga_end
			shr DX, 04h
			inc DX
			
			int 21h
			
			;ret
			endp
;--------------------------------------			
			
			
;--------------------------------------
Recature08h	proc
						
					;[SAVE 08H INT]
	
			xor BX, BX
			mov ES, BX
			mov BX, 08h * 4	
			
			mov AX, ES:[BX]
			mov word ptr OLD08H, AX
			
			mov AX, ES:[BX + 2]
			mov word ptr OLD08H + 2, AX

			
			mov AX, CS
			
					;[RECAPTURE OF 08H]
			
			cli
				mov word ptr ES:[BX], offset New08h
				mov word ptr ES:[BX + 2], AX
			sti

			
			ret
			endp
;--------------------------------------


;--------------------------------------
Recapture09h proc

					;[SAVE 09H INT]
			
			xor BX, BX
			mov ES, BX
			mov BX, 09h * 4	
			
			mov AX, ES:[BX]
			mov word ptr OLD09H, AX
			
			mov AX, ES:[BX + 2]
			mov word ptr OLD09H + 2, AX

			
			mov AX, CS
			
					;[RECAPTURE OF 09H]

			cli
				mov word ptr ES:[BX], offset New09h
				mov word ptr ES:[BX + 2], AX
			sti
			
			
			ret
			endp

;--------------------------------------
New08h		proc
					;[SAVE REGS]
			
			IRP REG,<AX,BX,CX,DX,DI,SI,BP,ES>
				push REG
			ENDM
			
			
					;[PREPARE FOR VIDEO OUTPUT]	
 						
			mov BX, 0b800h
			mov ES, BX
			scr_pos = (80d * 8d + 40d) * 2d
			mov DI, scr_pos
			
			
					;[PROCESS HOTKEY]
					; Output hotkey_status

			mov AH, 10100100b ; green back, red chars
			xor AL, AL
			mov AL, cs:[hotkey_status]
			add AL, '0'
			stosw

			mov AL, '!'
			stosw

			mov AL, ' '
			stosw
			
			
			
			cmp cs:[hotkey_status], PRESS1
			je draw
			
			cmp cs:[hotkey_status], PRESS2
			je stop_draw
			
			cmp cs:[hotkey_status], DEPRES
			je no_hot

			
			draw:
				lea SI, tick_buffer
				call SaveScreen
			
				lea SI, frame_buffer
				lea DI, tick_buffer
				lea BP, press_buffer
				call PutDiff
			
				;lea SI, tick_buffer
				;lea DI, press_buffer
				;call CheckChanges
			
				mov AH, 00010000b ; blue back, white chars
				call DrawFrame
				call DrawRegs
				
				lea SI, frame_buffer
				call SaveScreen
				
				jmp die_end
					
			stop_draw:
				lea SI, press_buffer
				call PutBuffer

				mov byte ptr cs:[hotkey_status], DEPRES

				jmp die_end
				
			no_hot:
				;mov AH, 01000000b ; black back, white chars 
				;call DrawFrame
				lea SI, press_buffer
				call SaveScreen

			die_end:

					;[SAVE REGS]
			
			IRP REG,<ES,BP,SI,DI,DX,CX,BX,AX>
				pop REG
			ENDM
					;[TRANSFER CONTROL TO THE ORIGINAL INT]

			db 0EAh		; FAR JMP OLD08H
			OLD08H dd 0
					
			endp
;--------------------------------------


;--------------------------------------
New09h		proc
			
			IRP REG,<AX,BX,CX,DX,DI,SI,BP,ES>
				push REG
			ENDM
			

				;[Get bukva]

			KbrdPrt = 60h
			in AL, KbrdPrt
			
	
				;[PROCESS KEYBOARD INPUT]
				
			cmp AL, HOTKEY
			je hotkey_pressed

			jmp return_control

			hotkey_pressed:

				add cs:[hotkey_status], 1d	;DEPRES (0) +1 % 3 = PRESS1 (1) 
							;PRESS1 (1) +1 % 3 = PRESS2 (2)
							;PRESS2 (2) +1 % 3 = DEPRES (0) 
				xor AX, AX
				mov AL, cs:[hotkey_status]
				mov BL, 3d
				
				div BL
				
				mov cs:[hotkey_status], AH
				
			return_control:

		
			IRP REG,<ES,BP,SI,DI,DX,CX,BX,AX>
				pop REG
			ENDM
		
			db 0EAh		; FAR JMP OLD09H
			OLD09H dd 0
	
			endp
;--------------------------------------


;--------------------------------------
;		[DrawLine]
;--------------------------------------
;[params]:
;	AH - color
;	CX - string length
;	DI - addr of start line 
;	SI - addr of 3 line elements
;	ES - videoseg addr
;--------------------------------------
;[destroy]:
;	
;--------------------------------------
DrawLine	proc

			mov AL, CS:[SI]
			inc SI
			;lodsb		; mov AL, [SI]  |  read left corner
						; inc SI  |  shift to middle element
						
			stosw		; mov ES:[DI], AX  |  put in video memory
						; add DI, 2  |  shift to next video cell
			
			mov AL, CS:[SI]
			inc SI
			;lodsb		; mov AL, [SI]  |  read middle element
			
			sub CX, 2   ; now CX is length of middle section 

			mid:		
				stosw		;mov ES:[DI], AX  | put middle element
							;add DI, 2 | shift to next video cell
				loop mid        	
			
			
			mov AL, CS:[SI]
			inc SI
			;lodsb		; read right corner
			
			stosw		; put it in video memory
	 
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
					
			IRP REG,<AX,BX,CX,DX,DI,SI,BP,ES>
				push REG
			ENDM
			
			mov DI, [BP + fourth]
			mov ES, [BP + third]
			mov BX, [BP + second]
			mov AX, [BP + first]
			
			call itoa2
		
		
			IRP REG,<ES,BP,SI,DI,DX,CX,BX,AX>
				pop REG
			ENDM
			
				;[EPILOG]
			pop BP						
			ret 8 
			endp
;------------------------------------------------


;------------------------------------------------
;		[ITOA for 2^n]
;------------------------------------------------
;[equal to ITOA]
;------------------------------------------------
itoa2		proc
		
		IRP REG,<AX,BX,CX,DX,DI,SI,BP,ES>
			push REG
		ENDM
		
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
			mov AL, cs:ma_alpha[SI]
			
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
		
		mov BX, CS
		mov ES, BX
		
		call perevorot
	
		IRP REG,<ES,BP,SI,DI,DX,CX,BX,AX>
			pop REG
		ENDM

		ret
		endp	

		ma_alpha db '0123456789ABCDEF'
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


;--------------------------------------
;		[PrntStr_stack]
;--------------------------------------
;[params]:
;	1 - color
;	2 - length
;	3 - start of string in videoseg
;	4 - start of source string
;	5 - videosegment addres
;--------------------------------------
;[returns]:
;	prints string on the screen
;--------------------------------------
PrntStr_stack		proc

					push BP
					mov BP, SP
					
						;[PARAMS]
					
					fifth = 2 + 2
					fourth = fifth + 2
					third = fourth + 2
					second = third + 2
					first = second + 2
							
					IRP REG,<AX,BX,CX,DX,DI,SI,BP,ES>
						push REG
					ENDM	
			
					
					mov ES, [BP + fifth]
					mov SI, [BP + fourth]
					mov DI, [BP + third]
					mov CX, [BP + second]
					mov AX, [BP + first]
					
					call PrntStr
						
					IRP REG,<ES,BP,SI,DI,DX,CX,BX,AX>
						pop REG
					ENDM
						
					pop BP
					ret 10

					endp
;--------------------------------------


;--------------------------------------
;          Print String
;--------------------------------------
; Params:
;	AH - color
;	CX - length
;	DI - start of string in videoseg
;	SI - start of source string
;	ES - videosegment addres
;
; Destroy: AX, CX, DI, SI
;--------------------------------------
PrntStr proc
	
	cld		; DF = 0

str_prnt:	
	
	mov AL, CS:[SI]
	inc SI
	;lodsb		; mov AL, [SI] ; put char from string
			;inc SI ; shift to the next	

	cmp AL, '$'
	je str_end


	mov ES:[DI], AX
	add DI, 2d
	;stosw		; mov ES:[DI], AX ; put symbol on screen
				; add DI, 2 ; shift to next symbol_place	
	loop str_prnt

	mov AL, ' '

	mov ES:[DI], AX
	add DI, 2d
str_end:
	ret

	endp
;--------------------------------------


;--------------------------------------
ROOF  db 0C9h, 0CDh, 0BBh
WALL  db 0BAh, ' ', 0BAh
FLOOR db 0C8h, 0CDh, 0BCh

hotkey_status db 0

proga_end db 0
;--------------------------------------

end start

