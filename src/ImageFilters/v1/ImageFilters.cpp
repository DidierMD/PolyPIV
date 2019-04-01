// ImageFilters.cpp by Didier
// Version 1
// Needs: ImageFFTW PIValgorithm

#include <vector>
#include <algorithm>
#include "ImageFilters.h"

// Global variable that stores the list of avaiable image filters
const std::tuple <  ImageFilter<unsigned, unsigned, unsigned, unsigned>
				  		, ImageFilter<>
				  		, ImageFilter<unsigned, unsigned>
						, ImageFilter<float>
						> _AvailableImageFilters_(
					////
					ImageFilter<unsigned, unsigned, unsigned, unsigned>(
						"Crop Images", // Nombre del algoritmo
						crop_images,	// Función del algoritmo
						{"Bottom","Top","Left","Right"}) // Nombres de los parámetros
					////
					, ImageFilter<>(
						"Minus the average",
						minus_the_average,
						{})
					////
					, ImageFilter<unsigned, unsigned>(
						"Statistical order",
						substract_percentile_image,
						{"Radius", "Percentile"})
					////
					, ImageFilter<float>(
						"Low threshold filter",
						low_threshold_filter,
						{"Threshold value"})
					);

/////////////////////////////////////////////////////////////////
/////////////////The image filters///////////////////////////////

//----------------Crop images------------------------------------

int crop_images(const std::list<RealImageFFTW>& _in, std::list<RealImageFFTW>& _out, 
									std::string& _outstr,
							 		unsigned _bottom, unsigned _top, unsigned _left, unsigned _right)
{
	for(auto iter=_in.cbegin(), itend=_in.cend(); iter != itend; ++iter){
		const RealImageFFTW& img_in(*iter);
		if( (_left + _right >= static_cast<unsigned>(img_in.ancho())) 
				|| (_bottom + _top >= static_cast<unsigned>(img_in.alto())) ){
			std::cerr << "ERROR: crop_images: couldn't crop images" << std::endl;
			_outstr = "ERROR: crop_images: couldn't crop images";
			return -1;
		}
	}
		
	for(auto iter=_in.cbegin(), itend=_in.cend(); iter != itend; ++iter){
		const RealImageFFTW& img_in(*iter);
		_out.emplace_back(_bottom, img_in.alto() - _top -1, _left, img_in.ancho() - _right -1, img_in);
	}
	_outstr = "Cropped images. New images of " + std::to_string(_out.back().alto()) 
					+ " px tall and " + std::to_string(_out.back().ancho()) + " px wide.";
	return 0;
}

//--------------------Minus the average--------------------------

int minus_the_average(const std::list<RealImageFFTW>& _in, std::list<RealImageFFTW>& _out, 
								std::string& _outstr)
{
   if(_in.size()<2){ 
		std::cerr <<  "ERROR: Minus the average: not enough images" << std::endl;
      _outstr = "ERROR: Minus the average: not enough images";
      return -1; 
   }
   int count;
   std::list<RealImageFFTW>::const_iterator iter, itend=_in.cend(); 
   --itend;
   for(iter=_in.cbegin(), count=0; iter != itend; ++count){
      auto iterprev(iter);
      ++iter;
      bool err = *iterprev != *iter; // Here we are comparing two consecutive images
      if(err){
         std::cerr << "Minus the average: Error: trying to use incompatible images" + 
																std::to_string(count) + " " + std::to_string(count+1);
         _outstr = "Minus the average: Error: trying to use incompatible images" + 
																std::to_string(count) + " " + std::to_string(count+1);
         return -1; 
      }
   }
	++itend;
	// First we generate the average image
	int he = _in.front().alto();
	int wi = _in.front().ancho();
	int ty = _in.front().tipo();
	RealImageFFTW AvgImg(he, wi, ty);
			// We create the sum image
   for(iter=_in.cbegin(); iter != itend; ++iter){
      const RealImageFFTW& Img(*iter);
		for(int t=0; t < ty; ++t)
			for(int i=0; i < he; ++i)
				for(int j=0; j < wi; ++j)
					AvgImg.picelU(t, i, j) += Img.pixelU(t, i, j);
   }
			// and then we divide
	double sz = static_cast<double>(_in.size());
	for(int t=0; t < ty; ++t)
		for(int i=0; i < he; ++i)
			for(int j=0; j < wi; ++j)
				AvgImg.picelU(t, i, j) /= sz;
	// Now we substract the average image from the original images
	for(iter=_in.cbegin(); iter != itend; ++iter){
      const RealImageFFTW& ImgIni(*iter); //reference to original image
		_out.emplace_back(he, wi, ty); // generate new image in output list
		RealImageFFTW& ImgNew(_out.back()); // reference to the recently created image
		for(int t=0; t < ty; ++t)
			for(int i=0; i < he; ++i)
				for(int j=0; j < wi; ++j){
					double aux;
					if((aux = ImgIni.pixelU(t, i, j) - AvgImg.pixelU(t, i, j)) < 0.) 
						aux = 0.;
					ImgNew.picelU(t, i, j) = aux;
				}
   }
   _outstr = "Minus the average: Successful processing";
   return 0;
}

//----------------------Statistical order filter----------------------

