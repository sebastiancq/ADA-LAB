#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

struct Arista {
    int destino;
    long long costo;
};

int main() {
    // Caso propio: elegir conexiones de transporte en cinco etapas.
    const vector<string> nombres = {"A", "B", "C", "D", "E", "F", "G", "H"};
    const vector<vector<int>> etapas = {{0}, {1, 2}, {3, 4}, {5, 6}, {7}};
    const vector<vector<Arista>> grafo = {
        {{1, 4}, {2, 2}},  // A
        {{3, 5}, {4, 1}},  // B
        {{3, 2}, {4, 6}},  // C
        {{5, 3}, {6, 5}},  // D
        {{5, 4}, {6, 2}},  // E
        {{7, 6}},          // F
        {{7, 1}},          // G
        {}                 // H
    };
    const int origen = 0, destino = 7;
    const long long SIN_RUTA = numeric_limits<long long>::lowest() / 4;
    vector<long long> dp(nombres.size(), SIN_RUTA);
    vector<int> siguiente(nombres.size(), -1);
    dp[destino] = 0;

    // Las aristas del caso van de una etapa a la siguiente.
    // Al recorrer etapas al reves, cada vecino ya esta resuelto.
    for (int e = static_cast<int>(etapas.size()) - 1; e >= 0; --e) {
        for (int u : etapas[e]) {
            if (u == destino) continue;
            for (const auto& arista : grafo[u]) {
                const int v = arista.destino;
                if (dp[v] == SIN_RUTA) continue;
                const long long candidato = arista.costo + dp[v];
                if (candidato > dp[u]) {
                    dp[u] = candidato;
                    siguiente[u] = v;
                }
            }
        }
    }

    cout << "Diligencia: camino de costo maximo\n";
    cout << "Etapas: {A} -> {B, C} -> {D, E} -> {F, G} -> {H}\n\n";
    cout << "Conexiones y costos:\n";
    for (size_t u = 0; u < grafo.size(); ++u) {
        for (const auto& arista : grafo[u]) {
            cout << nombres[u] << " -> " << nombres[arista.destino]
                 << ": " << arista.costo << "\n";
        }
    }

    cout << "\nTabla de costos maximos hasta H:\n";
    for (size_t u = 0; u < nombres.size(); ++u) {
        cout << nombres[u] << ": ";
        if (dp[u] == SIN_RUTA) cout << "sin ruta\n";
        else cout << dp[u] << "\n";
    }

    if (dp[origen] == SIN_RUTA) {
        cout << "\nNo existe una ruta del origen al destino.\n";
        return 0;
    }

    cout << "\nCosto maximo: " << dp[origen] << "\n";
    cout << "Ruta: ";
    for (int u = origen; u != -1; u = siguiente[u]) {
        cout << nombres[u];
        if (u == destino) break;
        cout << " -> ";
    }
    cout << "\n";
    return 0;
}
