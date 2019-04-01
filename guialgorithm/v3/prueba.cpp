// prueba.cpp by Didier Muñoz
// Version 3 de GUIAlgorithm
// Esta biblioteca requiere de GTK+ 3 y de imageFFTW

#include "GUIAlgorithm.h" 
#include "../../ImageFFTW/lastVersion/ImageFFTW.h"
#include "../../DisplacementMap/lastVersion/DisplacementMap.h"
#define PIV_GUI_FILE "gui.glade"

using namespace std;

//Función definida por el usuario
int too(const vector<RealImageFFTW>& _in, vector <RealImageFFTW>& _out, string& _str, int param1, float param2)
{
	cout << "too: 1_" << param1 << " 2_" << param2 << endl;
	return true;
}

int foo(const vector<RealImageFFTW>& _in, vector <RealImageFFTW>& _out, string& _str, string param1, int param2, double param3, unsigned int param4, bool param5)
{
	cout << "foo: 1_" << param1 << " 2_" << param2 << " 3_" << param3 << " 4_" << param4 << " 5_" << param5 << endl;
	return false;
}

int goo(const vector<RealImageFFTW>& _in, vector <DisplacementMap>& _out, string& _str, int param)
{
	cout << "goo: 1_" << param << endl;
	return false;
}

int hoo(const vector<RealImageFFTW>& _in, vector <DisplacementMap>& _out, string& _str, float param)
{
	cout << "hoo: 1_" << param << endl;
	return true;
}

PIValgorithm<vector<RealImageFFTW>, vector<RealImageFFTW>, int, float> alg_too("Alg_1_lone", too, {"1_lone_int","1_lone_float"});
//auto alg_too = Algorithm("Alg_1_lone", too, {"1_lone_int","1_lone_float"});
PIValgorithm<vector<RealImageFFTW>, vector<RealImageFFTW>, string, int, double, unsigned, bool> alg_foo("Adioss", foo, {"2_lone_string", "2_lone_int","2_lone_double", "2_lone_unsigned", "2_lone_bool"});

//Variable que contiene los datos de los Algorithms
//tuple <PIValgorithm<Alg_params...> ...algorithms_pack> _Algs_ = 
auto _Algs_ = make_tuple(PIValgorithm<vector<RealImageFFTW>, vector<RealImageFFTW>, int,float>("Algs11",too,{"Algs11_int","Algs11_float"}),
			 PIValgorithm<vector<RealImageFFTW>, vector<RealImageFFTW>, string,int,double, unsigned, bool>("Algs12",foo,{"Algs12_string","Algs12_int","Algs12_double"}));
auto _Algs2_ = make_tuple(PIValgorithm<vector<RealImageFFTW>, vector<DisplacementMap>, int>("Algs21",goo,{"Algs21_int"}), 
			PIValgorithm<vector<RealImageFFTW>, vector<DisplacementMap>, float>("Algs22",hoo,{"Algs22_float"}));

class Datas{
		const vector <RealImageFFTW> inputimgs;
		vector <RealImageFFTW> outputimgs;
		string outstring;
	public:
		const vector<RealImageFFTW>& in(void){ return inputimgs;};
		vector<RealImageFFTW>& out(void){ return outputimgs;};
		string& outStr(void){ return outstring;};
		void algorithmProcessed(int a){};
};

class Datas2{
		const vector <RealImageFFTW> inputimgs;
		vector <DisplacementMap> outputimgs;
		string outstring;
	public:
		const vector<RealImageFFTW>& in(void){ return inputimgs;};
		vector<DisplacementMap>& out(void){ return outputimgs;};
		string& outStr(void){ return outstring;};
		void algorithmProcessed(int a){};
};
//////////////Main

int main(int argc, char* argv[])
{
	Datas mydata;
	Datas2 mydata2;
	cout << "prueba GUIParam:" << endl;
	vector<string> auxvec = {"hola", "adios"};
	GUIParam <int, float> prueba_GUIParam;

	cout << "\nprueba GUIAlgorithm:" << endl;
	GUIAlgorithm<Datas, vector<RealImageFFTW>, vector<RealImageFFTW>, int, float>
																				 prueba_GUIAlgorithm(alg_too, mydata);
	cout << endl;
	//prueba_GUIAlgorithm.callFunction(Generator<decltype(prueba_GUIAlgorithm)::paramsSize()>::Type());
	
	cout << "\nprueba AlgorithmsInterface:" << endl;
	AlgorithmsInterface<Datas, vector<RealImageFFTW>, vector<RealImageFFTW>, 
			PIValgorithm<vector<RealImageFFTW>, vector<RealImageFFTW>, int, float>,
			PIValgorithm<vector<RealImageFFTW>, vector<RealImageFFTW>, string, int, double, unsigned, bool>> 
													prueba_AlgorithmsInterface(mydata, alg_too, alg_foo);
	cout << "\nprueba make_algorithms_from_tuple:" << endl;
	auto AlgosInter = make_algorithms_from_tuple  <vector<RealImageFFTW>, vector<DisplacementMap> >
						(mydata2, _Algs2_, Generator<tuple_size<decltype(_Algs2_)>::value>::Type());
	std::cout << endl;
   GtkBuilder *builder;
   GError *error = NULL;

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

	GtkWidget *ntbk_process = GTK_WIDGET(gtk_builder_get_object( builder, "notebook_process"));
	
	prueba_AlgorithmsInterface.generateGUI(GTK_NOTEBOOK(ntbk_process), "Pre-\nprocess");
	AlgosInter.generateGUI(GTK_NOTEBOOK(ntbk_process), "Post-\nprocess");

	GtkWidget *Window_Main = GTK_WIDGET(gtk_builder_get_object( builder, "main_window" ));
	g_signal_connect (Window_Main, "destroy", G_CALLBACK (gtk_main_quit), NULL);
   /* Destroy builder, since we don't need it anymore */
   g_object_unref( G_OBJECT( builder ) );
   /* Show window. All other widgets are automatically shown by GtkBuilder */
   gtk_widget_show( Window_Main );
   /* Start main loop */
   gtk_main();

 	return 0;
}
