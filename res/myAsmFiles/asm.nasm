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
	;[Begin compiling Definition: poshcheetayOdnoChlen]
	poshcheetayOdnoChlen: 
		;[Begin compiling Entry]
		push rbp
		mov rbp, rsp
		sub rsp, 0
		;[End compiling Entry]
		;[Begin compiling Block]
			;[Begin compiling Statement]
			;[Begin compiling Condition]
			;[Begin pushing Variable: A]
			mov rax, [rbp + (16)]
			;[End pushing Variable: A]
			push rax
			;[Begin pushing Number]
			mov rax, 0
			;[End pushing Number]
			pop rbx
			;[Begin compiling LogicOperators]
			cmp rbx, rax
			je LOGIC_0
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
				;[Begin compiling Condition]
				;[Begin pushing Variable: B]
				mov rax, [rbp + (24)]
				;[End pushing Variable: B]
				push rax
				;[Begin pushing Number]
				mov rax, 0
				;[End pushing Number]
				pop rbx
				;[Begin compiling LogicOperators]
				cmp rbx, rax
				je LOGIC_1
					xor rax, rax
					jmp END_LOGIC_1
					LOGIC_1: 
					mov rax, 1
				END_LOGIC_1:
				;[End compiling LogicOperators]
				xor rbx, rbx
				cmp rax, rbx
				je IF_END_1
				;[Begin compiling Block]
					;[Begin compiling Statement]
					;[Begin compiling Call]
						;[Begin compiling Print]
						;[Begin pushing Number]
						mov rax, 8
						;[End pushing Number]
						mov rdi, rax
						call _print
						;[End compiling Print]
					;[End compiling Statement]
				;[End compiling Block]
				jmp IF_ELSE_END_1
				IF_END_1:
				;[Begin compiling Block]
					;[Begin compiling Statement]
					;[Begin compiling Call]
						;[Begin compiling Print]
						;[Begin pushing Number]
						mov rax, 0
						;[End pushing Number]
						mov rdi, rax
						call _print
						;[End compiling Print]
					;[End compiling Statement]
				;[End compiling Block]
				IF_END_1:
				IF_ELSE_END_1: 
				;[End compiling Condition]
				;[End compiling Statement]
			;[End compiling Block]
			jmp IF_ELSE_END_0
			IF_END_0:
			;[Begin compiling Block]
				;[Begin compiling Statement]
				;[Begin compiling Call]
					;[Begin compiling Print]
					;[Begin pushing Number]
					mov rax, 1
					;[End pushing Number]
					mov rdi, rax
					call _print
					;[End compiling Print]
				;[End compiling Statement]
				;[Begin compiling Statement]
				;[Begin compiling Call]
					;[Begin compiling Print]
					;[Begin pushing Number]
					mov rax, -1
					;[End pushing Number]
					push rax
					;[Begin pushing Variable: B]
					mov rax, [rbp + (24)]
					;[End pushing Variable: B]
					pop rbx
					imul rax, rbx
					push rax
					;[Begin pushing Variable: A]
					mov rax, [rbp + (16)]
					;[End pushing Variable: A]
					pop rbx
					xor rdx, rdx
					xchg rax, rbx
					idiv rbx, sub
					mov rdi, rax
					call _print
					;[End compiling Print]
				;[End compiling Statement]
			;[End compiling Block]
			IF_END_0:
			IF_ELSE_END_0: 
			;[End compiling Condition]
			;[End compiling Statement]
		;[End compiling Block]
		;[Begin compiling Exit]
		leave
		ret
		;[End compiling Exit]
	;[End compiling Definition]

