#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <string>

using namespace std;

struct Ciudad {
    int x;
    int y;
};

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

vector<vector<double>> matrizDistancias(const vector<Ciudad>& c) {
    int n = c.size();
    vector<vector<double>> dist(n, vector<double>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double dx = c[i].x - c[j].x;
            double dy = c[i].y - c[j].y;
            dist[i][j] = sqrt(dx * dx + dy * dy);
        }
    }
    return dist;
}

double costoRuta(const vector<vector<double>>& dist, const vector<int>& ruta) {
    double costo = 0;
    for (size_t i = 0; i + 1 < ruta.size(); i++) costo += dist[ruta[i]][ruta[i + 1]];
    costo += dist[ruta.back()][ruta[0]];
    return costo;
}

vector<int> nearestNeighbor(const vector<vector<double>>& dist, int inicio) {
    int n = dist.size();
    vector<bool> visitado(n, false);
    vector<int> ruta = { inicio };
    visitado[inicio] = true;
    for (int paso = 1; paso < n; paso++) {
        int actual = ruta.back();
        int masCercano = -1;
        double mejorDist = 1e18;
        for (int c = 0; c < n; c++) {
            if (!visitado[c] && dist[actual][c] < mejorDist) {
                mejorDist = dist[actual][c];
                masCercano = c;
            }
        }
        ruta.push_back(masCercano);
        visitado[masCercano] = true;
    }
    return ruta;
}

int dosOpt(const vector<vector<double>>& dist, vector<int>& ruta) {
    int n = ruta.size();
    int movimientos = 0;
    bool mejora = true;
    while (mejora) {
        mejora = false;
        for (int i = 1; i < n - 1; i++) {
            for (int j = i + 1; j < n; j++) {
                int a = ruta[i - 1], b = ruta[i];
                int c = ruta[j], e = ruta[(j + 1) % n];
                double cambio = dist[a][c] + dist[b][e] - dist[a][b] - dist[c][e];
                if (cambio < -1e-9) {
                    reverse(ruta.begin() + i, ruta.begin() + j + 1);
                    movimientos++;
                    mejora = true;
                }
            }
        }
    }
    return movimientos;
}

// Fuerza bruta con origen fijo, usada solo para medir cuantas permutaciones
// por segundo evalua esta computadora.
double fuerzaBruta(const vector<vector<double>>& dist, long long& revisadas) {
    int n = dist.size();
    vector<int> ruta;
    for (int i = 0; i < n; i++) ruta.push_back(i);
    double mejor = 1e18;
    revisadas = 0;
    do {
        mejor = min(mejor, costoRuta(dist, ruta));
        revisadas++;
    } while (next_permutation(ruta.begin() + 1, ruta.end()));
    return mejor;
}

double log10Factorial(int n) {
    double s = 0;
    for (int i = 2; i <= n; i++) s += log10((double)i);
    return s;
}

// Devuelve 10^lg como texto: el numero completo si es pequeno y, si no, mantisa
// y exponente. Se trabaja con logaritmos porque 99! supera el rango de un double.
string notacion(double lg) {
    ostringstream os;
    if (lg < 6) {
        os << fixed << setprecision(0) << pow(10.0, lg);
    } else {
        double e = floor(lg);
        os << fixed << setprecision(2) << pow(10.0, lg - e) << "e" << (int)e;
    }
    return os.str();
}

