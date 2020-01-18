// JACpp.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

//extern "C" _declspec(dllexport)void MyProc2(unsigned char* bmptab, int hgth, int wdth, int threads, int index);


extern "C" _declspec(dllexport)void MyProc2(unsigned char* bmptab, int hgth, int wdth, int threads, int index)
{
	double color = 0.0;
	const int size = 4 * hgth*wdth;
	//Dla 4 w¹tkow i 4000 komorek tablicy, dla trzeciego w¹tku: begin = (3-1)*4000/4 = 2000, end = 3*4000/4=3000
	int begin = (index - 1)*size / threads; 
	int end = index * size / threads;
	for (int i = begin; i < end; i += 4)
	{
		color = 0.11*bmptab[i] + 0.59*bmptab[i + 1] + 0.3*bmptab[i + 2]; //BGR na skale szarosci przyjemniejsza dla oka
		bmptab[i], bmptab[i + 1], bmptab[i + 2] = color; 
	}
}
