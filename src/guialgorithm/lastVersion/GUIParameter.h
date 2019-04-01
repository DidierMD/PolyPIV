// GUIParameter.h by Didier
// Version 3
// Esta biblioteca requiere de GTK+ 3 y de imageFFTW

#ifndef GUIPARAMETER
#define GUIPARAMETER

#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <gtk/gtk.h>
#include "imageFFTW.h"

using namespace std;

/////////////Algoritmo

template <typename...Args>
class Algoritmo{
	private:
		int (* FuncPtr)(const vector<RealImageFFTW>& _in, vector<RealImageFFTW>& _out, Args...);
		const string Name;
		const vector<std::string> ParametersNames;
	
	public:
		Algoritmo(const string& nam, 
					int (* f_ptr)(const vector<RealImageFFTW>& _in, vector<RealImageFFTW>& _out,Args... params), 
					const vector<string>& _p_names) : Name(nam), ParametersNames(_p_names)  {
			FuncPtr = f_ptr;
		}
		const string& name(void) const{
			return Name;
		}
		int (*function(void) const)(const vector<RealImageFFTW>& _in, vector<RealImageFFTW>& _out, Args...){
			return FuncPtr;
		}
		const vector<std::string>& parametersNames(void) const{
			return ParametersNames;
		}
};

//////////////GUIParam

template <typename ...Ts> class GUIParam {};

template <typename ...Ts>
class GUIParam<bool, Ts...> : public GUIParam<Ts...> {
	private:
		GtkWidget* ToggleButton;

	public:	
         // Initializing with the father constructor
		GUIParam(void) : GUIParam<Ts...>() {
			cerr << "bool "; ToggleButton = NULL;
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
			cerr << "unsigned "; SpinButton = NULL;
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
			cerr << "int "; SpinButton = NULL;
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
			cerr << "float "; SpinButton = NULL;
		}
		void generateGUI(const std::string& name_, GtkGrid* grid_){
			// Create Widgets
			GtkAdjustment* adjustment = gtk_adjustment_new(0.0, -1000000.0, 1000000.0, 0.001, 1.0, 0.0);
			SpinButton = gtk_spin_button_new (adjustment, 0.01, 3);
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
			cerr << "double "; SpinButton = NULL;
		}
		void generateGUI(const std::string& name_, GtkGrid* grid_){
			// Create Widgets
			GtkAdjustment* adjustment = gtk_adjustment_new(0.0, -1000000.0, 1000000.0, 0.00001, 1.0, 0.0);
			SpinButton = gtk_spin_button_new (adjustment, 0.0001, 5);
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
			cerr << "string "; Entry = NULL;
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

//generateGUI for GUIParam

template <size_t K, typename T, typename... Ts>
void generate_GUI(vector<string>& names_, typename std::enable_if<K!=0, GtkGrid>::type *grid_, 
						GUIParam<T, Ts...> &params_) {
	GUIParam<Ts...>& following_params(params_);
	generate_GUI<K-1>(names_, grid_, following_params);
	std::string auxstr;
	if(names_.empty()){
		std::cerr << "generate_GUI: names vector is empty" << endl;
	}
	else{
		auxstr = names_.back();
		names_.pop_back();
	}
	params_.generateGUI(auxstr, grid_);
}

template <size_t K, typename... Ts>
void generate_GUI(vector<string>& names_, typename std::enable_if<K==0, GtkGrid>::type *grid_, 
						GUIParam<Ts...> &params_) {
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

template <typename D, typename ...Ts>
class GUIAlgorithm{
	private:
		string Name;
		int (* FunctionPointer)(const vector<RealImageFFTW>&, vector<RealImageFFTW>&, Ts...);
		GUIParam <Ts...> Parameters;
		std::vector <std::string> ParametersNames;
		const std::vector <RealImageFFTW>& InputImgs;
		std::vector <RealImageFFTW>& OutputImgs;

		static void cbProcessButtonClicked(GtkWidget* button_, void* data_){
			static_cast<GUIAlgorithm<D, Ts...>*>(data_) -> callFunction(typename Generator<paramsSize()>::Type());
		}

	public:
		constexpr static size_t paramsSize(void){
			return sizeof...(Ts);
		}

		GUIAlgorithm(const Algoritmo<Ts...> &alg, D& _data) 
						: InputImgs(_data.inImgs()), OutputImgs(_data.outImgs()) {
			Name = alg.name();
			cout << "Algorithm: " << Name << " ";
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
			GtkWidget* process_button = gtk_button_new_with_label("Process");
			g_signal_connect (process_button, "clicked", G_CALLBACK (cbProcessButtonClicked), this);
			
			gtk_grid_attach_next_to (GTK_GRID(grid), process_button, NULL, GTK_POS_TOP, 2, 1);

			generate_GUI<paramsSize()>(ParametersNames, GTK_GRID(grid), Parameters);
			
			gtk_container_add(GTK_CONTAINER(scrolled_win), grid);
			GtkWidget *LabelName = gtk_label_new (Name.c_str());
			gtk_notebook_append_page(notebook_, scrolled_win, LabelName);
		}

		template <int ...Seq>
		void callFunction (const Sequence<Seq...>& seq_){
			FunctionPointer(InputImgs, OutputImgs, get<Seq>(Parameters)...);
		}
};

//////////////AlgorithmsInterface

template <typename D, typename ...Ts> 
class AlgorithmsInterface {
	public:
		AlgorithmsInterface(D& _data, Ts... ts){};
		void generateGUI(GtkNotebook* main_notebook_, const std::string& name_){}
		void generateGUI(GtkNotebook* notebook_){}
};

template <typename D, typename...Us, typename ...Ts>
class AlgorithmsInterface <D, Algoritmo<Us...>, Ts...> : AlgorithmsInterface<D, Ts...>{
	private:
		GUIAlgorithm <D, Us...> ActualGUIAlgorithm;

	public:
		AlgorithmsInterface(D& _data, const Algoritmo<Us...> &algor, Ts... ts) : 
						AlgorithmsInterface<D, Ts...>(_data, ts...), 
						ActualGUIAlgorithm(algor, _data) {};
		
		void generateGUI(GtkNotebook* main_notebook_, const std::string& name_){
			GtkWidget *ntbk = gtk_notebook_new (); 
   		gtk_notebook_set_scrollable(GTK_NOTEBOOK(ntbk), true);
   		generateGUI(GTK_NOTEBOOK(ntbk));
   		GtkWidget *page_lbl = gtk_label_new(name_.c_str());
   		gtk_notebook_append_page(main_notebook_, ntbk, page_lbl);
			gtk_widget_show_all(ntbk);
		}

		void generateGUI(GtkNotebook* notebook_){
			ActualGUIAlgorithm.generateGUI(notebook_);
			AlgorithmsInterface<D, Ts...>& following_algorithms(*this);
			following_algorithms.generateGUI(notebook_);
		}
};

template <typename D, typename ...Ts, int ...Seq>
AlgorithmsInterface<D, Ts...> make_algorithms_from_tuple(D& _data, 
																		const tuple<Ts...>& _params, Sequence<Seq...> _seq){
	return AlgorithmsInterface<D, Ts...>(_data, get<Seq>(_params)...);
}

#endif
