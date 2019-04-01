BASEDIR = ../../
#dependencias para el enlazador
LINKDEP = 
#carpetas para buscar las cabeceras
INCLDIR = 
#carpetas para buscar las bibliotecas
LINKDIR = 

prueba: DisplacementMap.h prueba.cpp
	g++ -std=c++11 -Wall -o prueba prueba.cpp $(LINKDIR) $(LINKDEP) $(INCLDIR)
	echo "$(LINKDEP)" > ../lastVersion/lDependencies.txt
	echo "$(LINKDIR)" >> ../lastVersion/lDependencies.txt
	echo "$(INCLDIR)" >> ../lastVersion/lDependencies.txt
	cp DisplacementMap.h ../lastVersion

