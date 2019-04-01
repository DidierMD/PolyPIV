// PolyPIV by Didier Muñoz
// Version 6
// Este programa requiere de DrawingArea PIV Correlation ImageFilters GUIAlgorithm
// La interfaz gafica de este programa esta descrita en un xml aparte

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <fstream>
#include <gtk/gtk.h>
#include "DrawingArea.h"
#include "PIV.h"
#include "Correlation.h"
#include "ImageFilters.h"
#include "ImageFFTW.h"

#define PIV_GUI_FILE "../../PIV_GUI/lastVersion/piv_gui.glade"

/////////////////////////////////////ProgramData//////////////////////////////////////

// La clase que se encarga de manejar el programa

class ProgramData{
   public:
      // Imágenes originales
		std::list<RealImageFFTW> OriginalImages;
      // Imágenes que se trabajarán
		std::list<RealImageFFTW> PreprocessedImages;
      // DrawingArea
      GtkWidget *DrawingArea_Images;
		DAPaintersManager DA_Originals;
      DAPaintersManager DA_Preprocessed;
		int CurrentImageNum;
		// Mapas de desplazamientos
		std::list<DisplacementMap> DisplacementMaps;
		int CurrentDisplacementMap;
      bool PintarFlechas;
      // Diálogo de guardado
      GtkWidget *Dialog_Save; 
      GtkWidget *CheckButton_Save_DispMap;
      GtkWidget *CheckButton_Save_VelMap;
      GtkWidget *CheckButton_Save_Images;
      // Diálogo cargar imagen
      GtkWidget *Dialog_Load;
      // Diálogo de confirmación de salida
      GtkWidget *Dialog_Quit;
      // Main Window
      GtkWidget *Window_Main;
      // About this project
      GtkEntryBuffer *EntryBuffer_ProjectName;
      GtkWidget *ComboBoxText_LoadedImages;
		GtkWidget *RadioButton_LoadedImages;
      GtkWidget *ComboBoxText_PreprocessedImages;
		GtkWidget *RadioButton_PreprocessedImages;
      GtkWidget *ComboBoxText_DisplacementMaps;
		GtkWidget *CheckButton_ShowMaps;
		GtkWidget *RadioButton_PreUseOriginals;
		GtkWidget *RadioButton_ProUseOriginals;
		GtkWidget *RadioButton_PreUsePreprocessed;
		GtkWidget *RadioButton_ProUsePreprocessed;
      GtkAdjustment *Adjustment_DeltaT;
      GtkAdjustment *Adjustment_ScaleFactor;
		// PIV
		GtkWidget *Notebook_PIV;
      // Statusbar
      GtkWidget *StatusBar_MainWindow;

      // Métodos
      ProgramData(void);
      ~ProgramData(void);
      
      void statusMessage(const std::string& message);

		void loadImageFromFilename(const std::string& _filename);
		std::list<RealImageFFTW>& proInImgs(void);
      std::list<RealImageFFTW>& preInImgs(void);
      void setDisplacementMaps(const std::list<DisplacementMap>& _outmaps);
      void setPreOutImgs(const std::list<RealImageFFTW>& _outimgs);
      void processed(void);
      void preProcessed(void);
		int saveVelocityMapToFile(const std::string& _filename, double _scalefactor, double _deltat = 1.);
		void drawDrawingArea(cairo_t* cr);
		void reset(void);
		void showOriginalImages(void);
		void showPreprocessedImages(void);
		void showMaps(bool do_show);

	protected:
		// This are functions to enable or disable the radio buttons associated with the workflow
		void disableUseLoadedImgs(void);
		void enableUseLoadedImgs(void);
		void disableUsePreprocessedImgs(void);
		void enableUsePreprocessedImgs(void);
      void disableShowDisplacementMaps(void);
      void enableShowDisplacementMaps(void);
};

template <typename ...>
struct TypePack{};

template<typename Typepack, typename ...Ts>
class DataManager;

template <typename... Ss, typename... Ts>
class DataManager<TypePack<Ss...>, Ts...> : public ProgramData{
	private:
		PIVPreprocessing<ProgramData, Ss...> PreprocessingManager;
		PIVProcessing<ProgramData, Ts...> ProcessingManager;
		//PIVPreprocessing<DataManager<Ss..., Ts...>, Ss...> PreprocessingManager;
		//PIVProcessing<DataManager<Ss..., Ts...>, Ts...> ProcessingManager;

 	public:
		DataManager(const std::tuple<Ss...>& _pre_algs, const std::tuple<Ts...>& _pro_algs);
      void initialize_widgets(GtkBuilder* _builder);
      void retrieve_widgets_from_builder(GtkBuilder *builder);
};

template <typename... Ss, typename... Ts>
DataManager<TypePack<Ss...>, Ts...> make_DataManager(const std::tuple<Ss...>& _pre_algs, 
																		const std::tuple<Ts...>& _pro_algs);

/////////////////////////Signals Callbacks////////////////////////////////////////////////////////

extern "C" G_MODULE_EXPORT gboolean 
cb_mainwindow_delete_event( GtkWidget *window, GdkEvent *event, ProgramData *data);

extern "C" G_MODULE_EXPORT void
cb_toolbutton_load_clicked( GtkButton *button, ProgramData *data);

extern "C" G_MODULE_EXPORT void
cb_toolbutton_save_clicked(GtkButton *button, ProgramData *data);

extern "C" G_MODULE_EXPORT void
cb_toolbutton_clear_clicked(GtkButton *button, ProgramData *data);

extern "C" G_MODULE_EXPORT gboolean 
cb_drawingarea_draw_event (GtkWidget *widget, cairo_t *cr, ProgramData *data);

extern "C" G_MODULE_EXPORT gboolean 
cb_drawingarea_configure_event (GtkWidget *widget, GdkEventConfigure *event, ProgramData *data);

extern "C" G_MODULE_EXPORT void
cb_comboboxtext_loadedimages_changed(GtkComboBox *combobox, ProgramData *data);

extern "C" G_MODULE_EXPORT void
cb_radiobutton_loadedimages_toggled(GtkToggleButton* togglebutton, ProgramData *data);

extern "C" G_MODULE_EXPORT void
cb_comboboxtext_preprocessedimages_changed(GtkComboBox *combobox, ProgramData *data);

extern "C" G_MODULE_EXPORT void
cb_radiobutton_preprocessedimages_toggled(GtkToggleButton* togglebutton, ProgramData *data);

extern "C" G_MODULE_EXPORT void
cb_comboboxtext_displacementmaps_changed(GtkComboBox *combobox, ProgramData *data);

extern "C" G_MODULE_EXPORT void
cb_entrybuffer_projectname_inserted(GtkEntryBuffer *buffer, guint position, gchar *chars,
                                       guint n_chars, ProgramData *data);
extern "C" G_MODULE_EXPORT void
cb_entrybuffer_projectname_deleted(GtkEntryBuffer *buffer, guint position,
                                       guint n_chars, ProgramData *data);

//////////////////////////Funciones útiles para el programa////////////////

std::string getFilenameFromURI(const std::string& path);

/* Cambios:
	This program allows to create better interfaces with the processing algorithms, which means, the GUI of the processing part is generated by the program itself depending on the avaiable algorithms. Uses GUIAlgorithm for that matter.
*/
