// imageCorrelation.cpp by Didier
// Version 6
// Esta biblioteca requiere de ImageFFTW

#include "Correlation.h"

////// Global variable with the information about the available algorithms

const std::tuple < 
					AlgorithmPIV<unsigned, unsigned, unsigned, unsigned, bool>
             , AlgorithmPIV<unsigned, unsigned, unsigned, unsigned, bool, bool> 
						> _AvailablePIVAlgorithms_(
					AlgorithmPIV<unsigned, unsigned, unsigned, unsigned, bool>( 
            		"Direct correlation",
             		basic_DirectCorr_maxAverage,
             		{"Zones Height","Zones Width","Vertical Separation","Horizontal Separation","Is Burst"} )
					,
      			AlgorithmPIV<unsigned, unsigned, unsigned, unsigned, bool, bool>( 
            		"Fourier Transform correlation",
             		basic_TFCorr_maxAverage,
             		{"Zones Height","Zones Width","Vertical Separation","Horizontal Separation","Is Burst", "Calculate average"} )
   				);

/////////////////Clases y funciones para los algoritmos de PIV///////////////////////////////

//>>>>>>>>>>>>>>>>Algoritmo básico<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//---------Funciones útiles

// Calculan cantidad de filas y columnas de zonas de interrogación.
int numZonasV(unsigned int sep_v, unsigned int img_alto)
{
   return sep_v==0?0: img_alto%sep_v==0 ? img_alto/sep_v : img_alto/sep_v + 1;
}

int numZonasH(unsigned int sep_h, unsigned int img_ancho)
{
   return sep_h==0 ? 0 : img_ancho%sep_h==0 ? img_ancho/sep_h : img_ancho/sep_h + 1;
}

//----Correlación directa

// Calcula el mapa de correlación de dos imágenes. En h se guarda la correlación.
void imageCorrelation(const RealImageFFTW &f, const RealImageFFTW &g, RealImageFFTW &h)
{
   if(f.tipo() != g.tipo()){
      std::cerr << "Error: correlation between images of different type"<<std::endl;
      return;
   }
   if(h.alto()!=f.alto()+g.alto()-1 || h.ancho() != f.ancho()+g.ancho()-1)
      h = RealImageFFTW(f.alto()+g.alto()-1, f.ancho()+g.ancho()-1, f.tipo());
   int desfal, desfan, desgal, desgan;
   int i,j,k,l,m,n,i1,j1, m1, n1, t;

   h.borrar();        // Se inicializa con 0's el mapa de correlación
   desfal=f.alto()-1; // desfal tendrá que ver con el límite vertical inferior de la zona
   desgal=1;          // desgal es el límite vertical de g 
   // Se inicia avanzando por filas 
   for(k=0; k<h.alto(); k++, desfal--, desgal++){
      if(desfal >=0){
         i=desfal; // desfal inicia siendo el límite vertical inferior de f
         j=0;
      }
      else{
         i=0;      // y cuando se hace negativo continua siendo el de g
         j=-desfal;
      }
      if(desgal > g.alto()) desgal=g.alto(); // el valor máximo de desgal es g.alto
      // Ya se prepararon los límites verticales
      desfan=f.ancho()-1; // El proceso es análogo con desfan y desgan
      desgan=1;
      // Se continúa avanzando por columnas
      for(l=0; l<h.ancho(); l++, desfan--, desgan++){
         if(desfan >=0){
            m=desfan; // desfan inicia siendo el límite horizontal inferior de f
            n=0;
         }
         else{
            m=0;     // y cuando se hace negativo continua siendo el de g
            n=-desfan;
         }
         if(desgan > g.ancho()) desgan=g.ancho();
         // Ya se prepararon los límites horizontales
         for(t=0; t<h.tipo(); t++)
            for(i1=i, j1=j; i1<f.alto() && j1<desgal; i1++, j1++)
               for(m1=m, n1=n; m1<f.ancho() && n1<desgan; m1++, n1++){
                  h.picelU(t, k, l)+=f.pixelU(t, i1, m1) * g.pixelU(t, j1, n1);
               }
      }
   }
}

