#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>
#include <climits>

using namespace std;

struct Ciudad {
    int x;
    int y;
};

// Genera n ciudades en un plano de 100 x 100. La semilla fija permite
// que el ejercicio 2 reconstruya exactamente el mismo TSP.
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

// La distancia euclidiana se redondea al entero mas cercano para trabajar
// con una matriz de enteros, igual que en el ejemplo de la guia.
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

// Fija la ciudad 0 como origen y recorre las (n - 1)! permutaciones del resto.
int fuerzaBruta(const vector<vector<int>>& dist, vector<int>& mejorRuta, long long& revisadas) {
    int n = dist.size();
    vector<int> ruta;
    for (int i = 1; i < n; i++) ruta.push_back(i);

    int mejorCosto = INT_MAX;
    revisadas = 0;
    do {
        int costo = costoRuta(dist, ruta);
        revisadas++;
        if (costo < mejorCosto) {
            mejorCosto = costo;
            mejorRuta = ruta;
        }
    } while (next_permutation(ruta.begin(), ruta.end()));
    return mejorCosto;
}

int main() {
    const int n = 10;
    const unsigned semilla = 2026;
    const int repeticiones = 5;

    vector<Ciudad> ciudades = generarCiudades(n, semilla);
    vector<vector<int>> dist = matrizDistancias(ciudades);

    cout << "Ciudades generadas (semilla " << semilla << "):\n";
    for (int i = 0; i < n; i++) {
        cout << "Ciudad " << i << ": (" << ciudades[i].x << ", " << ciudades[i].y << ")\n";
    }

    cout << "\nMatriz de distancias\n    ";
    for (int j = 0; j < n; j++) cout << setw(4) << j;
    cout << "\n";
    for (int i = 0; i < n; i++) {
        cout << setw(4) << i;
        for (int j = 0; j < n; j++) cout << setw(4) << dist[i][j];
        cout << "\n";
    }

    vector<int> mejorRuta;
    long long revisadas = 0;
    int mejorCosto = 0;
    vector<double> tiempos;

    // Se repite la busqueda completa para reducir el efecto de otros procesos
    // en la medicion. Solo se cronometra la busqueda, no la impresion.
    for (int r = 0; r < repeticiones; r++) {
        auto inicio = chrono::steady_clock::now();
        mejorCosto = fuerzaBruta(dist, mejorRuta, revisadas);
        auto fin = chrono::steady_clock::now();
        tiempos.push_back(chrono::duration<double, milli>(fin - inicio).count());
    }

    cout << "\nPermutaciones revisadas: " << revisadas << " (rutas distintas: " << revisadas / 2 << ")\n";
    cout << "Ruta optima: 0";
    for (int c : mejorRuta) cout << "-" << c;
    cout << "-0\n";
    cout << "Costo optimo: " << mejorCosto << "\n";

    cout << fixed << setprecision(2);
    cout << "\nTiempo de cada repeticion (ms):";
    for (double t : tiempos) cout << " " << t;
    double suma = 0;
    for (double t : tiempos) suma += t;
    cout << "\nTiempo promedio: " << suma / repeticiones << " ms\n";
    cout << "Tiempo minimo: " << *min_element(tiempos.begin(), tiempos.end()) << " ms\n";
    return 0;
}
