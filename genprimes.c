#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]){
	double tstart = 0.0, tend=0.0, ttaken;
	FILE * fp;
	char output[100] = "";
	
	int n = atoi(argv[1]);
	int numbers[n+1];
	int thread_count = atoi(argv[2]);
	//printf("thread_count: %d", thread_count);
	#pragma omp parallel for
	for(int i = 2; i<=n; i++)
		numbers[i] = i;
	
	tstart = omp_get_wtime(); 
	#pragma omp parallel num_threads(thread_count)
	for(int i=2; i<((n+1)/2); i++){
		if(numbers[i] != 0){
			#pragma omp for \ 
			schedule(dynamic, 10)
			for(int j=2; j<=n/i; j++){
				if(numbers[j*i]!=0)
					numbers[j*i]=0;
			}
		}
	}
	
	ttaken = omp_get_wtime()-tstart;
	printf("Time take for the main part: %f\n", ttaken);
	
	sprintf(output,"%d.txt",n);
	fp = fopen(output,"w");
	if(!fp){
		printf("Cannot create the file %s\n", output);
		exit(1);
	}
	
	int a = 1;
	int prevB=2;
	fprintf(fp, "%d %d %d\n", a, 2, 0);
    for(int b = 3; b <= n; b++){
		if(numbers[b]!=0){
			a++;
			fprintf(fp,"%d %d %d\n", a, b, b-prevB);
			prevB = b;
		}
	}
	fclose(fp);
	exit(0);
}