// Obtiene el desplazamiento asociado al mapa de correlación obtenido con la función anterior
void desplazamiento(RealImageFFTW& cmap, int g_al, int g_an, double& dy, double& dx, int color)
{
   int y=0, x=0;
   int count=1;
   double maxval=0;
   int calto2 = cmap.alto() / 2;
   int cancho2 = cmap.ancho() / 2;
	int dalto = cmap.alto() - g_al;
	int dancho = cmap.ancho() - g_an;

   for(int i=dalto; i < g_al; ++i)
      for(int j=dancho; j<g_an; ++j)
         if(cmap.pixelU(color, i, j) > maxval){
           	maxval = cmap.pixelU(color, i, j);
         	count = 1;
            y = i-calto2;
            x = j-cancho2;
         }
         else if (cmap.pixelU(color, i, j) == maxval){
           	++count;
           	y += i-calto2;
          	x += j-cancho2;
         }
   dy = ((double)y)/((double)count);
   dx = ((double)x)/((double)count);
}

// Función para los hilos encargada de procesar las zonas de interrogación. Usada en la siguiente
void basic_calcDesplHilo(DobleFor& dblfor, const RealImageFFTW& ImgT0, const RealImageFFTW& ImgT1,
                     DisplacementMap& _desplazamientos,
                     const int& zonalto, const int& zonancho, const int& sepv, const int& seph)
{
   int i,j;
   int gdis_al = zonalto/2;
   int gdis_an = zonancho/2;
   RealImageFFTW f(zonalto, zonancho, ImgT0.tipo());
   RealImageFFTW g(zonalto+2*gdis_al, zonancho+2*gdis_an, ImgT0.tipo());
   RealImageFFTW h(2*zonalto+2*gdis_al-1, 2*zonancho+2*gdis_an-1, ImgT0.tipo()); //h será los mapas de correlación
   void (*corrMapFunc)(const RealImageFFTW &, const RealImageFFTW &, RealImageFFTW &);
   void (*displacementFunc)(RealImageFFTW&, int, int, double&, double&, int);

//Aquí se puede modificar si se desean utilizar otras funciones de correlación y desplazamiento
   //Función de correlación
   corrMapFunc = imageCorrelation;
   //Función que obtiene desplazamiento
   displacementFunc = desplazamiento;
/*--------------------------------------------------------------------------------------------*/

   while(dblfor.getIndexes(i,j)){
      // Se copian las zonas de interrogación en nuevas imágenes con copiaE
      f.copiaE(i*sepv, i*sepv+zonalto, j*seph, j*seph+zonancho, ImgT0);
      g.copiaE(i*sepv-gdis_al, i*sepv+zonalto+gdis_al, j*seph-gdis_an, j*seph+zonancho+gdis_an, ImgT1);
      corrMapFunc(f, g, h);       // Mapa de Correlación para todos los colores
		double dy_accumulated=0., dx_accumulated=0., dy_new = 0., dx_new=0.;
		int t;
      for(t=0; t<ImgT0.tipo(); t++){ // Obtener desplazamiento para cada color y promediarlos
         displacementFunc(h, zonalto+2*gdis_al, zonancho+2*gdis_an, dy_new, dx_new, t);
			dy_accumulated += dy_new;
			dx_accumulated += dx_new;
		}
		_desplazamientos.x(j, i) = dx_accumulated / static_cast<double>(t);
		_desplazamientos.y(j, i) = dy_accumulated / static_cast<double>(t);
   }
}

