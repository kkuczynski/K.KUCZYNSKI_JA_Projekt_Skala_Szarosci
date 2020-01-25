.DATA
three REAL8 3.0
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
mov r12, rax	;do r12 wpisujemy rax- pole
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

				movzx eax, byte ptr[rsi]	;pobieram pierwszy kolor do eax, niebieski
		;wektorowa instrukcja
				cvtsi2sd xmm1,eax			;zamiana 32bitowej wartosci ca³kowitoliczbowej do 64bajtowej zmiennoprzecinkowej czyli przekazanie wartosci koloru blue do xmm1
				movzx eax, byte ptr[rsi+1]  ;pobieram drugi kolor do eax, zielony
		;wektorowa isntrukcja
				cvtsi2sd xmm2,eax			;zamiana 32bitowej wartosci ca³kowitoliczbowej do 64bajtowej zmiennoprzecinkowej
				movzx eax, byte ptr[rsi+2]  ;pobieram trzeci kolor, czerwony
		;wektorowa instrukcja
				cvtsi2sd xmm3,eax			;zamiana 32bitowej wartosci ca³kowitoliczbowej do 64bajtowej zmiennoprzecinkowej

			;wektorowa isntrukcja
			;obliczanie sredniej kolorow
				addsd xmm1,xmm2				;dodanie do xmm1 wartosci z xmm2 (B+G)
				addsd xmm1,xmm3				;dodanie do poprzednio uzyskanej xmm1 wartosci z xmm3 ((B+G)+R)
				divsd xmm1,three			;dzielenie xmm1 przez 3 - czyli wyliczanie sredniej, by uzyskac kolor szarosci

			;wektorowa instrukcja
				cvttsd2si eax,xmm1		 ;zamiana 64bajtowej wartosci zmienorzecinkowej do 32bitowej ca³kowitoliczbowej, odwrotny proces do tego co wyzej
				mov byte ptr[rsi],al	 ;zapisanie nowego "koloru" w miejsce skad byl pobierany, niebieski
				mov byte ptr[rsi+1],al	 ;zapisanie nowego "koloru" w miejsce skad byl pobierany, zielony
				mov byte ptr[rsi+2],al	 ;zapisanie nowego "koloru" w miejsce skad byl pobierany, czerwony

		add rsi,3		;przejscie do kolejnego pixela
		inc r14			;inkrementacja j++
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

