// DrawingArea.cpp by Didier
// Version 5
// Esta biblioteca requiere de GTK+ 3

#include "DrawingArea.h"

using namespace std;

/*DAPainter :: DAPainter(const DAPainter& _other) : Img(_other.img())
{
	setAlto(_other.alto());
	setAncho(_other.ancho());
	Escala = _other.escala();
	Borde = _other.borde();
	SurfaceImg = _other.SurfaceImg;
	SurfaceShown = _other.SurfaceShown;
//	cairo_surface_reference(SurfaceImg); // Increases the reference count to avoid deletion
//	cairo_surface_reference(SurfaceShown);
}*/

DAPainter::DAPainter(const RealImageFFTW& _img) : Img(_img)
{
   SurfaceShown = NULL;
   SurfaceImg = NULL;
	Ancho = 0;
	Alto = 0;
}

DAPainter::~DAPainter(void)
{
    if(SurfaceShown)
        cairo_surface_destroy (SurfaceShown);
    if(SurfaceImg)
        cairo_surface_destroy (SurfaceImg);
} 

bool DAPainter :: horizontal(void)
{
   if((double)img().ancho()/(double)img().alto() >= (double)ancho()/(double)alto())
      return true;
   else
      return false;
}

void DAPainter :: calculaEscala(void)
{
   if(horizontal()){
      Borde = ((double)alto()*(double)img().ancho()/(double)ancho()-(double)(img().alto()))/2;
      Escala=(double)ancho()/(double)img().ancho();
   }
   else {
      Borde = ((double)ancho()*(double)img().alto()/(double)alto()-(double)(img().ancho()))/2;
      Escala = (double)alto()/(double)img().alto();
   }
}

// Se espera que img() sea distinto de NULL
void DAPainter :: cargarImagen(GtkWidget* _da)
{
   if(SurfaceShown == NULL || img().alto() == 0 || img().ancho() == 0) {
			cerr << "Error: cargarImagen:" << img().alto() << " " << img().ancho() << endl;
	return;
	}
   // Se crea superficie auxiliar y se pinta en ella la imagen
   cairo_surface_t *Surface_aux = gdk_window_create_similar_surface(gtk_widget_get_window(_da), 
																	CAIRO_CONTENT_COLOR, img().ancho(), img().alto());
   cairo_t *cr;
   cr =  cairo_create (Surface_aux);
   switch(img().tipo())
   {
   case GREY:
       cairo_set_line_width(cr, 1.);
       for(int h = img().alto()-1; h >= 0; h--)
       for(int k = img().ancho() - 1; k >= 0; k--){
           cairo_set_source_rgb(cr, img().pixelU(BRIGHT, h, k), 
                               img().pixelU(BRIGHT, h, k), 
                               img().pixelU(BRIGHT, h, k));
           cairo_move_to(cr, static_cast<double>(k), static_cast<double>(img().alto() - h-1)+0.5);
           cairo_rel_line_to(cr, 1., 0.);
           cairo_stroke(cr);
       }
   break;
   case RGB:
       cairo_set_line_width(cr, 1.);
       for(int h = img().alto()-1; h >= 0; --h)
       for(int k = 0; k < img().ancho(); ++k){
           cairo_set_source_rgb(cr, img().pixelU(RED, h, k), 
                               img().pixelU(GREEN, h, k), 
                               img().pixelU(BLUE, h, k));
           cairo_move_to(cr, static_cast<double>(k), static_cast<double>(img().alto() - h-1)+0.5);
           cairo_rel_line_to(cr, 1., 0.);
           cairo_stroke(cr);
       }
   break;
   default:
   cerr << "CargarImagen: Tipo de imagen no soportado" << endl;
   break;
   }
   // Se escoge la superficie que se ha creado
   if(SurfaceImg) 
      cairo_surface_destroy(SurfaceImg);
   SurfaceImg = Surface_aux;
   cairo_destroy(cr);
}

void DAPainter :: pintarSuperficieImg(void)
{
   if(SurfaceImg ==  NULL) {
		cerr << "Warning: pintarSuperficieImg: SurfaceImg == NULL" << endl;
		return;
	}
   if(SurfaceShown == NULL || img().alto() == 0 || img().ancho() == 0) {
		cerr << "Warning: pintarSuperficieImg: ";
		if(SurfaceShown==NULL) cerr << "SurfaceShown=NULL ";
		cerr << img().alto() << " " << img().ancho() << endl;
		return;
	}

   cairo_t *cr1;
   cr1 = cairo_create (SurfaceShown);
   calculaEscala();
   cairo_scale(cr1,escala(),escala());
   if(horizontal())
     cairo_set_source_surface(cr1, SurfaceImg, 0, borde());
   else 
     cairo_set_source_surface(cr1, SurfaceImg, borde(), 0);
   
   cairo_paint(cr1);
   cairo_destroy(cr1); 
}

