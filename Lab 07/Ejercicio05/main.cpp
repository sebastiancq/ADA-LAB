#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Compra {
    string nombre;
    int costo;
    int beneficio;
};

// Criterio de comparacion: toma las compras por mayor beneficio por sol mientras alcancen.
int seleccionVoraz(const vector<Compra>& compras, int presupuesto, int& gastado) {
    vector<int> orden;
    for (size_t i = 0; i < compras.size(); i++) orden.push_back(i);
    sort(orden.begin(), orden.end(), [&](int a, int b) {
        return compras[a].beneficio * compras[b].costo > compras[b].beneficio * compras[a].costo;
    });

    int beneficio = 0;
    gastado = 0;
    for (int i : orden) {
        if (gastado + compras[i].costo <= presupuesto) {
            gastado += compras[i].costo;
            beneficio += compras[i].beneficio;
        }
    }
    return beneficio;
}

int main() {
    // Caso ilustrativo: equipar un pequeno laboratorio con un presupuesto en soles.
    // El beneficio es una puntuacion estimada de 0 a 100.
    const vector<Compra> compras = {
        {"Laptop de repuesto", 2500, 90},
        {"Monitor de 24 pulgadas", 650, 40},
        {"Impresora multifuncional", 900, 45},
        {"Router WiFi", 300, 22},
        {"Disco externo", 350, 26},
        {"Licencias de ofimatica", 1200, 55},
        {"Proyector", 1800, 72},
        {"UPS", 700, 38},
        {"Sillas ergonomicas", 1500, 50},
        {"Webcam y audifonos", 280, 15}
    };
    const int presupuesto = 5000;
    const int n = compras.size();

    vector<vector<int>> dp(n + 1, vector<int>(presupuesto + 1, 0));
    for (int i = 1; i <= n; i++) {
        const Compra& compra = compras[i - 1];
        for (int s = 0; s <= presupuesto; s++) {
            dp[i][s] = dp[i - 1][s];
            if (compra.costo <= s) {
                dp[i][s] = max(dp[i][s], compra.beneficio + dp[i - 1][s - compra.costo]);
            }
        }
    }

    vector<int> elegidas;
    int s = presupuesto;
    for (int i = n; i >= 1; i--) {
        if (dp[i][s] != dp[i - 1][s]) {
            elegidas.push_back(i - 1);
            s -= compras[i - 1].costo;
        }
    }
    reverse(elegidas.begin(), elegidas.end());

    cout << "Presupuesto: S/ " << presupuesto << "\n\n";
    cout << left << setw(28) << "Compra posible" << right << setw(7) << "Costo"
         << setw(11) << "Beneficio" << "\n";
    for (const Compra& compra : compras) {
        cout << left << setw(28) << compra.nombre << right << setw(7) << compra.costo
             << setw(11) << compra.beneficio << "\n";
    }

    cout << "\nCombinacion elegida:\n";
    int gastado = 0;
    for (int i : elegidas) {
        cout << compras[i].nombre << " (S/ " << compras[i].costo
             << ", beneficio " << compras[i].beneficio << ")\n";
        gastado += compras[i].costo;
    }
    cout << "\nBeneficio maximo: " << dp[n][presupuesto] << "\n";
    cout << "Gasto total: S/ " << gastado << "\n";
    cout << "Saldo sin usar: S/ " << presupuesto - gastado << "\n";

    int gastoVoraz;
    int beneficioVoraz = seleccionVoraz(compras, presupuesto, gastoVoraz);
    cout << "\nSeleccion voraz por beneficio/costo: beneficio " << beneficioVoraz
         << ", gasto S/ " << gastoVoraz << "\n";
    return 0;
}
