BASEDIR = ../../
IMGFFTW = $(BASEDIR)ImageFFTW/lastVersion/
IMGCORR = $(BASEDIR)Correlation/lastVersion/
DRWAREA = $(BASEDIR)DrawingArea/lastVersion/
PIVDIR = $(BASEDIR)PIV/lastVersion/
DBLFOR = $(BASEDIR)DobleFor/lastVersion/
ALGOR = $(BASEDIR)PIValgorithm/lastVersion/
GUIALGOR = $(BASEDIR)GUIAlgorithm/lastVersion/
DISPLMAP = $(BASEDIR)DisplacementMap/lastVersion/
IMGFLTRS = $(BASEDIR)ImageFilters/lastVersion/
#dependencias para el enlazador
LINKDEP = -lDrawingArea -lCorrelation -lImageFilters -lImageFFTW `pkg-config --cflags --libs gtk+-3.0 gmodule-2.0` -lfftw3 -lm -lfreeimage -lpthread
#carpetas para buscar las cabeceras
INCLDIR = -iquote$(IMGFFTW) -iquote$(IMGCORR) -iquote$(IMGFLTRS) -iquote$(DRWAREA) -iquote$(PIVDIR) -iquote$(DBLFOR) -iquote$(ALGOR) -iquote$(GUIALGOR) -iquote$(DISPLMAP)
#carpetas para buscar las bibliotecas
LINKDIR = -L$(IMGFFTW) -L$(IMGCORR) -L$(IMGFLTRS) -L$(DRWAREA) 

PolyPIV: PolyPIV.cpp PolyPIV.h $(DRWAREA)libDrawingArea.a $(PIVDIR)PIV.h $(IMGCORR)libCorrelation.a $(IMGFFTW)libImageFFTW.a $(IMGFLTRS)libImageFilters.a  $(DBLFOR)DobleFor.h $(ALGOR)PIValgorithm.h $(GUIALGOR)GUIAlgorithm.h $(DISPLMAP)DisplacementMap.h
	g++ -std=c++11 -Wall -o PolyPIV PolyPIV.cpp $(LINKDIR) $(LINKDEP) $(INCLDIR)
	echo "$(LINKDEP)" > ../lastVersion/lDependencies.txt
	echo "$(LINKDIR)" >> ../lastVersion/lDependencies.txt
	echo "$(INCLDIR)" >> ../lastVersion/lDependencies.txt
	cp PolyPIV ../lastVersion/

