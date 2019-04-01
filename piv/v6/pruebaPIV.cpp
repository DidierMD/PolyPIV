// pruebaPIV.cpp by Didier
// Associated with PIV v6

#include <iostream>
#include <cstdlib>
#include <list>

#include "PIV.h"
//#include "../../Correlation/lastVersion/Correlation.h"
//#include "imageFFTW.h"
//#include "DisplacementMap.h"
//#include "../../ImageFilters/lastVersion/ImageFilters.h"

using namespace std;

class Datas{
	private:
		list<RealImageFFTW> AuxImages;
	public:
		list<RealImageFFTW>& proInImgs(void){return AuxImages;}
		list<RealImageFFTW>& preInImgs(void){return AuxImages;}
		void setDisplacementMaps(const list<DisplacementMap>& _outmaps){}
		void setPreOutImgs(const list<RealImageFFTW>& _outimgs){}
		void statusMessage(const string& _str){ cout << _str << endl;}
		void processed(void){ cout << "Procesado" << endl;}
		void preProcessed(void){ cout << "preProcesado" << endl;}
};

int main()
{
	Datas auxdata;
	//auto proc = make_PIVProcessing(auxdata, _AvailablePIVAlgorithms_);
	//PIVPreprocessing
	//proc.algorithmProcessed(0);
	//proc.algorithmProcessed(1);

   return 0;
}
