;		[UNHOLY CATCHER]

.model tiny

.code
org 100h

start:
	extrn DrawFrame_std:far, ROOF:word

	push DS
	mov BX, SEG ROOF
	mov DS, BX
	call DrawFrame_std
	pop DS
comment !
	;		[Recapture 08h interrupt]
	xor BX, BX
	mov ES, BX	; original 08h

	cli
	mov BX, 08h * 4 ; 
	mov ES:[BX],offset vorishka
	mov AX, CS
	mov ES:[BX + 2], AX
	sti

	mov AX, 3100h
	mov DX, offset proga_end
	shr DX, 4
	inc DX
	inc DX
	int 21h	
!
	xor AH, AH
	int 16h

	mov AX, 4c00h
	int 21h

vorishka		proc
			
			push AX
			push BX
			push ES
			push DI

			;		[Get bukva]
			KbrdPrt = 60h
			in AL, KbrdPrt

			KB = 11000001b
			mov AH, KB
			
			VDSEG = 0b800h
			mov BX, VDSEG
			mov ES, BX
			STD_SHIFT = (80d * 8d + 40d) * 2		
			mov DI, STD_SHIFT
			
			;		[Print user's bukva]
			
			cmp AL, 2
			jne skip_regs
			
			mov ES:[DI], AX
			
			mov ES:[DI + 160d], BX
			mov ES:[DI + 320d], CX
			mov ES:[DI + 480d], DX
			
			skip_regs:
			;		[Calm down keyboard]
			GREATEST_BIT = 80h
			mov AH, AL
			or AL, GREATEST_BIT
			
			KbrdPrtMig = 61h
			out KbrdPrtMig, AL

			mov AL, AH
			out KbrdPrtMig, AL

			mov AL, 20h
			out 20h, AL
			
			pop DI
			pop ES
			pop BX
			pop AX
			
			;add AL, 2			

			iret			
			endp
proga_end db 0

.data

end start

