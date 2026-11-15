#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

using namespace std;

struct Ciudad {
    int x;
    int y;
};

// Genera n ciudades en un plano de 100 x 100 con una semilla fija,
// de modo que el ejercicio 2 reconstruye el mismo TSP.
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

// Distancia euclidiana real, sin redondear, para evitar empates artificiales
// entre ciudades candidatas.
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

// La ruta guarda las n ciudades en orden de visita; el cierre vuelve a ruta[0].
double costoRuta(const vector<vector<double>>& dist, const vector<int>& ruta) {
    double costo = 0;
    for (size_t i = 0; i + 1 < ruta.size(); i++) costo += dist[ruta[i]][ruta[i + 1]];
    costo += dist[ruta.back()][ruta[0]];
    return costo;
}

// En cada paso se elige la ciudad no visitada mas cercana a la actual.
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

// Programacion dinamica de Held y Karp: mejor[S][j] es el costo minimo de salir
// de 0, visitar el conjunto S y terminar en j. Con 15 ciudades son 2^15 x 15
// estados, frente a las 14! permutaciones de la fuerza bruta.
double optimoExacto(const vector<vector<double>>& dist) {
    int n = dist.size();
    int total = 1 << n;
    vector<vector<double>> mejor(total, vector<double>(n, 1e18));
    mejor[1][0] = 0;
    for (int S = 1; S < total; S++) {
        if (!(S & 1)) continue;
        for (int j = 0; j < n; j++) {
            if (!(S & (1 << j)) || mejor[S][j] >= 1e18) continue;
            for (int k = 0; k < n; k++) {
                if (S & (1 << k)) continue;
                int T = S | (1 << k);
                mejor[T][k] = min(mejor[T][k], mejor[S][j] + dist[j][k]);
            }
        }
    }
    double resultado = 1e18;
    for (int j = 1; j < n; j++) resultado = min(resultado, mejor[total - 1][j] + dist[j][0]);
    return resultado;
}

// Porcentaje sobre el optimo; las diferencias menores que 1e-9 son error de
// redondeo de double y se tratan como cero.
double difPorcentaje(double costo, double optimo) {
    double d = costo - optimo;
    if (fabs(d) < 1e-9) d = 0;
    return 100.0 * d / optimo;
}

void mostrarRuta(const vector<int>& ruta) {
    for (int c : ruta) cout << c << "-";
    cout << ruta[0];
}

int main() {
    const int n = 15;
    const unsigned semilla = 2026;
    vector<Ciudad> ciudades = generarCiudades(n, semilla);
    vector<vector<double>> dist = matrizDistancias(ciudades);

    cout << "Ciudades generadas (semilla " << semilla << "):\n";
    for (int i = 0; i < n; i++) {
        cout << "Ciudad " << setw(2) << i << ": (" << ciudades[i].x << ", " << ciudades[i].y << ")\n";
    }

    double optimo = optimoExacto(dist);
    cout << fixed << setprecision(2);
    cout << "\nCosto optimo exacto (Held-Karp): " << optimo << "\n";

    cout << "\nNearest Neighbor desde cada ciudad de origen\n";
    cout << setw(7) << "Inicio" << setw(10) << "Costo" << setw(12) << "Dif. %" << "   Ruta\n";
    double mejor = 1e18, peor = 0, suma = 0;
    int inicioMejor = 0, inicioPeor = 0;
    for (int inicio = 0; inicio < n; inicio++) {
        vector<int> ruta = nearestNeighbor(dist, inicio);
        double costo = costoRuta(dist, ruta);
        cout << setw(7) << inicio << setw(10) << costo << setw(12) << difPorcentaje(costo, optimo) << "   ";
        mostrarRuta(ruta);
        cout << "\n";
        suma += costo;
        if (costo < mejor) { mejor = costo; inicioMejor = inicio; }
        if (costo > peor) { peor = costo; inicioPeor = inicio; }
    }
    cout << "\nMejor inicio: ciudad " << inicioMejor << ", costo " << mejor
         << " (" << difPorcentaje(mejor, optimo) << " % sobre el optimo)\n";
    cout << "Peor inicio: ciudad " << inicioPeor << ", costo " << peor
         << " (" << difPorcentaje(peor, optimo) << " % sobre el optimo)\n";
    cout << "Promedio de los 15 inicios: " << suma / n << "\n";
    cout << "Diferencia entre el peor y el mejor inicio: " << 100.0 * (peor - mejor) / mejor << " %\n";

    // Una sola instancia puede ser un caso particular; se repite el experimento
    // con 30 conjuntos de ciudades distintos.
    const int instancias = 30;
    cout << "\nResumen en " << instancias << " instancias de 15 ciudades (semillas 1 a " << instancias << ")\n";
    cout << setw(8) << "Semilla" << setw(10) << "Optimo" << setw(12) << "Mejor NN" << setw(12) << "Peor NN"
         << setw(14) << "Mejor dif %" << setw(13) << "Peor dif %" << setw(16) << "Inicios optimos\n";
    double sumaMejorDif = 0, sumaPeorDif = 0;
    int conOptimo = 0;
    for (int s = 1; s <= instancias; s++) {
        vector<vector<double>> d = matrizDistancias(generarCiudades(n, s));
        double opt = optimoExacto(d);
        double m = 1e18, p = 0;
        int aciertos = 0;
        for (int inicio = 0; inicio < n; inicio++) {
            double c = costoRuta(d, nearestNeighbor(d, inicio));
            m = min(m, c);
            p = max(p, c);
            if (c < opt + 1e-9) aciertos++;
        }
        if (aciertos > 0) conOptimo++;
        sumaMejorDif += difPorcentaje(m, opt);
        sumaPeorDif += difPorcentaje(p, opt);
        cout << setw(8) << s << setw(10) << opt << setw(12) << m << setw(12) << p
             << setw(14) << difPorcentaje(m, opt) << setw(13) << difPorcentaje(p, opt)
             << setw(10) << aciertos << " de " << n << "\n";
    }
    cout << "\nPromedio de la diferencia del mejor inicio: " << sumaMejorDif / instancias << " %\n";
    cout << "Promedio de la diferencia del peor inicio: " << sumaPeorDif / instancias << " %\n";
    cout << "Instancias en que algun inicio alcanza el optimo: " << conOptimo << " de " << instancias << "\n";
    return 0;
}
