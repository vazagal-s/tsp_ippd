#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define DEBUG

int tsp(int matrix[51][51], int cities, int distance, int initial, int current,
		int ok[51], int total, int shortest) {

	int i, d;

	if (total == cities)
		return distance + matrix[current][initial];

	if (shortest < distance)
		return distance;

	for (i = 1; i <= cities; i++)
		if (i != current && !ok[i]) {
			ok[i] = 1;
			#pragma omp task firstprivate(distance,current,total,ok) shared(shortest)
			{
				d = tsp(matrix, cities, distance + matrix[current][i], initial, i,
						ok, total + 1, shortest);
				#pragma omp critical
				{
					if (d < shortest)
						shortest = d;
				}
				ok[i] = 0;
			}
		}
	#pragma omp taskwait
	return shortest;
}

int main(int argc, char *argv[]) {

	int c, e;
	int i, j, w, v;
	int matrix[51][51], ok[51], d, D;
	int k;

	memset(matrix, 0, sizeof(matrix));
	memset(ok, 0, sizeof(ok));

	FILE *file;
	if ((file = fopen("tsp.in", "r")) == NULL) {
		perror("tsp.in");
		exit(1);
	}

	if(fscanf(file, "%d%d", &c, &e)!= 2){
		printf("Error reading values from the file.\n");
        fclose(file); // Close the file before exiting
        return 1; // Exiting the program with an error code
	};



#ifdef DEBUG
	printf("size: %d\nedges: %d\n", c, e);
	fflush(stdout);
#endif

	for (k = 0; k < e; k++) {
		if(fscanf(file, "%d%d%d", &i, &j, &w) != 3){
			printf("Error on reading values from file");
		}
		matrix[i][j] = w;
		matrix[j][i] = w;
	}

	D = 1000000;
#pragma omp parallel
{		
	#pragma omp single
	{
		#pragma omp taskloop shared(D)
		for (v = 1; v <= c; v++) {

#ifdef DEBUG
		printf("running: %d\n", v);
		fflush(stdout);
#endif

			ok[v] = 1;
			d = tsp(matrix, c, 0, v, v, ok, 1, D);
			ok[v] = 0;
			#pragma omp critical
			{
			if (d < D)
				D = d;
			}
		}
	}
}
	fclose(file);

	if ((file = fopen("tsp.out", "w")) == NULL) {
		perror("tsp.out");
		exit(1);
	}
	fprintf(file, "%d\n", D);
	fclose(file);

	return EXIT_SUCCESS;
}
