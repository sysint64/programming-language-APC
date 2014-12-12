	.text
	.file	"a.out.ll"
	.globl	createAllocator
	.align	16, 0x90
	.type	createAllocator,@function
createAllocator:                        # @createAllocator
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movq	%rsi, -16(%rsp)
	movq	%rdx, -24(%rsp)
	movq	-8(%rsp), %rax
	movq	%rdx, 8(%rax)
	movq	-16(%rsp), %rax
	movq	-8(%rsp), %rcx
	movq	%rax, 16(%rcx)
	movq	-8(%rsp), %rax
	movq	$0, 24(%rax)
	movq	-8(%rsp), %rax
	movq	$0, 32(%rax)
	retq
.Ltmp0:
	.size	createAllocator, .Ltmp0-createAllocator
	.cfi_endproc

	.globl	strLen
	.align	16, 0x90
	.type	strLen,@function
strLen:                                 # @strLen
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movb	(%rdi), %al
	movb	%al, -9(%rsp)
	movl	$0, -16(%rsp)
	jmp	.LBB1_1
	.align	16, 0x90
.LBB1_2:                                # %while.do
                                        #   in Loop: Header=BB1_1 Depth=1
	incl	-16(%rsp)
.LBB1_1:                                # %while
                                        # =>This Inner Loop Header: Depth=1
	cmpb	$0, -9(%rsp)
	jne	.LBB1_2
# BB#3:                                 # %while.end
	movl	-16(%rsp), %eax
	retq
.Ltmp1:
	.size	strLen, .Ltmp1-strLen
	.cfi_endproc

	.globl	setLength
	.align	16, 0x90
	.type	setLength,@function
setLength:                              # @setLength
	.cfi_startproc
# BB#0:                                 # %code
	subq	$56, %rsp
.Ltmp2:
	.cfi_def_cfa_offset 64
	movq	%rdi, 48(%rsp)
	movl	%esi, 44(%rsp)
	movq	48(%rsp), %rax
	cmpl	$0, 4(%rax)
	je	.LBB2_1
# BB#2:                                 # %if.else
	movl	44(%rsp), %eax
	movq	48(%rsp), %rcx
	movl	4(%rcx), %ecx
	leal	1(%rax,%rcx), %eax
	movslq	%eax, %rdi
	callq	new
	movq	%rax, 32(%rsp)
	movl	$0, 20(%rsp)
	jmp	.LBB2_3
	.align	16, 0x90
.LBB2_4:                                # %for.do
                                        #   in Loop: Header=BB2_3 Depth=1
	movq	48(%rsp), %rax
	movq	8(%rax), %rax
	movslq	20(%rsp), %rcx
	movb	(%rax,%rcx), %al
	movq	32(%rsp), %rdx
	movb	%al, (%rdx,%rcx)
	incl	20(%rsp)
.LBB2_3:                                # %for
                                        # =>This Inner Loop Header: Depth=1
	movl	20(%rsp), %eax
	movq	48(%rsp), %rcx
	cmpl	4(%rcx), %eax
	jle	.LBB2_4
# BB#5:                                 # %for.end
	leaq	8(%rsp), %rdi
	callq	delete
	addq	$56, %rsp
	retq
.LBB2_1:                                # %if
	movl	44(%rsp), %eax
	incl	%eax
	movq	48(%rsp), %rcx
	movl	%eax, 4(%rcx)
	movl	44(%rsp), %eax
	incl	%eax
	movslq	%eax, %rdi
	callq	new
	movq	48(%rsp), %rcx
	movq	%rax, 8(%rcx)
	addq	$56, %rsp
	retq
.Ltmp3:
	.size	setLength, .Ltmp3-setLength
	.cfi_endproc

	.globl	binary.add
	.align	16, 0x90
	.type	binary.add,@function
binary.add:                             # @binary.add
	.cfi_startproc
# BB#0:                                 # %code
	pushq	%rbx
.Ltmp4:
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
.Ltmp5:
	.cfi_def_cfa_offset 48
.Ltmp6:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	%rsi, 24(%rsp)
	movq	%rdx, 16(%rsp)
	movq	24(%rsp), %rax
	movl	4(%rax), %esi
	addl	4(%rdx), %esi
	leaq	(%rsp), %rdi
	callq	setLength
	movq	8(%rsp), %rdi
	movq	24(%rsp), %rax
	movq	8(%rax), %rsi
	movslq	4(%rax), %rdx
	callq	memcpy
	movq	24(%rsp), %rax
	movslq	4(%rax), %rdi
	addq	8(%rsp), %rdi
	movq	16(%rsp), %rax
	movq	8(%rax), %rsi
	movslq	4(%rax), %rdx
	callq	memcpy
	movq	8(%rsp), %rax
	movslq	4(%rsp), %rcx
	movb	$0, (%rax,%rcx)
	movb	(%rsp), %al
	movl	4(%rsp), %ecx
	movq	8(%rsp), %rdx
	movq	%rdx, 8(%rbx)
	movl	%ecx, 4(%rbx)
	movb	%al, (%rbx)
	movq	%rbx, %rax
	addq	$32, %rsp
	popq	%rbx
	retq
.Ltmp7:
	.size	binary.add, .Ltmp7-binary.add
	.cfi_endproc

	.globl	binary.add.1
	.align	16, 0x90
	.type	binary.add.1,@function
binary.add.1:                           # @binary.add.1
	.cfi_startproc
# BB#0:                                 # %code
	pushq	%rbx
.Ltmp8:
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
.Ltmp9:
	.cfi_def_cfa_offset 48
.Ltmp10:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	%rsi, 24(%rsp)
	movb	%dl, 23(%rsp)
	movq	24(%rsp), %rax
	movl	4(%rax), %esi
	incl	%esi
	leaq	(%rsp), %rdi
	callq	setLength
	movq	8(%rsp), %rdi
	movq	24(%rsp), %rax
	movq	8(%rax), %rsi
	movl	4(%rax), %eax
	incl	%eax
	movslq	%eax, %rdx
	callq	memcpy
	movl	4(%rsp), %eax
	decl	%eax
	movb	23(%rsp), %cl
	movq	8(%rsp), %rdx
	cltq
	movb	%cl, (%rdx,%rax)
	movq	8(%rsp), %rax
	movslq	4(%rsp), %rcx
	movb	$0, (%rax,%rcx)
	movb	(%rsp), %al
	movl	4(%rsp), %ecx
	movq	8(%rsp), %rdx
	movq	%rdx, 8(%rbx)
	movl	%ecx, 4(%rbx)
	movb	%al, (%rbx)
	movq	%rbx, %rax
	addq	$32, %rsp
	popq	%rbx
	retq
