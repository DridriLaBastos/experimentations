; This is the main program. It is loaded by the init program at 0x500

%define CRT_INDEX_PORT 0x3D4
%define CRT_DATA_PORT  0x3D5

%define MODE_SELECT_REGISTER_PORT 0x3D8
%define COLOR_SELECT_REGISTER_PORT 0x3D9

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

; Setting up graphic mode as medium resolution 320x200 graphic
; The sequence of events is described in the IBM PC documentation
; 2 - 60
;
; 1 - Determine mode of operation : we want CGA 320x200 APA (All Point Addressable)
; fetch more info

; 2 - Reset video mode
; Done by putting a 0 in the 3rd bit of the mode register
mov ax, 0
mov dx, MODE_SELECT_REGISTER_PORT
out dx, al

; 3 - program the 6845 registers for graphic mode
; Following IBM documentation page 2 - 56
LOAD_CRT_DATA 0x0, 0x38
LOAD_CRT_DATA 0x1, 0x28
LOAD_CRT_DATA 0x2, 0x2D
LOAD_CRT_DATA 0x3, 0x0A
LOAD_CRT_DATA 0x4, 0x7F
LOAD_CRT_DATA 0x5, 0x06
LOAD_CRT_DATA 0x6, 0x64
LOAD_CRT_DATA 0x7, 0x70
LOAD_CRT_DATA 0x8, 0x02
LOAD_CRT_DATA 0x9, 0x01
LOAD_CRT_DATA 0xA, 0x06
LOAD_CRT_DATA 0xB, 0x07
LOAD_CRT_DATA 0xC,    0
LOAD_CRT_DATA 0xD,    0

; 3 - Configure the mode select and color select register

; 3.1 - Programming the color select register
; This register will configure the background color
; We want a black background so we put a 0 on bit 5 (0 or 1 as no differences)
; to configure the colors by bit 0-3 (b g r i).
; To have a black background b g r bits are set to 0, and just to be fun we
; intensify the border color (bit 3 = 1)
mov ax, 0
mov dx, COLOR_SELECT_REGISTER_PORT
out dx, ax

; 3.2 We enable the requested video (value taken from IBM doc 2 - 59)
mov ax, 0b010101 ;0 at the end to disable blink attribute
mov dx, MODE_SELECT_REGISTER_PORT
out dx, al

; clearing the screen

mov cx, 8000
mov ax, 0xB800
mov es, ax
xor bx, bx
clear_screen:
	mov byte [es:bx], 0
	inc bx
	inc bx
	loop clear_screen

cli
hlt