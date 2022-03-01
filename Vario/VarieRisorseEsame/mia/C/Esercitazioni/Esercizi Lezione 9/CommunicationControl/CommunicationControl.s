	.file	"CommunicationControl.c"
	.text
	.section	.rodata
.LC0:
	.string	"scrivo pid"
.LC1:
	.string	"%d"
.LC2:
	.string	"Errore scrittura 2"
.LC3:
	.string	"scrivo random"
.LC4:
	.string	"Comando: "
.LC5:
	.string	"%s"
.LC6:
	.string	"Messaggio dal figlio --> %s\n"
.LC7:
	.string	"Out of range!"
	.align 8
.LC8:
	.string	"Terminazione di tutti i figli in corso...."
.LC9:
	.string	"Completeto!"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$408, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movl	%edi, -372(%rbp)
	movq	%rsi, -384(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -56(%rbp)
	xorl	%eax, %eax
	movq	-384(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -344(%rbp)
	movl	-344(%rbp), %eax
	movslq	%eax, %rdx
	subq	$1, %rdx
	movq	%rdx, -320(%rbp)
	movslq	%eax, %rdx
	movq	%rdx, -400(%rbp)
	movq	$0, -392(%rbp)
	movslq	%eax, %rdx
	movq	%rdx, -416(%rbp)
	movq	$0, -408(%rbp)
	cltq
	leaq	0(,%rax,8), %rdx
	movl	$16, %eax
	subq	$1, %rax
	addq	%rdx, %rax
	movl	$16, %ebx
	movl	$0, %edx
	divq	%rbx
	imulq	$16, %rax, %rax
	movq	%rax, %rdx
	andq	$-4096, %rdx
	movq	%rsp, %rbx
	subq	%rdx, %rbx
	movq	%rbx, %rdx
.L2:
	cmpq	%rdx, %rsp
	je	.L3
	subq	$4096, %rsp
	orq	$0, 4088(%rsp)
	jmp	.L2
.L3:
	movq	%rax, %rdx
	andl	$4095, %edx
	subq	%rdx, %rsp
	movq	%rax, %rdx
	andl	$4095, %edx
	testq	%rdx, %rdx
	je	.L4
	andl	$4095, %eax
	subq	$8, %rax
	addq	%rsp, %rax
	orq	$0, (%rax)
.L4:
	movq	%rsp, %rax
	addq	$3, %rax
	shrq	$2, %rax
	salq	$2, %rax
	movq	%rax, -312(%rbp)
	movl	-344(%rbp), %eax
	movslq	%eax, %rdx
	subq	$1, %rdx
	movq	%rdx, -304(%rbp)
	movslq	%eax, %rdx
	movq	%rdx, -432(%rbp)
	movq	$0, -424(%rbp)
	movslq	%eax, %rdx
	movq	%rdx, %r12
	movl	$0, %r13d
	cltq
	leaq	0(,%rax,8), %rdx
	movl	$16, %eax
	subq	$1, %rax
	addq	%rdx, %rax
	movl	$16, %ebx
	movl	$0, %edx
	divq	%rbx
	imulq	$16, %rax, %rax
	movq	%rax, %rdx
	andq	$-4096, %rdx
	movq	%rsp, %rbx
	subq	%rdx, %rbx
	movq	%rbx, %rdx
.L5:
	cmpq	%rdx, %rsp
	je	.L6
	subq	$4096, %rsp
	orq	$0, 4088(%rsp)
	jmp	.L5
.L6:
	movq	%rax, %rdx
	andl	$4095, %edx
	subq	%rdx, %rsp
	movq	%rax, %rdx
	andl	$4095, %edx
	testq	%rdx, %rdx
	je	.L7
	andl	$4095, %eax
	subq	$8, %rax
	addq	%rsp, %rax
	orq	$0, (%rax)
.L7:
	movq	%rsp, %rax
	addq	$3, %rax
	shrq	$2, %rax
	salq	$2, %rax
	movq	%rax, -296(%rbp)
	movl	$0, -348(%rbp)
	jmp	.L8
.L16:
	movl	-348(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-312(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, %rdi
	call	pipe@PLT
	movl	%eax, -332(%rbp)
	movl	-348(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-296(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, %rdi
	call	pipe@PLT
	movl	%eax, -328(%rbp)
	call	fork@PLT
	movl	%eax, -324(%rbp)
	cmpl	$0, -324(%rbp)
	jne	.L9
	movq	-312(%rbp), %rax
	movl	-348(%rbp), %edx
	movslq	%edx, %rdx
	movl	4(%rax,%rdx,8), %eax
	movl	%eax, %edi
	call	close@PLT
	movq	-296(%rbp), %rax
	movl	-348(%rbp), %edx
	movslq	%edx, %rdx
	movl	(%rax,%rdx,8), %eax
	movl	%eax, %edi
	call	close@PLT
.L15:
	movq	-312(%rbp), %rax
	movl	-348(%rbp), %edx
	movslq	%edx, %rdx
	movl	(%rax,%rdx,8), %eax
	leaq	-272(%rbp), %rcx
	movl	$1, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	read@PLT
	movzbl	-272(%rbp), %eax
	cmpb	$105, %al
	jne	.L10
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	call	getpid@PLT
	movl	%eax, %edx
	leaq	-160(%rbp), %rax
	leaq	.LC1(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	sprintf@PLT
	leaq	-160(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movq	%rax, %rsi
	movq	-296(%rbp), %rax
	movl	-348(%rbp), %edx
	movslq	%edx, %rdx
	movl	4(%rax,%rdx,8), %eax
	leaq	-160(%rbp), %rcx
	movq	%rsi, %rdx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	write@PLT
	cmpq	$-1, %rax
	jne	.L10
	leaq	.LC2(%rip), %rdi
	call	puts@PLT
.L10:
	movzbl	-272(%rbp), %eax
	cmpb	$114, %al
	jne	.L12
	movl	$0, %edi
	call	time@PLT
	movl	%eax, %edi
	call	srand@PLT
	leaq	.LC3(%rip), %rdi
	call	puts@PLT
	call	rand@PLT
	movl	%eax, %edx
	leaq	-160(%rbp), %rax
	leaq	.LC1(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	sprintf@PLT
	leaq	-160(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movq	%rax, %rsi
	movq	-296(%rbp), %rax
	movl	-348(%rbp), %edx
	movslq	%edx, %rdx
	movl	4(%rax,%rdx,8), %eax
	leaq	-160(%rbp), %rcx
	movq	%rsi, %rdx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	write@PLT
	cmpq	$-1, %rax
	jne	.L12
	leaq	.LC2(%rip), %rdi
	call	puts@PLT
.L12:
	movzbl	-272(%rbp), %eax
	cmpb	$113, %al
	jne	.L15
	movl	$0, %edi
	call	exit@PLT
.L9:
	movq	-312(%rbp), %rax
	movl	-348(%rbp), %edx
	movslq	%edx, %rdx
	movl	(%rax,%rdx,8), %eax
	movl	%eax, %edi
	call	close@PLT
	movq	-296(%rbp), %rax
	movl	-348(%rbp), %edx
	movslq	%edx, %rdx
	movl	4(%rax,%rdx,8), %eax
	movl	%eax, %edi
	call	close@PLT
	addl	$1, -348(%rbp)
.L8:
	movl	-348(%rbp), %eax
	cmpl	-344(%rbp), %eax
	jl	.L16
.L30:
	movl	$0, -340(%rbp)
	movl	$1, %edi
	call	sleep@PLT
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	stdout(%rip), %rax
	movq	%rax, %rdi
	call	fflush@PLT
	leaq	-272(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC5(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movzbl	-272(%rbp), %eax
	cmpb	$105, %al
	je	.L17
	movzbl	-272(%rbp), %eax
	cmpb	$114, %al
	jne	.L18
.L17:
	movq	%rsp, %rax
	movq	%rax, %r12
	leaq	-272(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movq	%rax, %rdx
	subq	$1, %rdx
	movq	%rdx, -288(%rbp)
	movq	%rax, %r14
	movl	$0, %r15d
	movq	%rax, -448(%rbp)
	movq	$0, -440(%rbp)
	movl	$16, %edx
	subq	$1, %rdx
	addq	%rdx, %rax
	movl	$16, %ecx
	movl	$0, %edx
	divq	%rcx
	imulq	$16, %rax, %rax
	movq	%rax, %rdx
	andq	$-4096, %rdx
	movq	%rsp, %rbx
	subq	%rdx, %rbx
	movq	%rbx, %rdx
.L19:
	cmpq	%rdx, %rsp
	je	.L20
	subq	$4096, %rsp
	orq	$0, 4088(%rsp)
	jmp	.L19
.L20:
	movq	%rax, %rdx
	andl	$4095, %edx
	subq	%rdx, %rsp
	movq	%rax, %rdx
	andl	$4095, %edx
	testq	%rdx, %rdx
	je	.L21
	andl	$4095, %eax
	subq	$8, %rax
	addq	%rsp, %rax
	orq	$0, (%rax)
.L21:
	movq	%rsp, %rax
	addq	$0, %rax
	movq	%rax, -280(%rbp)
	movl	$0, -352(%rbp)
	jmp	.L22
.L23:
	movl	-352(%rbp), %eax
	addl	$1, %eax
	cltq
	movzbl	-272(%rbp,%rax), %ecx
	movq	-280(%rbp), %rdx
	movl	-352(%rbp), %eax
	cltq
	movb	%cl, (%rdx,%rax)
	addl	$1, -352(%rbp)
.L22:
	movl	-352(%rbp), %eax
	movslq	%eax, %rbx
	leaq	-272(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	cmpq	%rax, %rbx
	jb	.L23
	movq	-280(%rbp), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -336(%rbp)
	movl	-336(%rbp), %eax
	cmpl	-344(%rbp), %eax
	jg	.L24
	movl	-336(%rbp), %eax
	leal	-1(%rax), %edx
	movq	-312(%rbp), %rax
	movslq	%edx, %rdx
	movl	4(%rax,%rdx,8), %eax
	leaq	-272(%rbp), %rcx
	movl	$1, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	write@PLT
	movl	-336(%rbp), %eax
	leal	-1(%rax), %edx
	movq	-296(%rbp), %rax
	movslq	%edx, %rdx
	movl	(%rax,%rdx,8), %eax
	leaq	-160(%rbp), %rcx
	movl	$100, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	read@PLT
	leaq	-160(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC6(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L25
.L24:
	leaq	.LC7(%rip), %rdi
	call	puts@PLT
.L25:
	movq	%r12, %rsp
.L18:
	movzbl	-272(%rbp), %eax
	cmpb	$113, %al
	jne	.L30
	movl	$0, -356(%rbp)
	jmp	.L27
.L28:
	movq	-312(%rbp), %rax
	movl	-356(%rbp), %edx
	movslq	%edx, %rdx
	movl	4(%rax,%rdx,8), %eax
	leaq	-272(%rbp), %rcx
	movl	$1, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	write@PLT
	addl	$1, -356(%rbp)
.L27:
	movl	-356(%rbp), %eax
	cmpl	-344(%rbp), %eax
	jl	.L28
	leaq	.LC8(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
.L29:
	movl	$0, %edi
	call	wait@PLT
	testl	%eax, %eax
	jg	.L29
	leaq	.LC9(%rip), %rdi
	call	puts@PLT
	movl	$0, %edi
	call	exit@PLT
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
