// use cubic spline method to get natural spline
// S''(x) = 0 at x0 and xn
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

// A points set in 2D space (x, y) * 3
// use atlest 3 points to construct a natural spline

// 3 splines, each spline has 4 coefficients
// S(xi) = ai + bi * x + ci * x^2 + di * x^3

//  coefficients

// cuz Si(x) = yi, so we can get ai = yi
// then we can get bi, ci, di
// bi = Si+1(x) - Si(x) - ci * (xi+1 - xi) - di * (xi+1 - xi)^2
// let Si(x) = ai + bi(x - xi) + ci(x - xi)^2 + di(x - xi)^3
// then Si'(x) = bi + 2ci(x - xi) + 3di(x - xi)^2
// and Si''(x) = 2ci + 6di(x - xi)

// cuz Si(xi) = yi, so ai = yi
// let hi = xi+1 - xi(delta x)
// cuz Si(xi+1) = yi+1, we can get ai + bihi + cihi^2 + dihi^3 = yi+1
// Si'(xi+1) = Si+1'(xi+1) = bi+1 + 2ci+1(xi+1 - xi) + 3di+1(xi+1 - xi)^2
// so we can get bi+1 = bi + 2cihi + 3dihi^2
// then we can get ci+1 = ci + 3dihi

// let mi = Si''(xi), then mi = 2ci + 6dihi
// di = (mi+1 - mi) / 6hi
// bi = (yi+1 - yi) / hi - hi/2* mi - hi/6 * (mi+1 - mi)

// take ai, bi, ci, di as coefficients in to bi + 2ci(x - xi) + 3di(x - xi)^2 = bi+1
// then we can get 
// hi * mi+1 + 2(hi + hi+1) * mi + hi+1 * mi+1 = 6 * (yi+1 - yi) / hi - 6 * (yi - yi-1) / hi-1
// for Mi!
// then we can get mi+1 = (6 * (yi+1 - yi) / hi - 6 * (yi - yi-1) / hi-1 - hi * mi) / (hi + hi+1)

// double x[] = {0, 1, 2};
// double y[] = {0, 1, 0};
struct NaturalSpline {
	double a, b, c, d, x;
};

vector<NaturalSpline> splineSolver(vector<double> x, vector<double> y) {
	int n = x.size();
	vector<double> h(n - 1);
	vector<double> alpha(n - 1);
	vector<double> l(n);
	vector<double> mu(n);
	vector<double> z(n);
	vector<double> c(n);
	vector<double> b(n);
	vector<double> d(n);
	vector<NaturalSpline> splines(n - 1);

	for (int i = 0; i < n - 1; i++) {
		h[i] = x[i + 1] - x[i];
	}

	for (int i = 1; i < n - 1; i++) {
		alpha[i] =
			(3 / h[i]) * (y[i + 1] - y[i]) - (3 / h[i - 1]) * (y[i] - y[i - 1]);
	}

	l[0] = 1;
	mu[0] = 0;
	z[0] = 0;

	for (int i = 1; i < n - 1; i++) {
		l[i] = 2 * (x[i + 1] - x[i - 1]) - h[i - 1] * mu[i - 1];
		mu[i] = h[i] / l[i];
		z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
	}

	l[n - 1] = 1;
	z[n - 1] = 0;
	c[n - 1] = 0;
    // print out the matrix
    for(int row = 0; row < n; row++){
        for(int col = 0; col < n; col++){
            if(row == col){
                // cout << l[row] << " ";
                printf("%5.2lf ", l[row]);
            }
            else if(row == col - 1){
                // cout << h[row] << " ";
                printf("%5.2lf ", h[row]);
            }
            else if(row == col + 1){
                // cout << h[col] << " ";
                printf("%5.2lf ", h[col]);
            }
            else{
                // cout << 0 << " ";
                double temp = 0;
                printf("%5.2lf ", temp);
            }
        }
        cout << endl;
    }
	for (int j = n - 2; j >= 0; j--) {
		c[j] = z[j] - mu[j] * c[j + 1];
		b[j] = (y[j + 1] - y[j]) / h[j] - h[j] * (c[j + 1] + 2 * c[j]) / 3;
		d[j] = (c[j + 1] - c[j]) / (3 * h[j]);
	}

	for (int i = 0; i < n - 1; i++) {
		splines[i].a = y[i];
		splines[i].b = b[i];
		splines[i].c = c[i];
		splines[i].d = d[i];
		splines[i].x = x[i];
	}
	return splines;
}
void printSpline(NaturalSpline splines){
  cout<< "x = " << splines.x << endl;
    cout << "S(x) = " << splines.a << " + " << splines.b << " * x + " << splines.c << " * x^2 + " << splines.d << " * x^3" << endl;
}
// Function to calculate the value of the polynomial
double calculatePolynomial(double x, NaturalSpline spline){
    double result = 0;
    result = spline.a + spline.b * x + spline.c * pow(x, 2) + spline.d * pow(x, 3);
    return result;
}
vector<double> xP;
vector<double> yP;
void drawSpline(vector<NaturalSpline> splines){
    int n = splines.size();
    double x = splines[0].x;
    double y = splines[0].a;
    double step = 0.01;
    while(x < splines[n-1].x){
        for(int i = 0; i < n; i++){
            if(x >= splines[i].x && x < splines[i+1].x){
                y = splines[i].a + splines[i].b * (x - splines[i].x) + splines[i].c * pow(x - splines[i].x, 2) + splines[i].d * pow(x - splines[i].x, 3);
                break;
            }
        }
        xP.push_back(x);
        yP.push_back(y);
        x += step;
    }
}
// int main() {
// //     8
// //   86.00    181.00
// //   177.00    211.00
// //   249.00    315.00
// //   311.00    460.00
// //   381.00    459.00
// //   416.00    311.00
// //   455.00    195.00
// //   551.00    155.00
//     double x[] = {86, 177, 249, 311, 381, 416, 455, 551};
//     double y[] = {181, 211, 315, 460, 459, 311, 195, 155};
//     vector<double> xV(x, x + sizeof(x) / sizeof(double));
//     vector<double> yV(y, y + sizeof(y) / sizeof(double));
//     vector<NaturalSpline> splines = splineSolver(xV, yV);
//     for(int i = 0; i < splines.size(); i++){
//         printSpline(splines[i]);
//     }
//     drawSpline(splines);
//     for(int i = 0; i < xP.size(); i++){
//         // cout << "x = " << xP[i] << " y = " << yP[i] << endl;
//     }
//     return 0;
// }


