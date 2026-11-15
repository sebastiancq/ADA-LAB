#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <set>

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

// Primera mejora, como en el ejemplo de la guia: se invierte el tramo de las
// posiciones i a j cuando eso baja el costo. Solo cambian dos aristas,
// (a, b) y (c, e), que se reemplazan por (a, c) y (b, e), asi que la mejora
// se calcula en O(1) sin recorrer toda la ruta. La posicion 0 queda fija.
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

// Escribe el ciclo desde la ciudad 0 y en el sentido cuyo segundo elemento es
// menor, para reconocer cuando dos inicios terminan en la misma ruta.
vector<int> normalizar(const vector<int>& ruta) {
    int n = ruta.size();
    int p = find(ruta.begin(), ruta.end(), 0) - ruta.begin();
    vector<int> r(n);
    for (int k = 0; k < n; k++) r[k] = ruta[(p + k) % n];
    if (r[1] > r[n - 1]) reverse(r.begin() + 1, r.end());
    return r;
}
int main() {
    const int n = 15;
    vector<vector<double>> dist = matrizDistancias(generarCiudades(n, 2026));
    double optimo = optimoExacto(dist);
    cout << fixed << setprecision(2);
    cout << "Costo optimo exacto (Held-Karp): " << optimo << "\n";

    cout << "\nNearest Neighbor seguido de 2-opt desde cada ciudad de origen\n";
    cout << setw(7) << "Inicio" << setw(9) << "NN" << setw(10) << "2-opt" << setw(12) << "Mejora %"
         << setw(13) << "Movimientos" << setw(9) << "Dif. %" << "   Ruta final desde 0\n";
    set<vector<int>> rutasFinales;
    double mejorNN = 1e18, peorNN = 0, mejorOpt = 1e18, peorOpt = 0;
    int alOptimo = 0;
    for (int inicio = 0; inicio < n; inicio++) {
        vector<int> ruta = nearestNeighbor(dist, inicio);
        double antes = costoRuta(dist, ruta);
        int movimientos = dosOpt(dist, ruta);
        double despues = costoRuta(dist, ruta);
        vector<int> normal = normalizar(ruta);
        rutasFinales.insert(normal);
        mejorNN = min(mejorNN, antes);
        peorNN = max(peorNN, antes);
        mejorOpt = min(mejorOpt, despues);
        peorOpt = max(peorOpt, despues);
        if (despues < optimo + 1e-9) alOptimo++;
        cout << setw(7) << inicio << setw(9) << antes << setw(10) << despues
             << setw(12) << 100.0 * (antes - despues) / antes << setw(13) << movimientos
             << setw(9) << difPorcentaje(despues, optimo) << "   ";
        mostrarRuta(normal);
        cout << "\n";
    }
    cout << "\nRango con Nearest Neighbor: " << mejorNN << " a " << peorNN
         << ", " << 100.0 * (peorNN - mejorNN) / mejorNN << " % entre extremos\n";
    cout << "Rango con 2-opt: " << mejorOpt << " a " << peorOpt
         << ", " << 100.0 * (peorOpt - mejorOpt) / mejorOpt << " % entre extremos\n";
    cout << "Rutas finales distintas: " << rutasFinales.size() << "\n";
    cout << "Inicios que terminan en el optimo: " << alOptimo << " de " << n << "\n";

    // Mismo experimento en 30 instancias para no sacar conclusiones de un solo caso.
    const int instancias = 30;
    cout << "\nResumen en " << instancias << " instancias de 15 ciudades (semillas 1 a " << instancias << ")\n";
    cout << setw(8) << "Semilla" << setw(10) << "Optimo" << setw(11) << "Mejor NN" << setw(12) << "Mejor 2opt"
         << setw(11) << "Peor 2opt" << setw(11) << "Peor dif%" << setw(8) << "Rutas" << setw(15) << "Al optimo\n";
    double sumaMejorNN = 0, sumaMejorOpt = 0, sumaPeorOpt = 0, sumaPromNN = 0, sumaPromOpt = 0;
    int instanciasOptimo = 0, dependeInicio = 0;
    for (int s = 1; s <= instancias; s++) {
        vector<vector<double>> d = matrizDistancias(generarCiudades(n, s));
        double opt = optimoExacto(d);
        double mNN = 1e18, mOpt = 1e18, pOpt = 0, sNN = 0, sOpt = 0;
        int aciertos = 0;
        set<vector<int>> distintas;
        for (int inicio = 0; inicio < n; inicio++) {
            vector<int> ruta = nearestNeighbor(d, inicio);
            double cNN = costoRuta(d, ruta);
            dosOpt(d, ruta);
            double cOpt = costoRuta(d, ruta);
            distintas.insert(normalizar(ruta));
            mNN = min(mNN, cNN);
            mOpt = min(mOpt, cOpt);
            pOpt = max(pOpt, cOpt);
            sNN += difPorcentaje(cNN, opt);
            sOpt += difPorcentaje(cOpt, opt);
            if (cOpt < opt + 1e-9) aciertos++;
        }
        if (aciertos > 0) instanciasOptimo++;
        if (pOpt > mOpt + 1e-9) dependeInicio++;
        sumaMejorNN += difPorcentaje(mNN, opt);
        sumaMejorOpt += difPorcentaje(mOpt, opt);
        sumaPeorOpt += difPorcentaje(pOpt, opt);
        sumaPromNN += sNN / n;
        sumaPromOpt += sOpt / n;
        cout << setw(8) << s << setw(10) << opt << setw(11) << mNN << setw(12) << mOpt
             << setw(11) << pOpt << setw(11) << difPorcentaje(pOpt, opt) << setw(8) << distintas.size()
             << setw(9) << aciertos << " de " << n << "\n";
    }
    cout << "\nPromedios sobre las " << instancias << " instancias, como diferencia con el optimo\n";
    cout << "Nearest Neighbor, promedio de los 15 inicios: " << sumaPromNN / instancias << " %\n";
    cout << "NN + 2-opt, promedio de los 15 inicios: " << sumaPromOpt / instancias << " %\n";
    cout << "Nearest Neighbor, mejor inicio: " << sumaMejorNN / instancias << " %\n";
    cout << "NN + 2-opt, mejor inicio: " << sumaMejorOpt / instancias << " %\n";
    cout << "NN + 2-opt, peor inicio: " << sumaPeorOpt / instancias << " %\n";
    cout << "Instancias donde algun inicio llega al optimo con 2-opt: " << instanciasOptimo << " de " << instancias << "\n";
    cout << "Instancias donde el costo final cambia segun el inicio: " << dependeInicio << " de " << instancias << "\n";
    return 0;
}