.Ltmp11:
	.size	binary.add.1, .Ltmp11-binary.add.1
	.cfi_endproc

	.globl	binary.add.2
	.align	16, 0x90
	.type	binary.add.2,@function
binary.add.2:                           # @binary.add.2
	.cfi_startproc
# BB#0:                                 # %code
	pushq	%rbx
.Ltmp12:
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
.Ltmp13:
	.cfi_def_cfa_offset 48
.Ltmp14:
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movb	%sil, 31(%rsp)
	movq	%rdx, 16(%rsp)
	movsbl	31(%rsp), %esi
	incl	%esi
	leaq	(%rsp), %rdi
	callq	setLength
	movq	8(%rsp), %rdi
	incq	%rdi
	movq	16(%rsp), %rax
	movq	8(%rax), %rsi
	movl	4(%rax), %eax
	incl	%eax
	movslq	%eax, %rdx
	callq	memcpy
	movb	31(%rsp), %al
	movq	8(%rsp), %rcx
	movb	%al, (%rcx)
	movq	8(%rsp), %rax
	movslq	4(%rsp), %rcx
	movb	$0, (%rax,%rcx)
	movb	(%rsp), %al
	movl	4(%rsp), %ecx
	movq	8(%rsp), %rdx
	movq	%rdx, 8(%rbx)
	movl	%ecx, 4(%rbx)
	movb	%al, (%rbx)
	movq	%rbx, %rax
	addq	$32, %rsp
	popq	%rbx
	retq
.Ltmp15:
	.size	binary.add.2, .Ltmp15-binary.add.2
	.cfi_endproc

	.globl	binary.add.3
	.align	16, 0x90
	.type	binary.add.3,@function
binary.add.3:                           # @binary.add.3
	.cfi_startproc
# BB#0:                                 # %code
	pushq	%rbp
.Ltmp16:
	.cfi_def_cfa_offset 16
	pushq	%r14
.Ltmp17:
	.cfi_def_cfa_offset 24
	pushq	%rbx
.Ltmp18:
	.cfi_def_cfa_offset 32
	subq	$32, %rsp
.Ltmp19:
	.cfi_def_cfa_offset 64
.Ltmp20:
	.cfi_offset %rbx, -32
.Ltmp21:
	.cfi_offset %r14, -24
.Ltmp22:
	.cfi_offset %rbp, -16
	movq	%rdi, %r14
	movq	%rsi, 24(%rsp)
	movq	%rdx, 16(%rsp)
	movq	24(%rsp), %rax
	movl	4(%rax), %ebp
	movq	%rdx, %rdi
	callq	strLen
	addl	%eax, %ebp
	leaq	(%rsp), %rdi
	movl	%ebp, %esi
	callq	setLength
	movq	8(%rsp), %rdi
	movq	24(%rsp), %rax
	movq	8(%rax), %rsi
	movslq	4(%rax), %rdx
	callq	memcpy
	movq	24(%rsp), %rax
	movslq	4(%rax), %rbp
	addq	8(%rsp), %rbp
	movq	16(%rsp), %rbx
	movq	%rbx, %rdi
	callq	strLen
	movslq	%eax, %rdx
	movq	%rbp, %rdi
	movq	%rbx, %rsi
	callq	memcpy
	movq	8(%rsp), %rax
	movslq	4(%rsp), %rcx
	movb	$0, (%rax,%rcx)
	movb	(%rsp), %al
	movl	4(%rsp), %ecx
	movq	8(%rsp), %rdx
	movq	%rdx, 8(%r14)
	movl	%ecx, 4(%r14)
	movb	%al, (%r14)
	movq	%r14, %rax
	addq	$32, %rsp
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
.Ltmp23:
	.size	binary.add.3, .Ltmp23-binary.add.3
	.cfi_endproc

	.globl	binary.add.4
	.align	16, 0x90
	.type	binary.add.4,@function
binary.add.4:                           # @binary.add.4
	.cfi_startproc
# BB#0:                                 # %code
	pushq	%rbp
.Ltmp24:
	.cfi_def_cfa_offset 16
	pushq	%r14
.Ltmp25:
	.cfi_def_cfa_offset 24
	pushq	%rbx
.Ltmp26:
	.cfi_def_cfa_offset 32
	subq	$32, %rsp
.Ltmp27:
	.cfi_def_cfa_offset 64
.Ltmp28:
	.cfi_offset %rbx, -32
.Ltmp29:
	.cfi_offset %r14, -24
.Ltmp30:
	.cfi_offset %rbp, -16
	movq	%rdi, %rbx
	movq	%rsi, 24(%rsp)
	movq	%rdx, 16(%rsp)
	movl	4(%rdx), %ebp
	movq	24(%rsp), %rdi
	callq	strLen
	addl	%eax, %ebp
	leaq	(%rsp), %rdi
	movl	%ebp, %esi
	callq	setLength
	movq	8(%rsp), %r14
	movq	24(%rsp), %rbp
	movq	%rbp, %rdi
	callq	strLen
	movslq	%eax, %rdx
	movq	%r14, %rdi
	movq	%rbp, %rsi
	callq	memcpy
	movq	24(%rsp), %rdi
	callq	strLen
	movslq	%eax, %rdi
	addq	8(%rsp), %rdi
	movq	16(%rsp), %rax
	movq	8(%rax), %rsi
	movslq	4(%rax), %rdx
	callq	memcpy
	movq	8(%rsp), %rax
	movslq	4(%rsp), %rcx
	movb	$0, (%rax,%rcx)
	movb	(%rsp), %al
	movl	4(%rsp), %ecx
	movq	8(%rsp), %rdx
	movq	%rdx, 8(%rbx)
	movl	%ecx, 4(%rbx)
	movb	%al, (%rbx)
	movq	%rbx, %rax
	addq	$32, %rsp
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
.Ltmp31:
	.size	binary.add.4, .Ltmp31-binary.add.4
	.cfi_endproc

	.globl	assign.idx
	.align	16, 0x90
	.type	assign.idx,@function
assign.idx:                             # @assign.idx
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movb	%sil, -9(%rsp)
	movl	%edx, -16(%rsp)
	movb	-9(%rsp), %al
	movq	-8(%rsp), %rcx
	movq	8(%rcx), %rcx
	movslq	-16(%rsp), %rdx
	movb	%al, (%rcx,%rdx)
	retq