// Función que calcula el mapa de desplazamientos
int basic_DirectCorr_maxAverage(const std::list<RealImageFFTW>& _in, std::list<DisplacementMap>& _out, std::string& _outstr,
                 unsigned int zonalto, unsigned int zonancho, unsigned int sepv, unsigned int seph, bool esrafaga)
{
	// See if the images are compatible
   if(_in.size() < 2){
      _outstr = "Error: not enough images";
      return -1;
   }
	if(zonalto == 0 || zonancho == 0){
      _outstr = "ERROR: Direct Correlation: wrong zones height or width";
      return -1;
	}
	// Prepare some variables
	int count;
	std::list<RealImageFFTW>::const_iterator iter, itend=_in.cend(); 
	// Second check of images compatibility
	if(esrafaga){
		--itend;
		for(iter=_in.cbegin(), count=0; iter != itend; ++count){
			auto iterprev(iter);
			++iter;
			bool err = *iterprev != *iter; // Here we are comparing two consecutive images
			if(err){
				_outstr = "Error: trying to use incompatible images" + std::to_string(count) + " " + std::to_string(count+1);
				return -1;
			}
		}
	}
	else{
		if(_in.size() % 2 != 0){
      	_outstr = "Error: not enough images (not even number of images)";
      	return -1;
		}
		for(iter=_in.cbegin(), count=0; iter != itend; ++count, ++iter){
			auto iterprev(iter);
			++iter;
			bool err = *iterprev != *iter; // Here we are comparing two consecutive images
			if(err){
				_outstr = "Error: trying to use incompatible images" + std::to_string(count) + " " + std::to_string(count+1);
				return -1;
			}
		}
	}

	for(iter=_in.cbegin(), count=0; iter != itend; ++count){
		const RealImageFFTW& ImgT0(*iter);
		const RealImageFFTW& ImgT1(*(++iter));
  		int numzonas_v = numZonasV(sepv, ImgT0.alto());
  		int numzonas_h = numZonasH(seph, ImgT0.ancho());

   	_out.emplace_back(numzonas_h, numzonas_v, zonancho, zonalto, seph, sepv);
   	DisplacementMap& Desplazamientos(_out.back());
     
   	std::vector<std::thread> hilos;
   	DobleFor dfor(0,0, numzonas_v, numzonas_h);
   	int concurrency = std::thread::hardware_concurrency();
   	std::cout << concurrency << " threads" << std::endl;

   	for (int k=1; k < concurrency; ++k) // Desde k=1 para que sea concurrency-1 hilos extra
      	hilos.push_back(std::thread(basic_calcDesplHilo, std::ref(dfor), std::ref(ImgT0), std::ref(ImgT1), std::ref(Desplazamientos),
                              std::ref(zonalto), std::ref(zonancho), std::ref(sepv), std::ref(seph)));
   	// Se usa este hilo también para calcular
   	basic_calcDesplHilo(dfor, ImgT0, ImgT1, Desplazamientos, zonalto, zonancho, sepv, seph);

   	for(auto& th : hilos)
      	th.join();

		if(!esrafaga) ++iter;
	}
	_outstr = "Succesful processing: " + std::to_string(count) + " displacement maps";
   return 0;
}

//----Correlación con transformada de Fourier

// Ésta función se provee para claridad acerca de cómo funciona el algoritmo.
// Calcula el mapa de correlación que calcula el algoritmo de correlación directa, pero usando FFTW
void example_imageCorrelationFFTW(const RealImageFFTW &f, const RealImageFFTW &g, RealImageFFTW &h)
{
   if(f.tipo() != g.tipo()){
      std::cerr << "Error: FFTW correlation between images of different type"<<std::endl;
      return;
   }

   // Se crean imágenes con tamaño adecuado para obtener el mismo mapa de correlación
   // Es decir, se le agregan suficientes ceros a las imágenes para obtener el mapa
   TransFourierR2C tf(f.alto()+g.alto()-1, f.ancho()+g.ancho()-1, f.tipo());
   TransFourierR2C tg(f.alto()+g.alto()-1, f.ancho()+g.ancho()-1, f.tipo());
   TransFourierC2R tifg(f.alto()+g.alto()-1, f.ancho()+g.ancho()-1, f.tipo());

   tf.entrada()->borrar();   
   tg.entrada()->borrar();   
   // Se copian las imágenes a trabajar a las imágenes internas de TransFourier
   for(int t=0; t<f.tipo(); t++)
   for(int i=0; i<f.alto(); i++)
      for(int j=0; j<f.ancho(); j++)
         tf.entrada()->picelU(t,i,j)=f.pixelU(t,i,j);
   for(int t=0; t<g.tipo(); t++)
   for(int i=0; i<g.alto(); i++)
      for(int j=0; j<g.ancho(); j++)
         tg.entrada()->picelU(t,i,j)=g.pixelU(t,i,j);

   tf.calcular();
   tg.calcular();
   // Se prepara la transformada inversa
   tifg.entrada()->multConj(*tf.salida(), *tg.salida());
   
   tifg.calcular();
   // Se copia la imagen a la variable pasada para recibir el mapa de correlación
   h = *tifg.salida();
return;
}

/* Función parecida a la anterior pero que recibe TransFourier en lugar de imágenes. Espera recibir
las TransFourier adecuadamente preparadas, es decir, con las imágenes del mismo tamaño y tipo, y cargadas 
en las entradas. El resultado queda en la salida de tifg.
Tiene la ventaja de que se pueden seguir reutilizando las TransFourier y de que no hay que copiar 
las imágenes
*/
void imageCorrelationFFTW(TransFourierR2C &tf, TransFourierR2C &tg, TransFourierC2R &tifg)
{
   tf.calcular();
   tg.calcular();
   // Se prepara la transformada inversa
   tifg.entrada()->multConj(*tf.salida(), *tg.salida());
   
   tifg.calcular();
return;
}

