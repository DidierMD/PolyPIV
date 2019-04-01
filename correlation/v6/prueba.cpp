// prueba.cpp by Didier
// Associated with Correlation v6

#include <iostream>
#include <cstdlib>

#include "Correlation.h"

using namespace std;

int main()
{
	int cont=0;
   RealImageFFTW f1(2, 2, GREY), g1(4, 4, GREY), h2;
   RealImageFFTW h3;
   TransFourierR2C tf1(f1.alto()+g1.alto()-1, f1.ancho()+g1.ancho()-1, GREY);
	TransFourierR2C tg1(f1.alto()+g1.alto()-1, f1.ancho()+g1.ancho()-1, GREY);
   TransFourierC2R tifg1(f1.alto()+g1.alto()-1, f1.ancho()+g1.ancho()-1, GREY);

	for(int i=0; i<g1.alto(); i++)
		for(int j=0; j<g1.ancho(); j++){
			cont++;
   		g1.picelU(BRIGHT, i, j)=cont;
		}

   cout << "g es:\n";
	cout << g1 << endl;

	for(int i=0; i<f1.alto(); i=i+f1.alto()-1)
		for(int j=0; j<f1.ancho(); j=j+f1.ancho()-1){
			f1.borrar();
			f1.picelU(BRIGHT, i, j)=1.;
			cout << "f es:" << endl;
			cout << f1 << endl;
   		cout << "La correlacion es:" << endl;
//   		imageCorrelation(f1,g1,h2);
   		cout << h2 << endl;
   		cout << "La correlacion usando FFTW es:" << endl;
   		tf1.entrada()->borrar();
   		tg1.entrada()->borrar();
     		tf1.entrada()->copiaE(0, f1.alto(), 0, f1.ancho(), f1);
     		tg1.entrada()->copiaE(0, g1.alto(), 0, g1.ancho(), g1);
//   		imageCorrelationFFTW(tf1, tg1, tifg1);
   		for(int k=0; k<tifg1.salida()->arraySize(); k++)
     			if((*tifg1.salida())[0][k] < 0.001)
       			(*tifg1.salida())[0][k] = 0.;
   		cout << (*tifg1.salida()) << endl << endl;
		}

   return 0;
}
