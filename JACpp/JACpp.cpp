// JACpp.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

//extern "C" _declspec(dllexport)void MyProc2(unsigned char* bmptab, int hgth, int wdth, int threads, int index);


extern "C" _declspec(dllexport)void MyProc2(unsigned char* bmptab, int wdth, int wdth_in_bytes, int start, int end)
{
	unsigned char padding = 0;		// dla uzyskania podzielnosci pikseli w rzedzie przez 4
	int index = start*wdth_in_bytes;				

	int color;
	if ((wdth * 3) % 4 != 0)
	{
		padding = 4 - ((wdth * 3) % 4);
	}

	for (int j = start; j < end; j++) // petla po wysokosci
	{
		for (int i = 0; i < wdth; ++i) // pêtla po szerokosci
		{

			color = (0.11*bmptab[index] + 0.59*bmptab[index + 1] + 0.30*bmptab[index]);	//obliczenie przyjaznej dla oka skali szarosci
			bmptab[index] = color;
			bmptab[index + 1] = color;
			bmptab[index + 2] = color;

			index += 3; //przesuniecie indeksu o 3 po przetworzeniu skladowych koloru jednego piksela
		}
		index += padding; // wyrownianie wiersza do podzielnosci przez 3
	
	}
	
}
