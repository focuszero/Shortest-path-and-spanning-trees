import subprocess
from sys import argv
from pathlib import Path

# Le pasamos 1 argumento en la entrada que es que test queremos correr
# 1 = único test actual

entrada = ''
csv = ''
algoritmos = ''

nombre = argv[2]

# test matriciales
if argv[1] == '1':
	entrada = '../ej2/dataset/test_' + nombre +'.in'
	csv = '../ej2/csv/test_matriciales_' + nombre +'.csv'
	print('Corriendo el test matriciales')
	algoritmos = 'n,m,tiempo_Floyd_Warshall,tiempo_Dantzig'

# test dijkstras
if argv[1] == '2':
	entrada = '../ej2/dataset/test_' + nombre +'.in'
	csv = '../ej2/csv/test_dijkstra_' + nombre +'.csv'
	print('Corriendo el test dijkstra')
	algoritmos = 'n,m,tiempo_Dijkstra,tiempo_DijkstraPQ'

# test Bellman-Ford
if argv[1] == '3':
	entrada = '../ej2/dataset/test_' + nombre +'.in'
	csv = '../ej2/csv/test_Bellman-Ford_' + nombre +'.csv'
	print('Corriendo el test Bellman-Ford')
	algoritmos = 'n,m,tiempo_BellmanFord'



with open(entrada) as f:
    lines = f.readlines()

# Me fijo si el csv existe, sino lo creo
if not Path(csv).is_file():                         
    print ("creo el csv")
    file = open(str(csv), 'w')
    file.write(algoritmos + '\n')
    file.close()

counter = 1
for line in lines:

	if((counter % 1) == 0):
		print(counter)
    
	temporal_file = open('temporal_line', 'w')
	temporal_file.write(str(line))
	temporal_file.close()
	bashCommand = './../ej2/Ej2.o <temporal_line' + ' ' + argv[1]

	output = subprocess.check_output(['bash','-c', bashCommand], stderr= subprocess.STDOUT)

	out = output.decode('ascii')
	counter += 1
	with open(str(csv), 'a') as the_file:
		the_file.write(str(out) + '\n')
