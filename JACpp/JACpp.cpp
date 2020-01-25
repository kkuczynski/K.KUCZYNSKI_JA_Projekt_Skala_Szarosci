// JACpp.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

//extern "C" _declspec(dllexport)void MyProc2(unsigned char* bmptab, int hgth, int wdth, int threads, int index);


extern "C" _declspec(dllexport)void MyProc2(unsigned char* bmptab, int wdth, int wdth_in_bytes, int start, int end)
{
	int color = 0;
	int _size = wdth * 4 * (end - start); //rozmiar obszaru przetwarzanego w tym watku
	for (int j = start * 4 * wdth; j < end * 4 * wdth; j += 4)
	{


		color = (bmptab[j] + bmptab[j + 1] + bmptab[j + 2]) / 3; //wyliczenie sredniej kolorow pixeli ->BGR z pominieciem czwartej komorki = 0
		bmptab[j] = color; 
		bmptab[j + 1] = color; 
		bmptab[j + 2] = color; //przypisanie tej samej wartosci rownej sredniej poszczegolnym wartosciom B G i R

	}









	//int color = 0;
	//const int size = 3*wdth*hgth;
	//Dla 4 w¹tkow i 4000 komorek tablicy, dla trzeciego w¹tku: begin = (3-1)*4000/4 = 2000, end = 3*4000/4=3000
	//int begin = (index - 1)*size / threads; 
	//int end = index * size / threads;
	//for (int i = begin; i < end; i += 3)
	//{
		//color = 0.11*bmptab[i] + 0.59*bmptab[i] + 0.3*bmptab[i]; //BGR na skale szarosci przyjemniejsza dla oka
	//	color = (bmptab[i] + bmptab[i + 1] + bmptab[i + 2]) / 3;
	//	bmptab[i] = color; 
	//	bmptab[i + 1] = color;
	//	bmptab[i + 2] = color;
	//}
}
