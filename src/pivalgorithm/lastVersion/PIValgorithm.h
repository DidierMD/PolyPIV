// PIValgorithm.h by Didier Muñoz
// Version 1
//

/* Esta clase se utiliza para generar el "empaquetado" de los algoritmos disponibles para el programa. Es la clase que "almacena y define" un algoritmo*/

#ifndef ALGO_RITHM
#define ALGO_RITHM

#include <iostream>
#include <vector>
#include <string>

/////////////PIValgorithm

template <typename Tin, typename Tout, typename...Args>
class PIValgorithm{
	private:
		int (* FuncPtr)(const Tin&, Tout&, std::string&, Args...);
		const std::string Name;
		const std::vector<std::string> ParametersNames;
	
	public:
		PIValgorithm(const std::string& nam, 
					int (* f_ptr)(const Tin&, Tout&, std::string&, Args... params), 
					const std::vector<std::string>& _p_names) : Name(nam), ParametersNames(_p_names)  {
			FuncPtr = f_ptr;
		}
		const std::string& name(void) const{
			return Name;
		}
		int (*function(void) const)(const Tin&, Tout&, std::string&, Args...){
			return FuncPtr;
		}
		const std::vector<std::string>& parametersNames(void) const{
			return ParametersNames;
		}
};

#endif