;[Begin compiling Definition: poshcheetayDvaChlen]
poshcheetayDvaChlen: 
	;[Begin compiling Entry]
	push rbp
	mov rbp, rsp
	sub rsp, 16
	;[End compiling Entry]
	;[Begin compiling Block]
		;[Begin compiling Statement]
		;[Begin compiling Assign]
			;[Begin pushing Variable: B]
			mov rax, [rbp + (24)]
			;[End pushing Variable: B]
			push rax
			;[Begin pushing Variable: B]
			mov rax, [rbp + (24)]
			;[End pushing Variable: B]
			pop rbx
			imul rax, rbx
			push rax
			;[Begin pushing Number]
			mov rax, 4
			;[End pushing Number]
			push rax
			;[Begin pushing Variable: A]
			mov rax, [rbp + (16)]
			;[End pushing Variable: A]
			pop rbx
			imul rax, rbx
			push rax
			;[Begin pushing Variable: C]
			mov rax, [rbp + (32)]
			;[End pushing Variable: C]
			pop rbx
			imul rax, rbx
			pop rbx
			xchg rax, rbx
			sub rax, rbx
			mov [rbp + (-8)], rax
			;[End compiling Assign]
		;[End compiling Statement]
		;[Begin compiling Statement]
		;[Begin compiling Condition]
		;[Begin pushing Variable: Diskreemenat]
		mov rax, [rbp + (-8)]
		;[End pushing Variable: Diskreemenat]
		push rax
		;[Begin pushing Number]
		mov rax, 0
		;[End pushing Number]
		pop rbx
		;[Begin compiling LogicOperators]
		cmp rbx, rax
		je LOGIC_2
			xor rax, rax
			jmp END_LOGIC_2
			LOGIC_2: 
			mov rax, 1
		END_LOGIC_2:
		;[End compiling LogicOperators]
		xor rbx, rbx
		cmp rax, rbx
		je IF_END_2
		;[Begin compiling Block]
			;[Begin compiling Statement]
			;[Begin compiling Call]
				;[Begin compiling Print]
				;[Begin pushing Number]
				mov rax, 1
				;[End pushing Number]
				mov rdi, rax
				call _print
				;[End compiling Print]
			;[End compiling Statement]
			;[Begin compiling Statement]
			;[Begin compiling Call]
				;[Begin compiling Print]
				;[Begin pushing Number]
				mov rax, -1
				;[End pushing Number]
				push rax
				;[Begin pushing Variable: B]
				mov rax, [rbp + (24)]
				;[End pushing Variable: B]
				pop rbx
				imul rax, rbx
				push rax
				;[Begin pushing Number]
				mov rax, 2
				;[End pushing Number]
				push rax
				;[Begin pushing Variable: A]
				mov rax, [rbp + (16)]
				;[End pushing Variable: A]
				pop rbx
				imul rax, rbx
				pop rbx
				xor rdx, rdx
				xchg rax, rbx
				idiv rbx, sub
				mov rdi, rax
				call _print
				;[End compiling Print]
			;[End compiling Statement]
		;[End compiling Block]
		jmp IF_ELSE_END_2
		IF_END_2:
		;[Begin compiling Block]
			;[Begin compiling Statement]
			;[Begin compiling Condition]
			;[Begin pushing Variable: Diskreemenat]
			mov rax, [rbp + (-8)]
			;[End pushing Variable: Diskreemenat]
			push rax
			;[Begin pushing Number]
			mov rax, 0
			;[End pushing Number]
			pop rbx
			;[Begin compiling LogicOperators]
			cmp rbx, rax
			jg LOGIC_3
				xor rax, rax
				jmp END_LOGIC_3
				LOGIC_3: 
				mov rax, 1
			END_LOGIC_3:
			;[End compiling LogicOperators]
			xor rbx, rbx
			cmp rax, rbx
			je IF_END_3
			;[Begin compiling Block]
				;[Begin compiling Statement]
				;[Begin compiling Call]
					;[Begin compiling Print]
					;[Begin pushing Number]
					mov rax, 2
					;[End pushing Number]
					mov rdi, rax
					call _print
					;[End compiling Print]
				;[End compiling Statement]
				;[Begin compiling Statement]
				;[Begin compiling Assign]
					;[Begin compiling Call]
						;[Begin pushing Variable: Diskreemenat]
						mov rax, [rbp + (-8)]
						;[End pushing Variable: Diskreemenat]
						mov rdi, rax
						call _sqrt
					mov [rbp + (-16)], rax
					;[End compiling Assign]
				;[End compiling Statement]
				;[Begin compiling Statement]
				;[Begin compiling Call]
					;[Begin compiling Print]
					;[Begin pushing Variable: koreyn_ot_Diskreemenatah]
					mov rax, [rbp + (-16)]
					;[End pushing Variable: koreyn_ot_Diskreemenatah]
					mov rdi, rax
					call _print
					;[End compiling Print]
				;[End compiling Statement]
				;[Begin compiling Statement]
				;[Begin compiling Call]
					;[Begin compiling Print]
					;[Begin pushing Number]
					mov rax, -1
					;[End pushing Number]
					push rax
					;[Begin pushing Variable: B]
					mov rax, [rbp + (24)]
					;[End pushing Variable: B]
					pop rbx
					imul rax, rbx
					push rax
					;[Begin pushing Variable: koreyn_ot_Diskreemenatah]
					mov rax, [rbp + (-16)]
					;[End pushing Variable: koreyn_ot_Diskreemenatah]
					pop rbx
					xchg rax, rbx
					sub rax, rbx
					push rax
					;[Begin pushing Number]
					mov rax, 2
					;[End pushing Number]
					push rax
					;[Begin pushing Variable: A]
					mov rax, [rbp + (16)]
					;[End pushing Variable: A]
					pop rbx
					imul rax, rbx
					pop rbx
					xor rdx, rdx
					xchg rax, rbx
					idiv rbx, sub
					mov rdi, rax
					call _print
					;[End compiling Print]
				;[End compiling Statement]
				;[Begin compiling Statement]
				;[Begin compiling Call]
					;[Begin compiling Print]
					;[Begin pushing Number]
					mov rax, -1
					;[End pushing Number]
					push rax
					;[Begin pushing Variable: B]
					mov rax, [rbp + (24)]
					;[End pushing Variable: B]
					pop rbx
					imul rax, rbx
					push rax
					;[Begin pushing Variable: koreyn_ot_Diskreemenatah]
					mov rax, [rbp + (-16)]
					;[End pushing Variable: koreyn_ot_Diskreemenatah]
					pop rbx
					add rax, rbx
					push rax
					;[Begin pushing Number]
					mov rax, 2
					;[End pushing Number]
					push rax
					;[Begin pushing Variable: A]
					mov rax, [rbp + (16)]
					;[End pushing Variable: A]
					pop rbx
					imul rax, rbx
					pop rbx
					xor rdx, rdx
					xchg rax, rbx
					idiv rbx, sub
					mov rdi, rax
					call _print
					;[End compiling Print]
				;[End compiling Statement]
			;[End compiling Block]
			jmp IF_ELSE_END_3
			IF_END_3:
			;[Begin compiling Block]
				;[Begin compiling Statement]
				;[Begin compiling Call]
					;[Begin compiling Print]
					;[Begin pushing Number]
					mov rax, 0
					;[End pushing Number]
					mov rdi, rax
					call _print
					;[End compiling Print]
				;[End compiling Statement]
			;[End compiling Block]
			IF_END_3:
			IF_ELSE_END_3: 
			;[End compiling Condition]
			;[End compiling Statement]
		;[End compiling Block]
		IF_END_2:
		IF_ELSE_END_2: 
		;[End compiling Condition]
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
	sub rsp, 24
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
		;[Begin compiling Assign]
			;[Begin compiling Call]
				mov rax, rsp
				mov rbx, 16
				xor rdx, rdx
				idiv rbx, sub
				sub rsp, rdx
				call _scan
				add rsp, rdx
			mov [rbp + (-16)], rax
			;[End compiling Assign]
		;[End compiling Statement]
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
			mov [rbp + (-24)], rax
			;[End compiling Assign]
		;[End compiling Statement]
		;[Begin compiling Statement]
		;[Begin compiling Condition]
		;[Begin pushing Variable: A]
		mov rax, [rbp + (-8)]
		;[End pushing Variable: A]
		push rax
		;[Begin pushing Number]
		mov rax, 0
		;[End pushing Number]
		pop rbx
		;[Begin compiling LogicOperators]
		cmp rbx, rax
		je LOGIC_4
			xor rax, rax
			jmp END_LOGIC_4
			LOGIC_4: 
			mov rax, 1
		END_LOGIC_4:
		;[End compiling LogicOperators]
		xor rbx, rbx
		cmp rax, rbx
		je IF_END_4
		;[Begin compiling Block]
			;[Begin compiling Statement]
			;[Begin compiling Call]
				;[Begin pushing Variable: C]
				mov rax, [rbp + (-24)]
				;[End pushing Variable: C]
				push rax
				;[Begin pushing Variable: B]
				mov rax, [rbp + (-16)]
				;[End pushing Variable: B]
				push rax
				call poshcheetayOdnoChlen
				add rsp, 16
			;[End compiling Call]
			;[End compiling Statement]
		;[End compiling Block]
		jmp IF_ELSE_END_4
		IF_END_4:
		;[Begin compiling Block]
			;[Begin compiling Statement]
			;[Begin compiling Call]
				;[Begin pushing Variable: C]
				mov rax, [rbp + (-24)]
				;[End pushing Variable: C]
				push rax
				;[Begin pushing Variable: B]
				mov rax, [rbp + (-16)]
				;[End pushing Variable: B]
				push rax
				;[Begin pushing Variable: A]
				mov rax, [rbp + (-8)]
				;[End pushing Variable: A]
				push rax
				call poshcheetayDvaChlen
				add rsp, 24
			;[End compiling Call]
			;[End compiling Statement]
		;[End compiling Block]
		IF_END_4:
		IF_ELSE_END_4: 
		;[End compiling Condition]
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
