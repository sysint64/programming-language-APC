	.def	 @feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
@feat.00 = 1
	.def	 _main;
	.scl	2;
	.type	32;
	.endef
	.text
	.globl	_main
	.align	16, 0x90
_main:                                  # @main
# BB#0:                                 # %code
	pushl	%ebp
	movl	%esp, %ebp
	calll	___main
	calll	__errno
	movl	$0, (%eax)
	calll	__errno
	movl	$22, (%eax)
	popl	%ebp
	ret

	.def	 _mmap;
	.scl	2;
	.type	32;
	.endef
	.globl	_mmap
	.align	16, 0x90
_mmap:                                  # @mmap
# BB#0:                                 # %entry
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	pushl	%edi
	pushl	%esi
	subl	$24, %esp
	movl	16(%ebp), %edx
	xorl	%esi, %esi
	testl	%edx, %edx
	movl	$0, %ebx
	je	LBB1_5
# BB#1:                                 # %if.end.i
	testb	$4, %dl
	je	LBB1_2
# BB#3:                                 # %__map_mmap_prot_page.exit
	movl	%edx, %esi
	shll	$4, %esi
	andl	$32, %esi
	addl	$32, %esi
	movl	%edx, %eax
	andl	$2, %eax
	jmp	LBB1_4
LBB1_2:                                 # %__map_mmap_prot_page.exit.thread8
	movl	%edx, %eax
	andl	$2, %eax
	leal	2(%eax), %esi
LBB1_4:                                 # %if.end.i4
	leal	(,%edx,4), %ecx
	andl	$4, %ecx
	orl	%eax, %ecx
	leal	(,%edx,8), %ebx
	andl	$32, %ebx
	orl	%ecx, %ebx
LBB1_5:                                 # %__map_mmap_prot_file.exit
	movl	%edx, %edi
	calll	__errno
	movl	$0, (%eax)
	cmpl	$0, 12(%ebp)
	je	LBB1_8
# BB#6:                                 # %lor.lhs.false
	movl	20(%ebp), %eax
	testb	$16, %al
	jne	LBB1_8
# BB#7:                                 # %lor.lhs.false
	cmpl	$4, %edi
	jne	LBB1_9
LBB1_8:                                 # %if.then
	calll	__errno
	movl	$22, (%eax)
LBB1_15:                                # %return
	movl	$-1, %esi
LBB1_16:                                # %return
	movl	%esi, %eax
	addl	$24, %esp
	popl	%esi
	popl	%edi
	popl	%ebx
	popl	%ebp
	ret
LBB1_9:                                 # %if.end
	testb	$32, %al
	movl	$-1, %eax
	jne	LBB1_12
# BB#10:                                # %cond.end
	movl	24(%ebp), %eax
	movl	%eax, (%esp)
	calll	__get_osfhandle
	cmpl	$-1, %eax
	je	LBB1_11
LBB1_12:                                # %if.end15
	movl	12(%ebp), %edi
	movl	%edi, 16(%esp)
	movl	%esi, 8(%esp)
	movl	%eax, (%esp)
	movl	$0, 20(%esp)
	movl	$0, 12(%esp)
	movl	$0, 4(%esp)
	calll	_CreateFileMappingA@24
	subl	$24, %esp
	testl	%eax, %eax
	je	LBB1_14
# BB#13:                                # %if.end22
	movl	28(%ebp), %ecx
	movl	%edi, 16(%esp)
	movl	%ecx, 12(%esp)
	movl	%ebx, 4(%esp)
	movl	%eax, (%esp)
	movl	$0, 8(%esp)
	movl	%eax, %edi
	calll	_MapViewOfFile@20
	subl	$20, %esp
	movl	%eax, %esi
	movl	%edi, (%esp)
	calll	_CloseHandle@4
	subl	$4, %esp
	testl	%esi, %esi
	jne	LBB1_16
LBB1_14:                                # %if.then26
	calll	_GetLastError@0
	movl	%eax, %esi
	calll	__errno
	movl	%esi, (%eax)
	jmp	LBB1_15
LBB1_11:                                # %if.then13
	calll	__errno
	movl	$9, (%eax)
	jmp	LBB1_15

	.def	 _munmap;
	.scl	2;
	.type	32;
	.endef
	.globl	_munmap
	.align	16, 0x90
_munmap:                                # @munmap
# BB#0:                                 # %entry
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%eax
	movl	8(%ebp), %eax
	movl	%eax, (%esp)
	calll	_UnmapViewOfFile@4
	subl	$4, %esp
	movl	%eax, %ecx
	xorl	%eax, %eax
	testl	%ecx, %ecx
	jne	LBB2_2
