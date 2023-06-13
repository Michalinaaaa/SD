#include <iostream>
#include <limits>
#include <chrono>
#include <random>
#include <algorithm>
#include <vector>

using namespace std;
using namespace std::chrono;

const int NIESKONCZONOSC = numeric_limits<int>::max();

struct Wezel {
    int odleglosc;
    int wierzcholek;
    Wezel* nastepny;
};

class KolejkaPriorytetowa {
private:
    Wezel* glowa;

public:
    KolejkaPriorytetowa() : glowa(nullptr) {}

    void dodaj(int odleglosc, int wierzcholek) {
        Wezel* nowyWezel = new Wezel;
        nowyWezel->odleglosc = odleglosc;
        nowyWezel->wierzcholek = wierzcholek;
        nowyWezel->nastepny = nullptr;

        if (glowa == nullptr || odleglosc < glowa->odleglosc) {
            nowyWezel->nastepny = glowa;
            glowa = nowyWezel;
        }
        else {
            Wezel* aktualny = glowa;
            while (aktualny->nastepny != nullptr && aktualny->nastepny->odleglosc <= odleglosc) {
                aktualny = aktualny->nastepny;
            }
            nowyWezel->nastepny = aktualny->nastepny;
            aktualny->nastepny = nowyWezel;
        }
    }

    bool pusta() {
        return glowa == nullptr;
    }

    int topWierzcholek() {
        return glowa->wierzcholek;
    }

    int topOdleglosc() {
        return glowa->odleglosc;
    }

    void usun() {
        if (glowa != nullptr) {
            Wezel* temp = glowa;
            glowa = glowa->nastepny;
            delete temp;
        }
    }
};

void Dijkstra(int** graf, int n, int zrodlo) {
    int* odleglosci = new int[n];
    for (int i = 0; i < n; i++) {
        odleglosci[i] = NIESKONCZONOSC;
    }

    odleglosci[zrodlo] = 0;

    KolejkaPriorytetowa kolejka;
    kolejka.dodaj(odleglosci[zrodlo], zrodlo);

    while (!kolejka.pusta()) {
        int u = kolejka.topWierzcholek();
        kolejka.usun();

        for (int v = 0; v < n; v++) {
            if (graf[u][v] && odleglosci[u] + graf[u][v] < odleglosci[v]) {
                odleglosci[v] = odleglosci[u] + graf[u][v];
                kolejka.dodaj(odleglosci[v], v);
            }
        }
    }

    cout << "Dlugosci przebytych drog dla grafu o rozmiarze " << n << " i zrodle " << zrodlo << ":\n";
    for (int i = 0; i < n; i++) {
        cout << "Do wierzcholka " << i << ": " << odleglosci[i] << endl;
    }

    delete[] odleglosci;
}

void GenerujGraf(int** graf, int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 10);

    for (int i = 0; i < n; i++) {
        int liczba_krawedzi = i;

        vector<int> wierzcholki;
        for (int j = 0; j < n; j++) {
            if (j != i) {
                wierzcholki.push_back(j);
            }
        }
        shuffle(wierzcholki.begin(), wierzcholki.end(), gen);

        for (int j = 0; j < liczba_krawedzi; j++) {
            int v = wierzcholki[j];
            int waga = dist(gen);
            graf[i][v] = waga;
            graf[v][i] = waga;
        }
    }
}

void WyswietlGraf(int** graf, int n) {
    cout << "Graf:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << graf[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int rozmiary[] = { 5 };
    int liczba_rozmiary = sizeof(rozmiary) / sizeof(rozmiary[0]);

    for (int i = 0; i < liczba_rozmiary; i++) {
        int n = rozmiary[i];

        int** graf = new int* [n];
        for (int j = 0; j < n; j++) {
            graf[j] = new int[n];
            for (int k = 0; k < n; k++) {
                graf[j][k] = 0;
            }
        }

        GenerujGraf(graf, n);

        WyswietlGraf(graf, n);

        auto start = high_resolution_clock::now();

        Dijkstra(graf, n, 0);

        auto end = high_resolution_clock::now();
        auto czas_trwania = duration_cast<milliseconds>(end - start);

        cout << "Czas wykonywania dla grafu o rozmiarze " << n << ": " << czas_trwania.count() << " ms\n";

        for (int j = 0; j < n; j++) {
            delete[] graf[j];
        }
        delete[] graf;

        cout << "---------------------------------\n";
    }

    return 0;
}
