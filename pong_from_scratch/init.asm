[BITS 16]
[CPU 386]

jmp 0x7C0:begin
begin:
mov ax, cs
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
;a small stack is set up at the end of the available memory from 0x7FF00 to 0x80000 (not included)
mov ax, 0x7FF0
mov ss, ax
mov sp, 0x100

; Reset video mode to 80*25
; mov ax, 0x3
; int 0x10

;Reset disk system
; xor ax, ax
;The BIOS set ds to the boot device
; int 0x13

;Then loads the 2 next sectors
;The datas will be written in memory after this bootsector
;ES have been set at the begining of the program
; mov bx, 512

; mov ax, 0x0202	;ah = BIOS Function (2 = read sectors) al = # of sectors to read: arbitrarly 2
; mov cx, 0x0002	;ch = Cylinder 0 cl = sector 2
; mov dh, 0		;dh = head 0 dl = boot drive (dl is set by the BIOS)
; int 0x13
; jc _error
; mov ax, 512
; jmp ax

; _error:
; ;In case of error display an error message, disable interrupts and then halt the cpu
; mov si, data.error_message
; call print

mov si, data.message
call print

cli
hlt

print:
	push ax
	push si
	mov ax, 0x0E00
	.loop:
		lodsb
		or al, al
		jz .end
		int 0x10
		jmp .loop

	.end:
		pop si
		pop ax
		ret

data:
.message: db "Hello world",10,13,0

times 510 - ($ - $$) db 0
dw 0xAA55
