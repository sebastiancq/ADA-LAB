#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Arista {
    int u, v, peso;
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

    int n;

    cout << "Ingrese el numero de edificios: ";
    cin >> n;

    vector<string> nombres(n);

    cout << "\nIngrese los nombres de los edificios:\n";

    for (int i = 0; i < n; i++) {
        cout << "Edificio " << i << ": ";
        cin >> nombres[i];
    }

    int m;

    cout << "\nIngrese el numero de conexiones: ";
    cin >> m;

    vector<Arista> aristas;

    cout << "\nIngrese las conexiones\n";
    cout << "Formato: edificio1 edificio2 costo\n\n";

    for (int i = 0; i < m; i++) {

        int u, v, peso;

        cout << "Conexion " << i + 1 << ": ";
        cin >> u >> v >> peso;

        aristas.push_back({u, v, peso});
    }


    sort(
        aristas.begin(),
        aristas.end(),
        [](Arista a, Arista b) {
            return a.peso < b.peso;
        }
    );


    padre.resize(n);
    rango.assign(n, 0);

    for (int i = 0; i < n; i++)
        padre[i] = i;


    int pesoTotal = 0;
    int aristasUsadas = 0;

    cout << "\nRed de cableado de menor costo\n";

    for (auto& a : aristas) {

        if (unir(a.u, a.v)) {

            cout << nombres[a.u]
                 << " - "
                 << nombres[a.v]
                 << " costo "
                 << a.peso
                 << endl;

            pesoTotal += a.peso;
            aristasUsadas++;

            if (aristasUsadas == n - 1)
                break;
        }
    }


    if (aristasUsadas != n - 1) {

        cout << "\nNo es posible conectar todos los edificios."
             << endl;

        return 0;
    }


    cout << "\nCosto minimo total: "
         << pesoTotal
         << endl;

    return 0;
}