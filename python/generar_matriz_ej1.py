import subprocess
import os
import random
from sys import argv

def generar_matriz(cant_elementos, cantidad_aristas_max):	
	n = cant_elementos
	k = cantidad_aristas_max
	
	elementos = []

	Matrix = [[0 for x in range(n+1)] for y in range(n+1)] 

	for x in range(2, n+1):

		numbers = [z for z in range(1, n)]
		anterior = random.randrange(1,x)
		Matrix[anterior][x] = random.randrange(1, 101)

		adyacencias = random.sample(numbers, random.randrange(1, k))

		for a in adyacencias:
			if a != x:
				lenght = random.randrange(1, 101)
				Matrix[x][a] = lenght
				Matrix[a][x] = lenght

	for a in range(1, n-1):
		for b in range(a, n+1):
			if Matrix[a][b] != 0:
				elementos.append([a, b, Matrix[a][b]])


	with open('../ej1/dataset/test_autogenerado.in', 'a') as file:
		file.write(str(n) + " " + str(len(elementos)) + " ")

		for i, j, a in elementos: 
			file.write(str(i) + " " + str(j) + " " + str(a) + " ")
		file.write("\n")


def generar_arboles(cant_elementos):	
	n = cant_elementos
	
	elementos = []

	for x in range(2, n+1):

		anterior = random.randrange(1,x)
		number = random.randrange(1, 101)
		
		elementos.append([x, anterior, number])

	with open('../ej1/dataset/test_arboles.in', 'a') as file:
		file.write(str(n) + " " + str(len(elementos)) + " ")

		for i, j, a in elementos: 
			file.write(str(i) + " " + str(j) + " " + str(a) + " ")
		file.write("\n")



def generar_Kn(cant_elementos):	
	n = cant_elementos
	
	elementos = []

	for a in range(1, n):
		for b in range(a+1, n+1):
			number = random.randrange(1, 101)
			elementos.append([a, b, number])


	with open('../ej1/dataset/test_Kn.in', 'a') as file:
		file.write(str(n) + " " + str(len(elementos)) + " ")

		for i, j, a in elementos: 
			file.write(str(i) + " " + str(j) + " " + str(a) + " ")
		file.write("\n")


def casos_random():
	for j in range(5,300,5):
		for _ in range(5):
			generar_matriz(j, random.randrange(2,j))	
			generar_Kn(j)

	for z in range(5, 1001, 5):
		for _ in range(5):
			generar_arboles(z)
casos_random()