.Ltmp32:
	.size	assign.idx, .Ltmp32-assign.idx
	.cfi_endproc

	.globl	assign
	.align	16, 0x90
	.type	assign,@function
assign:                                 # @assign
	.cfi_startproc
# BB#0:                                 # %code
	subq	$24, %rsp
.Ltmp33:
	.cfi_def_cfa_offset 32
	movq	%rdi, 16(%rsp)
	movb	%sil, 15(%rsp)
	movq	16(%rsp), %rdi
	movl	$1, %esi
	callq	setLength
	movb	15(%rsp), %al
	movq	16(%rsp), %rcx
	movq	8(%rcx), %rcx
	movb	%al, (%rcx)
	movq	16(%rsp), %rax
	movq	8(%rax), %rax
	movb	$0, 1(%rax)
	addq	$24, %rsp
	retq
.Ltmp34:
	.size	assign, .Ltmp34-assign
	.cfi_endproc

	.globl	assign.1
	.align	16, 0x90
	.type	assign.1,@function
assign.1:                               # @assign.1
	.cfi_startproc
# BB#0:                                 # %code
	pushq	%r14
.Ltmp35:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp36:
	.cfi_def_cfa_offset 24
	subq	$24, %rsp
.Ltmp37:
	.cfi_def_cfa_offset 48
.Ltmp38:
	.cfi_offset %rbx, -24
.Ltmp39:
	.cfi_offset %r14, -16
	movq	%rdi, 16(%rsp)
	movq	%rsi, 8(%rsp)
	movq	16(%rsp), %rbx
	movq	%rsi, %rdi
	callq	strLen
	movq	%rbx, %rdi
	movl	%eax, %esi
	callq	setLength
	movq	16(%rsp), %rax
	movq	8(%rax), %rcx
	movslq	4(%rax), %rax
	movb	$0, (%rcx,%rax)
	movq	16(%rsp), %rax
	movq	8(%rax), %r14
	movq	8(%rsp), %rbx
	movq	%rbx, %rdi
	callq	strLen
	movslq	%eax, %rdx
	movq	%r14, %rdi
	movq	%rbx, %rsi
	callq	memcpy
	addq	$24, %rsp
	popq	%rbx
	popq	%r14
	retq
.Ltmp40:
	.size	assign.1, .Ltmp40-assign.1
	.cfi_endproc

	.globl	binary.eq
	.align	16, 0x90
	.type	binary.eq,@function
binary.eq:                              # @binary.eq
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movq	%rsi, -16(%rsp)
	movq	-8(%rsp), %rax
	movl	4(%rax), %eax
	cmpl	4(%rsi), %eax
	jne	.LBB11_6
# BB#1:                                 # %if.end.1
	movl	$0, -20(%rsp)
	jmp	.LBB11_2
	.align	16, 0x90
.LBB11_4:                               # %for.inc.1
                                        #   in Loop: Header=BB11_2 Depth=1
	incl	-20(%rsp)
.LBB11_2:                               # %for.1
                                        # =>This Inner Loop Header: Depth=1
	movl	-20(%rsp), %eax
	movq	-8(%rsp), %rcx
	cmpl	4(%rcx), %eax
	jg	.LBB11_5
# BB#3:                                 # %for.do.1
                                        #   in Loop: Header=BB11_2 Depth=1
	movq	-8(%rsp), %rax
	movq	8(%rax), %rax
	movslq	-20(%rsp), %rcx
	movq	-16(%rsp), %rdx
	movq	8(%rdx), %rdx
	movzbl	(%rdx,%rcx), %edx
	movzbl	(%rax,%rcx), %eax
	cmpl	%edx, %eax
	je	.LBB11_4
.LBB11_6:                               # %if.2
	xorl	%eax, %eax
	retq
.LBB11_5:                               # %for.end.1
	movb	$1, %al
	retq
.Ltmp41:
	.size	binary.eq, .Ltmp41-binary.eq
	.cfi_endproc

	.globl	binary.eq.1
	.align	16, 0x90
	.type	binary.eq.1,@function
binary.eq.1:                            # @binary.eq.1
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movb	%sil, -9(%rsp)
	movq	-8(%rsp), %rax
	cmpl	$1, 4(%rax)
	sete	%al
	retq
.Ltmp42:
	.size	binary.eq.1, .Ltmp42-binary.eq.1
	.cfi_endproc

	.globl	alignForward
	.align	16, 0x90
	.type	alignForward,@function
alignForward:                           # @alignForward
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movb	%sil, -9(%rsp)
	movsbl	-9(%rsp), %ecx
	decl	%ecx
	sete	%al
	movzbl	%al, %eax
	andq	%rcx, %rax
	addq	-8(%rsp), %rax
	retq
.Ltmp43:
	.size	alignForward, .Ltmp43-alignForward
	.cfi_endproc

	.globl	alignBackward
	.align	16, 0x90
	.type	alignBackward,@function
alignBackward:                          # @alignBackward
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movb	%sil, -9(%rsp)
	movsbl	-9(%rsp), %eax
	cmpl	$1, %eax
	sete	%al
	movzbl	%al, %eax
	andq	-8(%rsp), %rax
	retq
.Ltmp44:
	.size	alignBackward, .Ltmp44-alignBackward
	.cfi_endproc

	.globl	alignForwardAdjustment
	.align	16, 0x90
	.type	alignForwardAdjustment,@function
alignForwardAdjustment:                 # @alignForwardAdjustment
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movb	%sil, -9(%rsp)
	movzbl	-9(%rsp), %eax
	leal	-1(%rax), %ecx
	andl	-8(%rsp), %ecx
	subl	%ecx, %eax
	movb	%al, -10(%rsp)
	movzbl	%al, %eax
	movzbl	-9(%rsp), %ecx
	cmpl	%ecx, %eax
	jne	.LBB15_2
# BB#1:                                 # %if.3
	xorl	%eax, %eax
	retq
.LBB15_2:                               # %if.end.3
	movb	-10(%rsp), %al
	retq
.Ltmp45:
	.size	alignForwardAdjustment, .Ltmp45-alignForwardAdjustment
	.cfi_endproc

	.globl	alignForwardAdjustmentWithHeader
	.align	16, 0x90
	.type	alignForwardAdjustmentWithHeader,@function
