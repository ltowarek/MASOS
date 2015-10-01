#include <iostream>
#include <vector>
#include <gmp.h>

using namespace std;

int main() {

	mpf_t f;

    int d;
	cin >> d;

	float sum = 0;

	/***Input***/
    vector<float> xn;
    float x;
    while (cin >> x) {
        xn.push_back(x);
		sum += x;
    }

	int n = xn.size();

#ifdef _DEBUG
	cout << "N: " << n << endl;
#endif 

	/***Exercise 1***/
	float average = sum / n;

#ifdef _DEBUG
	cout << "AVG: ";
#endif 
	cout << average << endl;

	/***Exercise 2***/
	float first = 0;
	float second = 0;
	
	for (int i = 0; i < n; i++) {
		first += pow(xn[i], 2);
		second += xn[i];
	}
	
	first /= n;
	second /= n;
	second = pow(second, 2);

	float variance = first - second;

#ifdef _DEBUG
	cout << "VARIANCE: ";
#endif 
	cout << variance << endl;

	/***Exercise 3***/
	int p, p_min = p = n;

	while (--p) {
		bool valid = true;
		for (int k = 0; k < n - p; k++) {
			if (xn[k] != xn[k+p]) {
				valid = false;
				break;
			}
		}
		if (valid) {
			p_min = p;
		}
	}

#ifdef _DEBUG
	cout << "Perdiod: ";
#endif 
	cout << p_min << endl;

    return 0;
}
