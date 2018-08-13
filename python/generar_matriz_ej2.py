import subprocess
import os
import random
from sys import argv

def generar_matriz(cant_elementos, cantidad_aristas_max):
	n = cant_elementos
	k = cantidad_aristas_max
	
	elementos = []
	nombre = argv[1]

	Matrix = [[0 for x in range(n+1)] for y in range(n+1)] 
	for x in range(2, n+1):

		numbers = [z for z in range(1, n)]
		anterior = random.randrange(1,x)
		Matrix[anterior][x] = random.randrange(60, 61)

		adyacencias = random.sample(numbers, k-1)

		for a in adyacencias:
		 	if a != x:
		 		lenght = random.randrange(1, 61)
		 		#lenght = 60
		 		Matrix[x][a] = lenght
		 		Matrix[a][x] = lenght

	for a in range(1, n-1):
		for b in range(a, n+1):
			if Matrix[a][b] != 0:
				elementos.append([a, b, Matrix[a][b]])

	with open('../ej2/dataset/test_' + nombre + '.in', 'a') as file:
		file.write(str(n) + " " + str(len(elementos)) + " ")

		for _ in range(n):
			precio = random.randrange(1, 101) 
			file.write(str(precio) + " ")

		for i, j, a in elementos: 
			file.write(str(i) + " " + str(j) + " " + str(a) + " ")
		file.write("\n")


def casos_random():
	for j in range(5,101):
		for _ in range(10):
			generar_matriz(j, random.randrange(4,j))

def casos_Kn():
	for j in range(20,101):
		for _ in range(1):
			generar_matriz(j, j)		

def casos_Arboles():
	for j in range(5,101):
		for _ in range(3):
			generar_matriz(j, 1)			

casos_Arboles()