push rsp
xchg rax, rbx
xor rax, rax
add rax, rbx
add r13, 287454020
sub rcx, rbx
sub r12, 287454020
idiv rcx, sub
cmp rax, rbx
cmp rbx, rcx
imul r13, rcx
call function
ret
jmp function
jne function
je function
jle function
jl function
jge function
jg function
syscall
leave
