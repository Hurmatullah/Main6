#include "calculate_eigenvalues.h"
#include <random>


float** matrix(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0, 1);

    float** a = new float* [size];
    for (int i = 0; i < size; i++) {
        a[i] = new float[size];
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                a[i][j] = dist(gen);
            }
            else {
                a[i][j] = 0;
            }
        }
    }

    return a;
}

float* vector(int size) {
    float* v = new float[size];
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 10000000);

    for (int i = 0; i < size; i++) {
        v[i] = dist(gen);
    }

    return v;
}
