// PolyPIV.cpp by Didier Muñoz
// Version 6

#include "PolyPIV.h"
#include <algorithm>

using namespace std;

//////////////ProgramData/////////////////////////////////////////////////

ProgramData::ProgramData(void) 
{
      // DrawingAreaPainters
      DrawingArea_Images = NULL;
		CurrentImageNum = -1;
      // DisplacementMaps
		CurrentDisplacementMap = -1;
      PintarFlechas = false;
      // Diálogo de guardado
      Dialog_Save = NULL; 
      CheckButton_Save_DispMap = NULL;
      CheckButton_Save_VelMap = NULL;
      CheckButton_Save_Images = NULL;
      // Diálogo cargar imagen
      Dialog_Load = NULL;
      // Diálogo de confirmación de salida
      Dialog_Quit = NULL;
      // Main Window
      Window_Main = NULL;
      // About this project
      EntryBuffer_ProjectName = NULL;
      ComboBoxText_LoadedImages = NULL;
		RadioButton_LoadedImages = NULL;
		ComboBoxText_PreprocessedImages = NULL;
		RadioButton_PreprocessedImages = NULL;
      ComboBoxText_DisplacementMaps = NULL;
		CheckButton_ShowMaps = NULL;
      RadioButton_PreUseOriginals = NULL;
      RadioButton_ProUseOriginals = NULL;
      RadioButton_PreUsePreprocessed = NULL;
      RadioButton_ProUsePreprocessed = NULL;
      Adjustment_DeltaT = NULL;
      Adjustment_ScaleFactor = NULL;
      // PIV
		Notebook_PIV = NULL;
      // Statusbar
      StatusBar_MainWindow = NULL;

   // Cargar la sabiduría de FFTW
   fftw_import_wisdom_from_filename("wisdom.fftw");
}

ProgramData :: ~ProgramData(void)
{
   fftw_export_wisdom_to_filename("wisdom.fftw");
}

void ProgramData :: statusMessage(const string& message)
{
   gtk_statusbar_pop (GTK_STATUSBAR(StatusBar_MainWindow), 1);  
   gtk_statusbar_push (GTK_STATUSBAR(StatusBar_MainWindow), 1, message.c_str());
}

void ProgramData :: loadImageFromFilename(const string& _filename)
{
	if(!_filename.empty()){
		// Leemos imagen
      OriginalImages.emplace_back(_filename.c_str(), getImageFormatFromFilename(_filename), GREY);
		CurrentImageNum = OriginalImages.size() - 1;
      PintarFlechas = false;
      // Actualizamos el DAPaintersManager correspondiente
      DA_Originals.addImage(OriginalImages.back());
		DA_Originals.configureLast();
      DA_Originals.loadLastImage();
      // Ponemos mensaje en la barra de estado
      stringstream auxstream;
      auxstream << "Images loaded: height: " << OriginalImages.back().alto() << "    width: " 
             << OriginalImages.back().ancho();
      statusMessage(auxstream.str());
      // Actualizamos la GtkComboBoxText de las imágenes 
		enableUseLoadedImgs();
      gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(ComboBoxText_LoadedImages),
              ("T"+to_string(CurrentImageNum)+": " + getFilenameFromURI(_filename)).c_str());
      gtk_combo_box_set_active (GTK_COMBO_BOX(ComboBoxText_LoadedImages), CurrentImageNum);
      // La función anterior activa la señal que procesa los cambios en la comboboxtext
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(RadioButton_LoadedImages), TRUE);
   }
   else
      statusMessage("Empty filename");
}

list<RealImageFFTW>& ProgramData :: proInImgs(void)
{
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(RadioButton_ProUsePreprocessed))){
		return PreprocessedImages;
	}
	else
		return OriginalImages;
}

list<RealImageFFTW>& ProgramData :: preInImgs(void)
{
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(RadioButton_PreUsePreprocessed))){
		return PreprocessedImages;
	}
	else{
		return OriginalImages;
	}
}

void ProgramData :: setDisplacementMaps(const list<DisplacementMap>& _outmaps)
{
	DisplacementMaps = _outmaps;
}

void ProgramData :: setPreOutImgs(const list<RealImageFFTW>& _outimgs)
{
	PreprocessedImages = _outimgs;
}

