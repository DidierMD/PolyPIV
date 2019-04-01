// prueba.cpp by Didier Muñoz
// Version 1 de PIValgorithm
// Esta biblioteca requiere de GTK+ 3 y de imageFFTW

#include "PIValgorithm.h" 
#include "../../ImageFFTW/lastVersion/ImageFFTW.h"
#include "../../DisplacementMap/lastVersion/DisplacementMap.h"

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

// Algoritmos individuales 
PIValgorithm <vector<RealImageFFTW>, vector<RealImageFFTW>, int, float> alg_too("Alg_1_lone", too, {"1_lone_int","1_lone_float"});
//auto alg_too = PIValgorithm("Alg_1_lone", too, {"1_lone_int","1_lone_float"});
PIValgorithm <vector<RealImageFFTW>, vector<RealImageFFTW>, string, int, double, unsigned, bool> alg_foo("Adioss", foo, {"2_lone_string", "2_lone_int","2_lone_double", "2_lone_unsigned", "2_lone_bool"});

//Variable que contiene los datos de los PIValgorithms
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

	cout << get<1>(_Algs_).name() << endl;

 	return 0;
}
