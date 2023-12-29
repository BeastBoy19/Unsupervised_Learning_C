direct: EDA.o libEDA.a
EDA.o: EDA.cpp
	gcc -Wall -c EDA.cpp -o lib\EDA.o  -I include
libEDA.a: EDA.o
	ar cr lib\libEDA.a lib\EDA.o