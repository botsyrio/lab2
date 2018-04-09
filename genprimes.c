#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]){
	double tstart = 0.0, tend=0.0, ttaken;
	FILE * fp;
	char output[100] = "";
	
	int n = atoi(argv[1]);
	int oddsLength = (n-2)/2;
	int odds[oddsLength];//if odds[i]=1, this means the number represented by i*2+3 is currently noted as a possible prime.
	int thread_count = atoi(argv[2]);
	//printf("thread_count: %d", thread_count);
	#pragma omp parallel for
	for(int i = 1; i<oddsLength; i++)//we can optimize by taking out all the evens besides 2
		odds[i] = 1;
	
	tstart = omp_get_wtime(); 
	#pragma omp parallel for num_threads(thread_count) schedule(dynamic, 10)
	for(int i=1; i<(oddsLength); i++){
		if(odds[i] != 0){
			for(int j=3; j*i<oddsLength; j+=2){
				if(odds[j*i]!=0)
					odds[j*i]=0;
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
	int b;
	fprintf(fp, "%d %d %d\n", a, 2, 0);
    for(int i = 1; i <= oddsLength; i++){
		if(odds[i]!=0){
			b=i*2+2;
			a++;
			fprintf(fp,"%d %d %d\n", a, b, b-prevB);
			prevB = b;
		}
	}
	fclose(fp);
	exit(0);
}