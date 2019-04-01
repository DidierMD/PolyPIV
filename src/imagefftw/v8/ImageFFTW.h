// ImageFFTW.h by Didier Muñoz
// Version 8
// Esta biblioteca requiere de FFTW y de FreeImage


#ifndef IMAGEFFTW
#define IMAGEFFTW

#include <iostream>
#include <cstdlib>

#include <complex> 
#include <fftw3.h>
#include <FreeImage.h>
#include <cmath>
#include <mutex>
#include <string>

enum{GREY=1, GREYDM=2, RGB=3}; // Usado en type de las imágenes
enum{RED=0,GREEN=1,BLUE=2,BRIGHT=0}; // Colores
enum{REAL=1, COMPLEX=2}; // Son los tipos de trasformada de Fourier
								 // Si se modifican estos valores hay que tener cuidado con TransFourier.
                         // También si se agregan tipos de transformadas
                        
typedef std::complex<double> complex_num;

class ImageFFTW {
   private:
      int Alto, Ancho, Tipo; // Tipo se refiere a tipo de imagen según cantidad de colores

   public:
      ImageFFTW(void);
      ImageFFTW(int h, int w, int t);
      virtual ~ImageFFTW(void){};  
		inline int alto(void) const {return Alto;};
		inline int ancho(void) const {return Ancho;};
		inline int tipo(void) const {return Tipo;};
      virtual int clase(void) const = 0;
      virtual void borrar(void) = 0;
      inline void setAlto(const int a) {Alto = a;}; 
      inline void setAncho(const int a) {Ancho = a;}; 
      inline void setTipo(const int a) {Tipo = a;}; 
      virtual int arraySize(void) const {return Alto*Ancho;}; // Se refiere al tamaño físico del
};                                                             // arreglo de Datos de un color

std::ostream& operator << (std::ostream&, const ImageFFTW&);
bool operator == (const ImageFFTW&, const ImageFFTW&);
bool operator != (const ImageFFTW&, const ImageFFTW&);

class RealImageFFTW : public ImageFFTW {
	private:
		double** Datos; // Son tres arreglos de double para imágenes RGB
		
	public:
      RealImageFFTW(void);
		~RealImageFFTW(void);
      RealImageFFTW(const RealImageFFTW& der);
      RealImageFFTW(int h, int w, int t); // Construye una imagen vacía
      RealImageFFTW(const char* nombre, const int formato, const int type);
      RealImageFFTW(int h0, int h1, int w0, int w1, const RealImageFFTW& Img); //A partir de otra
      void copia(int h0, int h1, int w0, int w1, const RealImageFFTW& Img); //Siguiendo la idea
      void copiaE(int h0, int h1, int w0, int w1, const RealImageFFTW& Img); //Igual que el anterior
                                       //pero poniendo en cero la imagen en donde no se haya copiado
		void subImagen(int x0, int y0, int i0, int j0, int i1, int j1, const RealImageFFTW& Img);// Toma una 
			// región (i0,j0)->(i1,j1) de otra imagen y la copia a partir de (x0,y0) haciendo 0 lo demás.
	
      inline virtual int clase(void) const {return REAL;};
      virtual void borrar(void);
      int guardaImagen(const char *nombre, const int formato) const;
		double* operator [](int); // Acceso a los datos, dominio depende de Tipo 
		const double* operator ()(int) const; // Acceso de lectura a datos, dominio depende de Tipo 
      RealImageFFTW& operator = (const RealImageFFTW&);
      double& picel(int color, int h, int w); // Acceso de escritura a los datos
      inline double& picelU(int color, int h, int w) // picel pero sin verificar parámetros
         {return Datos[color][h*ancho() + w];};
		double pixel(int color, int h, int w) const; // Acceso de consulta a los pixeles
		inline double pixelU(int color, int h, int w) const // pixel pero sin verificar parámetros
         {return Datos[color][h*ancho() + w];}

		const RealImageFFTW& substractImage(const RealImageFFTW& der);
};

enum{HALFIMG = 1, COMPLETEIMG = 2};