alignForwardAdjustmentWithHeader:       # @alignForwardAdjustmentWithHeader
	.cfi_startproc
# BB#0:                                 # %code
	subq	$24, %rsp
.Ltmp46:
	.cfi_def_cfa_offset 32
	movq	%rdi, 16(%rsp)
	movb	%sil, 15(%rsp)
	movb	%dl, 14(%rsp)
	movq	16(%rsp), %rdi
	movzbl	15(%rsp), %esi
	callq	alignForwardAdjustment
	movb	%al, 13(%rsp)
	movzbl	14(%rsp), %eax
	movb	%al, 12(%rsp)
	movzbl	13(%rsp), %ecx
	cmpl	%eax, %ecx
	jae	.LBB16_3
# BB#1:                                 # %if.4
	movb	12(%rsp), %al
	subb	13(%rsp), %al
	movb	%al, 12(%rsp)
	movb	15(%rsp), %cl
	cbtw
	idivb	%cl
	movb	%al, %dl
	movb	%cl, %al
	mulb	%dl
	addb	%al, 13(%rsp)
	movb	12(%rsp), %al
	cbtw
	idivb	15(%rsp)
	shrw	$8, %ax
	movsbl	%al, %eax
	testl	%eax, %eax
	jle	.LBB16_3
# BB#2:                                 # %if.5
	movb	15(%rsp), %al
	addb	%al, 13(%rsp)
.LBB16_3:                               # %if.end.4
	movb	13(%rsp), %al
	addq	$24, %rsp
	retq
.Ltmp47:
	.size	alignForwardAdjustmentWithHeader, .Ltmp47-alignForwardAdjustmentWithHeader
	.cfi_endproc

	.globl	alignBackwardAdjustment
	.align	16, 0x90
	.type	alignBackwardAdjustment,@function
alignBackwardAdjustment:                # @alignBackwardAdjustment
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movb	%sil, -9(%rsp)
	movzbl	-9(%rsp), %eax
	decl	%eax
	andl	-8(%rsp), %eax
	movb	%al, -10(%rsp)
	movzbl	%al, %eax
	movzbl	-9(%rsp), %ecx
	cmpl	%ecx, %eax
	jne	.LBB17_2
# BB#1:                                 # %if.6
	xorl	%eax, %eax
	retq
.LBB17_2:                               # %if.end.6
	movb	-10(%rsp), %al
	retq
.Ltmp48:
	.size	alignBackwardAdjustment, .Ltmp48-alignBackwardAdjustment
	.cfi_endproc

	.globl	ptrAdd
	.align	16, 0x90
	.type	ptrAdd,@function
ptrAdd:                                 # @ptrAdd
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movq	%rsi, -16(%rsp)
	addq	-8(%rsp), %rsi
	movq	%rsi, %rax
	retq
.Ltmp49:
	.size	ptrAdd, .Ltmp49-ptrAdd
	.cfi_endproc

	.globl	ptrSub
	.align	16, 0x90
	.type	ptrSub,@function
ptrSub:                                 # @ptrSub
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movq	%rsi, -16(%rsp)
	movq	-8(%rsp), %rax
	subq	%rsi, %rax
	retq
.Ltmp50:
	.size	ptrSub, .Ltmp50-ptrSub
	.cfi_endproc

	.globl	createLinearAllocator
	.align	16, 0x90
	.type	createLinearAllocator,@function
createLinearAllocator:                  # @createLinearAllocator
	.cfi_startproc
# BB#0:                                 # %code
	subq	$24, %rsp
.Ltmp51:
	.cfi_def_cfa_offset 32
	movq	%rdi, 16(%rsp)
	movq	%rsi, 8(%rsp)
	movq	%rdx, (%rsp)
	movq	16(%rsp), %rdi
	movq	8(%rsp), %rsi
	callq	createAllocator
	movq	(%rsp), %rax
	movq	16(%rsp), %rcx
	movq	%rax, 8(%rcx)
	addq	$24, %rsp
	retq
.Ltmp52:
	.size	createLinearAllocator, .Ltmp52-createLinearAllocator
	.cfi_endproc

	.globl	deleteLinearAllocator
	.align	16, 0x90
	.type	deleteLinearAllocator,@function
deleteLinearAllocator:                  # @deleteLinearAllocator
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	retq
.Ltmp53:
	.size	deleteLinearAllocator, .Ltmp53-deleteLinearAllocator
	.cfi_endproc

	.globl	linearAllocate
	.align	16, 0x90
	.type	linearAllocate,@function
linearAllocate:                         # @linearAllocate
	.cfi_startproc
# BB#0:                                 # %code
	subq	$40, %rsp
.Ltmp54:
	.cfi_def_cfa_offset 48
	movq	%rdi, 32(%rsp)
	movq	%rsi, 24(%rsp)
	movb	%dl, 23(%rsp)
	movq	32(%rsp), %rax
	movq	8(%rax), %rdi
	movzbl	23(%rsp), %esi
	callq	alignForwardAdjustment
	movb	%al, 22(%rsp)
	movq	32(%rsp), %rax
	movsbq	22(%rsp), %rcx
	addq	32(%rax), %rcx
	addq	24(%rsp), %rcx
	cmpq	24(%rax), %rcx
	jbe	.LBB22_2
# BB#1:                                 # %if.7
	xorl	%eax, %eax
	addq	$40, %rsp
	retq
.LBB22_2:                               # %if.end.7
	movq	32(%rsp), %rax
	movsbq	22(%rsp), %rcx
	addq	8(%rax), %rcx
	movq	%rcx, 8(%rsp)
	addq	24(%rsp), %rcx
	movq	32(%rsp), %rax
	movq	%rcx, 8(%rax)
	movq	32(%rsp), %rax
	movsbq	22(%rsp), %rcx
	addq	24(%rsp), %rcx
	addq	%rcx, 32(%rax)
	movq	32(%rsp), %rax
	incq	40(%rax)
	movq	8(%rsp), %rax
	addq	$40, %rsp
	retq
.Ltmp55:
	.size	linearAllocate, .Ltmp55-linearAllocate
	.cfi_endproc

	.globl	clearLinearAllocator
	.align	16, 0x90
	.type	clearLinearAllocator,@function
clearLinearAllocator:                   # @clearLinearAllocator
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movq	$0, 40(%rdi)
	movq	-8(%rsp), %rax
	movq	$0, 32(%rax)
	movq	-8(%rsp), %rax
	movq	16(%rax), %rcx
	movq	%rcx, 8(%rax)
	retq
