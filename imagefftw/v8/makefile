LDEPEND = -lfftw3 -lm -lfreeimage

prueba: prueba.cpp ImageFFTW.o ImageFFTW.h
	g++ -std=c++11 -Wall -o prueba prueba.cpp ImageFFTW.o $(LDEPEND)
	echo "$(LDEPEND)" > ../lastVersion/lDependencies.txt

ImageFFTW.o: ImageFFTW.cpp ImageFFTW.h
	g++ -std=c++11 -Wall -c ImageFFTW.cpp 
	cp ImageFFTW.o ../lastVersion/libImageFFTW.a
	cp ImageFFTW.h ../lastVersion
