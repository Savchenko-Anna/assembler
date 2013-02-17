; IRQ 0 - timer
use32
int8_handler:
	call reset_interrupt
	jmp task_switcher

; IRQ 1 - keyboard
int9_handler:
	push ax
	push edi
	xor ax, ax

	in al, 060h
	dec al

	mov ah, al
	and ah, 80h
	jnz clear_request

	; convert positional code to ASCII
	and al, 7Fh
	push edi
	mov edi, ascii
	add di, ax
	mov al, [edi]
	pop edi

	; print symbols
	mov edi, dword [cursor]
	shl edi, 1
	mov byte [es:edi], al
	inc dword [cursor]

	; send confirmation to keyboard port
	in al, 061h
	or al, 80
	out 061h, al
	xor al, 80
	out 061h, al

clear_request:
	pop edi
	pop ax
	jmp int_EOI

; Empty handler. Sends End-Of-Interrupt (EOI) to controller
int_EOI:
	push ax
	mov al, 20h
	out 020h, al	; ... to Master controller
	out 0a0h, al	; ... to Slave controller
	pop ax
	iretd

reset_interrupt:
	push ax
	mov al, 20h
	out 020h, al 
	out 0a0h, al 
	pop ax
	ret

; General protection
exGP_handler:
	pop eax
	mov esi, gp
	int 1
	iretd

syscall_handler:
	pushad
@@:
	lodsb
	mov edi, dword [cursor]
	mov [es:edi*2], al
	inc dword [cursor]
	test al, al
	jnz @b
	popad
	iretd
