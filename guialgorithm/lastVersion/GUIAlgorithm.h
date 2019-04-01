// GUIAlgorithm.h by Didier Muñoz
// Version 3
// Esta biblioteca requiere de GTK+ 3 PIValgorithm

#ifndef GUIALGORITHM
#define GUIALGORITHM

#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <chrono>
#include "PIValgorithm.h"
#include <gtk/gtk.h>

//////////////GUIParam

template <typename ...Ts> class GUIParam {};

template <typename ...Ts>
class GUIParam<bool, Ts...> : public GUIParam<Ts...> {
	private:
		GtkWidget* ToggleButton;

	public:	
         // Initializing with the father constructor
		GUIParam(void) : GUIParam<Ts...>() {
			ToggleButton = NULL;
		}
		void generateGUI(const std::string& name_, GtkGrid* grid_){
			// Create Widgets
			ToggleButton = gtk_check_button_new();
			GtkWidget* lbl_name = gtk_label_new(name_.c_str());
			// Add Widgets to grid
			gtk_grid_attach_next_to (grid_, lbl_name, NULL, GTK_POS_TOP, 1, 1);
			gtk_grid_attach_next_to (grid_, ToggleButton, lbl_name, GTK_POS_RIGHT, 1, 1);
		}
		bool value(void) const {
			return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ToggleButton));
		}
};

template <typename ...Ts>
class GUIParam<unsigned int, Ts...> : public GUIParam<Ts...> {
	private:
		GtkWidget* SpinButton;

	public:	
         // Initializing with the father constructor
		GUIParam(void) : GUIParam<Ts...>() {
			SpinButton = NULL;
		}
		void generateGUI(const std::string& name_, GtkGrid* grid_){
			// Create Widgets
			GtkAdjustment* adjustment = gtk_adjustment_new(0.0, 0.0, G_MAXUINT, 1.0, 5.0, 0.0);
			SpinButton = gtk_spin_button_new (adjustment, 1.0, 0);
			GtkWidget* lbl_name = gtk_label_new(name_.c_str());
			// Add Widgets to grid
			gtk_grid_attach_next_to (grid_, lbl_name, NULL, GTK_POS_TOP, 1, 1);
			gtk_grid_attach_next_to (grid_, SpinButton, lbl_name, GTK_POS_RIGHT, 1, 1);
		}
		float value(void) const {
			return gtk_spin_button_get_value(GTK_SPIN_BUTTON(SpinButton));
		}
};

template <typename ...Ts>
class GUIParam<int, Ts...> : public GUIParam<Ts...> {
	private:
		GtkWidget* SpinButton;

	public:	
         // Initializing with the father constructor
		GUIParam(void) : GUIParam<Ts...>() {
			SpinButton = NULL;
		}
		void generateGUI(const std::string& name_, GtkGrid* grid_){
			// Create Widgets
			GtkAdjustment* adjustment = gtk_adjustment_new(0.0, G_MININT, G_MAXINT, 1.0, 5.0, 0.0);
			SpinButton = gtk_spin_button_new (adjustment, 1.0, 0);
			GtkWidget* lbl_name = gtk_label_new(name_.c_str());
			// Add Widgets to grid
			gtk_grid_attach_next_to (grid_, lbl_name, NULL, GTK_POS_TOP, 1, 1);
			gtk_grid_attach_next_to (grid_, SpinButton, lbl_name, GTK_POS_RIGHT, 1, 1);
		}
		// For retrieving the actual value
		int value(void) const {
			return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(SpinButton));
		}
};

template <typename ...Ts>
class GUIParam<float, Ts...> : public GUIParam<Ts...> {
	private:
		GtkWidget* SpinButton;

	public:	
         // Initializing with the father constructor
		GUIParam(void) : GUIParam<Ts...>() {
			SpinButton = NULL;
		}
		void generateGUI(const std::string& name_, GtkGrid* grid_){
			// Create Widgets
			GtkAdjustment* adjustment = gtk_adjustment_new(0.0, -999999.0, 999999.0, 0.001, 1.0, 0.0);
			SpinButton = gtk_spin_button_new (adjustment, 0.005, 6);
			//SpinButton = gtk_spin_button_new_with_range (-1000000.0, 1000000.0, 0.0000001);
			GtkWidget* lbl_name = gtk_label_new(name_.c_str());
			// Add Widgets to grid
			gtk_grid_attach_next_to (grid_, lbl_name, NULL, GTK_POS_TOP, 1, 1);
			gtk_grid_attach_next_to (grid_, SpinButton, lbl_name, GTK_POS_RIGHT, 1, 1);
		}
		float value(void) const {
			return gtk_spin_button_get_value(GTK_SPIN_BUTTON(SpinButton));
		}
};

