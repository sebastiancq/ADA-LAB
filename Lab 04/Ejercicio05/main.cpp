#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

void mostrarMatriz(const vector<vector<int>>& grafo)
{
    cout << "\nMatriz de adyacencia\n";
    cout << setw(8) << right << "";

    for (size_t i = 0; i < grafo.size(); i++)
        cout << setw(8) << i;
    cout << '\n';

    for (size_t i = 0; i < grafo.size(); i++)
    {
        cout << setw(8) << i;

        for (int distancia : grafo[i])
            cout << setw(8) << distancia;
        cout << '\n';
    }
}

int main()
{
    int n = 7;

    vector<string> lugares = {
        "UNSA",
        "Plaza de Armas",
        "Terminal Terrestre",
        "Parque Lambramani",
        "Mall Aventura Porongoche",
        "Yanahuara",
        "Cerro Colorado"
    };

    vector<vector<int>> grafo(n, vector<int>(n, 0));

    auto agregarConexion = [&](int u, int v, int distancia)
    {
        grafo[u][v] = distancia;
        grafo[v][u] = distancia;
    };

    agregarConexion(0, 1, 2500); // UNSA - Plaza de Armas
    agregarConexion(0, 3, 1200); // UNSA - Parque Lambramani
    agregarConexion(0, 4, 2800); // UNSA - Mall Aventura Porongoche

    agregarConexion(1, 2, 3500); // Plaza de Armas - Terminal Terrestre
    agregarConexion(1, 5, 2200); // Plaza de Armas - Yanahuara
    agregarConexion(1, 3, 3000); // Plaza de Armas - Parque Lambramani

    agregarConexion(2, 3, 2500); // Terminal - Parque Lambramani
    agregarConexion(2, 4, 4200); // Terminal - Mall Aventura

    agregarConexion(3, 4, 1800); // Parque Lambramani - Mall Aventura

    agregarConexion(5, 6, 5000); // Yanahuara - Cerro Colorado
    agregarConexion(1, 6, 6500); // Plaza de Armas - Cerro Colorado

    cout << "MAPA DE LUGARES DE AREQUIPA\n";
    mostrarMatriz(grafo);

    cout << "\nLugares disponibles:\n";

    for (int i = 0; i < n; i++)
    {
        cout << i << ": " << lugares[i] << endl;
    }

    int origen;
    int destino;

    cout << "\nIngrese el numero del lugar de origen: ";
    cin >> origen;

    cout << "Ingrese el numero del lugar de destino: ";
    cin >> destino;

    if (origen < 0 || origen >= n ||
        destino < 0 || destino >= n)
    {
        cout << "Lugar invalido." << endl;
        return 1;
    }

    // Dijkstra
    vector<int> distancia(n, INT_MAX);
    vector<int> previo(n, -1);
    vector<bool> visitado(n, false);

    distancia[origen] = 0;

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > cola;

    cola.push({0, origen});

    while (!cola.empty())
    {
        int distanciaActual = cola.top().first;
        int u = cola.top().second;

        cola.pop();

        if (visitado[u])
            continue;

        visitado[u] = true;

        for (int v = 0; v < n; v++)
        {
            if (grafo[u][v] != 0)
            {
                int nuevaDistancia =
                    distanciaActual + grafo[u][v];

                if (nuevaDistancia < distancia[v])
                {
                    distancia[v] = nuevaDistancia;
                    previo[v] = u;

                    cola.push({distancia[v], v});
                }
            }
        }
    }

    if (distancia[destino] == INT_MAX)
    {
        cout << "\nNo existe una ruta entre los lugares."
             << endl;

        return 0;
    }

    // Reconstruccion de la ruta
    vector<int> ruta;

    int actual = destino;

    while (actual != -1)
    {
        ruta.push_back(actual);

        if (actual == origen)
            break;

        actual = previo[actual];
    }

    reverse(ruta.begin(), ruta.end());

    cout << "\nRESULTADO\n";

    cout << "Origen: "
         << lugares[origen] << endl;

    cout << "Destino: "
         << lugares[destino] << endl;

    cout << "Distancia minima: "
         << distancia[destino]
         << " metros" << endl;

    cout << "Ruta mas corta: ";

    for (size_t i = 0; i < ruta.size(); i++)
    {
        cout << lugares[ruta[i]];

        if (i + 1 < ruta.size())
            cout << " -> ";
    }

    cout << endl;

    return 0;
}