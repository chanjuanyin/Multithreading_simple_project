#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <future>
#include <pthread.h>
#include <atomic>
using namespace std;

void worker(std::vector<double>& inp, int start, int end, double& total, std::mutex& mu, pthread_barrier_t& bar) {
    double local_sum = 0.0;
    for (int i = start; i < end; i++) {
        local_sum += inp[i];
    }

    {
        std::unique_lock<std::mutex> lock(mu);
        total += local_sum;
    }
    pthread_barrier_wait(&bar);
    
    for (int i = start; i < end; i++) {
        inp[i] /= total;
    }
}

void sum_normalize(std::vector<double>& inp, int num_threads) {
    pthread_barrier_t bar;
    pthread_barrier_init(&bar, NULL, num_threads);
    std::mutex mu;
    int part = inp.size() / num_threads;
    double total = 0.0;
    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    for (int i = 0; i < num_threads; i++) {
        int start = i * part;
        int end = i * part + part;
        threads.emplace_back(worker, std::ref(inp), start, end, std::ref(total), std::ref(mu), std::ref(bar));
    }
    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
    }
}

int main() {
    std::vector<double> vec(100, 1);
    sum_normalize(vec, 5);
    for (int i = 0; i < 100; i++) {
        std::cout << vec[i] << ",";
    }
    std::cout << std::endl;
    return 0;
}
