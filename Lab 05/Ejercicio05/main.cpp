#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Arista {
    int u, v, costo;
};

vector<int> padre, rango;


// Union Find

int encontrar(int x) {
    if (padre[x] != x)
        padre[x] = encontrar(padre[x]);

    return padre[x];
}

bool unir(int x, int y) {
    int rx = encontrar(x);
    int ry = encontrar(y);

    if (rx == ry)
        return false;

    if (rango[rx] < rango[ry])
        swap(rx, ry);

    padre[ry] = rx;

    if (rango[rx] == rango[ry])
        rango[rx]++;

    return true;
}


// Programa principal

int main() {

    int n = 6;

    string localidades[] = {
        "Cercado",
        "Cayma",
        "Yanahuara",
        "Sachaca",
        "Paucarpata",
        "Socabaya"
    };


    vector<Arista> conexiones = {
        {0, 1, 18},
        {0, 2, 12},
        {0, 3, 25},
        {0, 4, 30},
        {1, 2, 10},
        {1, 3, 22},
        {2, 3, 15},
        {2, 4, 28},
        {3, 4, 24},
        {3, 5, 20},
        {4, 5, 14}
    };


    sort(
        conexiones.begin(),
        conexiones.end(),
        [](Arista a, Arista b) {
            return a.costo < b.costo;
        }
    );


    padre.resize(n);
    rango.assign(n, 0);

    for (int i = 0; i < n; i++)
        padre[i] = i;


    int costoTotal = 0;
    int conexionesUsadas = 0;


    cout << "Red de distribucion de agua en Arequipa\n\n";

    cout << "Conexiones seleccionadas\n";


    for (auto& conexion : conexiones) {

        if (
            unir(
                conexion.u,
                conexion.v
            )
        ) {

            cout << localidades[conexion.u]
                 << " - "
                 << localidades[conexion.v]
                 << " costo "
                 << conexion.costo
                 << " mil soles"
                 << endl;

            costoTotal +=
                conexion.costo;

            conexionesUsadas++;


            if (
                conexionesUsadas ==
                n - 1
            )
                break;
        }
    }


    cout << "\nCosto minimo total: "
         << costoTotal
         << " mil soles"
         << endl;


    return 0;
}