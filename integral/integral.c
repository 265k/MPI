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
  return exp (x);
}

double
rect (double (*f) (double), double ll, double ul, int n, int size, int rank)
{
  double s = 0, R, dx;
  int i;

  ul = ((rank + 1) * 1.0 / size) * ul;
  ll = ((rank + 1) * 1.0 / size) * ll;
  R = ul - ll;
  dx = R / n;

  for (i = 0; i < n; i++)
    s += (*f) (ll + dx * i) * dx;

  return s;
}

double
trapez (double (*f) (double), double ll, double ul, int n, int size, int rank)
{
  double s = 0, R, dx;
  int i;

  ul = ((rank + 1) * 1.0 / size) * ul;
  ll = ((rank + 1) * 1.0 / size) * ll;
  R = ul - ll;
  dx = R / n;

  s += 0.5 * ((*f) (ll) + (*f) (ul)) * dx;
  for (i = 1; i < n; i++)
    s += (*f) (ll + dx * i) * dx;

  return s;
}

int
main (int argc, char **argv)
{
  if (argc < 3)
    {
      printf
	("You must enter lower limit, upper limit and number of integration steps.\n");
      return 1;
    }
  int size, rank;
  double rp, tp, rt, tt;
  double (*f) (double);
  double ll = atof (argv[1]), ul = atof (argv[2]);
  int n = atoi (argv[3]);
  f = &function;

  MPI_Init (&argc, &argv);

  MPI_Comm_size (MPI_COMM_WORLD, &size);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  rp = rect (f, ll, ul, n, size, rank);
  tp = trapez (f, ll, ul, n, size, rank);

  //0 represents the master process
  MPI_Reduce (&rp, &rt, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Reduce (&tp, &tt, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0)
    {
      printf ("Rectangles: %f\n Trapeze: %f\n", rt, tt);
    }

  MPI_Finalize ();

  return 0;
}
