import sys
import numpy as np
import time
import multiprocessing

def ijkMul(A, B, C):
	for i in range(A.shape[0]):
		for j in range(B.shape[1]):
			for k in range(A.shape[1]):
				C[i][j] += A[i][k] * B[k][j]
	return C

def parallelMul(first):
	for i in range(first, first+part):
		for j in range(B.shape[1]):
			for k in range(A.shape[1]):
				C[i][j] += A[i][k] * B[k][j]
	return C

def optimize(first):
	for i in range(first, first+part):
		for k in range(B.shape[1]):
			for j in range(A.shape[1]):
				C[i][j] += A[i][k] * B[k][j]
	return C

def main():
	global part, A, B, C
	A = np.genfromtxt(sys.argv[1])
	B = np.genfromtxt(sys.argv[2])

	#print input matrix
	print("Matrix A:")
	print(A)
	print("Matrix B:")
	print(B)
	
	#numpy.dot
	start = time.time()
	C = np.dot(A, B)
	end = time.time()
	#print(C)
	#print the time result
	str = "Numpy.dot: %f sec" %(end - start)
	print(str)

	#clean matrix C
	C.fill(0)

	#naive
	start = time.time()
	C = ijkMul(A, B, C)
	end = time.time()
	#print(C)
	#print the time result
	str = "Naive: %f sec" %(end - start)
	print(str)

	#clean matrix C
	C.fill(0)

	#Parallel
	part = int(A.shape[0] / multiprocessing.cpu_count())
	if part < 1:
		part = 1
	start = time.time()
	pool = multiprocessing.Pool()
	pool.map(parallelMul, range(0, A.shape[0], part))
	end = time.time()
	#print(C)
	#print the time result
	str = "Parallel: %f sec" %(end - start)
	print(str)

	#Optimize
	part = int(A.shape[0] / multiprocessing.cpu_count())
	if part < 1:
		part = 1
	start = time.time()
	pool = multiprocessing.Pool()
	pool.map(optimize, range(0, A.shape[0], part))
	end = time.time()
	#print(C)
	#print the time result
	str = "Optimize: %f sec" %(end - start)
	print(str)

	#Print Ans Matrix
	# print("Ans Matrix:")
	# print(C)

if __name__ == "__main__":
	main()