BASEDIR = ../../
IMGFFTW = $(BASEDIR)ImageFFTW/lastVersion/
GUIALGOR = $(BASEDIR)GUIAlgorithm/lastVersion/
ALGOR = $(BASEDIR)PIValgorithm/lastVersion/
DISPLMAP = $(BASEDIR)DisplacementMap/lastVersion/
#dependencias para el enlazador
LINKDEP = -lImageFFTW -lfftw3 -lm -lfreeimage -lpthread
#carpetas para buscar las cabeceras
INCLDIR = -iquote$(IMGFFTW) -iquote$(GUIALGOR) -iquote$(ALGOR) -iquote$(DISPLMAP) `pkg-config --cflags --libs gtk+-3.0 gmodule-2.0`
#carpetas para buscar las bibliotecas
LINKDIR = -L$(IMGFFTW)

pruebaPIV: pruebaPIV.cpp PIV.h $(IMGFFTW)libImageFFTW.a $(GUIALGOR)GUIAlgorithm.h $(ALGOR)PIValgorithm.h $(DISPLMAP)DisplacementMap.h
	g++ -Wall -std=c++11 -o pruebaPIV pruebaPIV.cpp $(LINKDIR) $(LINKDEP) $(INCLDIR)
	echo "$(LINKDEP)" > ../lastVersion/lDependencies.txt
	echo "$(LINKDIR)" >> ../lastVersion/lDependencies.txt
	echo "$(INCLDIR)" >> ../lastVersion/lDependencies.txt
	cp PIV.h ../lastVersion
