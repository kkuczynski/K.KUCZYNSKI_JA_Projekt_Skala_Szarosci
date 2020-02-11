.DATA
red REAL4 0.30
green REAL4 0.59
blue REAL4 0.11
.CODE
DllEntry PROC hInstDLL:DWORD, reason:DWORD, reserved1:DWORD
mov rax, 1
ret
DllEntry ENDP

MyProc proc tab:qword,wdth:qword,wdthbytes:qword,start:dword,theend:dword

;inicjalizacja zmiennych

mov tab, rcx
mov wdth, rdx
mov wdthbytes, r8
mov start, r9d
mov r10d, theend

mov eax, blue
pinsrd xmm4, eax, 0 ;przygotowanie wektora z przelicznikiem koloru niebieskiego
pinsrd xmm4, eax, 1
pinsrd xmm4, eax, 2
pinsrd xmm4, eax, 3

mov eax, green
pinsrd xmm5, eax, 0 ;przygotowanie wektora z przelicznikiem koloru zielonego
pinsrd xmm5, eax, 1
pinsrd xmm5, eax, 2
pinsrd xmm5, eax, 3

mov eax, red
pinsrd xmm6, eax, 0 ;przygotowanie wektora z przelicznikiem koloru czerwonego
pinsrd xmm6, eax, 1
pinsrd xmm6, eax, 2
pinsrd xmm6, eax, 3

;obliczam ilosc zer
mov r11,r8		;do r11 wpisuje wartosc znajdujaca sie w r8: wdthbytes
mov rax, 3		;przygotowuje 3 
mul rdx			;mnoze zawartosc rdx razy rax - czyli szerokosc * 3
mov r12, rax	;do r12 wpisuje wynik, ktory jest w rax
sub r11,r12		;od r11 odejmuje r12 , byteswdth - 3*wdth

mov rdx, wdth	;do rdx wpisuje szerokosc

mov rsi, rcx	;wskaznik mapy obrazu, punkt poczatkowy obrazu
mov rax, 0		;zeruje rax
mov eax, r9d	;do eax wpisuje start - wysokosc poczatku dla danego watku
mul r8			;mnoz wdthbytes
mov r12, rax	;do r12 wpisuje rax- pole
add rsi,r12		;do rsi dodaje przesuniecie - przesuwanie wskaznika

mov rdx, wdth   ;do rdx wpisuje szerokosc


;pêtla for i
mov r15d, r9d	;int i = start
_fori:
cmp r15d, r10d	;i<theend
jae _end_fori

	;pêtla for j
		mov r14,0	;int j=0
		_forj:
		cmp r14, rdx	;j<wdth
		jae _end_forj
		movzx eax, byte ptr[rsi] ;przenosze 8 bitowy kolor z wypenieniem zerami do 32 bitowego akumulatora
		pinsrd xmm1, eax, 0  ;xmm1 z wartosciami kolorow niebieskich 4 pikseli
		movzx eax, byte ptr[rsi+3]
		pinsrd xmm1, eax, 1
		movzx eax, byte ptr[rsi+6]
		pinsrd xmm1, eax, 2
		movzx eax, byte ptr[rsi+9]
		pinsrd xmm1, eax, 3
		;cvtdq2ps xmm1, xmm1 - usunieta, zbedna konwersja na float
		mulps xmm1, xmm4 ;blue*0.11


		movzx eax, byte ptr[rsi+1]
		pinsrd xmm2, eax, 0  ;xmm2 z wartosciami kolorow zielonych 4 pikseli
		movzx eax, byte ptr[rsi+4]
		pinsrd xmm2, eax, 1
		movzx eax, byte ptr[rsi+7]
		pinsrd xmm2, eax, 2
		movzx eax, byte ptr[rsi+10]
		pinsrd xmm2, eax, 3
		;cvtdq2ps xmm2, xmm2
		mulps xmm2, xmm5 ;green*0.59


		movzx eax, byte ptr[rsi+2]
		pinsrd xmm3, eax, 0  ;xmm3 z wartosciami kolorow czerwonych 4 pikseli
		movzx eax, byte ptr[rsi+5]
		pinsrd xmm3, eax, 1
		movzx eax, byte ptr[rsi+8]
		pinsrd xmm3, eax, 2
		movzx eax, byte ptr[rsi+11]
		pinsrd xmm3, eax, 3
		;cvtdq2ps xmm3, xmm3
		mulps xmm3, xmm6 ;red*0.3

		;cvtps2dq xmm1, xmm1
		;cvtps2dq xmm2, xmm2
		;cvtps2dq xmm3, xmm3
		paddusb	 xmm1, xmm2	;dodanie w pionie wartosci xmm1 i xmm2 do xmm1, czyli 4x b+g
		paddusb xmm1, xmm3  ;dodanie w pionie wartosci xmm1 i xmm3 do xmm1, czyli 4x (b+g)+r

	   

		pextrd eax, xmm1, 0 ;zaladowanie wartosci sredniej kolorow pierwszego piksela do 32-bitowego akumulatora
		mov byte ptr[rsi], al ;przepisanie wartosci 8 najmlodszych bitów akumulatora na miejsce trzech kolorow pierwszego piksela
		mov byte ptr[rsi+1], al
		mov byte ptr[rsi+2], al

		pextrd	eax, xmm1, 1 ;drugi piksel
		mov	byte ptr[rsi+3], al
		mov	byte ptr[rsi+4], al
		mov	byte ptr[rsi+5], al

		pextrd	eax, xmm1, 2 ;trzeci piksel
		mov	byte ptr[rsi+6], al
		mov	byte ptr[rsi+7], al
		mov	byte ptr[rsi+8], al
		
		pextrd	eax, xmm1, 3 ;czwarty piksel
		mov	byte ptr[rsi+9], al
		mov	byte ptr[rsi+10], al
		mov	byte ptr[rsi+11], al



		add rsi,12		;przejscie do kolejnych 4 pikseli
		add r14, 4		;dodanie 4 do j - przesuniecie o 4 piksele w bok
		jmp _forj		;skok do petli j
		_end_forj:
		_retfori:
		

add rsi, r11	;przesuniecie wskaznika o ilosc zer, ktore sa na koncu wyliczonych wczesniej
inc r15d		;inkrementacja licznika i++
jmp _fori		;skok do petli fori
_end_fori:      ;jak sie petla konczy to return
ret

MyProc endp

END