// Extrae el desplazamiento promedio de un mapa de correlación obtenido con la función anterior en caso de que ambas funciones hayan sido extendidas con ceros al final
void desplazamientoFFTW(RealImageFFTW& cmap, int g_al, int g_an, double& dy, double& dx, int color)
{
   int y=0, x=0; 
   int i_dc=0, j_dc=0;
   int count=1;
   double maxval=0;
   int calto = cmap.alto() / 2;
   int cancho = cmap.ancho() / 2;
	int dalto = cmap.alto() - g_al;
	int dancho = cmap.ancho() - g_an;

   for(int i=0; i<cmap.alto(); i++)
      for(int j=0; j<cmap.ancho(); j++)
         if(cmap.pixelU(color, i, j) > maxval){
            if(i < g_al){
					i_dc = i + dalto;
            	if(j < g_an){
						j_dc = j + dancho;
			   		if(i_dc>=dalto && j_dc>=dancho && i_dc<g_al && j_dc<g_an){
            			maxval = cmap.pixelU(color, i, j);
            			count = 1;
               		y = i_dc - calto;
               		x = j_dc - cancho;
						}
					}
            	else{
						j_dc = j - g_an;
			   		if(i_dc>=dalto && j_dc>=dancho && i_dc<g_al && j_dc<g_an){
            			maxval = cmap.pixelU(color, i, j);
            			count = 1;
               		y = i_dc - calto;
               		x = j_dc - cancho;
						}
					}
				}
            else{
					i_dc = i - g_al;
            	if(j < g_an){
						j_dc = j + dancho;
			   		if(i_dc>=dalto && j_dc>=dancho && i_dc<g_al && j_dc<g_an){
            			maxval = cmap.pixelU(color, i, j);
            			count = 1;
               		y = i_dc - calto;
               		x = j_dc - cancho;
						}
					}
            	else{
						j_dc = j - g_an;
			   		if(i_dc>=dalto && j_dc>=dancho && i_dc<g_al && j_dc<g_an){
            			maxval = cmap.pixelU(color, i, j);
            			count = 1;
               		y = i_dc - calto;
               		x = j_dc - cancho;
						}
					}
				}
         }
         else if (cmap.pixelU(color, i, j) == maxval){
            if(i < g_al){
					i_dc = i + dalto;
            	if(j < g_an){
						j_dc = j + dancho;
                  if(i_dc>=dalto && j_dc>=dancho && i_dc<g_al && j_dc<g_an){
            			count++;
               		y += i_dc - calto;
               		x += j_dc - cancho;
						}
					}
            	else{
						j_dc = j - g_an;
						if(i_dc>=dalto && j_dc>=dancho && i_dc<g_al && j_dc<g_an){
            			count++;
               		y += i_dc - calto;
               		x += j_dc - cancho;
						}
					}
				}
            else{
					i_dc = i - g_al;
            	if(j < g_an){
						j_dc = j + dancho;
						if(i_dc>=dalto && j_dc>=dancho && i_dc<g_al && j_dc<g_an){
            			count++;
               		y += i_dc - calto;
               		x += j_dc - cancho;
						}
					}
            	else{
						j_dc = j - g_an;
						if(i_dc>=dalto && j_dc>=dancho && i_dc<g_al && j_dc<g_an){
            			count++;
               		y += i_dc - calto;
               		x += j_dc - cancho;
						}
					}
				}
         }
   dy = ((double)y)/((double)count);
   dx = ((double)x)/((double)count);
}

