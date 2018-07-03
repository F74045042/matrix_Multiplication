1. naive matrix multiplication:

	file:
		naive.c

	compile:
	gcc-6 -o naive naive.c -fopenmp

	execute:
		./naive input/input??x??.txt   (e.g. input256x256.txt)
	
	output:
		non-parallel & parallel computation time
		generates "result.txt" (answer matrix)


2. strassen non-parallel:
	file:
		strassen.c
	
	compile:
		gcc-6 -o strassen strassen.c -fopenmp
	
	execute:
		./strassen input/input??x??.txt
	
	output:
		non-parallel computation time
		generates "result.txt" (answer matrix)

	
3. strassen parallel:
	file:
		strassen_para.c
	
	compile:
		gcc-6 -o strassen_para strassen_para.c -fopenmp
	
	execute:
		./strassen_para input/input??x??.txt
	
	output:
		parallel computation time
		generates "result.txt" (answer matrix)
