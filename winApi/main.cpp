#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include "paralelKonvülsiyon.cpp" 
#include "seriKonvülsiyon.cpp"   

int main() {
    std::vector<int> N_degerleri = {10, 100, 1000, 10000, 100000}; // N değerlerinin listesi
    std::vector<int> threadSayilari = {2, 4, 8, 16}; // Kullanılacak thread sayıları
    int M = 10;

    std::ofstream veriDosyasi("performans_verileri.txt"); // Dataları buraya kaydediyoruz
    for (int N : N_degerleri) {
        std::vector<float> x(N); // Vector x
        std::vector<float> h(M); // Vector h

        // 0 ile 1 arasında random değerleri  x ve h değerlerine ata
        for (int i = 0; i < N; i++) {
            x[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        }
        for (int i = 0; i < M; i++) {
            h[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        }

        // seriKonvülsiyon değerlerini ölç
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<float> seriSonuc = seriKonvulsiyon(x, h);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> seriDuration = end - start;
        veriDosyasi << N << ",1," << seriDuration.count() << std::endl;

        // farklı thread sayıları için paralelKonvülsiyon değerlerini ölç
        for (int threadSayisi : threadSayilari) {
            start = std::chrono::high_resolution_clock::now();
            std::vector<float> paralelSonuc = paralelKonvulsiyon(x, h, threadSayisi);
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> paralelDuration = end - start;
            veriDosyasi << N << "," << threadSayisi << "," << paralelDuration.count() << std::endl;
        }
    }

    veriDosyasi.close();
    return 0;
}