void ProgramData :: processed(void)
{
	// Erase comboboxtext contents
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(ComboBoxText_DisplacementMaps));
	if(DisplacementMaps.size() > 0){
   	PintarFlechas=true;
		CurrentDisplacementMap = 0;
		// Update comboBoxText associated with displacement maps
		for(unsigned i=0; i < DisplacementMaps.size(); ++i){
      	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(ComboBoxText_DisplacementMaps),
																					("Map "+to_string(i)).c_str());
		}
		// This triggers the draw function for the DrawingArea
   	gtk_combo_box_set_active (GTK_COMBO_BOX(ComboBoxText_DisplacementMaps), CurrentDisplacementMap);
      enableShowDisplacementMaps();
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(CheckButton_ShowMaps), TRUE);
	}
	else{
		PintarFlechas = false;
		CurrentDisplacementMap = -1;
	}
}

void ProgramData :: preProcessed(void)
{
	PintarFlechas = false;
	// Update DrawingAreaPaintersManager
	DA_Preprocessed.setImages(PreprocessedImages);
	DA_Preprocessed.configureAll();
	DA_Preprocessed.loadAllImages();
	// Erase comboboxtext contents
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(ComboBoxText_PreprocessedImages));
	if(PreprocessedImages.size() > 0){
		CurrentImageNum = 0;
		// Update comboBoxText associated with Preprocessed images
		for(unsigned i=0; i < PreprocessedImages.size(); ++i){
      	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(ComboBoxText_PreprocessedImages),
																					("Img "+to_string(i)).c_str());
		}
		// This function triggers the draw signal
   	gtk_combo_box_set_active (GTK_COMBO_BOX(ComboBoxText_PreprocessedImages), CurrentImageNum);
		// And then we activate corresponding radio button
		enableUsePreprocessedImgs();
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(RadioButton_PreprocessedImages), TRUE);
	}
	else{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(RadioButton_LoadedImages), TRUE);
		disableUsePreprocessedImgs();
		/*
		if(OriginalImages.size() > 0)
			CurrentImageNum = 0;
		else
			CurrentImageNum = -1;
		*/
	}
}

int ProgramData :: saveVelocityMapToFile(const string& _filename, double _scalefactor, double _deltat)
{
   if( _deltat == 0.){
      statusMessage("Error: delta t is cero");
      return -1;
   }
   ofstream archivo(_filename);
	if(archivo.is_open()){
		archivo << "# " << gtk_entry_buffer_get_text(EntryBuffer_ProjectName) << "\n";
   	archivo << "# Delta t between images: \n" << _deltat << "\n";
   	archivo << "# Scale factor in cm/px: \n" << _scalefactor << "\n";
		archivo << "# Number of displacement/velocity maps: \n" << DisplacementMaps.size() << "\n";
		for(auto iter=DisplacementMaps.begin(), itend=DisplacementMaps.end(); iter!=itend; ++iter){
			(*iter).saveToStream(archivo, _scalefactor, _deltat);
      	archivo << "\n";
		}
   	return 0;
	}
	else{
		statusMessage("Error: Couldn't open file to save");
	return -1;
	}
}

void ProgramData :: drawDrawingArea(cairo_t* cr)
{
	if(CurrentImageNum >= 0){
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(RadioButton_LoadedImages))){
   		if(PintarFlechas){
				auto iter=DisplacementMaps.begin();
				for(int i=0; i<CurrentDisplacementMap; ++i) ++iter; // Avanzamos en la lista
   			DA_Originals.paint(CurrentImageNum, *iter, cr);
			}
			else{
   			DA_Originals.paint(CurrentImageNum, cr);
			}
		}
		else{
   		if(PintarFlechas){
				auto iter = DisplacementMaps.begin();
				for(int i=0; i < CurrentDisplacementMap; ++i) ++iter; // Avanzamos en la lista
   			DA_Preprocessed.paint(CurrentImageNum, (*iter), cr);
			}
			else{
   			DA_Preprocessed.paint(CurrentImageNum, cr);
			}
		}
	}
}

void ProgramData :: reset(void)
{
	// Images related
	OriginalImages.clear();
	PreprocessedImages.clear();
	DA_Originals.clearPainters();
	DA_Preprocessed.clearPainters();
	CurrentImageNum = -1;
	// DisplacementMaps related
	DisplacementMaps.clear();
	CurrentDisplacementMap = -1;
	PintarFlechas = false;

	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(ComboBoxText_LoadedImages));
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(ComboBoxText_PreprocessedImages));
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(ComboBoxText_DisplacementMaps));
	disableUseLoadedImgs();
	disableUsePreprocessedImgs();
   disableShowDisplacementMaps();

	// About this project related
	gtk_entry_buffer_delete_text(EntryBuffer_ProjectName, 0, -1);

	statusMessage(" Project resetted");
	// Redraw Drawing Area
   gtk_widget_queue_draw( DrawingArea_Images );
}

