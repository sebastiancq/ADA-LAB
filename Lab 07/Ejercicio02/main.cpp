#include <iostream>
#include <vector>
#include <map>
#include <limits>

using namespace std;

int main() {
    int k, monto;

    cout << "Cantidad de denominaciones: ";
    cin >> k;
    while (!cin.fail() && k <= 0) {
        cout << "Debe haber al menos una denominacion. Cantidad: ";
        cin >> k;
    }

    if (cin.fail()) {
        cout << "\nEntrada invalida: se esperaban numeros enteros.\n";
        return 1;
    }

    vector<int> monedas(k);
    for (int i = 0; i < k && !cin.fail(); i++) {
        cout << "Denominacion " << i + 1 << ": ";
        cin >> monedas[i];
        while (!cin.fail() && monedas[i] <= 0) {
            cout << "La denominacion debe ser positiva. Denominacion " << i + 1 << ": ";
            cin >> monedas[i];
        }
    }

    cout << "Monto objetivo: ";
    cin >> monto;
    while (!cin.fail() && monto < 0) {
        cout << "El monto no puede ser negativo. Monto objetivo: ";
        cin >> monto;
    }

    if (cin.fail()) {
        cout << "\nEntrada invalida: se esperaban numeros enteros.\n";
        return 1;
    }

    const int SIN_SOLUCION = numeric_limits<int>::max();

    // dp[m]: menor cantidad de monedas que suma exactamente m.
    // ultima[m]: moneda con la que se obtuvo ese minimo.
    vector<int> dp(monto + 1, SIN_SOLUCION);
    vector<int> ultima(monto + 1, 0);
    dp[0] = 0;

    for (int m = 1; m <= monto; m++) {
        for (int moneda : monedas) {
            // Se consulta el mismo arreglo: una moneda puede repetirse.
            if (moneda <= m && dp[m - moneda] != SIN_SOLUCION && dp[m - moneda] + 1 < dp[m]) {
                dp[m] = dp[m - moneda] + 1;
                ultima[m] = moneda;
            }
        }
    }

    cout << "\n\nDenominaciones:";
    for (int moneda : monedas) cout << " " << moneda;
    cout << "\nMonto objetivo: " << monto << "\n";

    if (monto <= 30) {
        cout << "\nm      dp[m]  ultima moneda\n";
        for (int m = 0; m <= monto; m++) {
            cout << m << (m < 10 ? "      " : "     ");
            if (dp[m] == SIN_SOLUCION) {
                cout << "sin solucion\n";
            } else {
                cout << dp[m] << (dp[m] < 10 ? "      " : "     ");
                if (m == 0) cout << "ninguna\n";
                else cout << ultima[m] << "\n";
            }
        }
    }

    if (dp[monto] == SIN_SOLUCION) {
        cout << "\nNo existe una combinacion que sume exactamente " << monto << ".\n";
        return 0;
    }

    map<int, int> combinacion;
    for (int m = monto; m > 0; m -= ultima[m]) {
        combinacion[ultima[m]]++;
    }

    cout << "\nMenor cantidad de monedas: " << dp[monto] << "\n";
    cout << "Combinacion:";
    for (const auto& par : combinacion) {
        cout << " " << par.second << " x " << par.first << ";";
    }
    cout << "\n";
    return 0;
}
