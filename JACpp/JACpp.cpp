// JACpp.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

//extern "C" _declspec(dllexport)void MyProc2(unsigned char* bmptab, int hgth, int wdth, int threads, int index);


extern "C" _declspec(dllexport)void MyProc2(unsigned char* bmptab, int wdth, int wdth_in_bytes, int start, int end)
{
	int color = 0;
	int _size = wdth * 3 * (end - start); //rozmiar obszaru przetwarzanego w tym watku
	for (int j = start * 3 * wdth; j < end * 3 * wdth; j += 3)
	{


		color = (bmptab[j] + bmptab[j + 1] + bmptab[j + 2]) / 3; //wyliczenie sredniej kolorow pixeli ->BGR 
		bmptab[j] = color; 
		bmptab[j + 1] = color; 
		bmptab[j + 2] = color; //przypisanie tej samej wartosci rownej sredniej poszczegolnym wartosciom B G i R

	}
	
}
