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

			;[SAVE 08H INT]
	
	xor BX, BX
	mov ES, BX
	mov BX, 08h * 4	
	
	mov AX, ES:[BX]
	mov word ptr OLD08H, AX
	
	mov AX, ES:[BX + 2]
	mov word ptr OLD08H + 2, AX


			;[RECAPTURE OF 08H]

	mov AX, CS
	
	cli
		mov word ptr ES:[BX], offset New08h
		mov word ptr ES:[BX + 2], AX
	sti
			

			;[SAVE 09H INT]
	
	xor BX, BX
	mov ES, BX
	mov BX, 09h * 4	
	
	mov AX, ES:[BX]
	mov word ptr OLD09H, AX
	
	mov AX, ES:[BX + 2]
	mov word ptr OLD09H + 2, AX


			;[RECAPTURE OF 09H]

	mov AX, CS
	
	cli
		mov word ptr ES:[BX], offset New09h
		mov word ptr ES:[BX + 2], AX
	sti


			;[STAY RESIDENT]

	mov AX, 3100h
	mov DX, offset proga_end
	shr DX, 4
	add DX, 10d
	int 21h	
	
	mov byte ptr hotkey_status, 0		
	
			;[PAUSE]

	xor AH, AH
	int 16h

			;[EXIT 0]

	mov AX, 4c00h
	int 21h

	
;--------------------------------------
New08h		proc
					;[SAVE REGS]

			IRP REG,<AX,BX,DI,ES>
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
			mov AL, hotkey_status
			add AL, '0'
			stosw

			mov AL, '!'
			stosw

			mov AL, ' '
			stosw
			
			
			
			cmp hotkey_status, PRESS1
			je draw
			
			cmp hotkey_status, PRESS2
			je stop_draw
			
			cmp hotkey_status, DEPRES
			je no_hot

			
			draw:
				mov AL, 'D'
				mov AH, 10010111b ; blue back, white chars
				stosw	
				
				jmp die_end
					
			stop_draw:
				mov AL, 'S'
				mov AH, 00000111b ; black back, white chars 
				stosw

				mov byte ptr hotkey_status, DEPRES

				jmp die_end
				
			no_hot:
				mov AL, 'N'
				mov AH, 01000111b ; red back, white chars
				stosw

			die_end:

					;[SAVE REGS]

			IRP REG,<ES,DI,BX,AX>
				pop REG
			ENDM

					;[TRANSFER CONTROL TO THE ORIGINAL INT]

			db 0EAh		; FAR JMP OLD08H
			OLD08H dd 0
					
			endp
;--------------------------------------


;--------------------------------------
New09h		proc
			
			push AX
			push BX
			

				;[Get bukva]

			KbrdPrt = 60h
			in AL, KbrdPrt
	
			cmp AL, HOTKEY
			je hotkey_pressed

			jmp return_control

		hotkey_pressed:

			add hotkey_status, 1d	;DEPRES (0) +1 % 3 = PRESS1 (1) 
						;PRESS1 (1) +1 % 3 = PRESS2 (2)
						;PRESS2 (2) +1 % 3 = DEPRES (0) 
			xor AX, AX
			mov AL, hotkey_status
			mov BL, 3d
			
			div BL
			
			mov hotkey_status, AH
			
			
		return_control:

			pop BX
			pop AX
		
			db 0EAh		; FAR JMP OLD09H
			OLD09H dd 0
	
			endp
;--------------------------------------


;--------------------------------------
DrawFrameRegs		proc
			
			push BX
			push DS

		;	mov BX, SEG ROOF
			mov DS, BX
			
		;	call DrawFrame_std
			
			pop DS
			pop BX 
			
			ret
			endp
;--------------------------------------
.data

hotkey_status db 0

proga_end db 0

end start