int main() {
    const vector<int> tamanos = { 20, 50, 100 };
    const int instancias = 10;
    cout << fixed << setprecision(4);

    // Cada medicion repite NN + 2-opt varias veces y divide, porque con 20
    // ciudades una sola ejecucion dura menos que la resolucion del reloj.
    cout << "Tiempo de Nearest Neighbor + 2-opt, " << instancias << " instancias por tamano (semillas 1 a "
         << instancias << ")\n";
    cout << setw(5) << "n" << setw(12) << "NN (ms)" << setw(14) << "2-opt (ms)" << setw(14) << "Total (ms)"
         << setw(15) << "Max total" << setw(13) << "Movim." << setw(12) << "Mejora %" << "\n";
    vector<double> tiempoTotal;
    for (int n : tamanos) {
        int repeticiones = n == 20 ? 2000 : (n == 50 ? 200 : 20);
        double sumaNN = 0, sumaOpt = 0, maxTotal = 0, sumaMov = 0, sumaMejora = 0;
        for (int s = 1; s <= instancias; s++) {
            vector<vector<double>> dist = matrizDistancias(generarCiudades(n, s));
            vector<int> rutaNN, ruta;
            int movimientos = 0;

            auto t0 = chrono::steady_clock::now();
            for (int r = 0; r < repeticiones; r++) rutaNN = nearestNeighbor(dist, 0);
            auto t1 = chrono::steady_clock::now();
            for (int r = 0; r < repeticiones; r++) {
                ruta = rutaNN;
                movimientos = dosOpt(dist, ruta);
            }
            auto t2 = chrono::steady_clock::now();

            double msNN = chrono::duration<double, milli>(t1 - t0).count() / repeticiones;
            double msOpt = chrono::duration<double, milli>(t2 - t1).count() / repeticiones;
            sumaNN += msNN;
            sumaOpt += msOpt;
            maxTotal = max(maxTotal, msNN + msOpt);
            sumaMov += movimientos;
            double cNN = costoRuta(dist, rutaNN);
            sumaMejora += 100.0 * (cNN - costoRuta(dist, ruta)) / cNN;
        }
        double total = (sumaNN + sumaOpt) / instancias;
        tiempoTotal.push_back(total);
        cout << setw(5) << n << setw(12) << sumaNN / instancias << setw(14) << sumaOpt / instancias
             << setw(14) << total << setw(15) << maxTotal << setw(13) << setprecision(1) << sumaMov / instancias
             << setw(12) << setprecision(2) << sumaMejora / instancias << setprecision(4) << "\n";
    }

    // Velocidad real de la fuerza bruta con 11 ciudades: 10! permutaciones.
    vector<vector<double>> chico = matrizDistancias(generarCiudades(11, 1));
    long long revisadas = 0;
    const int repFB = 3;
    double mejorTiempo = 1e18;
    for (int r = 0; r < repFB; r++) {
        auto t0 = chrono::steady_clock::now();
        fuerzaBruta(chico, revisadas);
        auto t1 = chrono::steady_clock::now();
        mejorTiempo = min(mejorTiempo, chrono::duration<double>(t1 - t0).count());
    }
    double velocidad = revisadas / mejorTiempo;
    cout << setprecision(2);
    cout << "\nFuerza bruta medida con 11 ciudades: " << revisadas << " permutaciones en "
         << mejorTiempo << " s, " << velocidad / 1e6 << " millones por segundo\n";

    // Estimacion sin ejecutar: (n - 1)! permutaciones divididas entre la velocidad medida.
    const double segundosAnio = 365.25 * 24 * 3600;
    cout << "\nEstimacion de fuerza bruta frente a NN + 2-opt medido\n";
    cout << setw(5) << "n" << setw(16) << "Permutaciones" << setw(16) << "Segundos FB" << setw(14)
         << "Anios FB" << setw(18) << "Veces mas lento" << "\n";
    for (size_t k = 0; k < tamanos.size(); k++) {
        int n = tamanos[k];
        double lgPerm = log10Factorial(n - 1);
        double lgSeg = lgPerm - log10(velocidad);
        double lgAnios = lgSeg - log10(segundosAnio);
        double lgVeces = lgSeg - log10(tiempoTotal[k] / 1000.0);
        cout << setw(5) << n << setw(16) << notacion(lgPerm) << setw(16) << notacion(lgSeg)
             << setw(14) << notacion(lgAnios) << setw(18) << notacion(lgVeces) << "\n";
    }
    return 0;
}
