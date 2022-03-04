;		[FRAME]
.286

.model tiny

.code

public DrawFrame_std, ROOF


org 100h

jmp main
		
main:
	VIDEOSEG = 0b800h
	
	; set frame sizes
	len = 30d  
	HEIGHT = 5d

	; set frame position on screen
	line_shift = 10d
	column_shift = 30d

	weedth = 80d * 2h ; one line shift in VIDEOSEG
	line = weedth - len * 2h ; transition to a new line for frame element
	total_shift = line_shift * weedth + column_shift * 2h
	
	wh_back_bl_chars = 01110000b ; white background black char

	call DrawFrame_std

			;[PAUSE]
	xor ax, ax
	int 16h

			;[EXIT 0]
	mov AX, 4c00h
	int 21h


;--------------------------------------
;		[DrawFrame_std]
;--------------------------------------
;[params]: -
;[return]: draws standard frame
;--------------------------------------
DrawFrame_std		proc
			
			pusha

			mov AH, wh_back_bl_chars
			
			call DrawFrame
			
			popa
			
			ret
			endp
;--------------------------------------


;--------------------------------------
;		[DrawFrame]
;--------------------------------------
;[params]:
;	AH - color
;	ROOF		\
;	WALL		|<- 3-byte arrays with filler symbols
;	FLOOR		/
;--------------------------------------
;[destroy]: 
;	AX, BX, CX, DX, DI, SI, BP, ES
;--------------------------------------
DrawFrame	proc

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
;	AX, CX, DI, SI
;--------------------------------------
DrawLine	proc

		lodsb		; mov AL, [SI]  |  read left corner
					; inc SI  |  shift to middle element
					
		stosw		; mov ES:[DI], AX  |  put in video memory
					; add DI, 2  |  shift to next video cell
		
		lodsb		; mov AL, [SI]  |  read middle element
		
		sub CX, 2   ; now CX is length of middle section 

		mid:		
			stosw		;mov ES:[DI], AX  | put middle element
						;add DI, 2 | shift to next video cell
			loop mid        	
		
		lodsb		; read right corner
		stosw		; put it in video memory
 
		ret
		endp
;--------------------------------------


;--------------------------------------
.data
;--------------------------------------
ROOF  db 0C9h, 0CDh, 0BBh
WALL  db 0BAh, ' ', 0BAh
FLOOR db 0C8h, 0CDh, 0BCh
;--------------------------------------


end