//na nastepne algorytm w dll cpp
//20 stycznia prezentacja 
using namespace std;
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <Windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <chrono> 
void MyProc2(unsigned char* tab, int wdth, int wdth_in_bytes, int start, int end);

int CPP(unsigned char* tab, int wdth, int wdth_in_bytes, int start, int end)
{
	HINSTANCE hGetProcIDDLL = LoadLibrary("JACpp.dll");
	FARPROC lpfnGetProcessID = GetProcAddress(HMODULE(hGetProcIDDLL), "MyProc2");
	typedef void(__cdecl * pICFUNC)(unsigned char*, int, int, int, int);
	int error = GetLastError();
	DWORD WINAPI FormatMessage(
		DWORD dwFlags,
		LPCVOID lpSource,
		DWORD dwMessageId,
		DWORD dwLanguageId,
		LPTSTR lpBuffer,
		DWORD nSize,
		va_list * Arguments
	);
	pICFUNC function;
	function = pICFUNC(lpfnGetProcessID);
	function(tab, wdth, wdth_in_bytes, start, end);
	FreeLibrary(hGetProcIDDLL);
	return 0;
}

int ASM(unsigned char* tab, int wdth, int wdth_in_bytes, int start, int end)
{
	HINSTANCE hGetProcIDDLL = LoadLibrary("JAAsm.dll");
	FARPROC lpfnGetProcessID = GetProcAddress(HMODULE(hGetProcIDDLL), "MyProc");
	typedef void(__cdecl * pICFUNC)(unsigned char*, int, int, int, int);
	int error = GetLastError();
	DWORD WINAPI FormatMessage(
		DWORD dwFlags,
		LPCVOID lpSource,
		DWORD dwMessageId,
		DWORD dwLanguageId,
		LPTSTR lpBuffer,
		DWORD nSize,
		va_list * Arguments
	);
	pICFUNC function;
	function = pICFUNC(lpfnGetProcessID);
	function(tab, wdth, wdth_in_bytes, start, end);
	FreeLibrary(hGetProcIDDLL);
	return 0;
}
class Byte {
public:
	Byte() //konstruktor domyslny, dla pojedynczego bajtu
	{
		bitcount = 8;
	}
	Byte(int _bitcount) //konstruktor dla sklejanych bajtow
	{
		bitcount = _bitcount;
	}
	int val;
	char tab[32]; 

	int bitcount;

	void to_dec() //zmiana na wartosc dziesietna
	{
		unsigned long _val = 1;
		this->val = 0;

		for (int i = 0; i < this->bitcount; i++)
		{
			if (this->tab[this->bitcount - i - 1] == '1')
				this->val += _val;
			_val = 2 * _val;
		}
		return;
	}


	void to_bin() //zmiana na wartosc binarna
	{
		int _val = this->val;
		
		for (int i = 0; i < this->bitcount; i++)
		{
			this->tab[this->bitcount - i - 1] = 48 + (_val % 2);
			_val = _val / 2;

		}
		return;
	}

	void to_32bit(Byte b1, Byte b2, Byte b3, Byte b4) //Sklejenie czterech bajt�w
	{
		b4.to_bin();
		b3.to_bin();
		b2.to_bin();
		b1.to_bin();
		for (int i = 0; i < 8; i++)
		{
			this->tab[i] = b4.tab[i];
		}
		for (int i = 8; i < 16; i++)
		{
			this->tab[i] = b3.tab[i-8];
		}
		for (int i = 16; i < 24; i++)
		{
			this->tab[i] = b2.tab[i-16];
		}
		for (int i = 24; i < 32; i++)
		{
			this->tab[i] = b1.tab[i-24];
		}
		
		
	}
};
class BMP {
public:
	unsigned char* tab;
	unsigned char* newtab;
	FILE* file;
	FILE* newfile;
	int size;
	int padding;
	int hght;
	int wdth;
	int wdth_in_bytes;
	int address;