template <typename ...Ts>
class GUIParam<double, Ts...> : public GUIParam<Ts...> {
	private:
		GtkWidget* SpinButton;

	public:	
         // Initializing with the father constructor
		GUIParam(void) : GUIParam<Ts...>() {
			SpinButton = NULL;
		}
		void generateGUI(const std::string& name_, GtkGrid* grid_){
			// Create Widgets
			GtkAdjustment* adjustment = gtk_adjustment_new(0.0, -999999.0, 999999.0, 0.000001, 1.0, 0.0);
			SpinButton = gtk_spin_button_new (adjustment, 0.000009, 8);
			GtkWidget* lbl_name = gtk_label_new(name_.c_str());
			// Add Widgets to grid
			gtk_grid_attach_next_to (grid_, lbl_name, NULL, GTK_POS_TOP, 1, 1);
			gtk_grid_attach_next_to (grid_, SpinButton, lbl_name, GTK_POS_RIGHT, 1, 1);
		}
		float value(void) const {
			return gtk_spin_button_get_value(GTK_SPIN_BUTTON(SpinButton));
		}
};

template <typename ...Ts>
class GUIParam<std::string, Ts...> : public GUIParam<Ts...> {
	private:
		GtkWidget* Entry;

	public:	
         // Initializing with the father constructor
		GUIParam(void) : GUIParam<Ts...>() {
			Entry = NULL;
		}
		void generateGUI(const std::string& name_, GtkGrid* grid_){
			// Create Widgets
			Entry = gtk_entry_new();
			GtkWidget* lbl_name = gtk_label_new(name_.c_str());
			// Add Widgets to grid
			gtk_grid_attach_next_to (grid_, lbl_name, NULL, GTK_POS_TOP, 1, 1);
			gtk_grid_attach_next_to (grid_, Entry, lbl_name, GTK_POS_RIGHT, 1, 1);
		}
		// For retrieving the actual value
		std::string value(void) const {
			return std::string(gtk_entry_get_text(GTK_ENTRY(Entry)));
		}
};

//GUIParamType

template <size_t, typename > 
struct GUIParamType;

template <size_t K, typename T, typename ...Ts>
struct GUIParamType<K, GUIParam<T, Ts...> >{
	typedef typename GUIParamType<K-1, GUIParam<Ts...> >::Type Type;
};

template <typename T, typename ...Ts>
struct GUIParamType<0, GUIParam<T, Ts...> > {
	typedef T Type;
};

//Get for GUIParam

template <size_t K, typename T, typename... Ts>
typename std::enable_if<K != 0, typename GUIParamType<K, GUIParam<T, Ts...> >::Type>::type
get(GUIParam<T, Ts...>& params_) {
  GUIParam<Ts...>& following_params(params_);
  return get<K - 1>(following_params);
}

template <size_t K, typename... Ts>
typename std::enable_if<K==0, typename GUIParamType<0, GUIParam<Ts...> >::Type>::type  
get(GUIParam<Ts...>& params_) {
   return params_.value();
}

//generate_GUI for GUIParam

template <typename... Ts>
void generate_GUI(std::vector<std::string>& names_, 
                  GtkGrid *grid_, 
						GUIParam<Ts...> &params_) {
}

template <typename T, typename... Ts>
void generate_GUI(std::vector<std::string>& names_, 
                  GtkGrid *grid_, 
						GUIParam<T, Ts...> &params_) {
	GUIParam<Ts...>& following_params(params_);
	generate_GUI(names_, grid_, following_params);
	std::string auxstr;
	if(names_.empty()){
		std::cerr << "generate_GUI: names vector is empty" << std::endl;
	}
	else{
		auxstr = names_.back();
		names_.pop_back();
	}
	params_.generateGUI(auxstr, grid_);
}

//////////////Sequence

//Helper struct to "unpack" tuples/GUIParams
template <int ...> 
struct Sequence{};

//////////////Generator

//Helper struct to generate a Sequence
template <int N, int ...Seq> 
struct Generator : Generator<N-1, N-1, Seq...> {};

template <int ...Seq> 
struct Generator <0, Seq...>{ 
	typedef Sequence<Seq...> Type; 
};

//////////////GUIAlgorithm

template <typename D, typename Fin, typename Fout, typename ...Ts>
class GUIAlgorithm{
	private:
		// About the algorithm
		std::string Name;
		int (* FunctionPointer)(const Fin&, Fout&, std::string&, Ts...);
		GUIParam <Ts...> Parameters;
		std::vector <std::string> ParametersNames;
		// Reference to the data class, preferably a class with this class as a member
      // or like in this case as a member of a member.
		D& Data;

