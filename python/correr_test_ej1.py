import subprocess
from sys import argv
from pathlib import Path

# Le pasamos 1 argumento en la entrada que es que test queremos correr
# 1 = test_arboles
# 2 = test_autogenerado
# 3 = test_Kn
entrada = ''
csv = ''

if argv[1] == '1':
	entrada = '../ej1/dataset/test_arboles.in'
	csv = '../ej1/csv/test_arboles.csv'
	print('Corriendo el test de árboles')

if argv[1] == '2':
	entrada = '../ej1/dataset/test_autogenerado.in'
	csv = '../ej1/csv/test_autogenerado.csv'
	print('Corriendo el test autogenerado')

if argv[1] == '3':
	entrada = '../ej1/dataset/test_Kn.in'
	csv = '../ej1/csv/test_Kn.csv'
	print('Corriendo el test Kn')

with open(entrada) as f:
    lines = f.readlines()

# Me fijo si el csv existe, sino lo creo
if not Path(csv).is_file():                         
	print ("creo el csv")
	file = open(str(csv), 'w')
	file.write('n,m,tiempo_Prim,tiempo_Kruskal,tiempo_Kruskal_PC,iguales' + '\n')
	file.close()

counter = 1
for line in lines:

	if((counter % 10) == 0):
		print(counter)

	temporal_file = open('temporal_line', 'w')
	temporal_file.write(str(line))
	temporal_file.close()
	bashCommand = './../ej1/Ej1.o <temporal_line'

	output = subprocess.check_output(['bash','-c', bashCommand], stderr= subprocess.STDOUT)

	out = output.decode('ascii')
	counter += 1
	with open(str(csv), 'a') as the_file:
		the_file.write(str(out) + '\n')