section .data
	freakMessage db "Chal Gaya"
	digit 		 db  0,10 

section .text
	global _spinlock,_spin_unlock,_lock,_unlock;,_test

_test: 							;tester function to be removed later
	mov r8,[rdi] 				;moving content of passed argument to r8
	mov rax,r8 					;returning the value received
ret

_spinlock:	

;	mov r8,rdi
;	mov rax,qword[rdi]
;	add rax,48
;	mov [digit],al
;	mov rax,1
;	mov rdi,1
;	mov rsi,digit
;	mov rdx,2
;	syscall
;	mov rdi,r8	

	mov rcx,0

	spin:
		mov rax,1	
		lock cmpxchg qword [rdi],rcx	
	jnz spin
ret

_spin_unlock:
	mov qword [rdi],1
	mov rax, rdi
ret

_lock:
 mov rcx , 0
 mov rax,1
 lock cmpxchg qword [rdi],rcx
 jnz ret_zero
 mov rax,1 ;if lock acquired return 1
 ret
 ret_zero:
 	mov rax,0 ; if lock already taken return 0
 	ret
ret

_unlock:
	mov qword [rdi],1
	mov rax,rdi
ret



_printf1:
	mov rax,1
	mov rdi,1
	mov rsi,freakMessage
	mov rdx,10
	syscall	
ret


