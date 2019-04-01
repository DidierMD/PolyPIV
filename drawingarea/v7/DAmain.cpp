#include <iostream>
#include <gtk/gtk.h>

#include "DrawingArea.h"

#define PIV_GUI_FILE "piv_gui.glade"

using namespace std;

class DatosPrograma{
   public:

      DatosPrograma(void);
};
DatosPrograma::DatosPrograma(void){}

void close_window(void){ 
gtk_main_quit();
}

int main(int argc, char* argv[]){
   GtkWidget *window;
   GtkBuilder *builder;
   GError *error = NULL;

   gtk_init (&argc, &argv);

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if( ! gtk_builder_add_from_file( builder, PIV_GUI_FILE, &error ) )
    {
        g_warning( "%s", error->message );
        g_free( error );
        return( 1 );
    }

    /* Get main window pointer from UI */
    window = GTK_WIDGET( gtk_builder_get_object( builder, "main_window" ) );

    // Get DrawingArea frame
//    DrawingAreaFrame = GTK_WIDGET( gtk_builder_get_object( builder, "frame_DrawingArea"));
//    DrawingArea dada(700, 600);
//    gtk_container_add (GTK_CONTAINER (DrawingAreaFrame), dada.dA());
   
    /* Connect signals */
    gtk_builder_connect_signals( builder, NULL );

    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );

    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show( window );

    /* Start main loop */
    gtk_main();

    return( 0 );
}
