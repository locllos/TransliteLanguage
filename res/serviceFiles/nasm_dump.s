
a.out:     формат файла elf64-x86-64


Дизассемблирование раздела .text:

0000000000401000 <_start>:
  401000:	e8 64 00 00 00       	call   401069 <zaglaveeye>
  401005:	b8 3c 00 00 00       	mov    eax,0x3c
  40100a:	0f 05                	syscall 

000000000040100c <fact>:
  40100c:	55                   	push   rbp
  40100d:	48 89 e5             	mov    rbp,rsp
  401010:	48 83 ec 00          	sub    rsp,0x0
  401014:	48 8b 45 10          	mov    rax,QWORD PTR [rbp+0x10]
  401018:	50                   	push   rax
  401019:	b8 02 00 00 00       	mov    eax,0x2
  40101e:	5b                   	pop    rbx
  40101f:	48 39 c3             	cmp    rbx,rax
  401022:	7c 05                	jl     401029 <LOGIC_0>
  401024:	48 31 c0             	xor    rax,rax
  401027:	eb 05                	jmp    40102e <END_LOGIC_0>

0000000000401029 <LOGIC_0>:
  401029:	b8 01 00 00 00       	mov    eax,0x1

000000000040102e <END_LOGIC_0>:
  40102e:	48 31 db             	xor    rbx,rbx
  401031:	48 39 d8             	cmp    rax,rbx
  401034:	74 0a                	je     401040 <IF_ELSE_END_0>
  401036:	b8 01 00 00 00       	mov    eax,0x1
  40103b:	c9                   	leave  
  40103c:	c3                   	ret    
  40103d:	c3                   	ret    
  40103e:	eb 00                	jmp    401040 <IF_ELSE_END_0>

0000000000401040 <IF_ELSE_END_0>:
  401040:	48 8b 45 10          	mov    rax,QWORD PTR [rbp+0x10]
  401044:	50                   	push   rax
  401045:	48 8b 45 10          	mov    rax,QWORD PTR [rbp+0x10]
  401049:	50                   	push   rax
  40104a:	b8 01 00 00 00       	mov    eax,0x1
  40104f:	5b                   	pop    rbx
  401050:	48 93                	xchg   rbx,rax
  401052:	48 29 d8             	sub    rax,rbx
  401055:	50                   	push   rax
  401056:	e8 b1 ff ff ff       	call   40100c <fact>
  40105b:	48 83 c4 08          	add    rsp,0x8
  40105f:	5b                   	pop    rbx
  401060:	48 0f af c3          	imul   rax,rbx
  401064:	c9                   	leave  
  401065:	c3                   	ret    
  401066:	c3                   	ret    
  401067:	c9                   	leave  
  401068:	c3                   	ret    

0000000000401069 <zaglaveeye>:
  401069:	55                   	push   rbp
  40106a:	48 89 e5             	mov    rbp,rsp
  40106d:	48 83 ec 10          	sub    rsp,0x10
  401071:	b8 06 00 00 00       	mov    eax,0x6
  401076:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
  40107a:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40107e:	50                   	push   rax
  40107f:	e8 88 ff ff ff       	call   40100c <fact>
  401084:	48 83 c4 08          	add    rsp,0x8
  401088:	48 89 45 f0          	mov    QWORD PTR [rbp-0x10],rax
  40108c:	c9                   	leave  
  40108d:	c3                   	ret    
