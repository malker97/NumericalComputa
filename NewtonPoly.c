#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <complex.h>
#include <time.h>

#define maxIter 50

void printComplexNum (complex c){
  double a,b ;
  a = creal(c) ;
  b = cimag(c) ;
  if (b > 0) {
    printf("%20.16lf + %20.16lf I \n",a,fabs(b)) ;
  } else if(b < 0) {
    printf("%20.16lf - %20.16lf I \n",a,fabs(b)) ;
  }
  else {
    printf("%20.16lf \n",a) ;
  }
}
// Find all roots (using synthetic division)
complex * synthetic(complex root, complex * coefficients, int degree) {
  complex * b;
  b = (complex *)malloc(sizeof(complex) * degree);
  b[0] =coefficients[0];
  for(int i = 1; i < degree ; i++) {
      b[i] = (b[i-1] * root) + coefficients[i];
  }
  return b;
}

complex f(complex guess, complex * coefficients, int degree){
  complex * deflate, remain;
  deflate = synthetic(guess, coefficients, degree);
  remain = deflate[degree-1];
  return remain;
}

complex fPrime(complex guess, complex * coefficients, int degree){
  complex cofs[degree], * once, * twice;
  once = synthetic(guess, coefficients, degree);
  for(int i = 0; i < degree; ++i) {
      cofs[i] = once[i];
  }
  twice = synthetic(guess, once, degree);
  return twice[degree-2];
}

complex newtonMethod(complex guess, complex * coefficients, int degree){
  for (int i = 0; i < maxIter; i++)
    guess = guess - (f(guess, coefficients, degree)/(fPrime(guess, coefficients, degree)));
  printf("After %d iterations, the root is: ", maxIter);
  printComplexNum(guess);
  return guess;
}

int main()
{
  int n,k ;
  double v ;
  complex * coefficients ;

  printf("\nPlease enter the degree of the polynomial: ") ;
  scanf("%d",&n) ;
  n++;
  
  coefficients = (complex double *)malloc(sizeof(complex double)*n);
  printf("\nEnter the coefficients with constant term last\n") ;
  for (k = 0 ; k < n ; k++) {
    scanf("%lf",&v) ;
    coefficients[k] = v + I*0 ;
  }

  printf("\nThe coefficients are : \n") ;

  for(int i = 0; i < n; ++i) {
      if(i == 0) {
          printf("%lf", creal(coefficients[i]));
      } else if(i == 1) {
          printf(" + %lfx", creal(coefficients[i]));
      } else {
          printf(" + %lfx^%d", creal(coefficients[i]), i);
      }
  }
  printf("\n") ;
  
  int RANGE_MIN = 1;
  int RANGE_MAX = 10;

  double rp, ip ;
  complex g, root, roots[n];

  srand((unsigned int)time(NULL));
  rp = (((double) rand() /(double) RAND_MAX) * RANGE_MAX + RANGE_MIN);
  ip = (((double) rand() /(double) RAND_MAX) * RANGE_MAX + RANGE_MIN);

  g = rp + ip * I ;
  printf("\nThe initial guess is: ");
  printComplexNum(g) ;
  printf("\n\n") ;
  root = g;
  
  complex * deflated;
  complex * cur_cofs = (complex *)malloc(sizeof(complex) * n);
  cur_cofs = coefficients;
  for (int i = 0; i < n-1; ++i) {
      printf("\t Trying To Finding Root#%d \n", i);
      root = newtonMethod(root, cur_cofs, n-i);
      cur_cofs = synthetic(root, cur_cofs, n-i-1);
      roots[i] = root;
  }
  printf("\nThe roots of the polynomial:\n");
  for (int i = 0; i < n-1; ++i) {
      printComplexNum(roots[i]);
  }
  return 0;
}
