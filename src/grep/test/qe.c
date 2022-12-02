/*
 * =====================================================================================
 *
 *       Filename:  qe.c
 *
 *    Description: from book 
 *
 *        Version:  1.0
 *        Created:  12/02/2022 12:18:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <math.h>

double discrim(double a, double b, double c)
{
  return b*b - 4*a*c;
}

int main(void)
{
  double p, q, r, d;
  int n;
  n = scanf("%lf %lf %lf", &p, &q, &r);
  if (3 != n) {
    printf("Error: wrong input.\n");
    return 1;
  }
  if (0 == p) {
    printf("Error: Not a quadratic equation!\n");
    return 2;
  }
  d = discrim(p, q, r);
  if (d < 0) {
    printf("No roots\n");
    return 0;
  }
  d = sqrt(d);
  printf("%.5lf %.5lf\n", (-q-d)/(2*p), (-q+d)/(2*p));
  return 0;
}

