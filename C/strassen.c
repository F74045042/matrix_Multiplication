#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>
int row;


void strassen_matmul(int size, double **fMat, double **sMat, double **ansMat);
void mallocMem(double ***mat, int size);
void freeMem(double ***mat, int size);
void matADD(int size, double **fMat, double **sMat, double **ansMat);
void matSUB(int size, double **fMat, double **sMat, double **ansMat);
void outputMatrix(int size, double **Mat);
void mat_mul(int size, double **fMat, double **sMat, double **ansMat);

int main(int argc, char *argv[]){

	int num, tmp, col;
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

	double start_time, end_time, total_time;
	start_time = omp_get_wtime();

	/*Strassen Mateix Multiplication*/
	strassen_matmul(row, fMatrix, sMatrix, ansMatrix);

	end_time = omp_get_wtime();
	total_time = end_time - start_time;

	/*result*/
	//printf("The Answer Matrix is: \n");
	//outputMatrix(row, ansMatrix);

	printf("Serial Strassen's Algorithm: %lf sec.\n", total_time);


	/*write file*/
	fp2 = fopen("result.txt", "w");
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++)
			fprintf(fp2, "%.0lf ", ansMatrix[i][j]);
		fprintf(fp2, "\n");
	}

	fclose(fp2);

	freeMem(&fMatrix, row);
	freeMem(&sMatrix, row);
	freeMem(&ansMatrix, row);

	return 0;
}

