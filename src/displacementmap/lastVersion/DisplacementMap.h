// DisplacementMap.h by Didier Muñoz
// Version 1
//

#ifndef DISPLMAP
#define DISPLMAP

#include <vector>
#include <string>
#include <fstream>

// DisplacementMap

class DisplacementMap{
	private:
		unsigned int Width;
		unsigned int Height;
		std::vector<double> XValues;
		std::vector<double> YValues;
		double ZonesXSize;
		double ZonesYSize;
		double ZonesXSeparation;
		double ZonesYSeparation;
		double X_0;
		double Y_0;
		std::string Info;

	public:
		DisplacementMap(unsigned int _width, unsigned int _height, 
								double _zones_x_size, double _zones_y_size,
								double _zones_x_sep, double _zones_y_sep,
								double _x_0 = 0., double _y_0 = 0.) 
								:
								Width(_width), Height(_height),
								XValues(_width*_height), YValues(_width*_height),
								ZonesXSize(_zones_x_size), ZonesYSize(_zones_y_size),
								ZonesXSeparation(_zones_x_sep), ZonesYSeparation(_zones_y_sep),
								X_0(_x_0), Y_0(_y_0) 
								{}
			
		unsigned int width (void) const {return Width;}
		unsigned int height (void) const {return Height;}
		double x(unsigned int _column, unsigned int _row) const {return XValues[_row*width()+_column];}
		double y(unsigned int _column, unsigned int _row) const {return YValues[_row*width()+_column];}
		double& x(unsigned int _column, unsigned int _row) {return XValues[_row*width()+_column];}
		double& y(unsigned int _column, unsigned int _row) {return YValues[_row*width()+_column];}
		double xZonesSize (void) const {return ZonesXSize;}
		double yZonesSize (void) const {return ZonesYSize;}
		double xSeparation (void) const {return ZonesXSeparation;}
		double ySeparation (void) const {return ZonesYSeparation;}
		double x0 (void) const {return X_0;}
		double y0 (void) const {return Y_0;}
		const std::string& info(void) const {return Info;}

		void setInfoStr (const std::string& _str) { Info = _str;}

		void saveToStream(std::ofstream& _stream, double _scalefactor, double _deltat) const{
			_stream << "# Map dimensions:\n" << width() << " " << height() << "\n";
			_stream << "# Zones dimensions:\n" << xZonesSize() << " " << yZonesSize() << "\n";
			_stream << "# Distance between zones:\n" << xSeparation() << " " << ySeparation() << "\n";
			_stream << "# Map's Bottom-Leftmost corner:\n" << x0() << " " << y0() << "\n";
			_stream << "# Additional info:\n" << info() << "\n";
			_stream << "# Data:\n";
         for(unsigned j=0; j<height(); ++j){
            for(unsigned i=0; i<width(); ++i){
               _stream << x(i, j) * _scalefactor / _deltat << " ";
               _stream << y(i, j) * _scalefactor / _deltat << "  ";
            }
         _stream << "\n";
         }
		}
};

#endif

/*Cambios:
*/