	bool loadfile(const char* name)
	{
		int loaded;
		//const char* name = "test.bmp";
		this->file = fopen(name, "rb");
		if (!this->file) //przy braku pliku zwraca false
			return false;

		
		
		Byte* b1 = new Byte(); //Tworzenie czterech bajtow, do ktorych zapisywane beda kolejno dane z pliku
		Byte* b2 = new Byte();
		Byte* b3 = new Byte();
		Byte* b4 = new Byte();

		fseek(this->file, 10, SEEK_SET); //Zczytanie adresu danych
		
		loaded = getc(this->file);
		b1->val = loaded;
		loaded = getc(this->file);
		b2->val = loaded;
		loaded = getc(this->file);
		b3->val = loaded;
		loaded = getc(this->file);
		b4->val = loaded;

		
		Byte* groupped = new Byte(32); //Obiekt przechowujacy cztery sklejone bajty
		groupped->to_32bit(*b1, *b2, *b3, *b4); //sklejenie bajtow
		groupped->to_dec(); //konwersja na system dziesietny

		this->address = groupped->val; //Przypisanie zaladowanej wartosci do adresu danych
		
		fseek(this->file, 18, SEEK_SET); //Zczytanie szerokosci 

		loaded = getc(this->file);
		b1->val = loaded;
		loaded = getc(this->file);
		b2->val = loaded;
		loaded = getc(this->file);
		b3->val = loaded;
		loaded = getc(this->file);
		b4->val = loaded;

		groupped->to_32bit(*b1, *b2, *b3, *b4);
		groupped->to_dec();

		
		this->wdth = groupped->val; //Przypisanie zaladowanej wartosci do szerokosci 

		
		fseek(this->file, 34, SEEK_SET); //Zczytanie wielkosci danych

		loaded = getc(this->file);
		b1->val = loaded;
		loaded = getc(this->file);
		b2->val = loaded;
		loaded = getc(this->file);
		b3->val = loaded;
		loaded = getc(this->file);
		b4->val = loaded;

		
		groupped->to_32bit(*b1, *b2, *b3, *b4);
		groupped->to_dec();

		
		this->size = groupped->val; //Przypisanie wartosci do rozmiaru danych

		getpadding();

		fseek(this->file, this->address, SEEK_SET);
		tab = new unsigned char[this->size + this->wdth_in_bytes];
		for (int i = 0; i < this->size; i++)
		{
			tab[i] = getc(this->file);
		}
		
		for (int i = this->size; i < this->size + this->wdth_in_bytes; i++)
		{
			tab[i] = tab[i - this->wdth_in_bytes];
		}

		delete b1;
		delete b2;
		delete b3;
		delete b4;
		delete groupped;
		return true;
	}
	void getpadding()
	{
		this->padding = (4 - ((this->wdth * 3) % 4)) % 4; 
		this->wdth_in_bytes = this->wdth * 3 + padding; //obliczenie szerokosci w bajtach po wyrownaniu do 4
		this->hght = this->size / this->wdth_in_bytes;
	}


	void CPPThreads(int threads) //funkcja tworzaca wektor watkow z funkcjami biblioteki CPP
	{
		vector<thread> threads_vec(threads); //utworzenie wektora watkow
		for (int i = 0; i < threads; i++)
		{
			threads_vec[i] = thread(CPP, this->tab, wdth, wdth_in_bytes, int(floor(float(hght) / float(threads) * i)), int(((float)hght / (float)threads) * (i + 1)));//wyliczenie poczatku i konca obszaru tab dla jednego watku
		}
		//threads_vec[threads-1] = thread(CPP, this->tab, wdth, wdth_in_bytes, int(floor(float(hght) / float(threads) * (threads-1))), int(((float)hght / (float)threads) * (threads)));//Ostatni obszar poza petla bez zaokraglenia end w gore

		for (int i = 0; i < threads; i++)
		{
			threads_vec[i].join();
		}
		
	}

