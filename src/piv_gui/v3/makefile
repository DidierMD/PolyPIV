BASEDIR = ../../
IMGFFTW = $(BASEDIR)ImageFFTW/lastVersion/
DRWAREA = $(BASEDIR)DrawingArea/lastVersion/
#dependencias para el enlazador
LINKDEP = -lDrawingArea -lImageFFTW `pkg-config --cflags --libs gtk+-3.0 gmodule-2.0` -lfftw3 -lm -lfreeimage
#carpetas para buscar las cabeceras
INCLDIR = -iquote$(IMGFFTW) -iquote$(DRWAREA)
#carpetas para buscar las bibliotecas
LINKDIR = -L$(IMGFFTW) -L$(DRWAREA)

pruebaGlade: pruebaGlade.cpp $(DRWAREA)libDrawingArea.a $(IMGFFTW)libImageFFTW.a
	g++ -std=c++11 -Wall -o pruebaGlade pruebaGlade.cpp $(LINKDIR) $(LINKDEP) $(INCLDIR)
	echo "$(LINKDEP)" > ../lastVersion/lDependencies.txt
	echo "$(LINKDIR)" >> ../lastVersion/lDependencies.txt
	echo "$(INCLDIR)" >> ../lastVersion/lDependencies.txt

