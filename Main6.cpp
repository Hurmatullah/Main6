#include <iostream>
#include <fstream>
#include <omp.h>
#include <cstring>
#include "calculate_eigenvalues.h"

// Creating simple iterative method based on our formula
float* simpleMode(float** a, float* b, int n, int iterations = 20) {

    float** H = new float* [n];
    for (int i = 0; i < n; i++) {
        H[i] = new float[n];
    }
    float* g = new float[n];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                H[i][j] = 0;
            }
            else {
                H[i][j] = -1 * a[i][j] / a[i][i];
            }
        }
        g[i] = b[i] / a[i][i];
    }
    float* x = new float[n];

    for (int k = 0; k < iterations; k++) {
        float* new_x = new float[n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                new_x[i] += H[i][j] * x[j];
            }
            new_x[i] += g[i];
        }
        x = new_x;
    }
    return x;
}

// Creating parallel mode to get speed to our process using OpenMP
float* parallelMode(float** a, float* b, int n, int iterations = 20, int num_threads = 3) {

    float** H = new float* [n];
    for (int i = 0; i < n; i++) {
        H[i] = new float[n];
    }
    float* g = new float[n];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                H[i][j] = 0;
            }
            else {
                H[i][j] = -1 * a[i][j] / a[i][i];
            }
        }
        g[i] = b[i] / a[i][i];
    }
    float* x = new float[n];

    // Sharing some variables in our OpenMP method and initialize the number of thread
#pragma omp parallel shared(H, g, n, iterations, x, std::cout) default(none) num_threads(num_threads)
    for (int k = 0; k < iterations; k++) {
        float* new_x = new float[n];

#pragma omp for
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                new_x[i] += H[i][j] * x[j];
            }
            x[i] = new_x[i] + g[i];
        }
    }

    return x;
}

int main(int argc, char* argv[]) {

    // Intializing our parameters that have to be stored in our file

    int number_of_thread = atoi(argv[1]);
    int rank = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    char* filename = argv[4];
    // Creating our file dynamically
    std::ofstream parameter_store(filename, std::ios::app);

    // Calling our metrix-generator and vector-generator from another file that we have already created
    auto a = matrix(rank);
    auto b = vector(rank);

    // Start tracking of our process
    double start_time, end_time;

    start_time = omp_get_wtime();
    float* result;
    if (number_of_thread == 1) {
        result = simpleMode(a, b, rank, iterations);
    }
    else {
        result = parallelMode(a, b, rank, iterations, number_of_thread);
    }
    end_time = omp_get_wtime();

    parameter_store << "(Threads=" << number_of_thread << "): ";
    parameter_store << "Rank=" << rank << " ";
    parameter_store << "Number of iteratons=" << iterations << " ";
    parameter_store << "Time=" << end_time - start_time << std::endl;
    parameter_store.close();
}
