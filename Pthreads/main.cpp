#include <iostream>
#include <vector>
#include <chrono>
#include <fstream> 
#include "paralelKonvülsiyon.cpp"
#include "seriKonvülsiyon.cpp"

int main() {
    std::vector<int> N_degerleri = {10, 100, 1000, 10000, 100000}; // N değerlerinin listesi
    std::vector<int> threadSayilari = {2, 4, 8, 16}; // Kullanılacak thread sayıları
    int M = 10;

    std::ofstream veriDosyasi("performans_verileri.txt"); // Performans verilerinin kaydedileceği dosya

    for (int N : N_degerleri) {
        std::vector<float> x(N); // x vektörü
        std::vector<float> h(M); // h vektörü

        // x ve h vektörlerini 0.0 ile 1.0 arasında rastgele float değerlerle başlatma
        for (int i = 0; i < N; i++) {
            x[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        }

        for (int i = 0; i < M; i++) {
            h[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        }

        // Seri kod zamanları
        auto baslangic = std::chrono::high_resolution_clock::now();
        std::vector<float> seriSonuc = seriKonvulsiyon(x, h);
        auto bitis = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> seriSure = bitis - baslangic;

        veriDosyasi << N << ",1," << seriSure.count() << std::endl; // Seri hesaplama için çıktı

        // Farklı thread sayıları için paralel zamanlar
        for (int threadSayisi : threadSayilari) {
            baslangic = std::chrono::high_resolution_clock::now();
            std::vector<float> paralelSonuc = paralelKonvulsiyon(x, h, threadSayisi);
            bitis = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> paralelSure = bitis - baslangic;
            veriDosyasi << N << "," << threadSayisi << "," << paralelSure.count() << std::endl; // Paralel hesaplama için çıktı
        }
    }

    veriDosyasi.close();
    return 0;
}
