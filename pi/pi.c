#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
	int M = 0;
	int N = 9999999;
	long double x, y;
	double pi;
	time_t t;

	srand((unsigned) time(&t));

	for (int i = 0; i < N; i++){
		x = (2.0*rand()/RAND_MAX) - 1;
		y = (2.0*rand()/RAND_MAX) - 1;
		if (x*x + y*y <= 1.0) {
			M++;
		}
	}

	pi = 4.0*M/N;
	printf("pi = %f\n", pi);
	return 0;
}