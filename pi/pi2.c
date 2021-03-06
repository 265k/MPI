#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"


int
main (int argc, char **argv)
{
  int M = 0;
  int N = 9999999;
  double x, y;
  double pi;
  time_t t;
  int size, rank;
  int MT;
  int n;

  srand ((unsigned) time (&t));

  MPI_Init (&argc, &argv);

  MPI_Comm_size (MPI_COMM_WORLD, &size);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  n = N / size;
  for (int i = 0; i < n; i++)
    {
      x = ((2.0 * rand () / RAND_MAX) / size) - 1 + 2.0 * rank / size;
      y = (2.0 * rand () / RAND_MAX) - 1;
      if (x * x + y * y <= 1.0)
	{
	  M++;
	}
    }

  MPI_Reduce (&M, &MT, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0)
    {
      pi = 4.0 * MT / N;
      printf ("pi = %f\n", pi);
    }
    
  MPI_Finalize ();
  return 0;
}
