// Correlation.h by Didier Muñoz
// Version 6
// Esta biblioteca requiere de ImageFFTW DobleFor PIValgorithm DisplacementMap 

#ifndef IMAGECORRELATION
#define IMAGECORRELATION

#include <list>
#include <string>
#include <thread>
#include "ImageFFTW.h"
#include "DobleFor.h"
#include "PIValgorithm.h"
#include "DisplacementMap.h"

template <typename... Ts> 
using AlgorithmPIV = PIValgorithm <std::list<RealImageFFTW>, std::list<DisplacementMap>, Ts...>;


///////////////Global variable declaration to contain the algorithms

extern const std::tuple <    
								AlgorithmPIV<unsigned int, unsigned int, unsigned int, unsigned int, bool>
							 , AlgorithmPIV<unsigned int, unsigned int, unsigned int, unsigned int, bool, bool>
						 > _AvailablePIVAlgorithms_;

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////Aquí empiezan las clases y funciones para los algoritmos de correlación////////

//>>>>>>>>>Algoritmo básico<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

///////////Utilizando correlación directa/////////////////////////////////////////////////////
//---------Cada desplazamiento se obtiene calculando el promedio de máximos-------------------

// Función que calcula el mapa de desplazamientos
int basic_DirectCorr_maxAverage(const std::list<RealImageFFTW>& _in, std::list<DisplacementMap>& _out, std::string& _outstr,
                 unsigned int zonalto, unsigned int zonancho, unsigned int sepv, unsigned int seph, bool esrafaga);

//////////Ulizando correlación por transformada de Fourier////////////////////////////////////
//---------Cada desplazamiento se obtiene calculando el promedio de máximos-------------------

// Función que calcula el mapa de desplazamientos usando transformada de Fourier
int basic_TFCorr_maxAverage(const std::list<RealImageFFTW>& _in, std::list<DisplacementMap>& _out, std::string& _outstr,
                 unsigned zonalto, unsigned zonancho, unsigned sepv, unsigned seph, bool rafaga, bool promedio);

//>>>>>>>>>>>Aquí pueden ir otros algoritmos<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif

/*Cambios:
	This version is the first one conditioned to use PIValgorithm and DisplacementMap.
*/
