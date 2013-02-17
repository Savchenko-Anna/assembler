main:
	; A20
	in	al, 92h
	or	al, 2
	out	92h, al

	; disable interrupts
	cli
	in	al, 70h
	or	al, 80h
	out	70h, al  ; disable NMI

	; linear GDT address
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, GDT
	mov	[GDTR + 2], eax

	; load GDTR
	lgdt fword [GDTR]

	; linear IDT address
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, IDT
	mov	[IDTR + 2], eax

	; load IDTR
	lidt fword [IDTR]

	; switch to PM
	mov	eax, cr0
	or	al, 1
	mov	cr0, eax
	jmp  00001000b:PROTECTED_ENTRY

include 'kernel_data.inc'
include 'kernel_interrupts.inc'
include 'kernel_display.inc' 

; Protected Mode
use32
PROTECTED_ENTRY:
	; DATA
	mov  ax, 00010000b	
	mov  ss, ax
	mov  ds, ax
	mov  esp, 0xFFFF

	mov  ax, 00011000b	; VIDEO
	mov  es, ax
	mov  edi, 0

	; enable interrupts and NMI
	in	 al, 70h
	and  al, 7Fh
	out  70h, al
	sti

	call clear_screen
	mov  dword [cursor], 160

	; waiting for interrupt
	jmp  $


