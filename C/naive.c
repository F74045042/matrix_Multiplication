#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#define NUM_THREAD 16

int main(int argc, char* argv[]){
	int num, tmp, row, col;
	FILE *fp, *fp2;

	/*File open and read*/
	if((fp = fopen(argv[1], "r")) == NULL){
		printf("File opening error.\n");
		return -1;
	}
	
	/*First matrix*/
	fscanf(fp, "%d", &row);
	num = row;
	fscanf(fp, "%d", &col);
	tmp = col;
	//Malloc
	double **fMatrix = (double**)malloc(row * sizeof(double*));
	for(int i=0; i<row; i++)
		fMatrix[i] = (double*)malloc(col * sizeof(double));
	if(fMatrix == NULL){
		printf("<Error>Unable to allocate first matrix memory.\n");
	}
	//Insert number to matrix array
	printf("First Matrix:\n");
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			fscanf(fp, "%lf", &fMatrix[i][j]);
			//printf("%d ", fMatrix[i][j]);
		}
		//printf("\n");
	}


	/*Second matrix*/
	fscanf(fp, "%d", &row);
	if(col != row){
		printf("This two matrix can't multiply.\n");
		return -1;
	}
	fscanf(fp, "%d", &col);
	//Malloc
	double **sMatrix = (double**)malloc(row * sizeof(double*));
	for(int i=0; i<row; i++)
		sMatrix[i] = (double*)malloc(col * sizeof(double));
	if(sMatrix == NULL){
		printf("<Error>Unable to allocate second matrix memory.\n");
	}
	//Insert number to matrix array
	printf("Second Matrix:\n");
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			fscanf(fp, "%lf", &sMatrix[i][j]);
			//printf("%d ", sMatrix[i][j]);
		}
		//printf("\n");
	}

	/*Answer matrix*/
	//Malloc
	row = num;
	double **ansMatrix = (double**)malloc(row * sizeof(double*));
	for(int i=0; i<row; i++)
		ansMatrix[i] = (double*)malloc(col * sizeof(double));
	if(ansMatrix == NULL){
		printf("<Error>Unable to allocate answer matrix memory.\n");
	}

	/*Matrix multiplication*/
	printf("%d*%d matrix\n", row, col);
	double start_time, end_time;
	double total_time;

	/*Serial*/
	start_time = omp_get_wtime();
	for (int i=0; i < row; i++){
		for (int j=0; j < col; j++){
			for(int k=0; k<tmp; k++){
				ansMatrix[i][j] += fMatrix[i][k]*sMatrix[k][j];
			}
		}
	}
	end_time = omp_get_wtime();
	total_time = (end_time - start_time);
	printf("Serial Naive MM Time: %lf sec\n", total_time);

	/*Parallel*/
	start_time = omp_get_wtime();
	
	#pragma omp parallel for num_threads(NUM_THREAD)
	for (int i=0; i < row; i++){
		for (int k=0; k < col; k++){
			for(int j=0; j < tmp; j++){
				ansMatrix[i][j] += fMatrix[i][k]*sMatrix[k][j];
			}
		}
	}


	end_time = omp_get_wtime();
	total_time = (end_time - start_time);


	// printf("Answer Matrix:\n");
	// for(int i=0; i<row; i++){
	// 	for(int j=0; j<col; j++)
	// 		printf("%.0lf ", ansMatrix[i][j]);
	// 	printf("\n");
	// }
	/*write file*/
	fp2 = fopen("result.txt", "w");
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++)
			fprintf(fp2, "%.0lf ", ansMatrix[i][j]);
		fprintf(fp2, "\n");
	}

	fclose(fp2);



	printf("Parallel Naive MM Time: %lf sec\n", total_time);

	return 0;
}
