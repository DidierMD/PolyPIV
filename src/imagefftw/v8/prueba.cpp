#include <iostream>
#include <cmath>
#include "ImageFFTW.h"
#define _PI 3.1415926

using namespace std;

int main()
{
    cout << "Se crea una imagen real particular"
    /*cout<<"Se crea una imagen real a partir de la función f(x,y)=sin(3cos(y/2)sin(x))\
 en [-pi,pi]² y se guarda con el nombre de sin.tif"*/
        << endl;
    TransFourierR2C tf(8,8,GREY);
    RealImageFFTW& fi= (*tf.entrada());
    ComplexImageFFTW& fo= (*tf.salida());
/*    double aux = 2.*_PI/40.;
    for(double i = 0.; i < (double)f.alto();i = i+1.)
        for(double j = 0.; j < (double)f.ancho();j = j+1.)
            f.picelU(BRIGHT,(int)i,(int)j) = sin(3.*cos((j*aux - _PI)/2.)*sin(i*aux - _PI));
*/
    double gas=70.;
    for(int i = 1; i<5; i++){
        for(int j = 2; j<5;j++){
            fi.picelU(BRIGHT, i, j) = gas; // Se crea la imagen
            gas += 10.;
        }
        gas -= 10.;
    }
    cout << fi;
    cout << endl;
   RealImageFFTW cp(4,4,GREY);
   cp.borrar();
   cp.copia(1,4,2,6,fi);
   cout << "1,4,2,6" << endl << cp << endl;
   //cp.borrar();
   cp.copia(-2,6,2,6,fi);
   cout << "-2,6,2,6" << endl << cp << endl;
   //cp.borrar();
   cp.copia(2,6,6,9,fi);
   cout << "2,6,6,9" << endl << cp << endl;
   RealImageFFTW cp1(1,4,2,6,fi);
   cout << "Creando una imagen nueva: 1,4,2,6" << endl << cp1 << endl;
   cp1.copiaE(-2,6,2,6,fi);
   cout << "copiaE: -2,6,2,6" << endl << cp1 << endl;

    cout << "Se crea TransFourier con la imagen y se calcula la transformada:" << endl;
    tf.calcular();
    cout << "Parte real:\n";
    for(int i = 0; i < fo.alto(); i = i+1){
        for(int j = 0; j < fo.ancho(); j = j+1)
            cout << real(fo.pixelU(BRIGHT,i,j)) <<" ";
        cout << endl;
    }
    cout << "Parte imaginaria:\n";
    for(int i = 0; i < fo.alto(); i = i+1){
        for(int j = 0; j < fo.ancho(); j = j+1)
            cout << imag(fo.pixelU(BRIGHT,i,j)) <<" ";
        cout << endl;
    }
    cout << "Se crea la transformada inversa y se calcula:" << endl;
    TransFourierC2R ft(fo.alto(),fo.ancho(),GREY);
    ComplexImageFFTW& fii= (*ft.entrada());
    RealImageFFTW& foo= (*ft.salida());
    fii = fo; // Tener cuidado con los tamaños de las imágenes
    ft.calcular();
    cout << foo << endl;
    
    TransFourierR2C lltf(3,3,GREY);
    for(int i =0; i<lltf.entrada()->alto(); i++)
        for(int j=0; j<lltf.entrada()->ancho(); j++)
            (lltf.entrada())->picel(BRIGHT,i,j) = 0.;
    (lltf.entrada())->picel(BRIGHT,1,1) = 1.;
    cout << "\nLa imagen f:\n";
    cout << *lltf.entrada();
    lltf.calcular();
    ComplexImageFFTW *ifo = (lltf.salida());
    cout << "\nLa transformada de la imagen f:\n";
    cout << *ifo;
    cout << endl;

    TransFourierR2C lltf2(3,3,GREY);
    for(int i =0; i<lltf2.entrada()->alto(); i++)
        for(int j=0; j<lltf2.entrada()->ancho(); j++)
            (lltf2.entrada())->picel(BRIGHT,i,j) = 0.;
    (lltf2.entrada())->picel(BRIGHT,0,1) = 1.;
    cout << "\nLa imagen g:\n";
    cout << *lltf2.entrada();
    lltf2.calcular();
    cout << "\nLa transformada de la imagen g:\n";
    cout << *lltf2.salida();
    cout << endl;

    cout << "\nLa transformada inversa del producto de las dos anteriores:\n";
    TransFourierC2R llft(3,3,GREY);
    //cout << ifo->alto() << " " << ifo->ancho() << " " << ifo->tipo() << endl;
    (llft.entrada())->multConj(*lltf.salida(),*lltf2.salida());
    llft.calcular();
    cout << (*llft.salida());
    cout << "Probando getImageFormatFromFilename:" << endl;
    string cadena1 = "soplete.tiff";
    string cadena2 = "nonono";
    string cadena3 = "seseis.txt";
    string cadena4 = "aveces.";
    cout << "cadena1: " << cadena1 << "  formato == FIF_TIFF: " 
         << (FIF_TIFF == getImageFormatFromFilename(cadena1)) << endl;
    cout << "cadena2: " << cadena2 << "  formato: " << getImageFormatFromFilename(cadena2) << endl;
    cout << "cadena3: " << cadena3 << "  formato: " << getImageFormatFromFilename(cadena3) << endl;
    cout << "cadena4: " << cadena4 << "  formato: " << getImageFormatFromFilename(cadena4) << endl;
   return 0;
}
