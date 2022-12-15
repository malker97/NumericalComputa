#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

// Use Quadratic Regression to get the best fit line for the data a + bx + cx^2

vector<double> gaussianElimination(double matrix[3][4]){
    double temp;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(j != i){
                temp = matrix[j][i]/matrix[i][i];
                for(int k = 0; k < 4; k++){
                    matrix[j][k] -= matrix[i][k]*temp;
                }
            }
        }
    }
    vector<double> result;
    for(int i = 0; i < 3; i++){
        result.push_back(matrix[i][3]/matrix[i][i]);
    }
    return result;
}
vector<double> quadraticRegression(double x[], double y[], int n){
    double X, Y, XY, X2, X3, X4, X2Y;
    X = Y = XY = X2 = X3 = X4 = X2Y = 0;
    for(int i = 0; i < n; i++){
        X += x[i];
        Y += y[i];
        XY += x[i] * y[i];
        X2 += x[i] * x[i];
        X3 += x[i] * x[i] * x[i];
        X4 += x[i] * x[i] * x[i] * x[i];
        X2Y += x[i] * x[i] * y[i];
    }
    // a + bx + cx^2
    // a SumX4 + b SumX3 + c SumX2 = SumX2Y
    // a SumX3 + b SumX2 + c SumX = SumXY
    // a SumX2 + b SumX + c n = SumY
    double matrix[3][4];
    matrix[0][0] = X4, matrix[0][1] = X3, matrix[0][2] = X2, matrix[0][3] = X2Y;
    matrix[1][0] = X3, matrix[1][1] = X2, matrix[1][2] = X, matrix[1][3] = XY;
    matrix[2][0] = X2, matrix[2][1] = X, matrix[2][2] = n, matrix[2][3] = Y;
    // Gaussian Elimination
    vector<double> solution = gaussianElimination(matrix);
    reverse(solution.begin(), solution.end());
    return solution;
}
void printquadratic(vector<double> v){
    cout << "y = " << v[0] << " + " << v[1] << "x + " << v[2] << "x^2" << endl;
}
double quadratic(vector<double> v, double x){
    return v[0] + v[1] * x + v[2] * x * x;
}
vector<double> xP;
vector<double> yP;
void getPoints(vector<double> v){
    for(int i = 0; i < 610; i++){
        if(quadratic(v, i) >= 0 && quadratic(v, i) <= 600){
            xP.push_back(i);
            yP.push_back(quadratic(v, i));
        }
    }
}
// int main(){


//     return 0;
// }