void ProgramData :: showOriginalImages(void)
{
	CurrentImageNum = gtk_combo_box_get_active(GTK_COMBO_BOX(ComboBoxText_LoadedImages));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(RadioButton_LoadedImages), TRUE);
   gtk_widget_queue_draw( DrawingArea_Images );
}

void ProgramData :: showPreprocessedImages(void)
{
	CurrentImageNum = gtk_combo_box_get_active(GTK_COMBO_BOX(ComboBoxText_PreprocessedImages));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(RadioButton_PreprocessedImages), TRUE);
   gtk_widget_queue_draw( DrawingArea_Images );
}

void ProgramData :: showMaps(bool _do_show)
{
	PintarFlechas = _do_show;
   gtk_widget_queue_draw( DrawingArea_Images );
}

void ProgramData :: disableUseLoadedImgs(void)
{
	gtk_widget_set_sensitive(ComboBoxText_LoadedImages, FALSE);
	gtk_widget_set_sensitive(RadioButton_LoadedImages, FALSE);
	gtk_widget_set_sensitive(RadioButton_PreUseOriginals, FALSE);
	gtk_widget_set_sensitive(RadioButton_ProUseOriginals, FALSE);
}	

void ProgramData :: enableUseLoadedImgs(void)
{
	gtk_widget_set_sensitive(ComboBoxText_LoadedImages, TRUE);
	gtk_widget_set_sensitive(RadioButton_LoadedImages, TRUE);
	gtk_widget_set_sensitive(RadioButton_PreUseOriginals, TRUE);
	gtk_widget_set_sensitive(RadioButton_ProUseOriginals, TRUE);
}

void ProgramData :: disableUsePreprocessedImgs(void)
{
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(RadioButton_PreUseOriginals), TRUE);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(RadioButton_ProUseOriginals), TRUE);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(RadioButton_LoadedImages), TRUE);
	gtk_widget_set_sensitive(RadioButton_PreUsePreprocessed, FALSE);
	gtk_widget_set_sensitive(RadioButton_ProUsePreprocessed, FALSE);
	gtk_widget_set_sensitive(RadioButton_PreprocessedImages, FALSE);
	gtk_widget_set_sensitive(ComboBoxText_PreprocessedImages, FALSE);
}

void ProgramData :: enableUsePreprocessedImgs(void)
{
	gtk_widget_set_sensitive(RadioButton_PreUsePreprocessed, TRUE);
	gtk_widget_set_sensitive(RadioButton_ProUsePreprocessed, TRUE);
	gtk_widget_set_sensitive(RadioButton_PreprocessedImages, TRUE);
	gtk_widget_set_sensitive(ComboBoxText_PreprocessedImages, TRUE);
}

void ProgramData :: disableShowDisplacementMaps(void)
{
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(CheckButton_ShowMaps), FALSE);
	gtk_widget_set_sensitive(ComboBoxText_DisplacementMaps, FALSE);
	gtk_widget_set_sensitive(CheckButton_ShowMaps, FALSE);
}

void ProgramData :: enableShowDisplacementMaps(void)
{
	gtk_widget_set_sensitive(ComboBoxText_DisplacementMaps, TRUE);
	gtk_widget_set_sensitive(CheckButton_ShowMaps, TRUE);
}

///////////////DataManager////////////////////////////////////////////////

template <typename... Ss, typename... Ts>
DataManager<TypePack<Ss...>, Ts...> :: DataManager(const tuple<Ss...>& _pre_algs, 
																	 const tuple<Ts...>& _pro_algs)
							: PreprocessingManager(*this, _pre_algs), ProcessingManager(*this, _pro_algs){
}