// Función para los hilos encargada de procesar las zonas de interrogación. Usada en la siguiente
void basic_calcDesplTFHilo(DobleFor& dblfor, const RealImageFFTW& ImgT0, const RealImageFFTW& ImgT1, 
                     DisplacementMap& _desplazamientos,
                    const int& zonalto, const int& zonancho, const int& sepv, const int& seph)
{
   int i, j;
   int gdis_al = zonalto/2;
   int gdis_an = zonancho/2;
   TransFourierR2C tf(2*zonalto+2*gdis_al-1, 2*zonancho+2*gdis_an-1, ImgT0.tipo());
   TransFourierR2C tg(2*zonalto+2*gdis_al-1, 2*zonancho+2*gdis_an-1, ImgT0.tipo());
   TransFourierC2R tifg(2*zonalto+2*gdis_al-1, 2*zonancho+2*gdis_an-1, ImgT0.tipo());
   void (*corrMapFunc)(TransFourierR2C &, TransFourierR2C &, TransFourierC2R &);
   void (*displacementFunc)(RealImageFFTW&, int, int, double&, double&, int);

// Aquí se puede modificar si se desean utilizar otras funciones de correlación y desplazamiento
   // Función de correlación
   corrMapFunc = imageCorrelationFFTW;
   //Función que obtiene desplazamiento
   displacementFunc = desplazamiento;
/*--------------------------------------------------------------------------------------------*/

   while(dblfor.getIndexes(i,j)){
     	// Se copian las zonas de interrogación en las entradas de las TransFourier con copiaE
		tf.entrada()->subImagen(0, 0, i*sepv, j*seph, i*sepv+zonalto, j*seph+zonancho, ImgT0);
		tg.entrada()->subImagen(zonalto-1,zonancho-1,i*sepv-gdis_al, j*seph-gdis_an, 
											i*sepv+zonalto+gdis_al, j*seph+zonancho+gdis_an, ImgT1);
     	corrMapFunc(tf, tg, tifg); // Mapa de correlación para todos los colores
		double dy_accumulated=0., dx_accumulated=0., dy_new = 0., dx_new=0.;
		int t;
     	for(t=0; t<ImgT0.tipo(); t++){
        	displacementFunc(*tifg.salida(), zonalto+2*gdis_al, zonancho+2*gdis_an, dy_new, dx_new, t);
			dy_accumulated += dy_new;
			dx_accumulated += dx_new;
		}
		_desplazamientos.x(j, i) = dx_accumulated / static_cast<double>(t);
		_desplazamientos.y(j, i) = dy_accumulated / static_cast<double>(t);
   }
}

