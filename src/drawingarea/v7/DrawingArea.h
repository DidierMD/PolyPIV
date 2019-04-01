// DrawingArea.h by Didier Muñoz
// Version 6
// Esta biblioteca requiere de GTK+3 ImageFFTW DisplacementMap

#ifndef DRAWINGAREA
#define DRAWINGAREA

#include <iostream>
#include <cstdlib>
#include <list>
#include <gtk/gtk.h>
#include "ImageFFTW.h"
#include "DisplacementMap.h"

class DAPainter{
   private:
      int Alto, Ancho; // Alto y ancho de la gtk_drawing_area
      const RealImageFFTW& Img; // Sólo es la imagen que se le asocia, 
                            //  no se debe crear ni destruir dentro de esta clase
      double Escala, Borde;

      void pintaFlecha(cairo_surface_t *sup, gdouble x0, gdouble y0, gdouble dx, gdouble dy);

   public: 
      cairo_surface_t *SurfaceShown; // Es la que se muestra
      cairo_surface_t *SurfaceImg; // Superficie que guarda imagen cargada

      //DAPainter(void);
		//DAPainter(const DAPainter& _other);
      DAPainter(const RealImageFFTW & _img); // Associates image
      ~DAPainter(void);
      const RealImageFFTW& img(void) const {return Img;};
      int alto(void) const {return Alto;};
      int ancho(void) const {return Ancho;};
      double escala(void) const {return Escala;};
      double borde(void) const {return Borde;};
      bool horizontal(void); 
      void calculaEscala(void);
      void setAlto(int a){Alto = a;};
      void setAncho(int a){Ancho = a;};
      void cargarImagen(GtkWidget* _da); // La imagen se carga en SurfaceImg
      void pintarSuperficieImg(void); // Se pinta la imagen en SurfaceShown
      void borrarSuperficie(void); 
		void configura(GtkWidget* _da); // La función que configura y genera SurfaceShown
      void pintaMapaDesplazamientos(const DisplacementMap& _dismap);
};

class DAPaintersManager{
	private:
		std::list <DAPainter> Painters;
      GtkWidget *DA; 

	public:
		DAPaintersManager(void){
			DA = NULL;
		}
		DAPaintersManager(GtkWidget * _da){
			DA = _da;
		}
		void setDAWidget(GtkWidget * _da){
			DA = _da;
		}
		void setImages(const std::list<RealImageFFTW>& _imgs){
   		Painters.clear();
			for(std::list<RealImageFFTW>::const_iterator iter=_imgs.cbegin(), itend=_imgs.cend();
						iter != itend; ++iter){
      		Painters.emplace_back(*iter);
   		}
		}
		void addImage(RealImageFFTW& _img){
      	Painters.emplace_back(_img);
		}
		void loadLastImage(void){
			Painters.back().cargarImagen(DA);
		}
		void loadAllImages(void){
			for(std::list<DAPainter>::iterator iter=Painters.begin(), itend=Painters.end(); 
																							iter != itend; ++iter)
				(*iter).cargarImagen(DA);
		}
		void paint(int _num, cairo_t* _cr){
			std::list<DAPainter>::iterator iter = Painters.begin();
			for(int i=0; i<_num; ++iter, ++i) ; // Avanzamos en la lista
			(*iter).borrarSuperficie();
			(*iter).pintarSuperficieImg();
			cairo_set_source_surface (_cr, (*iter).SurfaceShown, 0, 0);
			cairo_paint (_cr);
		}
		void paint(unsigned _num, const DisplacementMap& _dismap, cairo_t* _cr){
			std::list<DAPainter>::iterator iter = Painters.begin();
			for(unsigned i=0; i<_num; ++iter, ++i) ; // Avanzamos en la lista
			(*iter).borrarSuperficie();
			(*iter).pintarSuperficieImg();
			(*iter).pintaMapaDesplazamientos(_dismap);
			cairo_set_source_surface (_cr, (*iter).SurfaceShown, 0, 0);
			cairo_paint (_cr);
		}
		void configureLast(void){
			Painters.back().configura(DA);
		}
		void configureAll(void){
			for(std::list<DAPainter>::iterator iter=Painters.begin(), itend=Painters.end(); iter != itend; ++iter)
				(*iter).configura(DA);
		}
		void clearPainters(void){
			Painters.clear();
		}
};

#endif

/* Cambios: 
	Se cambió DrawingArea a DAPainter
	Se agregó soporte para DisplacementMap
	Se agregó la clase DAPaintersManager
*/
