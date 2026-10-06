#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <iomanip>
using namespace std;

const int INF = 1e9;

int main() {
    mt19937 generador(2026);
    uniform_int_distribution<int> pesoAleatorio(1, 100);

    for (int n : {10, 100, 500}) {
        vector<vector<pair<int, int>>> grafo(n);
        vector<vector<int>> matriz(n, vector<int>(n, INF));

        for (int i = 0; i < n; i++)
            matriz[i][i] = 0;

        int aristas = 0;

        auto agregarArista = [&](int u, int v, int peso) {
            if (u == v || matriz[u][v] != INF)
                return false;

            grafo[u].push_back({v, peso});
            grafo[v].push_back({u, peso});

            matriz[u][v] = peso;
            matriz[v][u] = peso;
            aristas++;
            return true;
        };

        // Asegura que todos los vertices esten conectados
        for (int i = 1; i < n; i++)
            agregarArista(i - 1, i, pesoAleatorio(generador));

        uniform_int_distribution<int> verticeAleatorio(0, n - 1);

        // Completa el grafo con carreteras aleatorias
        while (aristas < 2 * n) {
            int u = verticeAleatorio(generador);
            int v = verticeAleatorio(generador);
            agregarArista(u, v, pesoAleatorio(generador));
        }

        using reloj = chrono::steady_clock;

        // Dijkstra desde el vertice 0: codigo base con heap
        vector<int> dist(n, INF);
        dist[0] = 0;

        priority_queue<pair<int, int>,
                       vector<pair<int, int>>,
                       greater<pair<int, int>>> pq;
        pq.push({0, 0});

        auto inicioDijkstra = reloj::now();

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > dist[u]) continue;

            for (auto [v, peso] : grafo[u]) {
                if (dist[u] + peso < dist[v]) {
                    dist[v] = dist[u] + peso;
                    pq.push({dist[v], v});
                }
            }
        }

        auto finDijkstra = reloj::now();

        // Floyd-Warshall: codigo base sobre la misma red
        vector<vector<int>> distFW = matriz;

        auto inicioFloyd = reloj::now();

        for (int k = 0; k < n; k++)
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if (distFW[i][k] != INF &&
                        distFW[k][j] != INF &&
                        distFW[i][k] + distFW[k][j] < distFW[i][j]) {
                        distFW[i][j] = distFW[i][k] + distFW[k][j];
                    }

        auto finFloyd = reloj::now();

        // Comprobar que ambos dan las mismas distancias desde 0
        bool coinciden = true;
        for (int v = 0; v < n; v++) {
            if (dist[v] != distFW[0][v])
                coinciden = false;
        }

        double tiempoDijkstra =
            chrono::duration<double, milli>(
                finDijkstra - inicioDijkstra
            ).count();

        double tiempoFloyd =
            chrono::duration<double, milli>(
                finFloyd - inicioFloyd
            ).count();

        cout << "V=" << n << ", E=" << aristas
             << " | Dijkstra: " << fixed << setprecision(3)
             << tiempoDijkstra << " ms"
             << " | Floyd-Warshall: " << tiempoFloyd << " ms"
             << " | Distancias: "
             << (coinciden ? "coinciden" : "ERROR") << '\n';

        if (!coinciden) return 1;
    }

    return 0;
}