.Ltmp56:
	.size	clearLinearAllocator, .Ltmp56-clearLinearAllocator
	.cfi_endproc

	.globl	createStackAllocator
	.align	16, 0x90
	.type	createStackAllocator,@function
createStackAllocator:                   # @createStackAllocator
	.cfi_startproc
# BB#0:                                 # %code
	subq	$24, %rsp
.Ltmp57:
	.cfi_def_cfa_offset 32
	movq	%rdi, 16(%rsp)
	movq	%rsi, 8(%rsp)
	movq	%rdx, (%rsp)
	movq	16(%rsp), %rdi
	movq	8(%rsp), %rsi
	callq	createAllocator
	movq	16(%rsp), %rax
	movq	$0, 32(%rax)
	addq	$24, %rsp
	retq
.Ltmp58:
	.size	createStackAllocator, .Ltmp58-createStackAllocator
	.cfi_endproc

	.globl	deleteStackAllocator
	.align	16, 0x90
	.type	deleteStackAllocator,@function
deleteStackAllocator:                   # @deleteStackAllocator
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	retq
.Ltmp59:
	.size	deleteStackAllocator, .Ltmp59-deleteStackAllocator
	.cfi_endproc

	.globl	stackAllocate
	.align	16, 0x90
	.type	stackAllocate,@function
stackAllocate:                          # @stackAllocate
	.cfi_startproc
# BB#0:                                 # %code
	subq	$40, %rsp
.Ltmp60:
	.cfi_def_cfa_offset 48
	movq	%rdi, 32(%rsp)
	movq	%rsi, 24(%rsp)
	movb	%dl, 23(%rsp)
	movq	32(%rsp), %rax
	movq	40(%rax), %rdi
	movzbl	23(%rsp), %esi
	movl	$9, %edx
	callq	alignForwardAdjustmentWithHeader
	movb	%al, 22(%rsp)
	movq	32(%rsp), %rax
	movsbq	22(%rsp), %rcx
	addq	64(%rax), %rcx
	addq	24(%rsp), %rcx
	cmpq	56(%rax), %rcx
	jbe	.LBB26_2
# BB#1:                                 # %if.8
	xorl	%eax, %eax
	addq	$40, %rsp
	retq
.LBB26_2:                               # %if.end.8
	movq	32(%rsp), %rax
	movq	40(%rax), %rdi
	movsbq	22(%rsp), %rsi
	callq	ptrAdd
	movq	%rax, 8(%rsp)
	movl	$9, %esi
	movq	%rax, %rdi
	callq	ptrSub
	movq	%rax, (%rsp)
	movb	22(%rsp), %cl
	movb	%cl, 16(%rax)
	movq	32(%rsp), %rax
	movq	32(%rax), %rax
	movq	(%rsp), %rcx
	movq	%rax, 8(%rcx)
	movq	8(%rsp), %rax
	movq	32(%rsp), %rcx
	movq	%rax, 32(%rcx)
	movq	8(%rsp), %rdi
	movq	24(%rsp), %rsi
	callq	ptrAdd
	movq	32(%rsp), %rcx
	movq	%rax, 40(%rcx)
	movq	32(%rsp), %rax
	movsbq	22(%rsp), %rcx
	addq	24(%rsp), %rcx
	addq	%rcx, 64(%rax)
	movq	32(%rsp), %rax
	incq	72(%rax)
	movq	8(%rsp), %rax
	addq	$40, %rsp
	retq
.Ltmp61:
	.size	stackAllocate, .Ltmp61-stackAllocate
	.cfi_endproc

	.globl	stackDeallocate
	.align	16, 0x90
	.type	stackDeallocate,@function
stackDeallocate:                        # @stackDeallocate
	.cfi_startproc
# BB#0:                                 # %code
	subq	$24, %rsp
.Ltmp62:
	.cfi_def_cfa_offset 32
	movq	%rsi, %rax
	movq	%rdi, 16(%rsp)
	movq	%rax, 8(%rsp)
	movl	$9, %esi
	movq	%rax, %rdi
	callq	ptrSub
	movq	%rax, (%rsp)
	movq	16(%rsp), %rcx
	movq	40(%rcx), %rdx
	subq	8(%rsp), %rdx
	movsbq	16(%rax), %rax
	addq	%rdx, %rax
	subq	%rax, 64(%rcx)
	movq	8(%rsp), %rdi
	movq	(%rsp), %rax
	movsbq	16(%rax), %rsi
	callq	ptrSub
	movq	16(%rsp), %rcx
	movq	%rax, 40(%rcx)
	movq	(%rsp), %rax
	movq	8(%rax), %rax
	movq	16(%rsp), %rcx
	movq	%rax, 32(%rcx)
	movq	16(%rsp), %rax
	decq	72(%rax)
	addq	$24, %rsp
	retq
.Ltmp63:
	.size	stackDeallocate, .Ltmp63-stackDeallocate
	.cfi_endproc

	.globl	createFreeListAllocator
	.align	16, 0x90
	.type	createFreeListAllocator,@function
createFreeListAllocator:                # @createFreeListAllocator
	.cfi_startproc
# BB#0:                                 # %code
	subq	$24, %rsp
.Ltmp64:
	.cfi_def_cfa_offset 32
	movq	%rdi, 16(%rsp)
	movq	%rsi, 8(%rsp)
	movq	%rdx, (%rsp)
	movq	16(%rsp), %rdi
	movq	8(%rsp), %rsi
	callq	createAllocator
	movq	8(%rsp), %rax
	movq	16(%rsp), %rcx
	movq	32(%rcx), %rcx
	movq	%rax, 8(%rcx)
	movq	16(%rsp), %rax
	movq	32(%rax), %rax
	movq	$0, 16(%rax)
	addq	$24, %rsp
	retq
.Ltmp65:
	.size	createFreeListAllocator, .Ltmp65-createFreeListAllocator
	.cfi_endproc

	.globl	deleteFreeListAllocator
	.align	16, 0x90
	.type	deleteFreeListAllocator,@function
deleteFreeListAllocator:                # @deleteFreeListAllocator
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movq	$0, 32(%rdi)
	retq
.Ltmp66:
	.size	deleteFreeListAllocator, .Ltmp66-deleteFreeListAllocator
	.cfi_endproc

	.globl	getSizeFreeList
	.align	16, 0x90
	.type	getSizeFreeList,@function
