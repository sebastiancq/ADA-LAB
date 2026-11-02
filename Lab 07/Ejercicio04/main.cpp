#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

// Bytes reservados por la tabla: el vector externo, sus filas y los enteros.
size_t memoriaTabla(const vector<vector<int>>& dp) {
    size_t bytes = sizeof(dp) + dp.capacity() * sizeof(vector<int>);
    for (const auto& fila : dp) {
        bytes += fila.capacity() * sizeof(int);
    }
    return bytes;
}

size_t memoriaArreglo(const vector<int>& dp) {
    return sizeof(dp) + dp.capacity() * sizeof(int);
}

int mochilaTabla(const vector<int>& peso, const vector<int>& valor, int W, size_t& bytes) {
    int n = peso.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            dp[i][w] = dp[i - 1][w];
            if (peso[i - 1] <= w) {
                dp[i][w] = max(dp[i][w], valor[i - 1] + dp[i - 1][w - peso[i - 1]]);
            }
        }
    }
    bytes = memoriaTabla(dp);
    return dp[n][W];
}

int mochilaArreglo(const vector<int>& peso, const vector<int>& valor, int W, size_t& bytes) {
    int n = peso.size();
    vector<int> dp(W + 1, 0);
    for (int i = 0; i < n; i++) {
        // De derecha a izquierda: dp[w - peso] todavia es el valor de la fila anterior.
        for (int w = W; w >= peso[i]; w--) {
            dp[w] = max(dp[w], valor[i] + dp[w - peso[i]]);
        }
    }
    bytes = memoriaArreglo(dp);
    return dp[W];
}

void imprimirEncabezado(int W) {
    cout << "        w:";
    for (int w = 0; w <= W; w++) cout << setw(4) << w;
    cout << "\n";
}

void imprimirArreglo(int objeto, const vector<int>& dp) {
    cout << "objeto " << objeto << ": ";
    for (int x : dp) cout << setw(4) << x;
    cout << "\n";
}

int main() {
    vector<int> peso = { 1, 2, 3 };
    vector<int> valor = { 2, 3, 4 };
    int W = 4;

    cout << "Objetos: (peso 1, valor 2), (peso 2, valor 3), (peso 3, valor 4). Capacidad 4\n";

    cout << "\nDe derecha a izquierda (cada objeto una vez):\n";
    imprimirEncabezado(W);
    vector<int> derecha(W + 1, 0);
    for (size_t i = 0; i < peso.size(); i++) {
        for (int w = W; w >= peso[i]; w--) {
            derecha[w] = max(derecha[w], valor[i] + derecha[w - peso[i]]);
        }
        imprimirArreglo(i + 1, derecha);
    }

    cout << "\nDe izquierda a derecha (un objeto puede repetirse):\n";
    imprimirEncabezado(W);
    vector<int> izquierda(W + 1, 0);
    for (size_t i = 0; i < peso.size(); i++) {
        for (int w = peso[i]; w <= W; w++) {
            izquierda[w] = max(izquierda[w], valor[i] + izquierda[w - peso[i]]);
        }
        imprimirArreglo(i + 1, izquierda);
    }

    cout << "\nValor con recorrido de derecha a izquierda: " << derecha[W] << "\n";
    cout << "Valor con recorrido de izquierda a derecha: " << izquierda[W] << "\n";

    cout << "\nMemoria reservada (bytes)\n";
    cout << setw(6) << "n" << setw(8) << "W" << setw(9) << "Valor2D" << setw(9) << "Valor1D"
         << setw(12) << "Tabla2D" << setw(11) << "Arreglo1D" << setw(10) << "Relacion" << "\n";

    vector<vector<int>> casos = { {3, 4}, {8, 20}, {100, 1000}, {1000, 10000} };
    for (const auto& caso : casos) {
        int n = caso[0];
        int capacidad = caso[1];
        vector<int> p, v;
        if (n == 3) {
            p = peso;
            v = valor;
        } else {
            // Datos reproducibles generados con una formula fija.
            for (int i = 0; i < n; i++) {
                p.push_back(1 + (i * 7) % 50);
                v.push_back(10 + (i * 13) % 90);
            }
        }
        size_t bytes2D, bytes1D;
        int valor2D = mochilaTabla(p, v, capacidad, bytes2D);
        int valor1D = mochilaArreglo(p, v, capacidad, bytes1D);
        cout << setw(6) << n << setw(8) << capacidad << setw(9) << valor2D << setw(9) << valor1D
             << setw(12) << bytes2D << setw(11) << bytes1D
             << setw(9) << fixed << setprecision(1) << double(bytes2D) / bytes1D << "x\n";
    }
    return 0;
}
