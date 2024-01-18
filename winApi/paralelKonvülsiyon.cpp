#include <windows.h>
#include <vector>

struct ThreadVeri {
    const std::vector<float>* x;
    const std::vector<float>* h;
    std::vector<float>* y;
    int baslangic;
    int bitis;
};

DWORD WINAPI threadKonvulsiyon(LPVOID param) {
    ThreadVeri* veri = (ThreadVeri*)param;
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

    return 0;
}


std::vector<float> paralelKonvulsiyon(const std::vector<float>& x, const std::vector<float>& h, int threadSayisi) {
    int boyut = x.size() + h.size() - 1;
    std::vector<float> y(boyut, 0);
    std::vector<HANDLE> threads(threadSayisi);
    std::vector<ThreadVeri> threadVerileri(threadSayisi);

    // Create threads
    for (int i = 0; i < threadSayisi; ++i) {
        threadVerileri[i] = {&x, &h, &y, i * boyut / threadSayisi, (i + 1) * boyut / threadSayisi};
        threads[i] = CreateThread(NULL, 0, threadKonvulsiyon, (void*)&threadVerileri[i], 0, NULL);
    }

    // Wait for threads to complete
    WaitForMultipleObjects(threadSayisi, threads.data(), TRUE, INFINITE);

    // Close thread handles
    for (int i = 0; i < threadSayisi; ++i) {
        CloseHandle(threads[i]);
    }

    return y;
}