getSizeFreeList:                        # @getSizeFreeList
	.cfi_startproc
# BB#0:                                 # %code
	pushq	%rbx
.Ltmp67:
	.cfi_def_cfa_offset 16
	subq	$48, %rsp
.Ltmp68:
	.cfi_def_cfa_offset 64
.Ltmp69:
	.cfi_offset %rbx, -16
	movq	%rdi, 40(%rsp)
	movq	%rsi, 32(%rsp)
	movb	%dl, 31(%rsp)
	movq	40(%rsp), %rax
	movq	32(%rax), %rax
	movq	%rax, 16(%rsp)
	leaq	16(%rsp), %rbx
	jmp	.LBB30_1
	.align	16, 0x90
.LBB30_3:                               # %if.9
                                        #   in Loop: Header=BB30_1 Depth=1
	movq	16(%rsp), %rax
	movq	16(%rax), %rax
	movq	%rax, 16(%rsp)
.LBB30_1:                               # %while.1
                                        # =>This Inner Loop Header: Depth=1
	cmpq	$0, 16(%rsp)
	je	.LBB30_7
# BB#2:                                 # %while.do.1
                                        #   in Loop: Header=BB30_1 Depth=1
	movzbl	31(%rsp), %esi
	movl	$13, %edx
	movq	%rbx, %rdi
	callq	alignForwardAdjustmentWithHeader
	movb	%al, 15(%rsp)
	movsbq	15(%rsp), %rax
	addq	32(%rsp), %rax
	movq	%rax, (%rsp)
	movq	16(%rsp), %rcx
	cmpq	%rax, 8(%rcx)
	jb	.LBB30_3
# BB#4:                                 # %if.end.9
	movq	16(%rsp), %rax
	movq	8(%rax), %rax
	subq	(%rsp), %rax
	cmpq	$13, %rax
	ja	.LBB30_6
# BB#5:                                 # %if.10
	movq	16(%rsp), %rax
	movq	8(%rax), %rax
	movq	%rax, (%rsp)
.LBB30_6:                               # %if.end.10
	movb	(%rsp), %al
.LBB30_7:                               # %while.end.1
	addq	$48, %rsp
	popq	%rbx
	retq
.Ltmp70:
	.size	getSizeFreeList, .Ltmp70-getSizeFreeList
	.cfi_endproc

	.globl	freeListAllocate
	.align	16, 0x90
	.type	freeListAllocate,@function
freeListAllocate:                       # @freeListAllocate
	.cfi_startproc
# BB#0:                                 # %code
	pushq	%rbx
.Ltmp71:
	.cfi_def_cfa_offset 16
	subq	$80, %rsp
.Ltmp72:
	.cfi_def_cfa_offset 96
.Ltmp73:
	.cfi_offset %rbx, -16
	movq	%rdi, 72(%rsp)
	movq	%rsi, 64(%rsp)
	movb	%dl, 63(%rsp)
	movl	%ecx, 56(%rsp)
	movq	$0, 48(%rsp)
	movq	72(%rsp), %rax
	movq	32(%rax), %rax
	movq	%rax, 40(%rsp)
	leaq	40(%rsp), %rbx
	jmp	.LBB31_1
	.align	16, 0x90
.LBB31_3:                               # %if.11
                                        #   in Loop: Header=BB31_1 Depth=1
	movq	40(%rsp), %rax
	movq	%rax, 48(%rsp)
	movq	40(%rsp), %rax
	movq	16(%rax), %rax
	movq	%rax, 40(%rsp)
.LBB31_1:                               # %while.2
                                        # =>This Inner Loop Header: Depth=1
	cmpq	$0, 40(%rsp)
	je	.LBB31_14
# BB#2:                                 # %while.do.2
                                        #   in Loop: Header=BB31_1 Depth=1
	movzbl	63(%rsp), %esi
	movl	$13, %edx
	movq	%rbx, %rdi
	callq	alignForwardAdjustmentWithHeader
	movb	%al, 39(%rsp)
	movsbq	39(%rsp), %rax
	addq	64(%rsp), %rax
	movq	%rax, 24(%rsp)
	movq	40(%rsp), %rcx
	cmpq	%rax, 8(%rcx)
	jb	.LBB31_3
# BB#4:                                 # %if.end.11
	movq	40(%rsp), %rax
	movq	8(%rax), %rax
	subq	24(%rsp), %rax
	cmpq	$13, %rax
	ja	.LBB31_8
# BB#5:                                 # %if.12
	movq	40(%rsp), %rax
	movq	8(%rax), %rax
	movq	%rax, 24(%rsp)
	cmpq	$0, 48(%rsp)
	je	.LBB31_7
# BB#6:                                 # %if.13
	movq	40(%rsp), %rax
	movq	16(%rax), %rax
	jmp	.LBB31_10
.LBB31_8:                               # %if.else.1
	movq	24(%rsp), %rsi
	leaq	40(%rsp), %rdi
	callq	ptrAdd
	movq	%rax, 16(%rsp)
	movq	40(%rsp), %rcx
	movq	8(%rcx), %rcx
	subq	24(%rsp), %rcx
	movq	%rcx, 8(%rax)
	movq	40(%rsp), %rax
	movq	16(%rax), %rax
	movq	16(%rsp), %rcx
	movq	%rax, 16(%rcx)
	cmpq	$0, 48(%rsp)
	je	.LBB31_11
# BB#9:                                 # %if.14
	movq	16(%rsp), %rax
.LBB31_10:                              # %if.end.12
	movq	48(%rsp), %rcx
	movq	%rax, 16(%rcx)
	jmp	.LBB31_13
.LBB31_7:                               # %if.else.2
	movq	40(%rsp), %rax
	movq	16(%rax), %rax
	jmp	.LBB31_12
.LBB31_11:                              # %if.else.3
	movq	16(%rsp), %rax
.LBB31_12:                              # %if.end.12
	movq	72(%rsp), %rcx
	movq	%rax, 32(%rcx)
.LBB31_13:                              # %if.end.12
	movq	40(%rsp), %rax
	movsbq	39(%rsp), %rcx
	leaq	(%rax,%rcx), %rdx
	movq	%rdx, 8(%rsp)
	leaq	-13(%rax,%rcx), %rdx
	movq	%rdx, (%rsp)
	movq	24(%rsp), %rdx
	movq	%rdx, -5(%rax,%rcx)
	movb	24(%rsp), %al
	movq	(%rsp), %rcx
	movb	%al, 16(%rcx)
	movl	56(%rsp), %eax
	movq	(%rsp), %rcx
	movl	%eax, 20(%rcx)
	movq	72(%rsp), %rax
	movq	24(%rsp), %rcx
	addq	%rcx, 56(%rax)
	movq	72(%rsp), %rax
	incq	64(%rax)
	movq	8(%rsp), %rax
