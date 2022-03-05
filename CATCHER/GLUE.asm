;		[UNHOLY CATCHER]
.286
.model tiny

.code
org 100h

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
				mov AH, 00010000b ; blue back, white chars
				call DrawFrame
				call DrawRegs
				
				jmp die_end
					
			stop_draw:
				mov AH, 00100000b ; black back, white chars 
				call DrawFrame

				mov byte ptr cs:[hotkey_status], DEPRES

				jmp die_end
				
			no_hot:
				mov AH, 01000000b ; black back, white chars 
				call DrawFrame

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
;		[DrawFrame]
;--------------------------------------
;[params]: you have no choice)
;	AH - color
;--------------------------------------
;[destroy]: 
;	AX, BX, CX, DX, DI, SI, BP, ES
;--------------------------------------
DrawFrame	proc

		
		VIDEOSEG = 0b800h
	
		; set frame sizes
		len = 10d  
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


		ret
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

DrawRegs	proc

			text_field = total_shift + 82d * 2d
			
			mov DI, text_field
			mov BX, VIDEOSEG
			mov ES, BX
			mov AH, 10100101b
			mov AL, '7'
			stosw
			
			ret
			endp

ROOF  db 0C9h, 0CDh, 0BBh
WALL  db 0BAh, ' ', 0BAh
FLOOR db 0C8h, 0CDh, 0BCh

hotkey_status db 0

proga_end db 0

end start