// Función que calcula el mapa de desplazamientos usando transformada de Fourier
int basic_TFCorr_maxAverage(const std::list<RealImageFFTW>& _in, std::list<DisplacementMap>& _out, std::string& _outstr,
                 unsigned int zonalto, unsigned int zonancho, unsigned int sepv, unsigned int seph, bool esrafaga, bool promedio)
{   
	// See if the images are compatible
   if(_in.size() < 2){
      _outstr = "Error: not enough images";
      return -1;
   }
	if(zonalto == 0 || zonancho == 0){
      _outstr = "ERROR: Fourier Transform Correlation: wrong zones height or width";
      return -1;
	}

	int count;
	std::list<RealImageFFTW>::const_iterator iter, itend=_in.cend(); 
	// Second compatibility check
	if(esrafaga){
		--itend;
		for(iter=_in.cbegin(), count=0; iter != itend; ++count){
			auto iterprev(iter);
			++iter;
			bool err = *iterprev != *iter; // Here we are comparing two consecutive images
			if(err){
				_outstr = "Error: trying to use incompatible images" + std::to_string(count) + " " + std::to_string(count+1);
				return -1;
			}
		}
	}
	else{
		if(_in.size() % 2 != 0){
      	_outstr = "Error: not enough images (not even number of images)";
      	return -1;
		}
		for(iter=_in.cbegin(), count=0; iter != itend; ++count, ++iter){
			auto iterprev(iter);
			++iter;
			bool err = *iterprev != *iter; // Here we are comparing two consecutive images
			if(err){
				_outstr = "Error: trying to use incompatible images" + std::to_string(count) + " " + std::to_string(count+1);
				return -1;
			}
		}
	}
	for(iter=_in.cbegin(), count=0; iter != itend; ++count){
		const RealImageFFTW& ImgT0(*iter);
		const RealImageFFTW& ImgT1(*(++iter));
  		int numzonas_v = numZonasV(sepv, ImgT0.alto());
  		int numzonas_h = numZonasH(seph, ImgT0.ancho());

   	_out.emplace_back(numzonas_h, numzonas_v, zonancho, zonalto, seph, sepv);
   	DisplacementMap& Desplazamientos(_out.back());

   	std::vector<std::thread> hilos;
   	DobleFor dfor(0,0, numzonas_v, numzonas_h);
   	int concurrency = std::thread::hardware_concurrency();
   	std::cerr << concurrency << " threads" << std::endl;
   
   	for (int k=1; k < concurrency; ++k) // Desde k=1 para que sea concurrency-1 hilos extra
      	hilos.push_back(std::thread(basic_calcDesplTFHilo, std::ref(dfor), std::ref(ImgT0), std::ref(ImgT1), std::ref(Desplazamientos),
                              std::ref(zonalto), std::ref(zonancho), std::ref(sepv), std::ref(seph)));

   	// Se usa este hilo tmbn para calcular
   	basic_calcDesplTFHilo(dfor, ImgT0, ImgT1, Desplazamientos, zonalto, zonancho, sepv, seph);

   	for(auto& th : hilos)
      	th.join();

		if(!esrafaga) ++iter;
	}
	// if average is calculated:
	if(promedio && _out.size() > 1){
		DisplacementMap avg(_out.front().width(), _out.front().height(), _out.front().xZonesSize(), _out.front().yZonesSize(),
									_out.front().xSeparation(), _out.front().ySeparation());
		DisplacementMap avg2(avg.width(), avg.height(), avg.xZonesSize(), avg.yZonesSize(),
									avg.xSeparation(), avg.ySeparation());
		DisplacementMap var(avg.width(), avg.height(), avg.xZonesSize(), avg.yZonesSize(),
									avg.xSeparation(), avg.ySeparation());
		for(unsigned i=0; i< avg.width(); ++i)
			for(unsigned j=0; j< avg.height(); ++j){
				var.x(i,j) = avg.x(i,j) = avg2.x(i,j) = 0.;
				var.y(i,j) = avg.y(i,j) = avg2.y(i,j) = 0.;
			}
			
		for(std::list<DisplacementMap>::iterator it=_out.begin(); it != _out.end(); ++it){
			for(unsigned i=0; i< avg.width(); ++i)
				for(unsigned j=0; j< avg.height(); ++j){
					avg.x(i,j) += it->x(i,j);
					avg.y(i,j) += it->y(i,j);
				}
		}
		for(unsigned i=0; i< avg.width(); ++i)
			for(unsigned j=0; j< avg.height(); ++j){
				avg.x(i,j) /= static_cast<double>(_out.size());
				avg.y(i,j) /= static_cast<double>(_out.size());
			}
		// Ya se tiene el promedio
		// Se calcula la varianza

		for(std::list<DisplacementMap>::iterator it=_out.begin(); it != _out.end(); ++it){
			for(unsigned i=0; i< avg.width(); ++i)
				for(unsigned j=0; j< avg.height(); ++j){
					double aux = avg.x(i,j) - it->x(i,j); 
					double auy = avg.y(i,j) - it->y(i,j); 
					var.x(i,j) += aux*aux;
					var.y(i,j) += auy*auy;
				}
		}
		for(unsigned i=0; i< var.width(); ++i)
			for(unsigned j=0; j< var.height(); ++j){
				var.x(i,j) = var.x(i,j) / static_cast<double>(_out.size()-1);
				if(var.x(i,j) < 1.) var.x(i,j) = 1.;
				var.y(i,j) = var.y(i,j) / static_cast<double>(_out.size()-1);
				if(var.y(i,j) < 1.) var.y(i,j) = 1.;
			}
		// Ya se tiene la varianza
		//Se calcula un segundo promedio considerando varianza
		std::vector<std::vector<long> > avg2nums(avg.width(), std::vector<long>(avg.height(),0));
		for(std::list<DisplacementMap>::iterator it=_out.begin(); it != _out.end(); ++it){
			for(unsigned i=0; i< avg.width(); ++i)
				for(unsigned j=0; j< avg.height(); ++j){
					double aux = avg.x(i,j) - it->x(i,j);
					double auy = avg.y(i,j) - it->y(i,j);
					if((aux*aux < var.x(i,j)) && (auy*auy < var.y(i,j))){
						avg2.x(i,j) += it->x(i,j);
						avg2.y(i,j) += it->y(i,j);
						++avg2nums[i][j];
					}
				}
		}
		for(unsigned i=0; i< avg.width(); ++i)
			for(unsigned j=0; j< avg.height(); ++j){
				if(avg2nums[i][j] != 0) {
					avg2.x(i,j) /= static_cast<double>(avg2nums[i][j]);
					avg2.y(i,j) /= static_cast<double>(avg2nums[i][j]);
				}
			}

		_out.clear();
		_out.push_back(avg2);
	}

	_outstr = "Successful processing: " + std::to_string(_out.size()) + " displacement maps";
   return 0;
}



//>>>>>>>>>>>>>>>>>>>>>>>>Aquí pueden ir más algoritmos<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


