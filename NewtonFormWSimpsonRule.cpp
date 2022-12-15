#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

struct point {
	double x;
	double y;
};
vector<point> toPoint(vector<double> x, vector<double> y) {
	vector<point> result;
	for (int i = 0; i < x.size(); i++) {
		point p;
		p.x = x[i];
		p.y = y[i];
		result.push_back(p);
	}
	return result;
}
vector<vector<point>> toSubset32(vector<point> points) {
	// It's Cubic first and then Quadratic
	vector<vector<point>> result;
	//
	int n = points.size();
	int k = n - 1;
	int cut3 = 0, cut2 = 0;
	if (k % 3 == 2) {
		cut3 = k / 3;
		cut2 = 1;
	} else if (k % 3 == 1) {
		cut3 = (k / 3) - 1;
		cut2 = 2;
	} else {
		cut3 = k / 3;
		cut2 = 0;
	}
	// cout << "n = " << n << endl;
	// cout << "k = " << k << endl;
	// cout << "cut3 = " << cut3 << endl;
	// cout << "cut2 = " << cut2 << endl;
	while (cut3) {
		vector<point> subset;
		for (int i = 0; i < 4; i++) {
			subset.push_back(points[i]);
		}
		result.push_back(subset);
		points.erase(points.begin(), points.begin() + 3);
		cut3--;
	}
	while (cut2) {
		vector<point> subset;
		for (int i = 0; i < 3; i++) {
			subset.push_back(points[i]);
		}
		result.push_back(subset);
		points.erase(points.begin(), points.begin() + 2);
		cut2--;
	}
	return result;
}
vector<vector<point>> toSubset23(vector<point> points) {
	// 23 meaning take 2 points fisrt and then 3 points
	// It's Quadratic first and then Cubic
	vector<vector<point>> result;
	//
	int n = points.size();
	// every subset has 3 points
	// if there is 2 points left we need make last two subsets have 4 points
	int k = n - 1;
	int cut2 = k / 2, cut3 = k % 2;
	while (cut2) {
		vector<point> subset;
		for (int i = 0; i < 3; i++) {
			subset.push_back(points[i]);
		}
		result.push_back(subset);
		points.erase(points.begin(), points.begin() + 2);
		cut2--;
	}
	if (cut3) {
		result.back().push_back(points[points.size() - 1]);
	}
	return result;
}
// f(x) = y0 + z10(x-x0) + z210(x-x0)(x-x1) + z3210(x-x0)(x-x1)(x-x2)
struct spline {
	// it can be quadratic or cubic
	// S(x) = a + bx + cx^2 + dx^3
	vector<double> coefficients;
	// if its cubic, the coefficients size should be 4
	// y0, z10, z210, z3210
	// if its quardic, the coefficients size should be 3
	// y0, z10, z210
	vector<point> points;
	double interval[2];
};
spline getSpline(vector<point> p) {
	spline result;
	int n = p.size();
	for (int i = 0; i < n; i++) {
		result.points.push_back(p[i]);
	}
	vector<double> z1(n - 1);
	// z10 z21 z32
	vector<double> z2(n - 2); // if its cubic spline z3 is empty
	// z210  z321
	vector<double> z3(n - 3);
	// z3210
	for (int i = 0; i < n - 1; i++) {
		z1[i] = (p[i + 1].y - p[i].y) / (p[i + 1].x - p[i].x);
	}
	for (int i = 0; i < n - 2; i++) {
		z2[i] = (z1[i + 1] - z1[i]) / (p[i + 2].x - p[i].x);
	}
	if (n == 4) {
		z3[0] = (z2[1] - z2[0]) / (p[3].x - p[0].x);
	}
	result.coefficients.push_back(p[0].y);
	result.coefficients.push_back(z1[0]);
	result.coefficients.push_back(z2[0]);
	if (n == 4) {
		result.coefficients.push_back(z3[0]);
	}
	result.interval[0] = p[0].x;
	result.interval[1] = p[n - 1].x;
	return result;
}
vector<spline> getSplines(vector<vector<point>> subsets) {
	vector<spline> result;
	for (int i = 0; i < subsets.size(); i++) {
		result.push_back(getSpline(subsets[i]));
	}
	return result;
}
double f(spline s, double x) {
	if (s.points.size() == 4)
		return s.coefficients[0] + s.coefficients[1] * (x - s.points[0].x) +
			s.coefficients[2] * (x - s.points[0].x) * (x - s.points[1].x) +
			s.coefficients[3] * (x - s.points[0].x) * (x - s.points[1].x) *
			(x - s.points[2].x);
	return s.coefficients[0] + s.coefficients[1] * (x - s.points[0].x) +
		s.coefficients[2] * (x - s.points[0].x) * (x - s.points[1].x);
}
// calculate the area of the spline
// Simpson's Rule
double areaCalc(spline s, double left, double right) {
	double a = left;
	double b = right;
	double y0 = f(s, a);
	double y1 = f(s, (a + b) / 2);
	double y2 = f(s, b);
	return (b - a) / 6 * (y0 + 4 * y1 + y2);
}
double antiCalc(spline s) {
	int left = s.interval[0];
	int right = s.interval[1];
	double result = 0;
	while (left < right) {
		result += areaCalc(s, left, left + 1);
		left++;
	}
	return result;
}
double riemannSum(spline s) {
	double result = 0;
	int left = s.interval[0];
	int right = s.interval[1];
	while (left < right) {
		result += abs(areaCalc(s, left, left + 1));
		left++;
	}
	return result;
}
double riemannSumCalc(vector<spline> splines) {
	double result = 0;
	for (int i = 0; i < splines.size(); i++) {
		result += riemannSum(splines[i]);
	}
	cout << "The area under the curve is " << result << " (riemann)" << endl;
	return result;
}
double antidiffsCalc(vector<spline> splines) {
	double result = 0;
	for (int i = 0; i < splines.size(); i++) {
		double temp = antiCalc(splines[i]);
		for (int j = 0; j < splines[i].points.size(); j++) {
			cout << splines[i].points[j].x << " " << splines[i].points[j].y
				 << endl;
		}
		result += temp;
		cout << " area of spline " << i << " is " << temp << endl;
	}
	// The area under the curve is 123249(antidiffs)
	cout << "The area under the curve is " << result << " (antidiffs)" << endl;
	return result;
}
vector<point> drawGraph(vector<spline> splines) {
	vector<point> result;
	for (auto s : splines) {
		int left = s.interval[0];
		int right = s.interval[1];
		while (left < right) {
			point temp;
			temp.x = left;
			temp.y = f(s, left);
			result.push_back(temp);
			left++;
		}
	}
	// add Auxiliary line to the graph
	// (x, y) -> (x, 0)
	return result;
}
vector<point> drawSpline(spline s){
    vector<point> result;
    int left = s.interval[0];
    int right = s.interval[1];
    while(left < right){
        point temp;
        temp.x = left;
        temp.y = f(s, left);
        result.push_back(temp);
        left++;
    }
    return result;
}
vector<vector<point>> drawSplines(vector<spline> splines){
    vector<vector<point>> result;
    for(auto s: splines){
        result.push_back(drawSpline(s));
    }
    return result;
}
// int main(){
//     // vector<double> x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//     // vector<double> y = {0, 0.8415, 0.9093, 0.1411, -0.7568, -0.9589,
//     -0.2794, 0.656, 0.9894, 0.4121, -0.5440};
// //   132.00    380.00
// //   258.00    284.00
// //   382.00    325.00

