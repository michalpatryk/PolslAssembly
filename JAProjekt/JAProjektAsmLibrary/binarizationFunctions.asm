;File containing asmBinarization algorithms.
;Naming convention is asmBinarization{version number}
.code 
;asmBinarization1 proc beginptr: QWORD, endptr: QWORD, biWidth: DWORD, treshold DWORD

asmBinarization1 proc x: QWORD, y: QWORD
	mov eax, 1
	ret
asmBinarization1 endp
	;char* begin, char* end, long biWidth, float treshold



;	xor eax, eax
;	mov eax, x
;	mov ecx, y
;	ror ecx, 1
;	shld eax,ecx,2
;	jnc ET1
;	mul y
;	ret
;ET1: 
;	mul x
;	neg y
;	ret
	
;asmBinarization1 endp

end