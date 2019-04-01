// PIV.h by Didier Muñoz
// Version 6
// Esta biblioteca requiere de ImageFFTW DisplacementMap GUIAlgorithm

#ifndef PARTICLEIMAGEVELOCIMETRY
#define PARTICLEIMAGEVELOCIMETRY

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <list>
#include <string>

#include "ImageFFTW.h"
#include "DisplacementMap.h"
#include "GUIAlgorithm.h"


//////// Processing

template <typename D, typename... Ts>
class PIVProcessing{
	private:
		// Internally generates the GUIAlgorithms
		AlgorithmsInterface<PIVProcessing<D, Ts...>, std::list<RealImageFFTW>, 
                          std::list<DisplacementMap>, Ts...> PIVAlgorithms;
		// Reference to the parent class
		D& Data;
		// Output images and string to be used for the algorithms
		std::list<DisplacementMap> OutputMaps;
		std::string OutputString;

	public:
		// Constructor
   	PIVProcessing(D& _data, const std::tuple<Ts...>& _algs) 
		 : PIVAlgorithms(
         make_algorithms_from_tuple<std::list<RealImageFFTW>, std::list<DisplacementMap> >
			 (*this, _algs, typename Generator<std::tuple_size<std::tuple<Ts...> >::value>::Type()))
			, Data(_data) {}
		// Functions AlgorithmInterface requires for this class
		const std::list<RealImageFFTW>& in(void) const {return Data.proInImgs();}
		std::list<DisplacementMap>& out(void) {return OutputMaps;}
		std::string& outStr(void) {return OutputString;}
   	void algorithmProcessed(int _success){
      	Data.statusMessage(OutputString);
   		if(!_success){ // If succeeds
      		Data.setDisplacementMaps(OutputMaps);
				Data.processed();
   		}
      	OutputMaps.clear();
      	OutputString.clear();
		}
		// It's AlgorithmsInterface (PIVAlgorithms) who actually creates the GUI
		void generateGUI(GtkNotebook* _main_notebook, const std::string& _name){
			PIVAlgorithms.generateGUI(_main_notebook, _name);
		}
};

template <typename D, typename... Ts>
PIVProcessing<D,Ts...> make_PIVProcessing(D& _data, const std::tuple<Ts...>& _algs){
	return PIVProcessing<D, Ts...>(_data, _algs);
}

//////// Preprocessing

template <typename D, typename... Ts>
class PIVPreprocessing{
	private:
		// Internally generates the GUIAlgorithms
		AlgorithmsInterface<PIVPreprocessing<D, Ts...>, std::list<RealImageFFTW>, 
                           std::list<RealImageFFTW>, Ts...> PIVImageFilters;
		// Reference to the parent class
		D& Data;
		// Output images and string to be used for the algorithms
		std::list<RealImageFFTW> OutputImgs;
		std::string OutputString;

	public:
		// Constructor
   	PIVPreprocessing(D& _data, const std::tuple<Ts...>& _algs)
			: PIVImageFilters(
				make_algorithms_from_tuple< std::list<RealImageFFTW>, std::list<RealImageFFTW> >
				 (*this, _algs, typename Generator<std::tuple_size<std::tuple<Ts...> >::value>::Type()))
             , Data(_data) {}

		// Functions AlgorithmInterface requires for this class
		const std::list<RealImageFFTW>& in(void) const {return Data.preInImgs();}
		//std::list<RealImageFFTW>& in(void) {return Data.preInImgs();}
		std::list<RealImageFFTW>& out(void) {return OutputImgs;}
		std::string& outStr(void) {return OutputString;}

   	void algorithmProcessed(int _success){
      	Data.statusMessage(OutputString);
   		if(!_success){ // If succeeds
      		Data.setPreOutImgs(OutputImgs);
				Data.preProcessed();
   		}   
      	OutputImgs.clear();
      	OutputString.clear();
		}

		// It's AlgorithmsInterface (PIVImageFilters) who actually creates the GUI
		void generateGUI(GtkNotebook* _main_notebook, const std::string& _name){
			PIVImageFilters.generateGUI(_main_notebook, _name);
		}
};

template <typename D, typename... Ts>
PIVPreprocessing<D,Ts...> make_PIVPreprocessing(D& _data, const std::tuple<Ts...>& _algs){
	return PIVPreprocessing<D, Ts...>(_data, _algs);
}

#endif
/* Cambios:
	First version that uses GUIAlgorithm and PIValgorithm
*/
