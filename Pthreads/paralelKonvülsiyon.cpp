#include <iostream>
#include <vector>
#include <pthread.h>

struct ThreadVeri {
    const std::vector<float>* x;
    const std::vector<float>* h;
    std::vector<float>* y;
    int baslangic;
    int bitis;
};

void* threadKonvulsiyon(void* arg) {
    ThreadVeri* veri = (ThreadVeri*)arg;
    const std::vector<float>& x = *veri->x;
    const std::vector<float>& h = *veri->h;
    std::vector<float>& y = *veri->y;
    int M = h.size();
    int N = x.size();

    for (int n = veri->baslangic; n < veri->bitis; ++n) {
        for (int k = 0; k < M; ++k) {
            if (n - k >= 0 && n - k < N) {
                y[n] += x[n - k] * h[k];
            }
        }
    }

    return nullptr;
}

std::vector<float> paralelKonvulsiyon(const std::vector<float>& x, const std::vector<float>& h, int threadSayisi) {
    int boyut = x.size() + h.size() - 1;
    std::vector<float> y(boyut, 0);
    std::vector<pthread_t> threads(threadSayisi);
    std::vector<ThreadVeri> threadVerileri(threadSayisi);

    // Thread'leri oluşturma
    for (int i = 0; i < threadSayisi; ++i) {
        threadVerileri[i] = {&x, &h, &y, i * boyut / threadSayisi, (i + 1) * boyut / threadSayisi};
        pthread_create(&threads[i], nullptr, threadKonvulsiyon, (void*)&threadVerileri[i]);
    }

    // Thread'leri birleştirme
    for (int i = 0; i < threadSayisi; ++i) {
        pthread_join(threads[i], nullptr);
    }

    return y;
}