class ComplexImageFFTW : public ImageFFTW {
    private:
        fftw_complex **Datos; //fftw_complex por las ventajas al calcular con fftw
        int Formato; // Se refiere a la forma en que están guardados los valores
                    // HALFIMG  ->  Y[k0,k1] = Y[n0−k0, n1−k1]∗
    public:
        ComplexImageFFTW(void);
        ComplexImageFFTW(const int h, const int w, const int t);
        ComplexImageFFTW(const int h, const int w, const int t, const int f);
        ~ComplexImageFFTW(void);
        inline int formato(void) const {return Formato;};
        inline virtual int clase(void) const {return COMPLEX;};
        virtual int arraySize(void) const;
        virtual void borrar(void);
        fftw_complex* operator [](int); // Da acceso a los datos, el dominio depende de Tipo
        const fftw_complex* operator ()(int)const; // Acceso de lectura a datos, dominio depende Tipo
        ComplexImageFFTW& operator = (const ComplexImageFFTW&);
        ComplexImageFFTW& operator = (const RealImageFFTW&);
        complex_num& picel(int color, int h, int w); // Acceso de escritura a los datos
        complex_num& picelU(int color, int h, int w); //picel pero sin verificar parámetros
        complex_num pixel(int color, int h, int w) const; // Acceso de consulta a los valores
        complex_num pixelU(int color, int h, int w) const; //pixel sin verificar parámetros
        RealImageFFTW* realImage(void); // Crea una imagen real a partir de la "imagen módulo"
                                        // La imagen devuelta se tiene liberar con delete
        void multConj(const ComplexImageFFTW&, const ComplexImageFFTW&); // Si las dimensiones de 
        // ambas y de ésta son las mismas, las multiplica y las guarda en ésta (funcion útil para
        // cuando se aproveche el teorema de correlación
};

enum{R2C=1, C2R=2, C2CF=3, C2CB=4};

class TransFourier { 
   public:
      TransFourier(void){};
      virtual int tipo(void) const = 0;
      virtual void calcular(void) = 0;
   protected:
      static std::mutex SeguroPlan; // Para evitar problemas al crear fftw_plan's por varios hilos
}; 

class TransFourierR2C : public TransFourier {
    private:
        fftw_plan Plan;
        RealImageFFTW *Entrada;
        ComplexImageFFTW *Salida;   
        
    public:
        TransFourierR2C(void);
        TransFourierR2C(int h, int w, int imgtype);
        ~TransFourierR2C(void);
        inline RealImageFFTW* entrada(void) {return Entrada;};
        inline ComplexImageFFTW* salida(void) {return Salida;};
        inline virtual int tipo(void) const {return R2C;};
        virtual void calcular(void);
};

class TransFourierC2R : public TransFourier {
    private:
        fftw_plan Plan;
        ComplexImageFFTW *Entrada;
        RealImageFFTW *Salida;   
        
    public:
        TransFourierC2R(void);
        TransFourierC2R(int h, int w, int imgtype);
        ~TransFourierC2R(void);
        inline ComplexImageFFTW* entrada(void) {return Entrada;};
        inline RealImageFFTW* salida(void) {return Salida;};
        inline virtual int tipo(void) const {return C2R;};
        virtual void calcular(void);
};

class TransFourierC2CF : public TransFourier {
    private:
        fftw_plan Plan;
        ComplexImageFFTW *Entrada;
        ComplexImageFFTW *Salida;   
        
    public:
        TransFourierC2CF(void);
        TransFourierC2CF(int h, int w, int imgtype);
        ~TransFourierC2CF(void);
        inline ComplexImageFFTW* entrada(void) {return Entrada;};
        inline ComplexImageFFTW* salida(void) {return Salida;};
        inline virtual int tipo(void) const {return C2CF;};
        virtual void calcular(void);
};

class TransFourierC2CB : public TransFourier {
    private:
        fftw_plan Plan;
        ComplexImageFFTW *Entrada;
        ComplexImageFFTW *Salida;   
        
    public:
        TransFourierC2CB(void);
        TransFourierC2CB(int h, int w, int imgtype);
        ~TransFourierC2CB(void);
        ComplexImageFFTW* entrada(void) {return Entrada;};
        inline ComplexImageFFTW* salida(void) {return Salida;};
        inline virtual int tipo(void) const {return C2CB;};
        inline virtual void calcular(void);
};

////////////////////Funciones útiles///////////////////////////////////////////

int getImageFormatFromFilename(const std::string& filename);

#endif

/* Cambios:
  Modificación del makefile
  Cambio de usar <complex.h> (números complejos estándar de C) a <complex> (plantilla estándar de 
    números complejos de C++) debido a que al parecer el estándar C++11 entra en conflicto con 
    complex.h y por ello ahora al incluir <complex.h> se incluye <complex> provocando en este caso
    que fftw_complex siga siendo double[2], con los problemas que eso conlleva
  Se cambiaron a destructores virtuales en clases heredadas
  Se agregó la función copiaE en RealImage
  En TransFourier se agregó un mutex para proteger la llamada a funciones de fftw que no sean
  para ejecutar un plan.
  Se agregó una función útil: getImageFormatFromFilename
*/