.LBB31_14:                              # %while.end.2
	addq	$80, %rsp
	popq	%rbx
	retq
.Ltmp74:
	.size	freeListAllocate, .Ltmp74-freeListAllocate
	.cfi_endproc

	.globl	freeListDeallocate
	.align	16, 0x90
	.type	freeListDeallocate,@function
freeListDeallocate:                     # @freeListDeallocate
	.cfi_startproc
# BB#0:                                 # %code
	subq	$72, %rsp
.Ltmp75:
	.cfi_def_cfa_offset 80
	movq	%rsi, %rax
	movq	%rdi, 64(%rsp)
	movq	%rax, 56(%rsp)
	movl	$13, %esi
	movq	%rax, %rdi
	callq	ptrSub
	movq	%rax, 48(%rsp)
	movq	56(%rsp), %rcx
	movsbq	16(%rax), %rax
	subq	%rax, %rcx
	movq	%rcx, 40(%rsp)
	movq	48(%rsp), %rax
	movq	8(%rax), %rax
	movq	%rax, 32(%rsp)
	addq	40(%rsp), %rax
	movq	%rax, 24(%rsp)
	movq	$0, 16(%rsp)
	movq	64(%rsp), %rax
	movq	32(%rax), %rax
	jmp	.LBB32_1
	.align	16, 0x90
.LBB32_3:                               # %if.end.15
                                        #   in Loop: Header=BB32_1 Depth=1
	movq	8(%rsp), %rax
	movq	%rax, 16(%rsp)
	movq	8(%rsp), %rax
	movq	16(%rax), %rax
.LBB32_1:                               # %while.3
                                        # =>This Inner Loop Header: Depth=1
	movq	%rax, 8(%rsp)
	cmpq	$0, 8(%rsp)
	je	.LBB32_4
# BB#2:                                 # %while.do.3
                                        #   in Loop: Header=BB32_1 Depth=1
	movq	8(%rsp), %rax
	cmpq	24(%rsp), %rax
	jb	.LBB32_3
.LBB32_4:                               # %while.end.3
	cmpq	$0, 16(%rsp)
	je	.LBB32_5
# BB#6:                                 # %if.else.4
	movq	16(%rsp), %rax
	addq	8(%rax), %rax
	cmpq	40(%rsp), %rax
	jne	.LBB32_8
# BB#7:                                 # %if.17
	movq	16(%rsp), %rax
	movq	32(%rsp), %rcx
	addq	%rcx, 8(%rax)
.LBB32_8:                               # %if.end.17
	movq	40(%rsp), %rax
	movq	%rax, (%rsp)
	movq	32(%rsp), %rcx
	movq	%rcx, 8(%rax)
	movq	16(%rsp), %rax
	movq	16(%rax), %rax
	movq	(%rsp), %rcx
	movq	%rax, 16(%rcx)
	movq	(%rsp), %rax
	movq	16(%rsp), %rcx
	movq	%rax, 16(%rcx)
	movq	(%rsp), %rax
	movq	%rax, 16(%rsp)
	jmp	.LBB32_9
.LBB32_5:                               # %if.16
	movq	40(%rsp), %rax
	movq	%rax, 16(%rsp)
	movq	32(%rsp), %rcx
	movq	%rcx, 8(%rax)
	movq	64(%rsp), %rax
	movq	32(%rax), %rax
	movq	16(%rsp), %rcx
	movq	%rax, 16(%rcx)
	movq	16(%rsp), %rax
	movq	64(%rsp), %rcx
	movq	%rax, 32(%rcx)
.LBB32_9:                               # %if.end.16
	cmpq	$0, 8(%rsp)
	je	.LBB32_11
# BB#10:                                # %if.18
	movq	16(%rsp), %rax
	movq	8(%rsp), %rcx
	movq	8(%rcx), %rcx
	addq	%rcx, 8(%rax)
	movq	8(%rsp), %rax
	movq	16(%rax), %rax
	movq	16(%rsp), %rcx
	movq	%rax, 16(%rcx)
.LBB32_11:                              # %if.end.18
	movq	64(%rsp), %rax
	decq	64(%rax)
	movq	64(%rsp), %rax
	movq	32(%rsp), %rcx
	subq	%rcx, 56(%rax)
	addq	$72, %rsp
	retq
.Ltmp76:
	.size	freeListDeallocate, .Ltmp76-freeListDeallocate
	.cfi_endproc

	.globl	createPoolAllocator
	.align	16, 0x90
	.type	createPoolAllocator,@function
createPoolAllocator:                    # @createPoolAllocator
	.cfi_startproc
# BB#0:                                 # %code
	pushq	%rbx
.Ltmp77:
	.cfi_def_cfa_offset 16
	subq	$80, %rsp
.Ltmp78:
	.cfi_def_cfa_offset 96
.Ltmp79:
	.cfi_offset %rbx, -16
	movq	%rdi, 72(%rsp)
	movq	%rsi, 64(%rsp)
	movq	%rdx, 56(%rsp)
	movb	%cl, 55(%rsp)
	movq	%r8, 40(%rsp)
	movq	72(%rsp), %rdi
	movq	64(%rsp), %rsi
	movq	%r8, %rdx
	callq	createAllocator
	movq	40(%rsp), %rdi
	movzbl	55(%rsp), %esi
	callq	alignForwardAdjustment
	movb	%al, 39(%rsp)
	movq	40(%rsp), %rdi
	movsbq	39(%rsp), %rsi
	callq	ptrAdd
	movq	72(%rsp), %rcx
	movq	%rax, 24(%rcx)
	movq	64(%rsp), %rax
	movsbq	39(%rsp), %rcx
	subq	%rcx, %rax
	cqto
	idivq	56(%rsp)
	movq	%rax, 24(%rsp)
	movq	72(%rsp), %rax
	movq	24(%rax), %rax
	movq	%rax, 16(%rsp)
	movq	$0, 8(%rsp)
	leaq	16(%rsp), %rbx
	jmp	.LBB33_1
	.align	16, 0x90
