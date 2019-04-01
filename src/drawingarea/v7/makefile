BASEDIR = ../../
IMGFFTW = $(BASEDIR)ImageFFTW/lastVersion/
DISPLMAP = $(BASEDIR)DisplacementMap/lastVersion/
#dependencias para el enlazador
LINKDEP = -lImageFFTW `pkg-config --cflags --libs gtk+-3.0 gmodule-2.0` -lfftw3 -lm -lfreeimage
#carpetas para buscar las cabeceras
INCLDIR = -iquote$(IMGFFTW) -iquote$(DISPLMAP)
#carpetas para buscar las bibliotecas
LINKDIR = -L$(IMGFFTW)

DAmain: DAmain.cpp DrawingArea.o $(IMGFFTW)libImageFFTW.a 
	g++ -std=c++11 -Wall -o DAmain DAmain.cpp DrawingArea.o $(LINKDIR) $(LINKDEP) $(INCLDIR)
	echo "$(LINKDEP)" > ../lastVersion/lDependencies.txt
	echo "$(LINKDIR)" >> ../lastVersion/lDependencies.txt
	echo "$(INCLDIR)" >> ../lastVersion/lDependencies.txt

DrawingArea.o: DrawingArea.cpp DrawingArea.h $(IMGFFTW)libImageFFTW.a $(DISPLMAP)DisplacementMap.h
	g++ -std=c++11 DrawingArea.cpp -Wall -c `pkg-config --cflags --libs gtk+-3.0` $(INCLDIR)
	cp DrawingArea.o ../lastVersion/libDrawingArea.a
	cp DrawingArea.h ../lastVersion
