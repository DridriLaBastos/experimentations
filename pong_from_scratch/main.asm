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

%define PARAM(N) [bp + (N+1)*2]
%define ARG(N)   [bp + (N+2)*2]

; Setting up graphic mode as medium resolution 320x200 graphic using bios interrupt
; bochs doesn't simulate a 5150 PC so I can't rely on the 5150 documentation.
; instead I use interrupt 10
push sp
mov ax, 0x4; ah = 0 selecte graphic mode  al = 4 CGA 320x200 4 colors
int 0x10
pop sp

; Resetting the stack at the end of the available memory
; First byte of the stack aligned on even address :
;  ss << 4 + sp => 0x7F00 << 4 + 0xFFE => 0x7F000 + 0xFFE=> 0x7FFFE
; Last byte of the stack when sp = 0
mov ax, 0x7F00
mov ss, ax
mov sp, 0xFFE
mov bp, sp

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

; Draw a line horizontally
; %1 x_start
; %2 x_end
; %3 y
; %4 col
%macro DRAW_HLINE 4
	push %4
	push %3
	push %2
	push %1
	call draw_hline
	;TODO: Can optimized by directly modifying values in the stack
	pop ax
	pop ax
	pop ax
	pop ax
%endmacro

; Draw a line vertically
; %1 y_start
; %2 y_end
; %3 x
; %4 col
%macro DRAW_VLINE 4
	push %4
	push %3
	push %2
	push %1
	call draw_vline
	;TODO: Can optimized by directly modifying values in the stack
	pop ax
	pop ax
	pop ax
	pop ax
%endmacro

DRAW_HLINE 0,SCREEN_WIDTH,0,0b11
DRAW_HLINE 0,SCREEN_WIDTH,1,0b11

xchg bx, bx
DRAW_VLINE SCREEN_HEIGH/2-3,SCREEN_HEIGH/2+3,0,0b11

DRAW_HLINE 0,SCREEN_WIDTH,SCREEN_HEIGH-2,0b11
DRAW_HLINE 0,SCREEN_WIDTH,SCREEN_HEIGH-1,0b11

; DRAW_VLINE SCREEN_HEIGH/2-5,SCREEN_HEIGH/2+5,3,0b11

; DRAW_VLINE 2,SCREEN_HEIGH-1,SCREEN_WIDTH/2,0b11

; DRAW_VLINE SCREEN_HEIGH/2-5,SCREEN_HEIGH/2+5,SCREEN_WIDTH-4,0b11

; push word 0b11
; push word SCREEN_HEIGH-2
; push word SCREEN_WIDTH
; push word 0
; call draw_hline

; push word 0
; push word SCREEN_WIDTH
; push word SCREEN_HEIGH-1
; push word 0b11
; call draw_hline

cli
hlt

%macro STACK_GUARD 0
	push bp
	mov bp, sp
%endmacro

%define STACK_RELEASE pop bp

; Putting all the functions at the end so there are not ran when jumping into the top of the file
; parameters are popped from the stack
; Draw a line from the begining of the start offset to the end
; %3 col
; %2 y
; %1 x_end
; %0 x_start
;TODO: Need to perform the x calculation but it will do for now
draw_hline:
	STACK_GUARD

	; puting the color in the whole al register (temporary for simplicity)
	mov ax, ARG(3)
	mov bx, ax
	shl bx, 1
	shl bx, 1
	or ax, bx
	or bx, ax
	shl bx, 1
	shl bx, 1
	shl bx, 1
	shl bx, 1
	or ax, bx
	mov di, ax ; Saving the computed color value in the DI register to access it quicker instead of pushing it

	; * Computing (%2) % 2 == 0,0,0x2000) + ((%2)/2) * SCREEN_WIDTH/4 + (%0)/4)
	mov bx, ARG(2) ; Saving %2 in a register for later use

	; ** Computing (%2) % 2 == 0,0,0x2000)
	mov ax, 0x2000
	mov dx, bx
	and dx, 0b1
	mul dx
	mov si, ax ; Saving the result in the si register

	; ** Computing ((%2)/2) * SCREEN_WIDTH/4
	shr bx, 1		; param %2 is not used anymore so we can alter the value in bx
	mov ax, SCREEN_WIDTH/4
	mul bx
	add si, ax
	; ** Computing (%0)/4)
	mov ax, ARG(0)	; Retrieving the value of %0
	mov bx, ax		; %0 saved for next step
	shr ax, 1
	shr ax, 1		; dividing by 4
	add si, ax

	; Dividing cx by 4 because there is 4 pel per bytes
	mov cx, ARG(1)
	sub cx, bx 		; bx contains %0 from previous step
	shr cx, 1
	shr cx, 1
	mov ax, di
	.draw_loop:
		mov byte [es:si], al
		inc si
		loop .draw_loop
	
	STACK_RELEASE
	ret

; Draw a line vertically
; %3 color
; %2 x
; %1 y_end
; %0 y_start
draw_vline:
	STACK_GUARD

	; first compute the amount of time the loop will be run by putting 
	; %1 - %0 into cx
	mov ax, ARG(0)
	mov cx, ARG(1)
	sub cx, ax

	.draw_loop:
		mov bx, ARG(1)
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

		mov ax, ARG(1)
		shr ax,1
		shr ax,1
		add bp, ax

		; * Compute the position in the color 
		mov al, [es:bp] ; Get the current 4 bytes containing the PEL color data

		; ** Compute the mask containing the position of the requested pel data given by the formula
		;    ~(0b11 << (6 - 2*((%2) % 4)))
		;
		; *** First compute the value (6 - 2*((%2) % 4)) the gives the amount of time to
		;     shift the color data to put it in the right position for the requested PEL
		;
		; **** Compute 2*((%2) % 4)
		mov ax, ARG(2)
		mov dx, 4
		div dx ; ah contains ax % 4 and al contains ax // 4
		mov al, ah
		xor ah, ah
		shl ax, 1

		; **** compute 6 - value
		mov si, cx	; Saving the value of cx because will need the variable version of he shl
					; instructions that only uses the cx register
		mov cx, 6
		sub cx, ax

		; ** Compute ~(0b11 << value)
		mov ax, 0b11
		shl ax, cl
		not ax

		; * Applying the mask to the current PEL data
		mov bl, [es:bp]
		and bl, al

		; * Applying the color data for the PEL
		mov dl, ARG(3)
		shl dx, cl
		or al, dl
		mov [es:bp], dl

		; * restoring back cx
		mov cx, si

		loop .draw_loop
	STACK_RELEASE
