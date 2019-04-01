#include <iostream>
#include <gtk/gtk.h>

#include "DrawingArea.h"

#define PIV_GUI_FILE "piv_gui.glade"

using namespace std;

struct Data
{
    GtkWidget *quit;
    GtkWidget *about;
};

extern "C"
G_MODULE_EXPORT gboolean cb_delete_event( GtkWidget *window, GdkEvent  *event, Data *data)
{
    gint response = 1;

    /* Run dialog */
    response = gtk_dialog_run( GTK_DIALOG( data->quit ) );
    gtk_widget_hide( data->quit );

    return( 1 != response );
}

int main(int argc, char* argv[]){
   GtkWidget *window;
   GtkBuilder *builder;
   GError *error = NULL;
   Data data;

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
    data.quit = GTK_WIDGET( gtk_builder_get_object( builder, "dialog_quit" ) );
    /* Connect signals */
    gtk_builder_connect_signals( builder, &data);
    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );
    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show( window );
    /* Start main loop */
    gtk_main();

    return( 0 );
}
