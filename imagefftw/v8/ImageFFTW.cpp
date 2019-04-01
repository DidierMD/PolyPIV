// ImageFFTW.cpp by Didier
// version 8
// Esta biblioteca requiere de FFTW y de FreeImage


#include "ImageFFTW.h"

////////////////////////////////Métodos de ImageFFTW///////////////////////////////////////////////
ImageFFTW :: ImageFFTW(void){
    Alto = 0;
    Ancho = 0;
    Tipo = 0;
}

ImageFFTW :: ImageFFTW(int h, int w, int t){
    Alto = h;
    Ancho = w;
    Tipo = t;
}

std::ostream& operator << (std::ostream& os, const ImageFFTW& img)
{
if(img.clase() == REAL){
  const RealImageFFTW &aux = (RealImageFFTW&)img;
  switch(img.tipo()){
  case GREY:
    for (int i=0; i<aux.alto(); i++){
        for ( int j=0; j<aux.ancho(); j++){
        os << aux.pixelU(BRIGHT,i,j) << "; ";
        }
    os << "\n";
    }
  break;
  case GREYDM:
    for (int i=0; i<aux.alto(); i++){
        for ( int j=0; j<aux.ancho(); j++){
        os << aux.pixelU(BRIGHT,i,j) << "," << aux.pixelU(BRIGHT + 1,i,j) << "; ";
        }
    os << "\n";
    }
  break;
  case RGB:
    for (int i=0; i<aux.alto(); i++){
        for ( int j=0; j<aux.ancho(); j++){
        os << aux.pixelU(RED,i,j) <<" "<< aux.pixelU(GREEN,i,j) <<" "<< aux.pixelU(BLUE,i,j) << "; ";
        }
    os << "\n";
    }
  break;
  default:
  std::cerr << "Error: '<<' does not support REAL image type" << std::endl;
  break;
  }
}
else if(img.clase() == COMPLEX){
  const ComplexImageFFTW &aux = (ComplexImageFFTW&)img;
  switch(img.tipo()){
  case GREY:
    for (int i=0; i<aux.alto(); i++){
        for ( int j=0; j<aux.ancho(); j++){
        os << real(aux.pixelU(BRIGHT,i,j)) << "," << imag(aux.pixelU(BRIGHT,i,j)) << "; ";
        }
    os << "\n";
    }
  break;
  case RGB:
    for (int i=0; i<aux.alto(); i++){
        for ( int j=0; j<aux.ancho(); j++){
        os << real(aux.pixelU(RED,i,j)) << "," << imag(aux.pixelU(RED,i,j)) <<" "
           << real(aux.pixelU(GREEN,i,j)) << "," << imag(aux.pixelU(GREEN,i,j)) <<" "
           << real(aux.pixelU(BLUE,i,j)) << "," << imag(aux.pixelU(BLUE,i,j)) <<" "
           << "; ";
        }
    os << "\n";
    }
  break;
  default:
  std::cerr << "Error: '<<' does not support COMPLEX image type" << std::endl;
  break;
  }
}
return os;
}

bool operator == (const ImageFFTW& izq, const ImageFFTW& der)
{
   return izq.clase() == der.clase() && izq.tipo() == der.tipo() && izq.alto() == der.alto()
            && izq.ancho() == der.ancho();
}

bool operator != (const ImageFFTW& izq, const ImageFFTW& der)
{
   return !(izq == der);
}

////////////////////////////////Métodos de RealImageFFTW///////////////////////////////////////////

RealImageFFTW :: RealImageFFTW(void) : ImageFFTW()
{
    Datos = NULL;
}

RealImageFFTW :: RealImageFFTW(const RealImageFFTW& der)
{
   setAlto(der.alto());
   setAncho(der.ancho());
   setTipo(der.tipo());
   Datos = (double**)malloc(sizeof(double *) * tipo());
   for(int i=0; i < tipo(); i++){
      Datos [i] = (double *)fftw_malloc(sizeof(double) * arraySize());
      for (int k=0; k<arraySize(); k++)
         Datos[i][k] = der(i)[k];
   }
}

RealImageFFTW :: RealImageFFTW(int h, int w, int t) : ImageFFTW(h,w,t)
{
    Datos=(double**)malloc(sizeof(double *) * t);
    for(int i=0; i < t; i++)
        Datos [i] = (double *)fftw_malloc(sizeof(double) * arraySize());
    // Se utiliza fftw_malloc porque se recomienda por cuestiones de eficiencia para fftw
	borrar();
}

