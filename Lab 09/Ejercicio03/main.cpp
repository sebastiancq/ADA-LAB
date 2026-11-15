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

// Misma generacion y misma semilla que los ejercicios 1 y 2.
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

void mostrarRuta(const vector<int>& ruta) {
    for (int c : ruta) cout << c << "-";
    cout << ruta[0];
}

// Cheapest insertion: parte del ciclo formado por la ciudad de inicio y su
// vecina mas cercana. En cada paso prueba todas las ciudades no visitadas k en
// todas las aristas (a, b) del ciclo y elige el par que menos aumenta el costo,
// dist[a][k] + dist[k][b] - dist[a][b]. Con traza activa imprime cada decision.
vector<int> cheapestInsertion(const vector<vector<double>>& dist, int inicio, bool traza) {
    int n = dist.size();
    vector<bool> enRuta(n, false);
    int vecina = -1;
    for (int c = 0; c < n; c++) {
        if (c != inicio && (vecina == -1 || dist[inicio][c] < dist[inicio][vecina])) vecina = c;
    }
    vector<int> ruta = { inicio, vecina };
    enRuta[inicio] = enRuta[vecina] = true;
    if (traza) {
        cout << "Ciclo inicial: ";
        mostrarRuta(ruta);
        cout << ", costo " << costoRuta(dist, ruta) << "\n";
    }
    for (int paso = 2; paso < n; paso++) {
        double mejorAumento = 1e18;
        int mejorCiudad = -1, mejorPos = -1;
        for (int k = 0; k < n; k++) {
            if (enRuta[k]) continue;
            for (size_t p = 0; p < ruta.size(); p++) {
                int a = ruta[p];
                int b = ruta[(p + 1) % ruta.size()];
                double aumento = dist[a][k] + dist[k][b] - dist[a][b];
                if (aumento < mejorAumento) {
                    mejorAumento = aumento;
                    mejorCiudad = k;
                    mejorPos = p + 1;
                }
            }
        }
        int a = ruta[mejorPos - 1];
        int b = ruta[mejorPos % ruta.size()];
        ruta.insert(ruta.begin() + mejorPos, mejorCiudad);
        enRuta[mejorCiudad] = true;
        if (traza) {
            cout << "Paso " << paso - 1 << ": inserta " << mejorCiudad << " entre " << a << " y " << b
                 << ", aumento " << mejorAumento << ", ciclo ";
            mostrarRuta(ruta);
            cout << ", costo " << costoRuta(dist, ruta) << "\n";
        }
    }
    return ruta;
}

// Programacion dinamica de Held y Karp para conocer el optimo de referencia.
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

double difPorcentaje(double costo, double optimo) {
    double d = costo - optimo;
    if (fabs(d) < 1e-9) d = 0;
    return 100.0 * d / optimo;
}