void strassen_matmul(int size, double **fMat, double **sMat, double **ansMat)
{

	double **f11, **f12, **f21, **f22;
	double **s11, **s12, **s21, **s22;
	double **ans11, **ans12, **ans21, **ans22;
	double **M1, **M2, **M3, **M4, **M5, **M6, **M7;
	double **tmpfMat, **tmpsMat;

	if(size <= (row/4)){
		mat_mul(size, fMat, sMat, ansMat);
	}
	else{
		// malloc space for the matrices above
		mallocMem(&f11, size);
		mallocMem(&f12, size);
		mallocMem(&f21, size);
		mallocMem(&f22, size);

		mallocMem(&s11, size);
		mallocMem(&s12, size);
		mallocMem(&s21, size);
		mallocMem(&s22, size);

		mallocMem(&ans11, size);
		mallocMem(&ans12, size);
		mallocMem(&ans21, size);
		mallocMem(&ans22, size);

		mallocMem(&M1, size);
		mallocMem(&M2, size);
		mallocMem(&M3, size);
		mallocMem(&M4, size);
		mallocMem(&M5, size);
		mallocMem(&M6, size);
		mallocMem(&M7, size);

		mallocMem(&tmpfMat, size);
		mallocMem(&tmpsMat, size);

		// split fMat and sMat into 4 submatrices
		for(int i=0; i<size / 2; i++){
			for(int j=0; j<size / 2; j++){
				f11[i][j] = fMat[i][j];
				f12[i][j] = fMat[i][j + size/2];
				f21[i][j] = fMat[i + size/2][j];
				f22[i][j] = fMat[i + size/2][j + size/2];

				s11[i][j] = sMat[i][j];
				s12[i][j] = sMat[i][j + size/2];
				s21[i][j] = sMat[i + size/2][j];
				s22[i][j] = sMat[i + size/2][j + size/2];
			}
		}

		// M1 = (f11 + f22) * (s11 + s22)
		matADD(size/2, f11, f22, tmpfMat);
		matADD(size/2, s11, s22, tmpsMat);
		strassen_matmul(size/2, tmpfMat, tmpsMat, M1);

		// M2 = (f21 + f22) * s11
		matADD(size/2, f21, f22, tmpfMat);
		strassen_matmul(size/2, tmpfMat, s11, M2);

		// M3 = f11 * (s12 - s22)
		matSUB(size/2, s12, s22, tmpsMat);
		strassen_matmul(size/2, f11, tmpsMat, M3);

		// M4 = f22 * (s21 - s11);
		matSUB(size/2, s21, s11, tmpsMat);
		strassen_matmul(size/2, f22, tmpsMat, M4);

		// M5 = (f11 + f12) * s22
		matADD(size/2, f11, f12, tmpfMat);
		strassen_matmul(size/2, tmpfMat, s22, M5);

		// M6 = (f21 - f11) * (s11 + s12)
		matSUB(size/2, f21, f11, tmpfMat);
		matADD(size/2, s11, s12, tmpsMat);
		strassen_matmul(size/2, tmpfMat, tmpsMat, M6);

		// M7 = (f12 - f22) * (s21 + s22)
		matSUB(size/2, f12, f22, tmpfMat);
		matADD(size/2, s21, s22, tmpsMat);
		strassen_matmul(size/2, tmpfMat, tmpsMat, M7);


		// ans11 = M1 + M4 - M5 + M7
		matADD(size/2, M1, M4, tmpfMat);
		matSUB(size/2, M7, M5, tmpsMat);
		matADD(size/2, tmpfMat, tmpsMat, ans11);

		// ans12 = M3 + M5
		matADD(size/2, M3, M5, ans12);

		// ans21 = M2 + M4
		matADD(size/2, M2, M4, ans21);

		// ans22 = M1 - M2 + M3 + M6
		matADD(size/2, M3, M6, tmpfMat);
		matSUB(size/2, M1, M2, tmpsMat);
		matADD(size/2, tmpfMat, tmpsMat, ans22);


		// put result back into ansMatrix
		for(int i=0; i<size/2; i++){
			for(int j=0; j<size/2; j++){
				ansMat[i][j] = ans11[i][j];
				ansMat[i][j + size/2] = ans12[i][j];
				ansMat[i + size/2][j] = ans21[i][j];
				ansMat[i + size/2][j + size/2] = ans22[i][j];        
			}
		}


		// free memory

		freeMem(&f11, size);
		freeMem(&f12, size);
		freeMem(&f21, size);
		freeMem(&f22, size);

		freeMem(&s11, size);
		freeMem(&s12, size);
		freeMem(&s21, size);
		freeMem(&s22, size);

		freeMem(&ans11, size);
		freeMem(&ans12, size);
		freeMem(&ans21, size);
		freeMem(&ans22, size);

		freeMem(&M1, size);
		freeMem(&M2, size);
		freeMem(&M3, size);
		freeMem(&M4, size);
		freeMem(&M5, size);
		freeMem(&M6, size);
		freeMem(&M7, size);

		freeMem(&tmpfMat, size);
		freeMem(&tmpsMat, size);

	}

}

void mallocMem(double ***mat, int size)
{
	*mat = (double **)calloc(size, sizeof(double *));
	for(int i=0; i<size; i++)
		(*mat)[i] = (double *)calloc(size, sizeof(double));
	if((*mat) == NULL)
		printf("<ERROR> Unable to calloc for matrix memory.\n");
}

void freeMem(double ***mat, int size)
{
	double **m = *mat;
	for(int i=0; i<size; i++){
		free(m[i]);
	}
	free(m);
}

void matADD(int size, double **fMat, double **sMat, double **ansMat)
{
	for(int i=0; i<size; i++)
		for(int j=0; j<size; j++)
			ansMat[i][j] = fMat[i][j] + sMat[i][j];
}

void matSUB(int size, double **fMat, double **sMat, double **ansMat)
{
	for(int i=0; i<size; i++)
		for(int j=0; j<size; j++)
			ansMat[i][j] = fMat[i][j] - sMat[i][j];
}

void outputMatrix(int size, double **mat){
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++)
			printf("%.0lf ", mat[i][j]);
		printf("\n");
	}
}

void mat_mul(int size, double **fMat, double **sMat, double **ansMat){
	for(int i=0; i<size; i++){
		for(int k=0; k<size; k++){
			for(int j=0; j<size; j++){
				ansMat[i][j] += fMat[i][k] * sMat[k][j];
			}
		}

	}
}