RealImageFFTW:: RealImageFFTW(const char* nombre, const int formato, const int type) : ImageFFTW()
{
    
    FIBITMAP *img, *img1;
    int x;

    switch (formato){
    case FIF_JPEG:
        img = FreeImage_Load(FIF_JPEG, nombre, JPEG_ACCURATE);
        break;
    case FIF_PNG:
        img = FreeImage_Load(FIF_PNG, nombre, 0);
        break;
    case FIF_TIFF:
        img = FreeImage_Load(FIF_TIFF, nombre, 0);
        break;
    case FIF_BMP:
        img = FreeImage_Load(FIF_BMP, nombre, 0);
        break;
    default:
        std::cerr << "Error loading image: image format not supported\n";
        Datos = NULL;
        return;
        break;
    }
    if (NULL == img){
      std::cerr<<"Error loading image: could not load the image (Is the image filename correct?)"<<std::endl;
        Datos = NULL;
        return ;
    } 
    switch (type){
    case GREY:
        if (NULL == (img1 = FreeImage_ConvertToGreyscale(img))){
            std::cerr << "Greyscale conversion: Error! bitmap type not supported" << std::endl;
            FreeImage_Unload(img);
            Datos = NULL;
            return;
            }
        break;
    case RGB:
        if (NULL == (img1 = FreeImage_ConvertTo24Bits(img))){
            std::cerr << "24 Bits conversion: Error! bitmap type not supported" << std::endl;
            FreeImage_Unload(img);
            Datos = NULL;
            return ;
            } 
        break;
    default:
        std::cerr << "Error! Tipo de imagen no soportado" << std::endl;
        FreeImage_Unload(img);
        Datos = NULL;
        return ;
        break;
    }
    FreeImage_Unload(img);
    setAlto(FreeImage_GetHeight(img1));
    setAncho (FreeImage_GetWidth(img1));
    setTipo(type);
    Datos=(double**)malloc(sizeof(double *) * tipo());
    for(int i=0; i < tipo(); i++)
        Datos[i] = (double *)fftw_malloc(sizeof(double) * arraySize());
    BYTE *bits;
    switch (tipo()){
    case GREY:
        for(int h = 0; h < alto(); h++) {
            bits = (BYTE *)FreeImage_GetScanLine(img1, h);
            for(int w = 0; w < ancho(); w++) {
                Datos[0][h*ancho() + w] = ((double)bits[w]) /255.;
            }
        }
    break;
    case RGB:
        for(int h = 0; h < alto(); h++) {
            bits = (BYTE *)FreeImage_GetScanLine(img1, h);
            x = 0;
            for(int w = 0 ; w < ancho()*3; w = w+3) {
                Datos[RED][h*ancho() + x] = ((double)bits[w+FI_RGBA_RED])/255.;
                Datos[GREEN][h*ancho() + x] = ((double)bits[w+FI_RGBA_GREEN])/255.;
                Datos[BLUE][h*ancho() + x] = ((double)bits[w+FI_RGBA_BLUE])/255.;
                x++;
            }
        }
    break;
    }
FreeImage_Unload(img1);
}

RealImageFFTW ::RealImageFFTW(int h0, int h1, int w0, int w1, const RealImageFFTW& img):ImageFFTW()
{
   if(h0 < 0 || h0 >= img.alto() || h1 < 0 || h1 >= img.alto() || w0 < 0 || w0 >= img.ancho() 
      || w1 < 0 || w1 >= img.ancho() || h1<h0 || w1<w0){
        std::cerr << "Error al crear una imagen real a partir de otra imagen real" << std::endl;
        Datos = NULL;
        return;
    }

    setAlto(h1 - h0 + 1);
    setAncho(w1 - w0 +1);
    setTipo(img.tipo());

    Datos=(double**)malloc(sizeof(double *) * tipo());
    
    for(int k=0; k < tipo(); k++){
        Datos [k] = (double *)fftw_malloc(sizeof(double) * arraySize()); 
        for(int i=h0, i1=0; i <= h1 ; i++, i1++)
            for(int j=w0, j1=0; j <= w1 ; j++, j1++)
                Datos[k][i1 * ancho() + j1] = img.pixelU(k,i,j);
    } 
}

