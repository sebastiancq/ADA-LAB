#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    vector<int> peso = { 2, 3, 4, 5, 1 };
    vector<int> valor = { 3, 4, 5, 8, 2 };
    int n = peso.size();
    int W = 8;

    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            dp[i][w] = dp[i - 1][w];
            if (peso[i - 1] <= w) {
                dp[i][w] = max(dp[i][w], valor[i - 1] + dp[i - 1][w - peso[i - 1]]);
            }
        }
    }

    cout << "Objetos (capacidad " << W << "):\n";
    for (int i = 0; i < n; i++) {
        cout << "Objeto " << i + 1 << ": peso " << peso[i] << ", valor " << valor[i] << "\n";
    }

    cout << "\nTabla dp[i][w], filas i y columnas w\n";
    cout << "   ";
    for (int w = 0; w <= W; w++) cout << setw(4) << w;
    cout << "\n";
    for (int i = 0; i <= n; i++) {
        cout << setw(3) << i;
        for (int w = 0; w <= W; w++) cout << setw(4) << dp[i][w];
        cout << "\n";
    }

    cout << "\nValor maximo: dp[" << n << "][" << W << "] = " << dp[n][W] << "\n";

    cout << "\nReconstruccion desde dp[" << n << "][" << W << "]:\n";
    vector<int> elegidos;
    int w = W;
    for (int i = n; i >= 1; i--) {
        cout << "i = " << i << ": dp[" << i << "][" << w << "] = " << dp[i][w]
             << ", dp[" << i - 1 << "][" << w << "] = " << dp[i - 1][w];
        if (dp[i][w] != dp[i - 1][w]) {
            cout << ". Son distintos: se lleva el objeto " << i
                 << " y w pasa de " << w << " a " << w - peso[i - 1] << "\n";
            elegidos.push_back(i);
            w -= peso[i - 1];
        } else {
            cout << ". Son iguales: el objeto " << i << " no se lleva\n";
        }
    }
    reverse(elegidos.begin(), elegidos.end());

    cout << "\nObjetos elegidos:";
    for (int i : elegidos) cout << " " << i;
    cout << "\n";
    return 0;
}