template <typename... Ss, typename... Ts>
void DataManager<TypePack<Ss...>, Ts...> :: retrieve_widgets_from_builder(GtkBuilder *builder)
{
   // DrawingArea
   DrawingArea_Images = GTK_WIDGET(gtk_builder_get_object(builder, "drawingarea_imgs"));
   // Diálogo de guardado
   Dialog_Save = GTK_WIDGET(gtk_builder_get_object( builder, "dialog_save" ));
   CheckButton_Save_DispMap = GTK_WIDGET(gtk_builder_get_object( builder, 
                                                      "checkbutton_save_displacementmap" ));
   CheckButton_Save_VelMap = GTK_WIDGET(gtk_builder_get_object( builder, 
                                                      "checkbutton_save_velocitymap" ));
   CheckButton_Save_Images = GTK_WIDGET(gtk_builder_get_object( builder, 
                                                      "checkbutton_save_images" ));
   // Diálogo cargar imagenT0
   Dialog_Load = GTK_WIDGET(gtk_builder_get_object( builder, "dialog_loadimage" ));
   // Diálogo de confirmación de salida
   Dialog_Quit = GTK_WIDGET(gtk_builder_get_object( builder, "dialog_quit" ));
   // Main Window
   Window_Main = GTK_WIDGET(gtk_builder_get_object( builder, "main_window" ));
   // About this project
   EntryBuffer_ProjectName = GTK_ENTRY_BUFFER(gtk_builder_get_object( builder, 
                                                      "entrybuffer_projectname" ));
   ComboBoxText_LoadedImages = GTK_WIDGET(gtk_builder_get_object( builder, 
                                                      "comboboxtext_loadedimages" ));
	RadioButton_LoadedImages = GTK_WIDGET(gtk_builder_get_object( builder,
                                                      "radiobutton_loadedimages" ));
   ComboBoxText_PreprocessedImages = GTK_WIDGET(gtk_builder_get_object( builder, 
                                                      "comboboxtext_preprocessedimages" ));
	RadioButton_PreprocessedImages = GTK_WIDGET(gtk_builder_get_object( builder,
                                                      "radiobutton_preprocessedimages" ));
   ComboBoxText_DisplacementMaps = GTK_WIDGET(gtk_builder_get_object( builder, 
                                                      "comboboxtext_displacementmaps" ));
	CheckButton_ShowMaps = GTK_WIDGET(gtk_builder_get_object( builder,
                                                      "checkbutton_showmaps" ));
	RadioButton_PreUseOriginals = GTK_WIDGET(gtk_builder_get_object( builder,
                                                      "radiobutton_preuseoriginals" ));
	RadioButton_ProUseOriginals = GTK_WIDGET(gtk_builder_get_object( builder,
                                                      "radiobutton_prouseoriginals" ));
	RadioButton_PreUsePreprocessed = GTK_WIDGET(gtk_builder_get_object( builder,
                                                      "radiobutton_preusepreprocessed" ));
	RadioButton_ProUsePreprocessed = GTK_WIDGET(gtk_builder_get_object( builder,
                                                      "radiobutton_prousepreprocessed" ));
   Adjustment_DeltaT = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment_deltat"));
   Adjustment_ScaleFactor = GTK_ADJUSTMENT(gtk_builder_get_object(builder, 
                                                      "adjustment_scalefactor" ));
	Notebook_PIV = GTK_WIDGET(gtk_builder_get_object( builder, "notebook_piv"));
   // Statusbar
   StatusBar_MainWindow = GTK_WIDGET(gtk_builder_get_object( builder, 
                                                         "statusbar_mainwindow" ));
}

template <typename... Ss, typename... Ts>
void DataManager<TypePack<Ss...>, Ts...> :: initialize_widgets(GtkBuilder* _builder)
{
	retrieve_widgets_from_builder(_builder);

   gtk_builder_connect_signals( _builder, this);

	// Here is generated the GUI inside the PIV gtknotebook
	PreprocessingManager.generateGUI(GTK_NOTEBOOK(Notebook_PIV), "Preprocessing");
	ProcessingManager.generateGUI(GTK_NOTEBOOK(Notebook_PIV), "Processing");

	// Disable loaded images comboboxtext 
	disableUseLoadedImgs();

	// Disable the widgets related to preprocessed images
	disableUsePreprocessedImgs();

   // Disable the widgets related to showing displacement Maps
   disableShowDisplacementMaps();
	
	// Setup DAPaintersManager
	DA_Originals.setDAWidget(DrawingArea_Images);
	DA_Preprocessed.setDAWidget(DrawingArea_Images);
}

template <typename... Ss, typename... Ts>
DataManager<TypePack<Ss...>, Ts...> make_DataManager(const tuple<Ss...>& _pre_algs, 
																		const tuple<Ts...>& _pro_algs)
{
	return DataManager<TypePack<Ss...>, Ts...>(_pre_algs, _pro_algs);
}

