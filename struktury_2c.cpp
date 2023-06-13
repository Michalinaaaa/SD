#include <iostream>
#include <vector>
#include <limits>
#include <chrono>
#include <unordered_set>
#include <cstdlib>

using namespace std;

struct Krawedz {
    int cel;
    int waga;
};

class Stos {
private:
    int* tab;
    int top_element;
    int rozmiar;
    int push_ilosc;
    int pop_ilosc;
    int kroki_push;
    int kroki_pop;

public:
    Stos() {
        tab = nullptr;
        top_element = -1;
        rozmiar = 0;
        push_ilosc = 0;
        pop_ilosc = 0;
        kroki_push = 0;
        kroki_pop = 0;
    }

    ~Stos() {
        delete[] tab;
    }

    void push(int x) {
        if (czy_pelny()) {
            int* new_tab = new int[2 * rozmiar + 1];
            for (int i = 0; i <= top_element; i++) {
                new_tab[i] = tab[i];
                push_ilosc++;
            }
            rozmiar = 2 * rozmiar + 1;
            delete[] tab;
            tab = new_tab;
            push_ilosc++;
        }

        top_element++;
        tab[top_element] = x;
        push_ilosc++;
        kroki_push++;
    }

    int pop() {
        if (czy_pusty()) {
            cout << "Stos jest pusty. Nie można usunąć elementu." << endl;
            return -1;
        }

        int value = tab[top_element];
        top_element--;

        if (top_element < rozmiar / 2) {
            int* new_tab = new int[rozmiar / 2 + 1];
            for (int i = 0; i <= top_element; i++) {
                new_tab[i] = tab[i];
                pop_ilosc++;
            }
            new_tab[top_element + 1] = 0;
            rozmiar = rozmiar / 2 + 1;
            delete[] tab;
            tab = new_tab;
            pop_ilosc++;
        }

        pop_ilosc++;
        kroki_pop++;
        return value;
    }

    int wyswietl() {
        if (czy_pusty()) {
            cout << "BLAD! Stos jest pusty." << endl;
            return -1;
        }

        return tab[top_element];
    }

    bool czy_pusty() {
        return top_element == -1;
    }

    bool czy_pelny() {
        return top_element == rozmiar - 1;
    }

    int push_wyswietl() {
        return push_ilosc;
    }

    int pop_wyswietl() {
        return pop_ilosc;
    }

    int push_kroki() {
        return kroki_push;
    }

    int pop_kroki() {
        return kroki_pop;
    }

    void wyswietl_graf(const vector<vector<Krawedz>>& graf) {
        int liczba_wierzcholkow = graf.size();

        for (int i = 0; i < liczba_wierzcholkow; ++i) {
            cout << "Wierzcholek " << i << ": ";
            for (const Krawedz& krawedz : graf[i]) {
                cout << "(" << krawedz.cel << ", " << krawedz.waga << ") ";
            }
            cout << endl;
        }
    }
};

const int INF = numeric_limits<int>::max();

using Graf = vector<vector<Krawedz>>;

void dodajKrawedz(Graf& graf, int zrodlo, int cel, int waga) {
    // czy krawędź już istnieje
    for (const Krawedz& krawedz : graf[zrodlo]) {
        if (krawedz.cel == cel) {
            return;  // Krawędź już istnieje
        }
    }

    Krawedz krawedz;
    krawedz.cel = cel;
    krawedz.waga = waga;

    graf[zrodlo].push_back(krawedz);

    // Dodanie  krawędzi w drugą stronę
    Krawedz krawedz_odwrotna;
    krawedz_odwrotna.cel = zrodlo;
    krawedz_odwrotna.waga = waga;

    graf[cel].push_back(krawedz_odwrotna);
}

vector<int>  Dijkstra(const Graf& graf, int zrodlo, Stos& stos) {
    int liczba_wierzcholkow = graf.size();
    vector<int> odleglosci(liczba_wierzcholkow, INF);
    odleglosci[zrodlo] = 0;

    stos.push(zrodlo);

    while (!stos.czy_pusty()) {
        int obecny = stos.pop();

        if (odleglosci[obecny] == INF) {
            continue;
        }

        for (const Krawedz& krawedz : graf[obecny]) {
            int cel = krawedz.cel;
            int waga = krawedz.waga;

            if (odleglosci[obecny] + waga < odleglosci[cel]) {
                odleglosci[cel] = odleglosci[obecny] + waga;
                stos.push(cel);
            }
        }
    }

    return odleglosci;
}

void wykonaj(const Graf& graf, int liczba_wierzcholkow, Stos& stos) {
    // Wyświetlanie grafu
    cout << "Graf:\n";
    stos.wyswietl_graf(graf);

    // Pomiar czasu wykonania
    auto start = chrono::steady_clock::now();
    int zrodlo = 0;
    vector<int> odleglosci = Dijkstra(graf, zrodlo, stos);
    auto end = chrono::steady_clock::now();

    // Wyświetlanie wyników
    cout << "Wyniki dla grafu z " << liczba_wierzcholkow << " wierzcholkami:\n";
    cout << "Czas wykonania: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
     cout << "Najkrotsze drogi:\n";
     for (int i = 0; i < liczba_wierzcholkow; ++i) {
         cout << "Od wierzcholka " << zrodlo << " do wierzcholka " << i << ": ";
         if (odleglosci[i] == INF) {
             cout << "brak sciezki" << std::endl;
         }
         else {
             cout << odleglosci[i] << std::endl;
         }
     }

    cout << "--------------------------------------\n";
}

int main() {
    vector<int> liczba_wierzcholkow_lista = { 5, 10 };

    for (int liczba_wierzcholkow : liczba_wierzcholkow_lista) {
        Graf graf(liczba_wierzcholkow);

        // Tworzenie losowego grafu 
        for (int i = 0; i < liczba_wierzcholkow; ++i) {
            int liczba_krawedzi = rand() % (liczba_wierzcholkow - 1) + 1;
            unordered_set<int> polaczone_wierzcholki;

            while (polaczone_wierzcholki.size() < liczba_krawedzi) {
                int cel = rand() % liczba_wierzcholkow;
                int waga = rand() % 10 + 1;

                if (cel != i && polaczone_wierzcholki.count(cel) == 0) {
                    dodajKrawedz(graf, i, cel, waga);
                    polaczone_wierzcholki.insert(cel);
                }
            }
        }

        
        Stos stos_1;

        wykonaj(graf, liczba_wierzcholkow, stos_1);
    }

    return 0;
}