	void ASMThreads(int threads)//funkcja tworzaca wektor watkow z funkcjami biblioteki asm
	{
		vector<thread> threads_vec(threads); //utworzenie wektora watkow
		for (int i = 0; i < threads; i++)
		{
			threads_vec[i] = thread(ASM, this->tab, wdth, wdth_in_bytes, int(floor(float(hght) / float(threads) * i)), int(((float)hght / (float)threads) * (i + 1)));//wyliczenie poczatku i konca obszaru tab dla jednego watku
		}
		//threads_vec[threads - 1] = thread(ASM, this->tab, wdth, wdth_in_bytes, int(floor(float(hght) / float(threads) * (threads-1))), int(((float)hght / (float)threads) * (threads)));//Ostatni obszar poza petla bez zaokraglenia end w gore
		for (int i = 0; i < threads; i++)
		{
			threads_vec[i].join();
		}

	}

	void savefile(const char* newname)
	{
		int val;

		newfile = fopen(newname, "wb");		//Tworzenie nowej mapy bitowej
		fseek(file, 0L, SEEK_SET);				//Ustawienie na poczatku starej bitmapy w celu przepisania naglowka

		for (int i = 0; i < this->address; ++i)	//Przepisanie naglowka
		{
			val = getc(file);
			putc(val, newfile);
		}

		for (int i = 0; i < size; ++i)	//!Przepisuj kolejne bajty obszaru danych zmieniaj�c ostatni bit ka�dego koloru
			putc(tab[i], newfile);

		fclose(newfile);

		return;
	}
	
};



void emptyTest() //funkcja tymczasowa do wywolania w watkach
{
	cout << "x";
}
int main()
{
	BMP bmp;
	int threads = 0;
	int input = 0;
	char choice;
	const char* name = "bigtest.bmp";
	const char* newname = "graytest.bmp";
	bool ifAsm = false;
	string flush = "";
	
    cout << "Podaj liczbe watkow (0-64, gdzie 0 - optymalna): "; 
	cin >> input;
	if (input > 0 && input < 65)
	{
		threads = input;
	}
	else if (input > 64) //powyzej max ustawiane na max
	{
		threads = 64;
	}
	else //dla zera i niepoprawnych wartosci - optymalna
	{
		threads = thread::hardware_concurrency(); //liczba watkow procesora
	}
	cout << "Wybrana liczba watkow: " << threads;
	cout << "\nCzy program powinien skorzystac z biblioteki asm? (Y/N): ";
	while ((getchar()) != '\n'); //flush
	choice = getchar(); //uzytkownik wybiera biblioteke
	if (choice == 'y' || choice == 'Y')
	{
		ifAsm = true;
		cout << "Wybrana biblioteka: asm\n";
	}
	else
	{
		cout << "Wybrana biblioteka: c++\n";
	}	
	
	if (ifAsm)//korzystaj z biblioteki asm, jesli uzytkownik tego chce
	{
		if (bmp.loadfile(name))
		{
			auto start = chrono::high_resolution_clock::now();
			bmp.ASMThreads(threads);
			auto stop = chrono::high_resolution_clock::now();
			auto dura = chrono::duration_cast<chrono::microseconds>(stop - start);
			cout << "Biblioteka: asm - Watki: " << threads << " - czas: "<< dura.count() << endl;
			bmp.savefile(newname);
		}
	}
	else
	{
		if (bmp.loadfile(name))
		{	
			auto start = chrono::high_resolution_clock::now();
			bmp.CPPThreads(threads);
			auto stop = chrono::high_resolution_clock::now();
			auto dura = chrono::duration_cast<chrono::microseconds>(stop - start);
			cout << "Biblioteka: c++ - Watki: " << threads << " - czas: " << dura.count() << endl;
			bmp.savefile(newname);
		}
		
	}
	
	
	
	return 0;
}