void calculaPercentil_hilo(DobleFor& dblfor, const RealImageFFTW& img_in, RealImageFFTW& percentileImage,
								unsigned rad, unsigned per, int t)
{
	int i,j;
	int radius = rad, percentile = per;
	while(dblfor.getIndexes(i,j)){
		// Se crean los límites de la vecindad
		int i2_inf=0, i2_sup=img_in.alto(), j2_inf=0, j2_sup=img_in.ancho();
		if(i-radius > 0) 
			i2_inf = i-radius;
		if(i+radius+1 < img_in.alto()) 
			i2_sup = i+radius+1;
		if(j-radius > 0) 
			j2_inf = j-radius;
		if(j+radius+1 < img_in.ancho()) 
			j2_sup = j+radius+1;
		// Se meten los valores de la vecindad en un vector
		std::vector<double> auxvec;
		for(int i2=i2_inf; i2 < i2_sup; ++i2)
			for(int j2=j2_inf; j2 < j2_sup; ++j2)
				auxvec.push_back(img_in.pixelU(t, i2, j2));
		std::sort(auxvec.begin(), auxvec.end());
		// Se calcula el índice correspondiente al percentil
		unsigned index = (percentile * auxvec.size()) / 100;
		if(index == auxvec.size()) --index;
		// Se guarda el valor en la imagen
		percentileImage.picelU(t,i,j) = auxvec[index];
	}
}

int substract_percentile_image (const std::list<RealImageFFTW>& _in, std::list<RealImageFFTW>& _out, 
									std::string& _outstr,
							 		unsigned radius, unsigned percentile)
{
	if(percentile > 100) percentile = 100;

	for(auto iter=_in.cbegin(), itend=_in.cend(); iter != itend; ++iter){ // Advance one image at a time
		const RealImageFFTW& img_in(*iter);
		RealImageFFTW percentileImage(img_in.alto(), img_in.ancho(), img_in.tipo()); // La imagen donde se guardarán los percentiles
		for(int t=0; t < img_in.tipo(); ++t){
      	std::vector<std::thread> hilos;
      	DobleFor dfor(0,0, img_in.alto(), img_in.ancho());
      	int concurrency = std::thread::hardware_concurrency();
      	std::cout << concurrency << " threads" << std::endl;
   
      	for (int k=1; k < concurrency; ++k) // Desde k=1 para que sea concurrency-1 hilos extra
         	hilos.push_back(std::thread(calculaPercentil_hilo, std::ref(dfor), std::ref(img_in), 
										std::ref(percentileImage), radius, percentile, t));
      	// Se usa este hilo tmbn para calcular
      	calculaPercentil_hilo(dfor, img_in, percentileImage, radius, percentile, t);
      	for(auto& thr : hilos)
         	thr.join();
		}
		RealImageFFTW maxImage(img_in.alto(), img_in.ancho(), img_in.tipo()); // La imagen donde se guardará la imagen ``dilatada''
		for(int t=0; t < img_in.tipo(); ++t){
      	std::vector<std::thread> hilos;
      	DobleFor dfor(0,0, img_in.alto(), img_in.ancho());
      	int concurrency = std::thread::hardware_concurrency();
      	std::cout << concurrency << " threads" << std::endl;
   
      	for (int k=1; k < concurrency; ++k) // Desde k=1 para que sea concurrency-1 hilos extra
         	hilos.push_back(std::thread(calculaPercentil_hilo, std::ref(dfor), std::ref(percentileImage), 
										std::ref(maxImage), 1, 100, t));
      	// Se usa este hilo tmbn para calcular
      	calculaPercentil_hilo(dfor, percentileImage, maxImage, 1, 100, t);
      	for(auto& thr : hilos)
         	thr.join();
		}
		_out.emplace_back(img_in); // Creamos la imagen en la salida
		(_out.back()).substractImage(maxImage);// Se resta la imagen de percentiles
	}
	_outstr = "Percentile image of " + std::to_string(percentile) 
					+ " percentile and " + std::to_string(radius) + " radius, substracted.";
	return 0;
}

//--------------------------Low threshold--------------------------------------------

int low_threshold_filter(const std::list<RealImageFFTW>& _in, std::list<RealImageFFTW>& _out, 
									std::string& _outstr,
							 	float _threshold)
{
	if (0. > _threshold) _threshold = 0.; 
	if (1. < _threshold) _threshold = 1.; 

	for(auto iter=_in.cbegin(), itend=_in.cend(); iter != itend; ++iter){
		const RealImageFFTW& img_in(*iter);
		_out.emplace_back(img_in.alto(), img_in.ancho(), img_in.tipo());
		RealImageFFTW& img_out(_out.back());

		for(int t=0; t < img_in.tipo(); ++t)
			for(int i=0; i < img_in.alto(); ++i)
				for(int j=0; j < img_in.ancho(); ++j)
					if(img_in.pixelU(t, i, j) < _threshold)
						img_out.picelU(t, i, j) = 0.;
					else
						img_out.picelU(t, i, j) = img_in.pixelU(t, i, j);
	}
	_outstr = "Threshold filter: threshold value used: " + std::to_string(_threshold) + 
															"  Number of output images: " + std::to_string(_out.size());
	return 0;
}

//--------------------------Substract mask--------------------------

int substract_mask(const std::list<RealImageFFTW>& _in, std::list<RealImageFFTW>& _out, 
							std::string& _outstr,
							std::string &filename)
{
	
	return 0;
}
		//_out.emplace_back(img_in); // Creamos la imagen en la salida
		//(_out.back()).substractImage(maxImage);// Se resta la imagen de percentiles
