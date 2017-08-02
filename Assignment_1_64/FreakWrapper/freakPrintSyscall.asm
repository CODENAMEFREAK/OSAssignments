STDOUT equ 1 					;STDOUT constant defined. It is the value of the standard output file descriptor.
SYS_WRITE equ 1					;SYS_WRITE constant defined. It is the ID of the sys_write system call.

extern displayReturnValue		;displayReturnValue is declared extern so that from assembly can invoke it.


section .text
global freakPrint 				;freakPrint is marked global so that from C code can call it.

;System call takes input from:
;	ID	rax
;	1	rdi
;	2	rsi
;	3	rdx
;	4	r10
;	5	r8
;	6	r9


;	rax	Return Value
;	rdi	Arg1
;	rsi	Arg2

freakPrint:						;freakPrint is invoked as (length , messagePtr).
	mov r8,rdi					;1st argument i.e. length is moved to r8.
	;mov r9,rsi					;2nd argument i.e. message pointer is moved to r9.
	mov rax,SYS_WRITE			;SYS_CALL ID of SYS_WRITE copied to rax.
	mov rdi,STDOUT				;filedescriptor is copied to rdi.
	;mov rsi,r9					;message pointer is copied to rsi.
	mov rdx,r8					;length is copied to rdx.
	syscall						;make the system call.

	;at this point return value will be present in rax register.
	;Now Invoke C function

	mov rdi,rax 				;move rax to first argument to the displayReturnValue.
	call displayReturnValue		;Call the C function to display the return value.
ret								;return.
