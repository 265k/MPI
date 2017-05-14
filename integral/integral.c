#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* A little convention that is used in this file:
   ll - lower limit
   ul - upper limit
   n - number of steps
   R - integration range
   f - target f
   s - sum
    */

double
function (double x)
{
  return exp(x);
}

double
rect (double (*f) (double), double ll, double ul, int n)
{
  double s = 0, R, dx;
  int i;
  R = ul - ll;
  dx = R / n;
  for (i = 0; i < n; i++)
    s += (*f) (ll + dx * i) * dx;
  return s;
}

double
trapez (double (*f) (double), double ll, double ul, int n)
{
  double s = 0, R, dx;
  int i;
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
  double (*f) (double);
  double ll = atof (argv[1]), ul = atof (argv[2]);
  int n = atoi (argv[3]);
  f = &function;

  printf ("Rectangles: %f\n Trapeze: %f\n",
	  rect (f, ll, ul, n), trapez (f, ll, ul, n));

  return 0;
}