///////////////Signals Callbacks//////////////////////////////////////////////////////////

extern "C" G_MODULE_EXPORT gboolean 
cb_mainwindow_delete_event( GtkWidget *window, GdkEvent *event, ProgramData *data)
{
    gint resp = 1;

    resp = gtk_dialog_run( GTK_DIALOG( data->Dialog_Quit ) );
    gtk_widget_hide( data->Dialog_Quit );

    return( 1 != resp );
}

extern "C" G_MODULE_EXPORT void
cb_toolbutton_load_clicked( GtkButton *button, ProgramData *data)
{
   gint resp;

   resp = gtk_dialog_run( GTK_DIALOG( data->Dialog_Load ) );
   gtk_widget_hide( data->Dialog_Load );

   if (resp == 1){
		std::vector<std::string> filenames;
      GSList *fnames;
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (data->Dialog_Load);

      fnames = gtk_file_chooser_get_filenames (chooser);
      if(fnames != NULL){
			filenames.push_back(static_cast<const gchar*>(fnames->data));
			for(GSList* ptr = fnames->next; ptr != NULL; ptr = ptr->next){	
				filenames.push_back(static_cast<const gchar*>(ptr->data));
			}
		}
      g_slist_free_full(fnames, g_free);
		
		std::sort(filenames.begin(), filenames.end());
		for(unsigned i=0; i < filenames.size(); ++i)
			data -> loadImageFromFilename(filenames[i]);
   }
}

extern "C" G_MODULE_EXPORT void
cb_toolbutton_save_clicked(GtkButton *button, ProgramData *data)
{
   gint res;
   GtkFileChooser *chooser = GTK_FILE_CHOOSER (data->Dialog_Save);

   // Inicializaciones del diálogo
   gtk_file_chooser_set_current_name (chooser, 
													gtk_entry_buffer_get_text(data->EntryBuffer_ProjectName));
   gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(data->CheckButton_Save_DispMap), FALSE);
   gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(data->CheckButton_Save_VelMap), TRUE);
   gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(data->CheckButton_Save_Images), FALSE);
   
   double delta_t = gtk_adjustment_get_value (data->Adjustment_DeltaT);
   double scale_factor = gtk_adjustment_get_value (data->Adjustment_ScaleFactor);

   res = gtk_dialog_run( GTK_DIALOG( data->Dialog_Save ) );
   gtk_widget_hide( data->Dialog_Save );

   if (res == 1){
      string filename; 
      string statusmessage("Saved: ");
      gchar *texto = gtk_file_chooser_get_filename (chooser);
      if( texto != NULL)
         filename = texto;
      g_free(texto);
      gboolean save_disp = gtk_toggle_button_get_active(
                                       GTK_TOGGLE_BUTTON(data->CheckButton_Save_DispMap));
      gboolean save_vel = gtk_toggle_button_get_active(
                                       GTK_TOGGLE_BUTTON(data->CheckButton_Save_VelMap));
      gboolean save_images = gtk_toggle_button_get_active(
                                       GTK_TOGGLE_BUTTON(data->CheckButton_Save_Images));
      if(save_disp){
         if(data->saveVelocityMapToFile(filename + ".des.txt", scale_factor)) // if error in saving
            statusmessage += "DisplacementMap=error  ";
			else 	// if saving was good
            statusmessage += "DisplacementMap=good  ";
      }
      if(save_vel){
         if(data->saveVelocityMapToFile(filename + ".vel.txt", scale_factor, delta_t))
            statusmessage += "VelocityMap=error  ";
         else
            statusmessage += "VelocityMap=good  ";
      } 
      if(save_images){
         bool err=false;
			int i=0;
			for(std::list<RealImageFFTW>::iterator iter=data->PreprocessedImages.begin(); iter != data->PreprocessedImages.end(); ++iter){
				// Beware short-circuit evaluation!!!
            err = err || ~iter->guardaImagen((filename+".t"+to_string(i)+".tif").c_str(), FIF_TIFF);
				++i;
         }
         if (err)
            statusmessage += "PreprocessedImages=error  ";
         else
            statusmessage += "PreprocessedImages=good  ";
      } 
		data->statusMessage(statusmessage);
   }
	else
		data->statusMessage("Nothing to save");
}

extern "C" G_MODULE_EXPORT void
cb_toolbutton_clear_clicked(GtkButton *button, ProgramData *data)
{
	// Call reset function in ProgramData
	data->reset();
}	

