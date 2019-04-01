BASEDIR = ../../
IMGFFTW = $(BASEDIR)ImageFFTW/lastVersion/
DBLFOR = $(BASEDIR)DobleFor/lastVersion/
ALGOR = $(BASEDIR)PIValgorithm/lastVersion/
#dependencias para el enlazador
LINKDEP = -lImageFFTW -lfftw3 -lm -lfreeimage -lpthread
#carpetas para buscar las cabeceras
INCLDIR = -iquote$(IMGFFTW) -iquote$(DBLFOR) -iquote$(ALGOR) `pkg-config --cflags --libs gtk+-3.0  gmodule-2.0`
#carpetas para buscar las bibliotecas
LINKDIR = -L$(IMGFFTW) -L$(DBLFOR)

prueba: ImageFilters.o ImageFilters.h prueba.cpp
	g++ -std=c++11 -Wall -o prueba prueba.cpp ImageFilters.o $(LINKDIR) $(LINKDEP) $(INCLDIR)
	echo "$(LINKDEP)" > ../lastVersion/lDependencies.txt
	echo "$(LINKDIR)" >> ../lastVersion/lDependencies.txt
	echo "$(INCLDIR)" >> ../lastVersion/lDependencies.txt

ImageFilters.o: ImageFilters.cpp ImageFilters.h $(IMGFFTW)libImageFFTW.a $(DBLFOR)DobleFor.h $(ALGOR)PIValgorithm.h
	g++ -std=c++11 -Wall -c ImageFilters.cpp $(INCLDIR)
	cp ImageFilters.o ../lastVersion/libImageFilters.a
	cp ImageFilters.h ../lastVersion

