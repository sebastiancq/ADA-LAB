#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Objeto {
    int peso;
    int valor;
};

int main() {
    int n, W;

    cout << "Cantidad de objetos (minimo 8): ";
    cin >> n;
    while (!cin.fail() && n < 8) {
        cout << "Se necesitan al menos 8 objetos. Cantidad de objetos: ";
        cin >> n;
    }

    cout << "Capacidad de la mochila: ";
    cin >> W;
    while (!cin.fail() && W < 0) {
        cout << "La capacidad no puede ser negativa. Capacidad: ";
        cin >> W;
    }

    if (cin.fail()) {
        cout << "\nEntrada invalida: se esperaban numeros enteros.\n";
        return 1;
    }

    vector<Objeto> objetos(n);
    for (int i = 0; i < n && !cin.fail(); i++) {
        cout << "Objeto " << i + 1 << " (peso valor): ";
        cin >> objetos[i].peso >> objetos[i].valor;
        while (!cin.fail() && (objetos[i].peso <= 0 || objetos[i].valor < 0)) {
            cout << "El peso debe ser positivo y el valor no negativo. Objeto "
                 << i + 1 << " (peso valor): ";
            cin >> objetos[i].peso >> objetos[i].valor;
        }
    }

    if (cin.fail()) {
        cout << "\nEntrada invalida: se esperaban numeros enteros.\n";
        return 1;
    }

    // B: la fila 0 y la columna 0 quedan en 0.
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            dp[i][w] = dp[i - 1][w];
            if (objetos[i - 1].peso <= w) {
                dp[i][w] = max(dp[i][w],
                               objetos[i - 1].valor + dp[i - 1][w - objetos[i - 1].peso]);
            }
        }
    }

    // Si la celda difiere de la fila anterior, el objeto i forma parte de la solucion.
    vector<int> elegidos;
    int w = W;
    for (int i = n; i >= 1; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            elegidos.push_back(i);
            w -= objetos[i - 1].peso;
        }
    }
    reverse(elegidos.begin(), elegidos.end());

    cout << "\n\nObjetos ingresados (capacidad " << W << "):\n";
    for (int i = 0; i < n; i++) {
        cout << "Objeto " << i + 1 << ": peso " << objetos[i].peso
             << ", valor " << objetos[i].valor << "\n";
    }

    cout << "\nValor maximo: " << dp[n][W] << "\n";
    cout << "Objetos elegidos:";
    int pesoTotal = 0;
    for (int i : elegidos) {
        cout << " " << i;
        pesoTotal += objetos[i - 1].peso;
    }
    if (elegidos.empty()) cout << " ninguno";
    cout << "\nPeso usado: " << pesoTotal << " de " << W << "\n";
    return 0;
}
