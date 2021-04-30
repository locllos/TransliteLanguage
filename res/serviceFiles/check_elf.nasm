global _start

section .text
_start:
	nop
	nop
	
	mov rax, 100
	mov rbx, 1000
	add rax, rbx
	
	mov rax, 3ch
	syscall