// Ésta función no modifica el tamaño ni el tipo de la imagen
// Los extremos superiores no son incluyentes
void RealImageFFTW::copia(int h0, int h1, int w0, int w1, const RealImageFFTW& Img)
{
   if(tipo() != Img.tipo()){
      std::cerr << "Error: trying to copy images of different type"<<std::endl;
      return;
   }
   int i,j,m,n,i1,j1, m1, n1, t;

      if(h0 >= 0){
         i=0; // i va a ser el índice que recorra esta imagen
         j=h0; // j va a ser el índice que recorra Img
      }
      else{
         i=-h0;
         j=0;
      }
      if(h1 > Img.alto()) h1=Img.alto();
      // Ya se prepararon los límites verticales
      if(w0 >=0){
         m=0;
         n=w0;
      }
      else{
         m=-w0;
         n=0;
      }
      if(w1 > Img.ancho()) w1=Img.ancho();
      // Ya se prepararon los límites horizontales
      for(t=0; t<tipo(); t++)
         for(i1=i, j1=j; i1<alto() && j1<h1; i1++, j1++)
            for(m1=m, n1=n; m1<ancho() && n1<w1; m1++, n1++)
               picelU(t, i1, m1) = Img.pixelU(t, j1, n1);
            
   return;
}

// Igual a la anterior pero poniendo en ceros donde no se haya copiado nada
void RealImageFFTW::copiaE(int h0, int h1, int w0, int w1, const RealImageFFTW& Img)
{
   if(tipo() != Img.tipo()){
      std::cerr << "Error: trying to copy images of different type"<<std::endl;
      return;
   }
   int i,j,m,n;          // Límites de la imagen
   int i1,j1, m1, n1, t; // Índices que se mueven
   int zero_h_beg, zero_h_end, zero_w_beg, zero_w_end; // Índices para borrar las partes no usadas

      if(h0 >= 0){
         i=0; // i1 va a ser el índice que recorra esta imagen
         j=h0; // j1 va a ser el índice que recorra Img
      }
      else{
         i=-h0;
         j=0;
      }
      if(h1 > Img.alto()) h1=Img.alto();
      // Ya se prepararon los límites verticales
      if(w0 >=0){
         m=0; // m1 recorre esta imagen
         n=w0; //n1 recorre Img
      }
      else{
         m=-w0;
         n=0;
      }
      if(w1 > Img.ancho()) w1=Img.ancho();
      // Ya se prepararon los límites horizontales

      // Se copia la imagen
      for(t=0; t<tipo(); t++)
         for(i1=i, j1=j; i1<alto() && j1<h1; i1++, j1++)
            for(m1=m, n1=n; m1<ancho() && n1<w1; m1++, n1++)
               picelU(t, i1, m1) = Img.pixelU(t, j1, n1);

      // Aquí se van a borrar las partes que no se utilizaron
      for(t=0; t<tipo(); t++){
      // Empezamos con la parte inferior vertical
         for(zero_h_beg=0; zero_h_beg < i; zero_h_beg++)
            for(int aux = ancho()-1; aux >= 0; aux--)
               picelU(t, zero_h_beg, aux) = 0.;
      // Continuamos con la parte inferior horizontal
         for(zero_w_beg=0; zero_w_beg < m; zero_w_beg++)
            for(int aux = alto()-1; aux >= zero_h_beg; aux--)
               picelU(t, aux, zero_w_beg) = 0.;
      // Seguimos con la parte superior horizontal
         for(zero_w_end = ancho()-1; zero_w_end >= m1; zero_w_end--)
            for(int aux = alto()-1; aux >= zero_h_beg; aux--)
               picelU(t, aux, zero_w_end) = 0.;
      // Finalizamos con la parte superior vertical
         for(zero_h_end = alto()-1; zero_h_end >= i1; zero_h_end--)
            for(int aux = zero_w_beg; aux <= zero_w_end; aux++)
               picelU(t, zero_h_end, aux) = 0.;
      }

   return;
}

