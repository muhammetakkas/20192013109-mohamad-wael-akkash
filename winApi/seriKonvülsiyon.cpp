#include <iostream>
#include <vector>

std::vector<float> seriKonvulsiyon(const std::vector<float>& x, const std::vector<float>& h) {
    int N = x.size();
    int M = h.size();
    int boyut = N + M - 1;
    std::vector<float> y(boyut, 0);

    for (int n = 0; n < boyut; ++n) {
        for (int k = 0; k < M; ++k) {
            if (n - k >= 0 && n - k < N) {
                y[n] += x[n - k] * h[k];
            }
        }
    }

    return y;
}

