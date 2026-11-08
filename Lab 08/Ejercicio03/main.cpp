#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <climits>
#include <algorithm>

using namespace std;

// Una solucion es un vector asignacion donde asignacion[i] indica la linea de la tarea i.
// El makespan es la carga de la linea que termina mas tarde.
int makespan(const vector<int>& duracion, const vector<int>& asignacion, int lineas) {
    vector<int> carga(lineas, 0);
    for (size_t i = 0; i < duracion.size(); i++) carga[asignacion[i]] += duracion[i];
    return *max_element(carga.begin(), carga.end());
}

int busquedaAleatoria(const vector<int>& duracion, int lineas, int iteraciones,
                      mt19937& rng, vector<int>& mejorAsignacion, int& iteracionMejor) {
    uniform_int_distribution<int> linea(0, lineas - 1);
    vector<int> asignacion(duracion.size());
    int mejor = INT_MAX;
    for (int it = 1; it <= iteraciones; it++) {
        for (size_t i = 0; i < duracion.size(); i++) asignacion[i] = linea(rng);
        int m = makespan(duracion, asignacion, lineas);
        if (m < mejor) {
            mejor = m;
            mejorAsignacion = asignacion;
            iteracionMejor = it;
        }
    }
    return mejor;
}

// Con 6 tareas y 2 lineas solo hay 2^6 = 64 asignaciones; se recorren todas
// para conocer el optimo y evaluar el resultado de la busqueda aleatoria.
int optimoExacto(const vector<int>& duracion, int& cuantasOptimas) {
    int n = duracion.size();
    int mejor = INT_MAX;
    vector<int> asignacion(n);
    for (int mascara = 0; mascara < (1 << n); mascara++) {
        for (int i = 0; i < n; i++) asignacion[i] = (mascara >> i) & 1;
        mejor = min(mejor, makespan(duracion, asignacion, 2));
    }
    cuantasOptimas = 0;
    for (int mascara = 0; mascara < (1 << n); mascara++) {
        for (int i = 0; i < n; i++) asignacion[i] = (mascara >> i) & 1;
        if (makespan(duracion, asignacion, 2) == mejor) cuantasOptimas++;
    }
    return mejor;
}

void mostrarAsignacion(const vector<int>& duracion, const vector<int>& asignacion, int lineas) {
    for (int l = 0; l < lineas; l++) {
        int carga = 0;
        cout << "Linea " << l + 1 << ":";
        for (size_t i = 0; i < duracion.size(); i++) {
            if (asignacion[i] == l) {
                cout << " T" << i + 1 << "(" << duracion[i] << "h)";
                carga += duracion[i];
            }
        }
        cout << ", total " << carga << " h\n";
    }
}

int main() {
    const vector<int> duracion = { 4, 3, 2, 5, 6, 7 };
    const int lineas = 2;

    cout << "Tareas:";
    int total = 0;
    for (size_t i = 0; i < duracion.size(); i++) {
        cout << " T" << i + 1 << "=" << duracion[i] << "h";
        total += duracion[i];
    }
    cout << "\nSuma de duraciones: " << total << " h, cota inferior del makespan: "
         << (total + lineas - 1) / lineas << " h\n";

    int cuantasOptimas = 0;
    int optimo = optimoExacto(duracion, cuantasOptimas);
    cout << "Makespan optimo (64 asignaciones revisadas): " << optimo << " h, alcanzado por "
         << cuantasOptimas << " asignaciones\n";

    cout << "\nCorrida de ejemplo con 50 iteraciones\n";
    mt19937 rng(11);
    vector<int> mejorAsignacion;
    int iteracionMejor = 0;
    int resultado = busquedaAleatoria(duracion, lineas, 50, rng, mejorAsignacion, iteracionMejor);
    mostrarAsignacion(duracion, mejorAsignacion, lineas);
    cout << "Makespan encontrado: " << resultado << " h (desde la iteracion " << iteracionMejor << ")\n";

    const vector<int> niveles = { 5, 10, 50, 200 };
    const int corridas = 100;
    cout << fixed << setprecision(2);
    cout << "\nResumen de " << corridas << " corridas por nivel\n";
    cout << setw(12) << "Iteraciones" << setw(10) << "Mejor" << setw(10) << "Promedio"
         << setw(8) << "Peor" << setw(17) << "Llego al optimo\n";
    for (int k : niveles) {
        int mejor = INT_MAX, peor = 0, aciertos = 0;
        double suma = 0;
        for (int s = 1; s <= corridas; s++) {
            mt19937 r(s);
            vector<int> a;
            int it = 0;
            int m = busquedaAleatoria(duracion, lineas, k, r, a, it);
            mejor = min(mejor, m);
            peor = max(peor, m);
            suma += m;
            if (m == optimo) aciertos++;
        }
        cout << setw(12) << k << setw(10) << mejor << setw(10) << suma / corridas
             << setw(8) << peor << setw(8) << aciertos << " de " << corridas << "\n";
    }
    return 0;
}
