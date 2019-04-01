// ImageFilters.h by Didier Muñoz
// Version 1
// Esta biblioteca requiere de ImageFFTW DobleFor PIValgorithm

#ifndef IMAGEFILTERS
#define IMAGEFILTERS

#include <list>
#include <string>
#include <thread>
#include <tuple>
#include "ImageFFTW.h"
#include "DobleFor.h"
#include "PIValgorithm.h"

template <typename... Ts> 
using ImageFilter = PIValgorithm <std::list<RealImageFFTW>, std::list<RealImageFFTW>, Ts...>;


///////////////Global variable declaration to contain the algorithms

extern const std::tuple <    ImageFilter<unsigned,unsigned,unsigned,unsigned>
                             , ImageFilter<>
									  , ImageFilter<unsigned,unsigned>
									  , ImageFilter<float>
                        >   
            _AvailableImageFilters_;


///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////Image filters and transformations/////////////////////////////////

//>>>>>>>>>>>>>>Crop images

int crop_images(const std::list<RealImageFFTW>& _in, std::list<RealImageFFTW>& _out, std::string& _outstr,
                           unsigned _bottom, unsigned _top, unsigned _left, unsigned _right);

//>>>>>>>>>>>>>>Minus the average filter

int minus_the_average(const std::list<RealImageFFTW>&, std::list<RealImageFFTW>&, std::string& _outstr);

//>>>>>>>>>>>>>>Substract percentile image<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

int substract_percentile_image (const std::list<RealImageFFTW>& _in, std::list<RealImageFFTW>& _out,
                            std::string& _outstr,
                            unsigned radius, unsigned percentile);

//>>>>>>>>>>>>>>ThresholdFilter

int low_threshold_filter(const std::list<RealImageFFTW>&, std::list<RealImageFFTW>&, std::string& _outstr, 
									float _threshold);


#endif

/*Cambios:
*/