void RealImageFFTW::subImagen(int x0, int y0, int i0, int j0, int i1, int j1, const RealImageFFTW& img)
{
   if(tipo() != img.tipo()){
      std::cerr << "ERROR: subImagen: trying to copy images of different type"<<std::endl;
      return;
   }
	// Se definen las coordenadas locales de la región tomando en cuenta los límites de la img
	int i0_local=0, j0_local=0; // Coordenadas inferiores locales
	if(i0 < 0) i0_local = -i0;
	if(j0 < 0) j0_local = -j0;
	int i1_local=i1-i0, j1_local=j1-j0; //Coordenadas superiores locales
	if(i1 > img.alto()) i1_local = img.alto() - i0;
	if(j1 > img.ancho()) j1_local = img.ancho() - j0;
	// Se definen las coordenadas con respecto a esta imagen
	int x_inf = x0 + i0_local;
	int y_inf = y0 + j0_local;
	int x_sup = x0 + i1_local;
	int y_sup = y0 + j1_local;
	// Se adecúan las coordenadas considerando esta imagen
	int x_adj_inf=0, y_adj_inf=0; // Ajustes finales, regiones ya sobre esta imagen
	if(x_inf < 0){
		x_adj_inf = -x_inf;
		x_inf = 0;
	}	
	if(y_inf < 0){
		y_adj_inf = -y_inf;
		y_inf = 0;
	}	
	if(x_sup > alto()){ // Ajustes finales superiores 
		x_sup = alto();
	}
	if(y_sup > ancho()){
		y_sup = ancho();
	}
	// Se borra la imagen y se copia la región
	borrar();
   for(int t=0; t<tipo(); t++)
		for(int x=x_inf, i=i0+i0_local+x_adj_inf; x < x_sup; ++x, ++i)
			for(int y=y_inf, j=j0+j0_local+y_adj_inf; y < y_sup; ++y, ++j)
            picelU(t, x, y) = img.pixelU(t, i, j);
return;
}

int RealImageFFTW::guardaImagen(const char *nombre, const int formato) const
{
    FIBITMAP *img;
    int x; 
    bool error_happened = false;
    BYTE *bits;

    switch (tipo()){
    case GREY:
        img = FreeImage_Allocate(ancho(), alto(), 8);
        if(!img) {
            std::cerr << "Error saving image: FreeImage_Allocate\n";
            return -1;
        }
        for(int h = 0; h < alto(); h++) {
            bits = (BYTE *)FreeImage_GetScanLine(img, h);
            for(int w = 0; w < ancho(); w++) {
                bits[w] = 255.*Datos[0][h*ancho() + w];  // Por cuestiones de eficiencia se accede
                                                      // a datos privados aunque sea mala practica
            }
        }
    break;
    case RGB:
        img = FreeImage_Allocate(ancho(), alto(), 24);
        if(!img) {
            std::cerr << "Error saving image: FreeImage_Allocate\n";
            return -1;
        }
        for(int h = 0; h < alto(); h++) {
            bits = (BYTE *)FreeImage_GetScanLine(img, h);
            x = 0;
            for(int w = 0 ; w < ancho()*3; w = w+3) {
                bits[w+2] = 255.*Datos[RED][h*ancho() + x];
                bits[w+1] = 255.*Datos[GREEN][h*ancho() + x];
                bits[w] = 255.*Datos[BLUE][h*ancho() + x];
                x++;
            }
        }
    break;
    default:
    std::cerr << "Error saving image: image type not supported\n";
    return -1;
    break;
    }

    switch (formato){
    case FIF_JPEG:
        if(!FreeImage_Save(FIF_JPEG, img, nombre, JPEG_QUALITYSUPERB))
            std::cerr << "Error al guardar imagen: FreeImage_Save\n";
            error_happened = true;
        break;
    case FIF_PNG:
        if(!FreeImage_Save(FIF_PNG, img, nombre, PNG_DEFAULT))
            std::cerr << "Error al guardar imagen: FreeImage_Save\n";
            error_happened = true;
        break;
    case FIF_TIFF:
        if(!FreeImage_Save(FIF_TIFF, img, nombre, TIFF_DEFAULT))
            std::cerr << "Error al guardar imagen: FreeImage_Save\n";
            error_happened = true;
        break;
    default:
        std::cerr << "Error al guardar imagen: image format not supported\n";
        error_happened = true;
        break;
    }
    FreeImage_Unload(img);
   if(error_happened)
      return -1;
   return 0;
}

RealImageFFTW :: ~RealImageFFTW(void)
{
    for(int i=0; i < tipo(); i++)
        fftw_free(Datos [i]);
    
    free(Datos);
}

double* RealImageFFTW :: operator [](int k) //Acceder al vector de datos
{
    if(k >= tipo() || k < 0 || Datos == NULL){
        std::cerr << "Error al obtener vector de datos" << std::endl;
        return NULL;
    }
    
    return Datos[k];
}

const double* RealImageFFTW :: operator ()(int k) const//Acceder al vector de datos
{
    if(k >= tipo() || k < 0 || Datos == NULL){
        std::cerr << "Error al obtener vector de datos" << std::endl;
        return NULL;
    }
    return Datos[k];
}