//     // vector<double> x = {132, 258, 382};
//     // vector<double> y = {380, 284, 325};
// //   115.00    284.00
// //   166.00    158.00
// //   252.00    296.00
// //   300.00    293.00
// //   357.00    254.00
// //   469.00    284.00

//     // vector<double> x = {115, 166, 252, 300, 357, 469};
//     // vector<double> y = {284, 158, 296, 293, 254, 284};

// //    80.00    334.00
// //    98.00    287.00
// //   122.00    270.00
// //   150.00    258.00
// //   177.00    257.00
// //   206.00    263.00
// //   230.00    278.00
// //   253.00    308.00
// //   286.00    339.00
// //   316.00    353.00
// //   365.00    344.00
// //   408.00    291.00
// //   467.00    251.00
// //   499.00    255.00

//     vector<double> x = {80, 98, 122, 150, 177, 206, 230, 253, 286, 316, 365,
//     408, 467, 499}; vector<double> y = {334, 287, 270, 258, 257, 263, 278,
//     308, 339, 353, 344, 291, 251, 255}; vector<point> points = toPoint(x, y);
//     vector<vector<point>> subsets = toSubset32(points);
//     vector<vector<point>> subsets23 = toSubset23(points);
//     vector<spline> splines, splines23;
//     splines = getSplines(subsets);
//     splines23 = getSplines(subsets23);
//     double result = antidiffsCalc(splines);
//     // the cubic pieces first integral is 123249.258390
//     cout << "the cubic pieces first integral is " << result << endl;
//     double result23 = antidiffsCalc(splines23);
//     // the quad pieces first integral is 122908.189379
//     cout << "the quad pieces first integral is " << result << endl;
//     return 0;
// }
