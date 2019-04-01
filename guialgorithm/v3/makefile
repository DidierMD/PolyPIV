BASEDIR = ../../
ALGOR = $(BASEDIR)PIValgorithm/lastVersion/
#dependencias para el enlazador
LINKDEP = `pkg-config --cflags --libs gtk+-3.0 gmodule-2.0`
#carpetas para buscar las cabeceras
INCLDIR = -iquote$(ALGOR)
#carpetas para buscar las bibliotecas
LINKDIR = 

prueba: prueba.cpp GUIAlgorithm.h $(ALGOR)PIValgorithm.h
	g++ -std=c++11 -Wall -o prueba prueba.cpp $(LINKDIR) $(LINKDEP) $(INCLDIR)
	echo "$(LINKDEP)" > ../lastVersion/lDependencies.txt
	echo "$(LINKDIR)" >> ../lastVersion/lDependencies.txt
	echo "$(INCLDIR)" >> ../lastVersion/lDependencies.txt
	cp GUIAlgorithm.h ../lastVersion
