.file	"lcd_update.c"
.text
.globl	set_tod_from_secs
.type	set_tod_from_secs, @function
set_tod_from_secs:
.LFB38:
.cfi_startproc
cmpl	$86400, %edi
ja	.L7
movl	$-1851608123, %edx
movl	%edi, %eax
imull	%edx
addl	%edi, %edx
sarl	$11, %edx
movl	%edi, %eax
sarl	$31, %eax
subl	%eax, %edx
movw	%dx, (%rsi)
testw	%dx, %dx
jne	.L3
movw	$12, (%rsi)
movb	$0, 6(%rsi)
jmp	.L4
.L3:
cmpw	$12, %dx
jle	.L5
subl	$12, %edx
movw	%dx, (%rsi)
movb	$1, 6(%rsi)
jmp	.L4
.L5:
cmpw	$12, %dx
jne	.L6
movb	$1, 6(%rsi)
jmp	.L4
.L6:
movb	$0, 6(%rsi)
.L4:
movl	$-1851608123, %edx
movl	%edi, %eax
imull	%edx
addl	%edi, %edx
sarl	$11, %edx
movl	%edi, %eax
sarl	$31, %eax
subl	%eax, %edx
imull	$3600, %edx, %edx
subl	%edx, %edi
movl	%edi, %edx
movswl	%di, %eax
imull	$-30583, %eax, %eax
shrl	$16, %eax
addl	%edi, %eax
sarw	$5, %ax
movl	%edi, %ecx
sarw	$15, %cx
subl	%ecx, %eax
movw	%ax, 2(%rsi)
leal	0(,%rax,4), %ecx
sall	$6, %eax
subl	%ecx, %eax
subl	%eax, %edx
movw	%dx, 4(%rsi)
movl	$0, %eax
ret
.L7:
movl	$1, %eax
ret
.cfi_endproc
.LFE38:
.size	set_tod_from_secs, .-set_tod_from_secs
.globl	set_display_bits_from_tod
.type	set_display_bits_from_tod, @function
set_display_bits_from_tod:
.LFB39:
.cfi_startproc
subq	$72, %rsp
.cfi_def_cfa_offset 80
movq	%fs:40, %rax
movq	%rax, 56(%rsp)
xorl	%eax, %eax
cmpw	$12, %di
jg	.L14
movl	%edi, %eax
sarl	$16, %eax
cmpw	$59, %ax
jg	.L15
movq	%rdi, %rdx
salq	$16, %rdx
sarq	$48, %rdx
cmpw	$59, %dx
jg	.L16
movq	%rdi, %rdx
salq	$8, %rdx
sarq	$56, %rdx
cmpb	$1, %dl
jg	.L17
movl	$63, (%rsp)
movl	$6, 4(%rsp)
movl	$91, 8(%rsp)
movl	$79, 12(%rsp)
movl	$102, 16(%rsp)
movl	$109, 20(%rsp)
movl	$125, 24(%rsp)
movl	$7, 28(%rsp)
movl	$127, 32(%rsp)
movl	$111, 36(%rsp)
movl	$0, 40(%rsp)
testb	%dl, %dl
je	.L18
movl	$2, %edx
jmp	.L10
.L18:
movl	$1, %edx
.L10:
sall	$7, %edx
cmpw	$9, %di
jle	.L11
orl	$6, %edx
sall	$7, %edx
movswl	%di, %ecx
imull	$26215, %ecx, %ecx
sarl	$18, %ecx
movl	%edi, %r8d
sarw	$15, %r8w
subl	%r8d, %ecx
leal	(%rcx,%rcx,4), %ecx
leal	(%rcx,%rcx), %r8d
subl	%r8d, %edi
movswq	%di, %rdi
orl	(%rsp,%rdi,4), %edx
jmp	.L12
.L11:
sall	$7, %edx
movswq	%di, %rdi
orl	(%rsp,%rdi,4), %edx
.L12:
sall	$7, %edx
imull	$26215, %eax, %ecx
sarl	$18, %ecx
movl	%eax, %edi
sarw	$15, %di
subl	%edi, %ecx
movswq	%cx, %rdi
orl	(%rsp,%rdi,4), %edx
sall	$7, %edx
leal	(%rcx,%rcx,4), %ecx
leal	(%rcx,%rcx), %edi
subl	%edi, %eax
movswq	%ax, %rax
orl	(%rsp,%rax,4), %edx
movl	%edx, (%rsi)
movl	$0, %eax
jmp	.L9
.L14:
movl	$1, %eax
jmp	.L9
.L15:
movl	$1, %eax
jmp	.L9
.L16:
movl	$1, %eax
jmp	.L9
.L17:
movl	$1, %eax
.L9:
movq	56(%rsp), %rcx
xorq	%fs:40, %rcx
je	.L13
call	__stack_chk_fail
.L13:
addq	$72, %rsp
.cfi_def_cfa_offset 8
ret
.cfi_endproc
.LFE39:
.size	set_display_bits_from_tod, .-set_display_bits_from_tod
.globl	lcd_update
.type	lcd_update, @function
lcd_update:
.LFB40:
.cfi_startproc
subq	$24, %rsp
.cfi_def_cfa_offset 32
movq	%fs:40, %rax
movq	%rax, 8(%rsp)
xorl	%eax, %eax
movq	%rsp, %rsi
movl	TIME_OF_DAY_SEC(%rip), %edi
call	set_tod_from_secs
cmpl	$1, %eax
je	.L23
movl	$LCD_DISPLAY_PORT, %esi
movq	(%rsp), %rdi
call	set_display_bits_from_tod
cmpl	$1, %eax
jne	.L24
jmp	.L21
.L23:
movl	$1, %eax
jmp	.L21
.L24:
movl	$0, %eax
.L21:
movq	8(%rsp), %rdx
xorq	%fs:40, %rdx
je	.L22
call	__stack_chk_fail
.L22:
addq	$24, %rsp
.cfi_def_cfa_offset 8
ret
.cfi_endproc
.LFE40:
.size	lcd_update, .-lcd_update
.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.6) 5.4.0 20160609"
.section	.note.GNU-stack,"",@progbits
