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
void MyProc2(unsigned char* bmptab, int hgth, int wdth, int threads, int index);

int CPP(unsigned char* data, int hght, int wdth, int threads, int index)
{
	HINSTANCE hGetProcIDDLL = LoadLibrary("JACpp.dll");
	FARPROC lpfnGetProcessID = GetProcAddress(HMODULE (hGetProcIDDLL), "MyProc2");
	typedef void(__cdecl* pICFUNC)(unsigned char*, int, int, int, int);
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
	function(data, hght, wdth, threads, index);

	FreeLibrary(hGetProcIDDLL);
	return 0;
}
/*void ASM(unsigned char* data, int hght, int wdth, int threads, int index)
{
	HINSTANCE hGetProcIDDLL = LoadLibrary("JAAsm.dll");
	FARPROC lpfnGetProcessID = GetProcAddress(HMODULE(hGetProcIDDLL), "MyProc2");
	typedef void(__cdecl* pICFUNC)(unsigned char*, int, int, int, int);
	pICFUNC function;
	function(data, hght, wdth, threads, index);
}*/

void emptyTest() //funkcja tymczasowa do wywolania w watkach
{
	cout << "x";
}
int main()
{
	int threads = 0;
	int input = 0;
	char choice;
	const char* name = "yoda.bmp";
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
	
	FILE* f = fopen(name, "rb");
	unsigned char header[54];
	fread(header, sizeof(unsigned char), 54, f); //zczytanie naglowka pliku bmp

	//wydobycie szerokosci i wysokosci z naglowka
	int wdth = *(int*)&header[18];
	int hght = *(int*)&header[22];

	int size = 3 * wdth * hght; // rozmiar tablicy pikseli (b, g, r)
	unsigned char* data = new unsigned char[size]; //alokowanie 3 bajtow na piksel
	fread(data, sizeof(unsigned char), size, f); //zczytanie wartosci kolorow pikseli
	fclose(f);
	//for (int i = 0; i < size/100;)
	//{
	//	cout << (float)data[i] << " " << (float)data[i + 1] << " " << (float)data[i + 2] << "\n";
	//	i += 3;
	//}
	if (ifAsm)//korzystaj z biblioteki asm, jesli uzytkownik tego chce
	{
		
	}
	else
	{
		vector<thread>threadsVec(threads);
		for (int i = 0; i < threads; i++)
		{
			threadsVec[i] = thread(CPP,data, hght, wdth, threads, i+1);
			
			threadsVec[i].join();
		}
	}
	FILE* newf = fopen(newname, "w"); //utworzenie pustego pliku
	fclose(newf);
	newf = fopen(newname, "a");  //dopisanie do pliku nowej bitmapy
	
	fwrite(header, sizeof(unsigned char), 54, newf); //dodanie naglowka do nowego pliku bmp
	fwrite(data, sizeof(unsigned char), size, newf); //dopisanie wartosci pikseli do pliku bmp
	fclose(newf);
	//cout << "\n\nNOWE\n\n";
	//for (int i = 0; i < size / 100;)
	//{
	//	cout << (float)data[i] << " " << (float)data[i + 1] << " " << (float)data[i + 2] << "\n";
	//	i += 3;
	//}
	
	
	return 0;
}

