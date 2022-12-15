#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;


// the input should be x[], y[]
// x[i] and y[i] should be the one point
// the output should be the points set, that need to be connected, to make a convex hull
// the output should be a vector of pair<double, double>
// the 1st element of output is the point at the leftmost

vector<pair<double, double>> giftWrapping(vector<double> x, vector<double> y){
    vector<pair<double, double>> result;
    int n = x.size();
    int leftmost = 0;
    for(int i = 1; i < n; i++){
        if(x[i] < x[leftmost]){
            leftmost = i;
        }
    }
    int p = leftmost;
    do{
        result.push_back(make_pair(x[p], y[p]));
        int q = (p + 1) % n;
        for(int i = 0; i < n; i++){
            if((x[i] - x[p]) * (y[q] - y[p]) - (x[q] - x[p]) * (y[i] - y[p]) > 0){
                q = i;
            }
        }
        p = q;
    }while(p != leftmost);
    return result;    
}

// int main(){
//     vector<double> x = {0, 1, 2, 4, 0, 1, 3};
//     vector<double> y = {3, 1, 2, 4, 0, 1, 3};
//     vector<pair<double, double>> result = giftWrapping(x, y);
//     for(int i = 0; i < result.size(); i++){
//         cout << result[i].first << " " << result[i].second << endl;
//     }
//     return 0;
// }