RealImageFFTW& RealImageFFTW :: operator = (const RealImageFFTW& der)
{
  if(der.alto() != alto() || der.ancho() != ancho() || der.tipo() != tipo()){
    for(int i=0; i < tipo(); i++)
        fftw_free(Datos [i]);
    free(Datos);
    setAlto(der.alto());
    setAncho(der.ancho());
    setTipo(der.tipo());
    Datos = (double**)malloc(sizeof(double *) * tipo());
    for(int i=0; i < tipo(); i++)
      	Datos [i] = (double *)fftw_malloc(sizeof(double) * arraySize());
  }
   for(int i=0; i < tipo(); ++i)
   	for (int k=0; k<arraySize(); ++k)
         Datos[i][k] = der(i)[k];

  return *this;
}

double& RealImageFFTW :: picel(int color, int h, int w)
{
    if(color >= tipo() || color < 0 || Datos == NULL || h >= alto() || h < 0 
       || w >= ancho() || w < 0){
        std::cerr << "Error al obtener color " << color << std::endl;
        static double gas = 0;
        return gas;
        }
    return Datos[color][h*ancho() + w];
}
    
double RealImageFFTW :: pixel(int color, int h, int w) const
{
    if(color >= tipo() || color < 0 || Datos == NULL || h >= alto() || h < 0 
       || w >= ancho() || w < 0){
        std::cerr << "Error al obtener saturación de color " << color << std::endl;
        return 0;
    }
    return Datos[color][h*ancho() + w];
} 

void RealImageFFTW :: borrar(void)
{
    for(int i=0; i<tipo(); i++)
    for(int j=0; j<arraySize(); j++)
        Datos[i][j]=0;
} 

const RealImageFFTW& RealImageFFTW :: substractImage(const RealImageFFTW& der)
{
	if((*this) != der){
		std::cerr << "ERROR: substractImage: substracting incompatible images." << std::endl;
		return *this;
	}

   for(int t=0; t < tipo(); ++t)
      for(int i=0; i < alto(); ++i)
         for(int j=0; j < ancho(); ++j){
            double aux;
            if((aux = pixelU(t, i, j) - der.pixelU(t, i, j)) < 0.)
               aux = 0.;
            picelU(t, i, j) = aux;
         }
	return *this;
}

////////////////////////////Métodos de ComplexImageFFTW///////////////////////////////////////

ComplexImageFFTW :: ComplexImageFFTW(void) : ImageFFTW(){
    Datos=NULL;
    Formato = 0;  
}

ComplexImageFFTW :: ComplexImageFFTW(const int h, const int w, const int t) : ImageFFTW(h,w,t){
    Datos = (fftw_complex **)malloc(sizeof(fftw_complex *) * t);
    Formato = COMPLETEIMG; 
    for(int i=0; i<t; i++)
        Datos[i] = (fftw_complex *)fftw_malloc(sizeof(fftw_complex)*arraySize());
}

ComplexImageFFTW :: ComplexImageFFTW(const int h, const int w, const int t, const int f)
                  : ImageFFTW(h,w,t){
    Formato = f; 
    Datos = (fftw_complex **)malloc(sizeof(fftw_complex *) * t);
    for(int i=0; i<t; i++)
        Datos[i] = (fftw_complex *)fftw_malloc(sizeof(fftw_complex)*arraySize());
}

ComplexImageFFTW :: ~ComplexImageFFTW(void){
    for(int i=0; i < tipo(); i++)
        fftw_free(Datos [i]);

    free(Datos);
}

int ComplexImageFFTW :: arraySize(void) const {
    if (formato() == HALFIMG) return alto()*(ancho()/2 + 1);
    else if (formato() == COMPLETEIMG) return alto()*ancho();
   return 0;
}

fftw_complex* ComplexImageFFTW :: operator [](int k)
{
/*  if(k >= tipo() || k < 0 || Datos == NULL){
    std::cerr << "Error al obtener vector de datos" << std::endl;
    return NULL;
  }
*/
//  fftw_complex* o_o = reinterpret_cast <complex_num*> (Datos[k]); 
  return Datos[k];
}

const fftw_complex* ComplexImageFFTW :: operator ()(int k) const
{
/*   if(k >= tipo() || k < 0 || Datos == NULL){
      std::cerr << "Error al obtener vector de datos" << std::endl;
      return NULL;
   }
*/
//   complex_num* o_o = reinterpret_cast <complex_num*> (Datos[k]); 
   return Datos[k];
}