		static void cbProcessButtonClicked(GtkWidget* button_, void* data_){
			static_cast<GUIAlgorithm<D, Fin, Fout, Ts...>*>(data_) -> 
													callFunction(typename Generator<paramsSize()>::Type());
		}
		constexpr static size_t paramsSize(void){
			return sizeof...(Ts);
		}

	public:

		GUIAlgorithm(const PIValgorithm<Fin, Fout, Ts...> &alg, D& _data) 
						: Data(_data) {
			Name = alg.name();
			FunctionPointer = alg.function();
			ParametersNames = alg.parametersNames();
		}

		void generateGUI (GtkNotebook* notebook_){
			//ScrolledWin
			GtkWidget* scrolled_win = gtk_scrolled_window_new(NULL, NULL);
			//Grid
			GtkWidget* grid = gtk_grid_new ();
			gtk_grid_set_row_homogeneous (GTK_GRID(grid), true);
			gtk_grid_set_column_spacing (GTK_GRID(grid), 4);
			//Process button
			GtkWidget* process_button = gtk_button_new_with_label("      Process      ");
			g_signal_connect (process_button, "clicked", G_CALLBACK (cbProcessButtonClicked), this);
			
			gtk_grid_attach_next_to (GTK_GRID(grid), process_button, NULL, GTK_POS_TOP, 2, 1);

			generate_GUI(ParametersNames, GTK_GRID(grid), Parameters);
			
			gtk_container_add(GTK_CONTAINER(scrolled_win), grid);
         //gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_win), grid);
			GtkWidget *LabelName = gtk_label_new (Name.c_str());
			gtk_notebook_append_page(notebook_, scrolled_win, LabelName);
		}

		template <int ...Seq>
		void callFunction (const Sequence<Seq...>& seq_){
			std::cout << "Executing: " + Name << std::endl;
			auto start_time = std::chrono::high_resolution_clock::now();
			int success = FunctionPointer(Data.in(),Data.out(), Data.outStr(), get<Seq>(Parameters)...);
			auto end_time = std::chrono::high_resolution_clock::now();
   		std::cout << std::chrono::duration_cast<
                                     std::chrono::microseconds>(end_time-start_time).count()
         			 <<" microseconds" << std::endl;
			
			Data.algorithmProcessed(success);
		}
};

//////////////AlgorithmsInterface

template <typename D, typename Fin, typename Fout, typename ...Ts> 
class AlgorithmsInterface {
	public:
		AlgorithmsInterface(D& _data, Ts... ts){};
		void generateGUI(GtkNotebook* main_notebook_, const std::string& name_){}
		void generateGUI(GtkNotebook* notebook_){}
};

template <typename D, typename Fin, typename Fout, typename...Us, typename ...Ts>
class AlgorithmsInterface <D, Fin, Fout, PIValgorithm<Fin, Fout, Us...>, Ts...> 
									: AlgorithmsInterface<D, Fin, Fout, Ts...>{
	private:
		GUIAlgorithm <D, Fin, Fout, Us...> ActualGUIAlgorithm;

	protected:
		void generateGUI(GtkNotebook* notebook_){
			ActualGUIAlgorithm.generateGUI(notebook_);
         AlgorithmsInterface<D, Fin, Fout, Ts...>::generateGUI(notebook_);
		}

	public:
		AlgorithmsInterface(D& _data, const PIValgorithm<Fin, Fout, Us...> &algor, Ts... ts) : 
						AlgorithmsInterface<D, Fin, Fout, Ts...>(_data, ts...), 
						ActualGUIAlgorithm(algor, _data) {};
		
		void generateGUI(GtkNotebook* main_notebook_, const std::string& name_){
			GtkWidget *ntbk = gtk_notebook_new (); 
   		gtk_notebook_set_scrollable(GTK_NOTEBOOK(ntbk), true);
   		generateGUI(GTK_NOTEBOOK(ntbk)); // Generates all Algorithms GUI
   		GtkWidget *page_lbl = gtk_label_new(name_.c_str());
   		gtk_notebook_append_page(main_notebook_, ntbk, page_lbl);
			gtk_widget_show_all(ntbk);
		}

};

template <typename Fin, typename Fout, typename D, typename ...Ts, int ...Seq>
AlgorithmsInterface<D, Fin, Fout, Ts...> 
make_algorithms_from_tuple( D& _data, const std::tuple<Ts...>& _algs, Sequence<Seq...> _seq) 
{
	return AlgorithmsInterface<D, Fin, Fout, Ts...>(_data, std::get<Seq>(_algs)...);
}

#endif
