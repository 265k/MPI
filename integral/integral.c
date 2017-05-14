#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

/* A little name convention that is used in this file:
   ll - lower limit
   ul - upper limit
   n - number of steps
   R - integration range
   f - target f
   ps - partial sum
   ts - total sum*/

double
function (double x)
{
  return exp(x);
}

double
rect (double (*f) (double), double ll, double ul, int n, int size, int rank)
{
  double ps = 0, ts = 0, R, dx;
  int i;

  ul = (rank * 1.0 / size) * ul;
  ll = (rank * 1.0 / size) * ll;
  R = ul - ll;
  dx = R / n;

  for (i = 0; i < n; i++)
    ps += (*f) (ll + dx * i) * dx;

  MPI_Reduce(&ps, &ts, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); //0 represents the master process

  return ts;
}

double
trapez (double (*f) (double), double ll, double ul, int n, int size, int rank)
{
  double ps = 0, ts = 0, R, dx;
  int i;
  
  ul = (rank * 1.0 / size) * ul;
  ll = (rank * 1.0 / size) * ll;
  R = ul - ll;
  dx = R / n;

  ps += 0.5 * ((*f) (ll) + (*f) (ul)) * dx;
  for (i = 1; i < n; i++)
    ps += (*f) (ll + dx * i) * dx;

  MPI_Reduce(&ps, &ts, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); //0 represents the master process

  return ts;
}

int
main (int argc, char **argv)
{
  if (argc < 3)
  {
    printf("You must enter lower limit, upper limit and number of integration steps.\n");
    return 1;
  }
  int size, rank;
  double (*f) (double);
  double ll = atof (argv[1]), ul = atof (argv[2]);
  int n = atoi (argv[3]);
  f = &function;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf ("Rectangles: %f\n Trapeze: %f\n",
	  rect (f, ll, ul, n, size, rank), trapez (f, ll, ul, n, size, rank));

  MPI_Finalize();
  return 0;
}