int main() {
    cout << fixed << setprecision(2);

    // Matriz de 5 ciudades del ejemplo de la guia, cuyo optimo es 70.
    vector<vector<double>> guia = {
        { 0, 10, 15, 20, 25 },
        { 10, 0, 35, 25, 30 },
        { 15, 35, 0, 30, 5 },
        { 20, 25, 30, 0, 15 },
        { 25, 30, 5, 15, 0 }
    };
    cout << "TSP de 5 ciudades de la guia, cheapest insertion desde la ciudad 2\n";
    cheapestInsertion(guia, 2, true);
    cout << "\nComparacion por ciudad de inicio en el TSP de 5 ciudades\n";
    cout << setw(7) << "Inicio" << setw(10) << "NN" << setw(10) << "CI" << "\n";
    for (int inicio = 0; inicio < 5; inicio++) {
        cout << setw(7) << inicio << setw(10) << costoRuta(guia, nearestNeighbor(guia, inicio))
             << setw(10) << costoRuta(guia, cheapestInsertion(guia, inicio, false)) << "\n";
    }

    const int n = 15;
    vector<vector<double>> dist = matrizDistancias(generarCiudades(n, 2026));
    double optimo = optimoExacto(dist);
    cout << "\nTSP de 15 ciudades (semilla 2026), optimo exacto " << optimo << "\n";
    cout << setw(7) << "Inicio" << setw(10) << "NN" << setw(9) << "Dif %" << setw(10) << "CI"
         << setw(9) << "Dif %" << "   Ruta CI\n";
    double sumaNN = 0, sumaCI = 0, mejorNN = 1e18, peorNN = 0, mejorCI = 1e18, peorCI = 0;
    int ganaCI = 0, empate = 0;
    for (int inicio = 0; inicio < n; inicio++) {
        double cNN = costoRuta(dist, nearestNeighbor(dist, inicio));
        vector<int> rutaCI = cheapestInsertion(dist, inicio, false);
        double cCI = costoRuta(dist, rutaCI);
        sumaNN += cNN;
        sumaCI += cCI;
        mejorNN = min(mejorNN, cNN);
        peorNN = max(peorNN, cNN);
        mejorCI = min(mejorCI, cCI);
        peorCI = max(peorCI, cCI);
        if (fabs(cCI - cNN) < 1e-9) empate++;
        else if (cCI < cNN) ganaCI++;
        cout << setw(7) << inicio << setw(10) << cNN << setw(9) << difPorcentaje(cNN, optimo)
             << setw(10) << cCI << setw(9) << difPorcentaje(cCI, optimo) << "   ";
        mostrarRuta(rutaCI);
        cout << "\n";
    }
    cout << "\nNN: mejor " << mejorNN << ", promedio " << sumaNN / n << ", peor " << peorNN << "\n";
    cout << "CI: mejor " << mejorCI << ", promedio " << sumaCI / n << ", peor " << peorCI << "\n";
    cout << "CI mejor que NN en " << ganaCI << " de " << n << " inicios, empate en " << empate << "\n";

    // Repeticion en 30 instancias, promediando la diferencia de los 15 inicios.
    const int instancias = 30;
    cout << "\nResumen en " << instancias << " instancias de 15 ciudades, diferencia con el optimo en %\n";
    cout << setw(8) << "Semilla" << setw(10) << "NN prom" << setw(10) << "CI prom" << setw(11) << "NN mejor"
         << setw(11) << "CI mejor" << setw(10) << "NN peor" << setw(10) << "CI peor" << "\n";
    double tNNprom = 0, tCIprom = 0, tNNmejor = 0, tCImejor = 0, tNNpeor = 0, tCIpeor = 0;
    int instGanaCI = 0;
    for (int s = 1; s <= instancias; s++) {
        vector<vector<double>> d = matrizDistancias(generarCiudades(n, s));
        double opt = optimoExacto(d);
        double sNN = 0, sCI = 0, mNN = 1e18, mCI = 1e18, pNN = 0, pCI = 0;
        for (int inicio = 0; inicio < n; inicio++) {
            double a = difPorcentaje(costoRuta(d, nearestNeighbor(d, inicio)), opt);
            double b = difPorcentaje(costoRuta(d, cheapestInsertion(d, inicio, false)), opt);
            sNN += a;
            sCI += b;
            mNN = min(mNN, a);
            mCI = min(mCI, b);
            pNN = max(pNN, a);
            pCI = max(pCI, b);
        }
        if (sCI < sNN) instGanaCI++;
        tNNprom += sNN / n;
        tCIprom += sCI / n;
        tNNmejor += mNN;
        tCImejor += mCI;
        tNNpeor += pNN;
        tCIpeor += pCI;
        cout << setw(8) << s << setw(10) << sNN / n << setw(10) << sCI / n << setw(11) << mNN
             << setw(11) << mCI << setw(10) << pNN << setw(10) << pCI << "\n";
    }
    cout << setw(8) << "Media" << setw(10) << tNNprom / instancias << setw(10) << tCIprom / instancias
         << setw(11) << tNNmejor / instancias << setw(11) << tCImejor / instancias
         << setw(10) << tNNpeor / instancias << setw(10) << tCIpeor / instancias << "\n";
    cout << "Instancias donde CI tiene menor promedio que NN: " << instGanaCI << " de " << instancias << "\n";
    return 0;
}