ComplexImageFFTW& ComplexImageFFTW :: operator = (const RealImageFFTW& der)
{ // Tal vez se pueda cambiar un poco la parte de formato() != COMPLETEIMG
  if(der.alto() != alto() || der.ancho() != ancho() || der.tipo() != tipo() || formato() != COMPLETEIMG){
    for(int i=0; i < tipo(); i++)
        fftw_free(Datos [i]);
    free(Datos);
    setAlto(der.alto());
    setAncho(der.ancho());
    setTipo(der.tipo());
    Formato = COMPLETEIMG;
    Datos=(fftw_complex**)malloc(sizeof(fftw_complex *) * tipo());
    for(int i=0; i < tipo(); i++){
        Datos [i] = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * arraySize());
        for (int k=0; k<arraySize(); k++)
            reinterpret_cast<complex_num &>(Datos[i][k]) = der(i)[k];
            // Tener cuidado con esta asignación, Datos[i][k] devuelve un arreglo de dos double
            // que es compatible bit a bit con complex_num (aka complex <double>)
    }
  }
  else{
    for(int i=0; i < tipo(); i++)
        for (int k=0; k<arraySize(); k++)
            reinterpret_cast<complex_num &>(Datos[i][k]) = der(i)[k];
            // Igual al caso anterior. Notar que der(i)[k] es un double y se asigna a un complex_num
  }
    return *this;
}

ComplexImageFFTW& ComplexImageFFTW :: operator = (const ComplexImageFFTW& der)
{
  if(der.alto() != alto() || der.ancho() != ancho() || der.tipo() != tipo() ||
       der.formato() != formato()){
    for(int i=0; i < tipo(); i++)
        fftw_free(Datos [i]);
    free(Datos);
    setAlto(der.alto());
    setAncho(der.ancho());
    setTipo(der.tipo());
    Formato = der.formato();
    Datos=(fftw_complex**)malloc(sizeof(fftw_complex *) * tipo());
    
    for(int i=0; i < tipo(); i++){
        Datos [i] = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * arraySize());
        for (int k=0; k<arraySize(); k++)
            reinterpret_cast<complex_num &> (Datos[i][k]) = 
                                                 reinterpret_cast<const complex_num &> (der(i)[k]);
    }
  }
  else{
    for(int i=0; i < tipo(); i++)
        for (int k=0; k<arraySize(); k++)
            reinterpret_cast<complex_num &> (Datos[i][k]) = 
                                                 reinterpret_cast<const complex_num &> (der(i)[k]);
  }
  return *this;
}    
// Cuidado con estas dos funciones si se usan para acceder a una imagen con el formato HALFIMG
// pues no se puede acceder a la mitad inexistente de la imagen
complex_num& ComplexImageFFTW :: picel(int color, int h, int w)
{
    static complex_num gas(0,0);
    if(color >= tipo() || color < 0 || Datos == NULL || h >= alto() || h < 0
       || w >= ancho() || w < 0){
        std::cerr << "Error al obtener color " << color << std::endl;
        return gas;
        }
    if(formato() == COMPLETEIMG)
        return reinterpret_cast<complex_num &> (Datos[color][h*ancho() + w]);
    else if(formato() == HALFIMG){
        if(w > ancho()/2){
            std::cerr << "Alerta: intentando acceder a un dato inexistente\n";
            return gas;
            }
        else 
            return reinterpret_cast<complex_num &> (Datos[color][h*((ancho()/2)+1) + w]);
    }
    return gas;
}

complex_num& ComplexImageFFTW :: picelU(int color, int h, int w)
{
    static complex_num gas(0,0);
    if(formato() == COMPLETEIMG)
        return reinterpret_cast<complex_num &> (Datos[color][h*ancho() + w]);
    else if(formato() == HALFIMG){
        if(w > ancho()/2){
            std::cerr << "Alerta: intentando acceder a un dato inexistente\n";
            return gas;
            }
        else 
            return reinterpret_cast<complex_num &> (Datos[color][h*((ancho()/2)+1) + w]);
    }
    return gas;
}
   
