#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[]){
	int num = atoi(argv[1]);
	
	srand(time(NULL));
	FILE* fp;

	/*File Open*/
	if((fp = fopen("./input.txt", "w")) == NULL){
		printf("File opening error.\n");
		return -1;
	}else{
		printf("File open correctly.\n");
	}

	printf("%d*%d matrix generating.\n", num, num);

	/*First Matrix*/
	fprintf(fp, "%d %d\n", num, num);
	for(int i=0; i<num; i++){
		for (int j=0; j<num; j++){
			fprintf(fp, "%d ", (rand()%100)+1);
		}
		fprintf(fp, "\n");
	}

	/*Second Matrix*/
	fprintf(fp, "%d %d\n", num, num);
	for(int i=0; i<num; i++){
		for (int j=0; j<num; j++){
			fprintf(fp, "%d ", (rand()%100)+1);
		}
		fprintf(fp, "\n");
	}

	printf("Generate complete.\n");
}