.LBB33_2:                               # %for.do.2
                                        #   in Loop: Header=BB33_1 Depth=1
	movq	56(%rsp), %rsi
	movq	%rbx, %rdi
	callq	ptrAdd
	movq	16(%rsp), %rcx
	movq	%rax, (%rcx)
	movq	16(%rsp), %rax
	movq	(%rax), %rax
	movq	%rax, 16(%rsp)
	incq	8(%rsp)
.LBB33_1:                               # %for.2
                                        # =>This Inner Loop Header: Depth=1
	movq	24(%rsp), %rax
	decq	%rax
	cmpq	%rax, 8(%rsp)
	jle	.LBB33_2
# BB#3:                                 # %for.end.2
	movq	16(%rsp), %rax
	movq	$0, (%rax)
	addq	$80, %rsp
	popq	%rbx
	retq
.Ltmp80:
	.size	createPoolAllocator, .Ltmp80-createPoolAllocator
	.cfi_endproc

	.globl	deletePoolAllocator
	.align	16, 0x90
	.type	deletePoolAllocator,@function
deletePoolAllocator:                    # @deletePoolAllocator
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movq	$0, 24(%rdi)
	retq
.Ltmp81:
	.size	deletePoolAllocator, .Ltmp81-deletePoolAllocator
	.cfi_endproc

	.globl	poolAllocate
	.align	16, 0x90
	.type	poolAllocate,@function
poolAllocate:                           # @poolAllocate
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movq	%rsi, -16(%rsp)
	movb	%dl, -17(%rsp)
	movq	-8(%rsp), %rax
	cmpq	$0, 24(%rax)
	je	.LBB35_1
# BB#2:                                 # %if.end.19
	movq	-8(%rsp), %rax
	addq	$24, %rax
	movq	%rax, -32(%rsp)
	movq	-8(%rsp), %rax
	movq	24(%rax), %rcx
	movq	(%rcx), %rcx
	movq	%rcx, 24(%rax)
	movq	-8(%rsp), %rax
	movq	-16(%rsp), %rcx
	addq	%rcx, 48(%rax)
	movq	-8(%rsp), %rax
	incq	56(%rax)
	movq	-32(%rsp), %rax
	retq
.LBB35_1:                               # %if.19
	xorl	%eax, %eax
	retq
.Ltmp82:
	.size	poolAllocate, .Ltmp82-poolAllocate
	.cfi_endproc

	.globl	poolDeallocate
	.align	16, 0x90
	.type	poolDeallocate,@function
poolDeallocate:                         # @poolDeallocate
	.cfi_startproc
# BB#0:                                 # %code
	movq	%rdi, -8(%rsp)
	movq	%rsi, -16(%rsp)
	movq	-8(%rsp), %rax
	addq	$24, %rax
	movq	%rax, -24(%rsp)
	movq	-16(%rsp), %rcx
	movq	%rax, (%rcx)
	movq	-16(%rsp), %rax
	movq	-8(%rsp), %rcx
	movq	%rax, 24(%rcx)
	movq	-8(%rsp), %rax
	movq	8(%rax), %rcx
	subq	%rcx, 48(%rax)
	movq	-8(%rsp), %rax
	decq	56(%rax)
	retq
.Ltmp83:
	.size	poolDeallocate, .Ltmp83-poolDeallocate
	.cfi_endproc

	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:                                 # %code
	movl	$0, -4(%rsp)
	retq
.Ltmp84:
	.size	main, .Ltmp84-main
	.cfi_endproc

	.type	.LPROT_NONE,@object     # @PROT_NONE
	.section	.rodata.cst4,"aM",@progbits,4
	.align	4
.LPROT_NONE:
	.long	0                       # 0x0
	.size	.LPROT_NONE, 4

	.type	.LPROT_READ,@object     # @PROT_READ
	.align	4
.LPROT_READ:
	.long	1                       # 0x1
	.size	.LPROT_READ, 4

	.type	.LPROT_WRITE,@object    # @PROT_WRITE
	.align	4
.LPROT_WRITE:
	.long	2                       # 0x2
	.size	.LPROT_WRITE, 4

	.type	.LPROT_EXEC,@object     # @PROT_EXEC
	.align	4
.LPROT_EXEC:
	.long	4                       # 0x4
	.size	.LPROT_EXEC, 4

	.type	.LMAP_FILE,@object      # @MAP_FILE
	.align	4
.LMAP_FILE:
	.long	0                       # 0x0
	.size	.LMAP_FILE, 4

	.type	.LMAP_SHARED,@object    # @MAP_SHARED
	.align	4
.LMAP_SHARED:
	.long	1                       # 0x1
	.size	.LMAP_SHARED, 4

	.type	.LMAP_PRIVATE,@object   # @MAP_PRIVATE
	.align	4
.LMAP_PRIVATE:
	.long	2                       # 0x2
	.size	.LMAP_PRIVATE, 4

	.type	.LMAP_TYPE,@object      # @MAP_TYPE
	.align	4
.LMAP_TYPE:
	.long	0                       # 0x0
	.size	.LMAP_TYPE, 4

	.type	.LMAP_FIXED,@object     # @MAP_FIXED
	.align	4
.LMAP_FIXED:
	.long	0                       # 0x0
	.size	.LMAP_FIXED, 4

	.type	.LMAP_ANONYMOUS,@object # @MAP_ANONYMOUS
	.align	4
.LMAP_ANONYMOUS:
	.long	0                       # 0x0
	.size	.LMAP_ANONYMOUS, 4

	.type	.LMAP_ANON,@object      # @MAP_ANON
	.align	4
.LMAP_ANON:
	.long	0                       # 0x0
	.size	.LMAP_ANON, 4

	.type	.LMAP_FAILED,@object    # @MAP_FAILED
	.align	4
.LMAP_FAILED:
	.long	1                       # 0x1
	.size	.LMAP_FAILED, 4

	.type	.LMS_ASYNC,@object      # @MS_ASYNC
	.align	4
.LMS_ASYNC:
	.long	1                       # 0x1
	.size	.LMS_ASYNC, 4

	.type	.LMS_SYNC,@object       # @MS_SYNC
	.align	4
.LMS_SYNC:
	.long	2                       # 0x2
	.size	.LMS_SYNC, 4

	.type	.LMS_INVALIDATE,@object # @MS_INVALIDATE
	.align	4
.LMS_INVALIDATE:
	.long	4                       # 0x4
	.size	.LMS_INVALIDATE, 4


	.section	".note.GNU-stack","",@progbits