complex_num ComplexImageFFTW :: pixel(int color, int h, int w) const
{
    if(color >= tipo() || color < 0 || Datos == NULL || h >= alto() || h < 0
       || w >= ancho() || w < 0){
        std::cerr << "Error al obtener saturación de color " << color << std::endl;
        return 0;
    }
    if(formato() == COMPLETEIMG)
        return reinterpret_cast<complex_num &> (Datos[color][h*ancho() + w]);
    else if(formato() == HALFIMG){
        if(w > ancho()/2){
            if(h==0)
                return conj(reinterpret_cast<complex_num &>(Datos[color][ancho()-w]));
            else 
                return conj(reinterpret_cast<complex_num &>(Datos[color][(alto()-h)*((ancho()/2)+1) 
                                                                          + ancho()-w]));
        }
        else 
            return reinterpret_cast<complex_num &>(Datos[color][h*((ancho()/2)+1) + w]);
    }
    return 0;
}
    
complex_num ComplexImageFFTW :: pixelU(int color, int h, int w) const
{
    if(formato() == COMPLETEIMG)
        return reinterpret_cast<complex_num &> (Datos[color][h*ancho() + w]);
    else if(formato() == HALFIMG){
        if(w > ancho()/2){
            if(h==0)
                return conj(reinterpret_cast<complex_num &>(Datos[color][ancho()-w]));
            else 
                return conj(reinterpret_cast<complex_num &>(Datos[color][(alto()-h)*((ancho()/2)+1) 
                                                                          + ancho()-w]));
        }
        else 
            return reinterpret_cast<complex_num &>(Datos[color][h*((ancho()/2)+1) + w]);
    }
    return 0;
}

RealImageFFTW* ComplexImageFFTW :: realImage(void)
{
    RealImageFFTW *a = new RealImageFFTW(alto(), ancho(), tipo());
    for (int i=0; i<alto(); i++)
        for (int j=0; j<ancho(); j++)
            for (int k=0; k<tipo(); k++)
                a->picelU(k, i, j) = abs(this->pixelU(k, i, j));
    return a;
}

void ComplexImageFFTW :: multConj(const ComplexImageFFTW& a, const ComplexImageFFTW& b)
{
    if(a.alto() != b.alto() || a.alto() != alto() || a.ancho() != b.ancho() || a.ancho() != ancho() 
        || a.arraySize() != b.arraySize() || a.arraySize() != arraySize() ||
           a.tipo() != b.tipo() || a.tipo() != tipo()){
        std::cerr << "Error: multConj received images non compatibles" << std::endl;
        return;
    }
    for(int i=0; i<tipo() ; i++)
        for(int j=0; j<arraySize() ; j++)
            reinterpret_cast<complex_num &>((*this)[i][j]) = 
                                            conj(reinterpret_cast<const complex_num &>(a(i)[j])) * 
                                                 reinterpret_cast<const complex_num &>(b(i)[j]);
}
    
void ComplexImageFFTW :: borrar(void)
{
    for(int i=0; i<tipo(); i++)
    for(int j=0; j<arraySize(); j++)
        reinterpret_cast<complex_num &>((*this)[i][j]) = 0;
} 

///////////////////////////Métodos de TransFourier/////////////////////////////////////////////

std::mutex TransFourier::SeguroPlan;

///////////////////////////////Métodos de TransFourierR2C//////////////////////////////////////

TransFourierR2C :: TransFourierR2C(void){
    Entrada = NULL;
    Salida = NULL;
}

TransFourierR2C :: TransFourierR2C(int h, int w, int imgtype){
   Entrada = new RealImageFFTW(h, w, imgtype);
   Salida = new ComplexImageFFTW(h, w, imgtype, HALFIMG);
   std::lock_guard<std::mutex> guard(SeguroPlan);
   Plan = fftw_plan_dft_r2c_2d(h, w, (*Entrada)[0], (*Salida)[0], FFTW_MEASURE);
}

TransFourierR2C:: ~TransFourierR2C(void){
   {
      std::lock_guard<std::mutex> guard(SeguroPlan);
      fftw_destroy_plan(Plan);
   }
    delete Entrada;
    delete Salida;
}

void TransFourierR2C :: calcular(void){
    for(int i = 0; i<Entrada->tipo(); i++){
        fftw_execute_dft_r2c(Plan, (*Entrada)[i], (*Salida)[i]);
    }
}

////////////////////////////Métodos de TransFourierC2R//////////////////////////////////////

TransFourierC2R :: TransFourierC2R(void){
   Entrada = NULL;
   Salida = NULL;
}