void DAPainter :: borrarSuperficie(void)
{
    cairo_t *cr;
    cr = cairo_create(SurfaceShown);     // Pintamos de negro la superficie
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_paint (cr);
    cairo_destroy (cr);
} 

void DAPainter :: configura(GtkWidget* _da)
{
   setAncho( gtk_widget_get_allocated_width(_da) );
   setAlto( gtk_widget_get_allocated_height(_da) );

   if(SurfaceShown){ // Si tiene algo la destruimos
      cairo_surface_destroy (SurfaceShown); 
	}
   SurfaceShown = gdk_window_create_similar_surface(
											gtk_widget_get_window(_da), CAIRO_CONTENT_COLOR, ancho(), alto());
}

void DAPainter::pintaFlecha(cairo_surface_t *sup, gdouble x0, gdouble y0, gdouble dx, gdouble dy)
{
   cairo_t *cr;
   dy=-dy;
	double scale=1.;
   double lado=0.5*sqrt(dx*dx+dy*dy);
   double theta=atan2(dy,dx);
   // Paint to the surface, where we store our state. Se crea un contexto 
   cr = cairo_create (sup);
   cairo_move_to(cr, x0, y0);
   cairo_line_to(cr, x0+scale*dx, y0+scale*dy);
   cairo_line_to(cr, x0+scale*dx-lado*cos(0.523599-theta), y0+scale*dy+lado*sin(0.523599-theta));
   cairo_move_to(cr, x0+scale*dx, y0+scale*dy);
   cairo_line_to(cr, x0+scale*dx-lado*cos(0.523599+theta), y0+scale*dy-lado*sin(0.523599+theta));
   cairo_set_source_rgb(cr, 0,0.4,1);
   cairo_stroke(cr);
   //Aquí ya se pintó
   cairo_destroy (cr);
}

void DAPainter::pintaMapaDesplazamientos(const DisplacementMap& _dismap)
{
	int seph = _dismap.xSeparation();
	int sepv = _dismap.ySeparation();
   double sepvReal = _dismap.ySeparation() * escala();
   double sephReal = _dismap.xSeparation() * escala();
   double bordeReal = borde() * escala();
   int modV = img().alto() % sepv, modH = img().ancho() % seph;
   int topeV=_dismap.height()-1, topeH=_dismap.width()-1;
   int i, j;
   double coordV, coordH, aux;

   if(img().alto() % sepv == 0){//Caso en que las últimas zonas no son completas
      modV=0;
      topeV++;
   }
   if(img().ancho() % seph == 0){
      modH=0;
      topeH++;
   }
// Se preparan los inicios a partir de donde se pondrá la cuadrícula de centros de flecha
   if(horizontal()){
      coordV=(double)alto()-bordeReal-sepvReal/2.;
      coordH=sephReal/2.;
   }
   else{
      coordV=(double)alto() - sepvReal/2.;
      coordH=bordeReal + sephReal/2.;
   }
// Se recorre por filas, si la última zona de interrogación es incompleta la flecha se adecua
   for(i=0; i<topeV; i++, coordV-=sepvReal){
      for(j=0, aux=coordH; j<topeH; j++, aux+=sephReal){
         pintaFlecha(SurfaceShown, aux, coordV, escala()*_dismap.x(j, i), escala()*_dismap.y(j, i));
      }
      if(modH != 0){
         pintaFlecha(SurfaceShown, aux-(sephReal/2.)+((double)modH)*escala()/2., coordV, 
            				escala()*_dismap.x(j, i), escala()*_dismap.y(j, i));
      } 
   }  
   if(modV != 0){
      for(j=0, aux=coordH; j<topeH; j++, aux+=sephReal){
         pintaFlecha(SurfaceShown, aux, coordV+(sepvReal/2.)-((double)modV)*escala()/2., 
            escala()*_dismap.x(j, i), escala()*_dismap.y(j, i));
      }
      if(modH != 0){
         pintaFlecha(SurfaceShown, aux-(sephReal/2.)+((double)modH)*escala()/2., 
         coordV+(sepvReal/2.)-((double)modV)*escala()/2., 
         escala()*_dismap.x(j, i), escala()*_dismap.y(j, i));
      } 
   }
}
