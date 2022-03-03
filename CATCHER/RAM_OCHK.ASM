;                                    FRAME

.model tiny

.code

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
	
	mov BP, 82h
	cmp byte ptr [BP], '2'
	mov BP, 83h
	jne non_user
	
	mov SI, BP
	lea DI, ROOF
	mov BX, DS
	mov ES, BX
	mov CX, 3
	REP movsb

	lea DI, WALL
	mov CX, 3
	REP movsb
	
	lea DI, FLOOR
	mov CX, 3
	REP movsb
	
	mov DI, 82h
	mov AL, '1'
	stosb
	
	non_user:

	wh_back_bl_chars = 01110000b ; white background black char

;	comment !

	mov AH, wh_back_bl_chars
	call DrawFrame

	mov AH, wh_back_bl_chars
	mov SI, 83h
	call PrntTxt
;	!

	; LEN
	mov AH, 02h
	mov BP, 82h
	mov DX, [BP]
	int 21h

	; PAUSE
	xor ax, ax
	int 16h

	; EXIT
	mov AX, 4c00h
	int 21h
;--------------------------------------
;       DrawFrame
; Params:
;	AH - color
;	ROOF		\
;	WALL		|<- 3-byte arrays with filler symbols
;	FLOOR		/
;
; Destroy: AX, BX, CX, DX, DI, SI, BP, ES
;--------------------------------------
DrawFrame	proc

	
		mov BX, VIDEOSEG
		mov ES, BX
		mov BP, 0082h

		; PRINT ROOF

	mov CX, len
	mov SI, offset ROOF
	
	cmp byte ptr [BP], '1'
	je skip
	mov SI, offset ROOF1
	skip:
	
	xor DI, DI
	mov DI, total_shift 
	call DrawLine
	add DI, line

	mov DX, HEIGHT

	; PRINT WALL
	ma_wall:

		mov CX, len
		mov SI, offset WALL

		cmp byte ptr [BP], '1'
		je wall0
		mov SI, offset WALL1
		wall0:

		call DrawLine
		add DI, line

	dec DX
	CMP DX, 0

	JNZ ma_wall

	; PRINT FLOOR

	mov CX, len
	mov SI, offset FLOOR
	
	cmp byte ptr [BP], '1'
	je floor0
	mov SI, offset FLOOR1
	floor0:

	call DrawLine
	
	ret
	endp
;--------------------------------------


;--------------------------------------
;          PrntTxt
; Params:
;	AH - color
;	CX - string length
;	DI - addr of start cell in videoseg
;	SI - addr of text
;
; Destroy: AX, BX, CX, DI, SI, ES
;--------------------------------------
PrntTxt	proc

	; PRINT TEXT
	mov DI, total_shift + (80d + 1d) * 2d
	mov CX, len - 2
	mov BX, VIDEOSEG
	mov ES, BX

	content:

		call PrntStr
		mov CX, len - 2d
		add DI, line + 4d
		
	cmp SI, 0 	; SI gets 0 when string is done
	jne content
 
	ret
	endp
;--------------------------------------


;--------------------------------------
;          Draw Line
; Params:
;	AH - color
;	CX - string length
;	DI - addr of start line 
;	SI - addr of 3 line elements
;	ES - videoseg addr
;
; Destroy: AX, CX, DI, SI
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
;          Print String
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

	cmp SI, 0 ; check whether str is done 
	je str_end

	cld		; DF = 0

str_prnt:	
	
	lodsb		; mov AL, [SI] ; put char from string
			;inc SI ; shift to the next	

	cmp AL, '$'
	je str_end

	stosw		; mov ES:[DI], AX ; put symbol on screen
			; add DI, 2 ; shift to next symbol_place	
	loop str_prnt
	
	ret

str_end:

	mov SI, 0
	ret

	endp
;--------------------------------------


.data

ROOF  db 0C9h, 0CDh, 0BBh
WALL  db 0BAh, ' ', 0BAh
FLOOR db 0C8h, 0CDh, 0BCh

ROOF1  db '/+\'
WALL1  db '+ +'
FLOOR1 db '\+/'

TEST_STR db 'It is another day of sun! City of stars... Why are you shinig so bright?$'

end