# BB#1:                                 # %if.end
	calll	_GetLastError@0
	movl	%eax, %esi
	calll	__errno
	movl	%esi, (%eax)
	movl	$-1, %eax
LBB2_2:                                 # %return
	addl	$4, %esp
	popl	%esi
	popl	%ebp
	ret

	.def	 _mprotect;
	.scl	2;
	.type	32;
	.endef
	.globl	_mprotect
	.align	16, 0x90
_mprotect:                              # @mprotect
# BB#0:                                 # %entry
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	subl	$20, %esp
	movl	16(%ebp), %eax
	xorl	%esi, %esi
	testl	%eax, %eax
	movl	$0, %ecx
	je	LBB3_5
# BB#1:                                 # %if.end.i
	testb	$4, %al
	je	LBB3_3
# BB#2:                                 # %if.then2.i
	shll	$4, %eax
	andl	$32, %eax
	addl	$32, %eax
	jmp	LBB3_4
LBB3_3:                                 # %if.else.i
	andl	$2, %eax
	addl	$2, %eax
LBB3_4:                                 # %__map_mmap_prot_page.exit
	movl	%eax, %ecx
LBB3_5:                                 # %__map_mmap_prot_page.exit
	movl	12(%ebp), %eax
	movl	8(%ebp), %edx
	movl	$0, -12(%ebp)
	leal	-12(%ebp), %edi
	movl	%edi, 12(%esp)
	movl	%ecx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	%edx, (%esp)
	calll	_VirtualProtect@16
	subl	$16, %esp
	testl	%eax, %eax
	jne	LBB3_7
# BB#6:                                 # %if.end
	calll	_GetLastError@0
	movl	%eax, %esi
	calll	__errno
	movl	%esi, (%eax)
	movl	$-1, %esi
LBB3_7:                                 # %return
	movl	%esi, %eax
	addl	$20, %esp
	popl	%esi
	popl	%edi
	popl	%ebp
	ret

	.def	 _msync;
	.scl	2;
	.type	32;
	.endef
	.globl	_msync
	.align	16, 0x90
_msync:                                 # @msync
# BB#0:                                 # %entry
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	subl	$8, %esp
	movl	8(%ebp), %eax
	movl	12(%ebp), %ecx
	movl	%ecx, 4(%esp)
	movl	%eax, (%esp)
	calll	_FlushViewOfFile@8
	subl	$8, %esp
	movl	%eax, %ecx
	xorl	%eax, %eax
	testl	%ecx, %ecx
	jne	LBB4_2
# BB#1:                                 # %if.end
	calll	_GetLastError@0
	movl	%eax, %esi
	calll	__errno
	movl	%esi, (%eax)
	movl	$-1, %eax
LBB4_2:                                 # %return
	addl	$8, %esp
	popl	%esi
	popl	%ebp
	ret

	.def	 _mlock;
	.scl	2;
	.type	32;
	.endef
	.globl	_mlock
	.align	16, 0x90
_mlock:                                 # @mlock
# BB#0:                                 # %entry
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	subl	$8, %esp
	movl	8(%ebp), %eax
	movl	12(%ebp), %ecx
	movl	%ecx, 4(%esp)
	movl	%eax, (%esp)
	calll	_VirtualLock@8
	subl	$8, %esp
	movl	%eax, %ecx
	xorl	%eax, %eax
	testl	%ecx, %ecx
	jne	LBB5_2
# BB#1:                                 # %if.end
	calll	_GetLastError@0
	movl	%eax, %esi
	calll	__errno
	movl	%esi, (%eax)
	movl	$-1, %eax
LBB5_2:                                 # %return
	addl	$8, %esp
	popl	%esi
	popl	%ebp
	ret

	.def	 _munlock;
	.scl	2;
	.type	32;
	.endef
	.globl	_munlock
	.align	16, 0x90
_munlock:                               # @munlock
# BB#0:                                 # %entry
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	subl	$8, %esp
	movl	8(%ebp), %eax
	movl	12(%ebp), %ecx
	movl	%ecx, 4(%esp)
	movl	%eax, (%esp)
	calll	_VirtualUnlock@8
	subl	$8, %esp
	movl	%eax, %ecx
	xorl	%eax, %eax
	testl	%ecx, %ecx
	jne	LBB6_2
# BB#1:                                 # %if.end
	calll	_GetLastError@0
	movl	%eax, %esi
	calll	__errno
	movl	%esi, (%eax)
	movl	$-1, %eax
LBB6_2:                                 # %return
	addl	$8, %esp
	popl	%esi
	popl	%ebp
	ret