TransFourierC2R :: TransFourierC2R(int h, int w, int imgtype){
   Entrada = new ComplexImageFFTW(h, w, imgtype, HALFIMG);
   Salida = new RealImageFFTW(h, w, imgtype);
   std::lock_guard<std::mutex> guard(SeguroPlan);
   Plan = fftw_plan_dft_c2r_2d(h, w, (*Entrada)[0], (*Salida)[0], FFTW_MEASURE);
}

TransFourierC2R:: ~TransFourierC2R(void){
   {
      std::lock_guard<std::mutex> guard(SeguroPlan);
      fftw_destroy_plan(Plan);
   }
   delete Entrada;
   delete Salida;
}

void TransFourierC2R :: calcular(void){
    double coef = Salida->alto()*Salida->ancho();
    for(int i = 0; i < Entrada->tipo(); i++){
        fftw_execute_dft_c2r(Plan, (*Entrada)[i], (*Salida)[i]);
        for (int j = 0; j<Salida->arraySize(); j++)
            (*Salida)[i][j]/=coef;
    }
}

///////////////////////////////Métodos de TransFourierC2CF//////////////////////////////////////

TransFourierC2CF :: TransFourierC2CF(void){
    Entrada = NULL;
    Salida = NULL;
}

TransFourierC2CF :: TransFourierC2CF(int h, int w, int imgtype){
   Entrada = new ComplexImageFFTW(h, w, imgtype, COMPLETEIMG);
   Salida = new ComplexImageFFTW(h, w, imgtype, COMPLETEIMG);
   std::lock_guard<std::mutex> guard(SeguroPlan);
   Plan = fftw_plan_dft_2d(h, w, (*Entrada)[0], (*Salida)[0], FFTW_FORWARD, FFTW_MEASURE);
}

TransFourierC2CF:: ~TransFourierC2CF(void){
   {
      std::lock_guard<std::mutex> guard(SeguroPlan);
      fftw_destroy_plan(Plan);
   }
   delete Entrada;
   delete Salida;
}

void TransFourierC2CF :: calcular(void){
    for(int i = 0; i < Entrada->tipo(); i++){
        fftw_execute_dft(Plan, (*Entrada)[i], (*Salida)[i]);
    }
}

/////////////////////////////Métodos de TransFourierC2CB//////////////////////////////////////

TransFourierC2CB :: TransFourierC2CB(void){
    Entrada = NULL;
    Salida = NULL;
}

TransFourierC2CB :: TransFourierC2CB(int h, int w, int imgtype){
   Entrada = new ComplexImageFFTW(h, w, imgtype, COMPLETEIMG);
   Salida = new ComplexImageFFTW(h, w, imgtype, COMPLETEIMG);
   std::lock_guard<std::mutex> guard(SeguroPlan);
   Plan = fftw_plan_dft_2d(h, w, (*Entrada)[0], (*Salida)[0], FFTW_BACKWARD, FFTW_MEASURE);
}

TransFourierC2CB:: ~TransFourierC2CB(void){
   {
      std::lock_guard<std::mutex> guard(SeguroPlan);
      fftw_destroy_plan(Plan);
   }
   delete Entrada;
   delete Salida;
}

void TransFourierC2CB :: calcular(void){
    double coef = Salida->alto() * Salida->ancho();
    for(int i = 0; i < Entrada->tipo(); i++){
        fftw_execute_dft(Plan, (*Entrada)[i], (*Salida)[i]);
        for (int j = 0; j<Salida->arraySize(); j++)
            reinterpret_cast<complex_num &>((*Salida)[i][j]) /= coef;
    }
}

////////////////////////////Funciones útiles//////////////////////////////////////////////////

int getImageFormatFromFilename(const std::string& filename)
{
   if (filename.empty()){
      std::cerr << "Error: getImageFormatFromFilename: empty filename" << std::endl;
      return -1;
   }
   int index, point_pos;

   for(index = filename.size() -1; index >= 0 ; --index)
      if(filename[index] == '.'){
         point_pos = index + 1;
         index = -3;
      }
   if(index == -4){
      std::string extension = filename.substr(point_pos);
      if(extension == "tif" || extension == "tiff")
         return FIF_TIFF;
      else if(extension == "jpg" || extension == "jpeg")
         return FIF_JPEG;
      else if(extension == "png")
         return FIF_PNG;
      else if(extension == "bmp")
         return FIF_BMP;
   }
   std::cerr << "getImageFormatFromFilename: file format not supported" << std::endl;
   return -1;
}


