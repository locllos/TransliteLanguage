extern _print
extern _scan
extern _sqrt
;Show must go on!
global _start
section .text
_start:
	call zaglaveeye
	mov rax, 60
	syscall
	;[Begin compiling Definition: fact]
	fact: 
		;[Begin compiling Entry]
		push rbp
		mov rbp, rsp
		sub rsp, 0
		;[End compiling Entry]
		;[Begin compiling Block]
			;[Begin compiling Statement]
			;[Begin compiling Condition]
			;[Begin pushing Variable: n]
			mov rax, [rbp + (16)]
			;[End pushing Variable: n]
			push rax
			;[Begin pushing Number]
			mov rax, 2
			;[End pushing Number]
			pop rbx
			;[Begin compiling LogicOperators]
			cmp rbx, rax
			jl LOGIC_0
				xor rax, rax
				jmp END_LOGIC_0
				LOGIC_0: 
				mov rax, 1
			END_LOGIC_0:
			;[End compiling LogicOperators]
			xor rbx, rbx
			cmp rax, rbx
			je IF_END_0
			;[Begin compiling Block]
				;[Begin compiling Statement]
				;[Begin compiling Return]
				;[Begin pushing Number]
				mov rax, 1
				;[End pushing Number]
				;[Begin compiling Exit]
				leave
				ret
				;[End compiling Exit]
				ret
				;[End compiling Return
				;[End compiling Statement]
			;[End compiling Block]
			jmp IF_ELSE_END_0
			IF_END_0:
			IF_END_0:
			IF_ELSE_END_0: 
			;[End compiling Condition]
			;[End compiling Statement]
			;[Begin compiling Statement]
			;[Begin compiling Return]
			;[Begin pushing Variable: n]
			mov rax, [rbp + (16)]
			;[End pushing Variable: n]
			push rax
			;[Begin compiling Call]
				;[Begin pushing Variable: n]
				mov rax, [rbp + (16)]
				;[End pushing Variable: n]
				push rax
				;[Begin pushing Number]
				mov rax, 1
				;[End pushing Number]
				pop rbx
				xchg rax, rbx
				sub rax, rbx
				push rax
				call fact
				add rsp, 8
			;[End compiling Call]
			pop rbx
			imul rax, rbx
			;[Begin compiling Exit]
			leave
			ret
			;[End compiling Exit]
			ret
			;[End compiling Return
			;[End compiling Statement]
		;[End compiling Block]
		;[Begin compiling Exit]
		leave
		ret
		;[End compiling Exit]
	;[End compiling Definition]

;[Begin compiling Definition: zaglaveeye]
zaglaveeye: 
	;[Begin compiling Entry]
	push rbp
	mov rbp, rsp
	sub rsp, 8
	;[End compiling Entry]
	;[Begin compiling Block]
		;[Begin compiling Statement]
		;[Begin compiling Assign]
			;[Begin compiling Call]
				mov rax, rsp
				mov rbx, 16
				xor rdx, rdx
				idiv rbx, sub
				sub rsp, rdx
				call _scan
				add rsp, rdx
			mov [rbp + (-8)], rax
			;[End compiling Assign]
		;[End compiling Statement]
		;[Begin compiling Statement]
		;[Begin compiling Call]
			;[Begin compiling Print]
			;[Begin compiling Call]
				;[Begin pushing Variable: a]
				mov rax, [rbp + (-8)]
				;[End pushing Variable: a]
				push rax
				call fact
				add rsp, 8
			;[End compiling Call]
			mov rdi, rax
			call _print
			;[End compiling Print]
		;[End compiling Statement]
	;[End compiling Block]
	;[Begin compiling Exit]
	leave
	ret
	;[End compiling Exit]
;[End compiling Definition]

;Inside my heart is breaking
;My make up may be flaking
;But my smile, still, stays on
