
#include <iostream>
#include "DisplacementMap.h"

using namespace std;

int main()
{
	DisplacementMap pruebaa(4, 5, 10, 10, 10, 10);
	ofstream arch("probando.txt");
	pruebaa.setInfoStr("nada por aqui");
	pruebaa.saveToStream(arch, 1., 1.);
	return 0;
}
