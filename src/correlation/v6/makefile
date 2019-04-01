BASEDIR = ../../
IMGFFTW = $(BASEDIR)ImageFFTW/lastVersion/
DBLFOR = $(BASEDIR)DobleFor/lastVersion/
ALGOR = $(BASEDIR)PIValgorithm/lastVersion/
DISPLMAP = $(BASEDIR)DisplacementMap/lastVersion/
#dependencias para el enlazador
LINKDEP = -lImageFFTW -lfftw3 -lm -lfreeimage -lpthread
#carpetas para buscar las cabeceras
INCLDIR = -iquote$(IMGFFTW) -iquote$(DBLFOR) -iquote$(ALGOR) -iquote$(DISPLMAP) 
#carpetas para buscar las bibliotecas
LINKDIR = -L$(IMGFFTW)

prueba: Correlation.o Correlation.h $(IMGFFTW)libImageFFTW.a prueba.cpp
	g++ -std=c++11 -Wall -o prueba prueba.cpp Correlation.o $(LINKDIR) $(LINKDEP) $(INCLDIR)
	echo "$(LINKDEP)" > ../lastVersion/lDependencies.txt
	echo "$(LINKDIR)" >> ../lastVersion/lDependencies.txt
	echo "$(INCLDIR)" >> ../lastVersion/lDependencies.txt

Correlation.o: Correlation.cpp Correlation.h $(IMGFFTW)libImageFFTW.a $(DBLFOR)DobleFor.h $(ALGOR)PIValgorithm.h $(DISPLMAP)DisplacementMap.h
	g++ -std=c++11 -Wall -c Correlation.cpp $(INCLDIR)
	cp Correlation.o ../lastVersion/libCorrelation.a
	cp Correlation.h ../lastVersion

