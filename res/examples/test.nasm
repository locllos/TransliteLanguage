global main

extern _scan
extern _print
extern _sqrt

section .text
main:
    call foo

    mov rdi, 16
    call _sqrt

    mov rdi, rax
    call _print

    mov rax, 3ch
    syscall

foo:
    mov rdi, 3
    call _print
    push 0x123456
    ret



    