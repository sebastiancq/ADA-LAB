#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <climits>

using namespace std;

struct Ciudad {
    int x;
    int y;
};

// Misma generacion y misma semilla que el ejercicio 1: el TSP es identico.
vector<Ciudad> generarCiudades(int n, unsigned semilla) {
    mt19937 rng(semilla);
    uniform_int_distribution<int> coord(0, 100);
    vector<Ciudad> ciudades(n);
    for (int i = 0; i < n; i++) {
        ciudades[i].x = coord(rng);
        ciudades[i].y = coord(rng);
    }
    return ciudades;
}

vector<vector<int>> matrizDistancias(const vector<Ciudad>& c) {
    int n = c.size();
    vector<vector<int>> dist(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double dx = c[i].x - c[j].x;
            double dy = c[i].y - c[j].y;
            dist[i][j] = (int)lround(sqrt(dx * dx + dy * dy));
        }
    }
    return dist;
}

int costoRuta(const vector<vector<int>>& dist, const vector<int>& ruta) {
    int costo = dist[0][ruta[0]];
    for (size_t i = 0; i + 1 < ruta.size(); i++) costo += dist[ruta[i]][ruta[i + 1]];
    costo += dist[ruta.back()][0];
    return costo;
}

// El optimo real se obtiene con fuerza bruta para medir que tan lejos queda Monte Carlo.
int fuerzaBruta(const vector<vector<int>>& dist) {
    int n = dist.size();
    vector<int> ruta;
    for (int i = 1; i < n; i++) ruta.push_back(i);
    int mejor = INT_MAX;
    do {
        mejor = min(mejor, costoRuta(dist, ruta));
    } while (next_permutation(ruta.begin(), ruta.end()));
    return mejor;
}

// Cada iteracion mezcla las ciudades 1..n-1 y conserva la mejor ruta vista.
int monteCarlo(const vector<vector<int>>& dist, int iteraciones, mt19937& rng, vector<int>& mejorRuta) {
    int n = dist.size();
    vector<int> ruta;
    for (int i = 1; i < n; i++) ruta.push_back(i);
    int mejorCosto = INT_MAX;
    for (int it = 0; it < iteraciones; it++) {
        shuffle(ruta.begin(), ruta.end(), rng);
        int costo = costoRuta(dist, ruta);
        if (costo < mejorCosto) {
            mejorCosto = costo;
            mejorRuta = ruta;
        }
    }
    return mejorCosto;
}

int main() {
    const int n = 10;
    vector<vector<int>> dist = matrizDistancias(generarCiudades(n, 2026));
    int optimo = fuerzaBruta(dist);
    cout << "Costo optimo real (fuerza bruta): " << optimo << "\n";

    const vector<int> niveles = { 100, 1000, 10000 };
    const int corridas = 30;
    cout << fixed << setprecision(2);

    // Una corrida de ejemplo por nivel, con semilla fija para poder repetirla.
    cout << "\nCorrida de ejemplo por nivel de iteraciones\n";
    for (int k : niveles) {
        mt19937 rng(7);
        vector<int> ruta;
        int costo = monteCarlo(dist, k, rng, ruta);
        cout << k << " iteraciones: 0";
        for (int c : ruta) cout << "-" << c;
        cout << "-0, costo " << costo
             << ", diferencia " << 100.0 * (costo - optimo) / optimo << " %\n";
    }

    // Como el resultado depende del azar, cada nivel se repite con 30 semillas distintas.
    cout << "\nResumen de " << corridas << " corridas por nivel\n";
    cout << setw(12) << "Iteraciones" << setw(10) << "Mejor" << setw(10) << "Promedio"
         << setw(10) << "Peor" << setw(14) << "Dif. prom %" << setw(18) << "Llego al optimo\n";
    for (int k : niveles) {
        int mejor = INT_MAX, peor = 0, aciertos = 0;
        double suma = 0;
        for (int s = 1; s <= corridas; s++) {
            mt19937 rng(s);
            vector<int> ruta;
            int costo = monteCarlo(dist, k, rng, ruta);
            mejor = min(mejor, costo);
            peor = max(peor, costo);
            suma += costo;
            if (costo == optimo) aciertos++;
        }
        double promedio = suma / corridas;
        cout << setw(12) << k << setw(10) << mejor << setw(10) << promedio << setw(10) << peor
             << setw(14) << 100.0 * (promedio - optimo) / optimo
             << setw(10) << aciertos << " de " << corridas << "\n";
    }
    return 0;
}
