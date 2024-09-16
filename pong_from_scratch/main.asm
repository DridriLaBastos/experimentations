; This is the main program. It is loaded by the init program at 0x500

%define CRT_INDEX_PORT 0x3D4
%define CRT_DATA_PORT  0x3D5

%define MODE_SELECT_REGISTER_PORT 0x3D8
%define COLOR_SELECT_REGISTER_PORT 0x3D9

%define SCREEN_WIDTH 320
%define SCREEN_HEIGH 200

; Macro for output data to the CRT device
; - %1 index of the crt register
; - %2 data to write
%macro LOAD_CRT_DATA 2

mov dx, CRT_INDEX_PORT
mov ax, %1
out dx, al

mov dx, CRT_DATA_PORT
mov ax, %2
out dx, al

%endmacro

; Write a pel at the specified location
; Register modified: AX, BX, CX, DI
; %1 pos x
; %2 pos y
; %3 2 bits color
%macro WRITE_PEL 3
	; even lines start at memory location 0
	; odd  lines start at memory location 8000
	;
	mov di, %2	;contains Y coordinate
	mov dx, di	;contains Y coordinate
	and dx, 0b1	;0 on even Y coordinate and 1 on odd coordinate
	mov ax, 0x2000
	mul dx		;0 on even Y and 8000 on odd Y
	shr di, 1	;divide by 2
	add di, ax	;8000+Y = memory location of the line to write

	mov ax, %1
	mov bx, 4	;al will contain the quotient (coordinate in the line of the pel)
	div bl		;ah will contain the remainer (position in the 4bits pel of the requested pel)

	;cl is the only register available for variadic shift so we want ah in it
	;but I want it ti hold the amount of shift to the left because first pel is at bits 6-7
	mov cl, 4	;
	sub cl, ah	;
	
	xor bx, bx
	mov bl, al
	add di, bx 	;Now di contains the address of 4 pel bytes where to write the asked pel

	mov dl, [es:di]	;dl contains the 4pel data bytes
	mov al, 0b11
	shl al, cl
	not al
	and dl, al
	mov al, %3
	shl al, cl
	or dl, al
	mov [es:di], dl

%endmacro

; Draw a line from the begining of the start offset to the end
; %1 start x
; %2 end x
; %3 y
; %4 col
;TODO: Need to perform the x calculation but it will do for now
%macro DRAW_HLINE 4

	%assign COL ((%4) & 0b11)

	mov ax, %eval(COL << 6 | COL << 4 | COL << 2 | COL)
	mov bx, %eval(%cond((%3) % 2 == 0,0,0x2000) + ((%3)/2) * SCREEN_WIDTH/4 + (%1)/4)
	
	mov cx, ((%2) - (%1)) / 4
	%%draw:
		mov byte [es:bx], al
		inc bx
		loop %%draw
%endmacro

; Draw a line vertically
; %1 y_start
; %2 y_end
; %3 x
; %4 color
%macro DRAW_VLINE 4
	%assign COLOR ((%4) & 0b11)

	mov cx, (%2) - (%1)
	%%drawing:
		mov bx, (%2)
		sub bx, cx		; contains the current line computed from y_end

		mov ax, 0x2000
		mov dx, bx
		and dx, 0b1
		mul dx			; if current line is odd ax contains 0, otherwise ax contains 0x2000
		mov bp, ax		; saving the value so all other registers can be used

		mov ax, SCREEN_WIDTH/4
		shr bx, 1
		mul bx
		add bp, ax

		add bp, %3/4

		mov al, [es:bp]

		and ax, ~(0b11 << (8 - 2*(%3 % 4)))
		mov dx, COLOR << (8 - 2*(%3 % 4))
		or ax, dx
		mov  [es:bp], al

		loop %%drawing
%endmacro

; Setting up graphic mode as medium resolution 320x200 graphic using bios interrupt
; bochs doesn't simulate a 5150 PC so I can't rely on the 5150 documentation.
; instead I use interrupt 10
push sp
mov ax, 0x4; ah = 0 selecte graphic mode  al = 4 CGA 320x200 4 colors
int 0x10
pop sp

; Programming the color select register
; This register will configure the background color
; We want a black background
; to configure the colors by bit 0-3 (b g r i).
; To have a black background b g r bits are set to 0, and just to be fun we
; intensify the border color (bit 3 = 1)
mov ax, 0b100000
mov dx, COLOR_SELECT_REGISTER_PORT
out dx, ax

mov cx, 8000
mov ax, 0xB800
mov es, ax

DRAW_HLINE 0,SCREEN_WIDTH,0,0b11
DRAW_HLINE 0,SCREEN_WIDTH,1,0b11

; xchg bx, bx
DRAW_VLINE 0,SCREEN_HEIGH-1,0,0b11
DRAW_VLINE 0,SCREEN_HEIGH-1,1,0b10
DRAW_VLINE 0,SCREEN_HEIGH-1,2,0b01
DRAW_VLINE 0,SCREEN_HEIGH-1,3,0b11
DRAW_VLINE 0,SCREEN_HEIGH-1,4,0b10

DRAW_HLINE 0,SCREEN_WIDTH,SCREEN_HEIGH-2,0b11
DRAW_HLINE 0,SCREEN_WIDTH,SCREEN_HEIGH-1,0b11

cli
hlt