extern "C" G_MODULE_EXPORT gboolean 
cb_drawingarea_draw_event (GtkWidget *widget, cairo_t *cr, ProgramData *data)
{
	data->drawDrawingArea(cr);
return FALSE; //Todavía no acabamos de utilizar la señal ::draw
}

extern "C" G_MODULE_EXPORT gboolean 
cb_drawingarea_configure_event (GtkWidget *widget, GdkEventConfigure *event, ProgramData *data)
{
	data->DA_Originals.configureAll();
	data->DA_Preprocessed.configureAll();
   gtk_widget_queue_draw( data->DrawingArea_Images );
   return TRUE; // Porque ya manejamos el evento "configure"
}

extern "C" G_MODULE_EXPORT void
cb_comboboxtext_loadedimages_changed(GtkComboBox *combobox, ProgramData *data)
{
	data->showOriginalImages();
}

extern "C" G_MODULE_EXPORT void
cb_radiobutton_loadedimages_toggled(GtkToggleButton* togglebutton, ProgramData *data)
{
	if(gtk_toggle_button_get_active(togglebutton))
		data->showOriginalImages();
}

extern "C" G_MODULE_EXPORT void
cb_comboboxtext_preprocessedimages_changed(GtkComboBox *combobox, ProgramData *data)
{
	data->showPreprocessedImages();
}

extern "C" G_MODULE_EXPORT void
cb_radiobutton_preprocessedimages_toggled(GtkToggleButton* togglebutton, ProgramData *data)
{
	if(gtk_toggle_button_get_active(togglebutton))
		data->showPreprocessedImages();
}

extern "C" G_MODULE_EXPORT void
cb_checkbutton_showmaps_toggled(GtkToggleButton* togglebutton, ProgramData *data)
{
	data->showMaps(gtk_toggle_button_get_active(togglebutton));
}

extern "C" G_MODULE_EXPORT void
cb_comboboxtext_displacementmaps_changed(GtkComboBox *combobox, ProgramData *data)
{
	int activenum = gtk_combo_box_get_active(combobox);
	data->CurrentDisplacementMap = activenum;
   gtk_widget_queue_draw( data->DrawingArea_Images );
}

extern "C" G_MODULE_EXPORT void
cb_entrybuffer_projectname_inserted(GtkEntryBuffer *buffer, guint position, gchar *chars,
                                       guint n_chars, ProgramData *data)
{
   const gchar *texto = gtk_entry_buffer_get_text(data->EntryBuffer_ProjectName);
   string texto_aux;
   if(texto != NULL)
      texto_aux = texto;
   data->statusMessage("Project name read: " + texto_aux);

}

extern "C" G_MODULE_EXPORT void
cb_entrybuffer_projectname_deleted(GtkEntryBuffer *buffer, guint position, 
                                       guint n_chars, ProgramData *data)
{
   const gchar *texto = gtk_entry_buffer_get_text(data->EntryBuffer_ProjectName);
   string texto_aux;
   if(texto != NULL)
      texto_aux = texto;
   data->statusMessage("Project name read: " + texto_aux);

}

///////////////////Useful functions/////////////////////////////////////////////////////

string getFilenameFromURI(const string& path)
{
   if (path.empty()){
      cerr << "Error: getFilenameFromURI: empty URI/absolute path" << endl;
      return ""; 
   }   
   int index; 
   int slash_pos = 0;

   for(index = path.size() -1; index >= 0 ; --index)
      if(path[index] == '/'){
         slash_pos = index + 1;
         index = -3; 
      }   
   return path.substr(slash_pos);
}

///////////////////Main/////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]){
   GtkBuilder *builder;
   GError *error = NULL;
   auto data = make_DataManager(_AvailableImageFilters_, _AvailablePIVAlgorithms_);

   gtk_init (&argc, &argv);
   // Create new GtkBuilder object 
   builder = gtk_builder_new();
   // Load UI from file. If error occurs, report it and quit application.
   if( ! gtk_builder_add_from_file( builder, PIV_GUI_FILE, &error ) ) 
   {   
       g_warning( "%s", error->message );
       g_free( error );
       return( 1 );
   }   

   data.initialize_widgets(builder);
   /* Destroy builder, since we don't need it anymore */
   g_object_unref( G_OBJECT( builder ) );
   /* Show window. All other widgets are automatically shown */
   gtk_widget_show( data.Window_Main );
	
   /* Start main loop */
   gtk_main();

return( 0 );
}

