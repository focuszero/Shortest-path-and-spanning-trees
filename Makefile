make: Ejercicio1/Ej1.cpp Ejercicio2/Ej2.cpp
	g++ -std=c++11 Ejercicio1/Ej1.cpp -o Ejercicio1/Ej1.o
	g++ -std=c++11 Ejercicio2/Ej2.cpp -o Ejercicio2/Ej2.o
	
clean:
	rm -f *.o