#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;
using Reloj = chrono::steady_clock;

long long fibonacciIngenuo(int n) {
    if (n <= 1)
        return n;
    return fibonacciIngenuo(n - 1) + fibonacciIngenuo(n - 2);
}

long long resolverMemorizado(int n, vector<long long>& memoria) {
    if (n <= 1)
        return n;
    if (memoria[n] != -1)
        return memoria[n];

    memoria[n] = resolverMemorizado(n - 1, memoria)
               + resolverMemorizado(n - 2, memoria);
    return memoria[n];
}

long long fibonacciMemorizado(int n) {
    vector<long long> memoria(n + 1, -1);
    return resolverMemorizado(n, memoria);
}

long long fibonacciTabulado(int n) {
    if (n <= 1)
        return n;

    vector<long long> tabla(n + 1, 0);
    tabla[1] = 1;
    for (int i = 2; i <= n; ++i)
        tabla[i] = tabla[i - 1] + tabla[i - 2];
    return tabla[n];
}

struct Medicion {
    long long resultado;
    double microsegundos;
};

Medicion medir(long long (*funcion)(int), int n, int repeticiones) {
    // El calentamiento no forma parte del intervalo medido.
    funcion(n);
    volatile int entrada = n;
    volatile long long resultado = 0;
    const auto inicio = Reloj::now();
    for (int i = 0; i < repeticiones; ++i)
        resultado = funcion(entrada);
    const auto fin = Reloj::now();

    const double total = chrono::duration<double, micro>(fin - inicio).count();
    return {resultado, total / repeticiones};
}

int main() {
    const vector<int> valores = {10, 20, 30};
    const vector<long long> esperados = {55, 6765, 832040};
    const int repeticiones = 30;

    // Casos base y resultados conocidos; solo se informa si hay un error.
    for (int n : {0, 1, 2}) {
        const long long esperado = n == 0 ? 0 : 1;
        if (fibonacciIngenuo(n) != esperado
            || fibonacciMemorizado(n) != esperado
            || fibonacciTabulado(n) != esperado) {
            cerr << "Error en los casos base de Fibonacci.\n";
            return 1;
        }
    }

    cout << "Tiempo promedio por calculo en microsegundos ("
         << repeticiones << " repeticiones).\n";
    cout << left << setw(6) << "n" << setw(14) << "F(n)"
         << setw(18) << "Ingenuo" << setw(18) << "Memorizacion"
         << "Tabulacion\n";
    cout << fixed << setprecision(6);

    for (size_t i = 0; i < valores.size(); ++i) {
        const int n = valores[i];
        const Medicion ingenuo = medir(fibonacciIngenuo, n, repeticiones);
        const Medicion memorizado = medir(fibonacciMemorizado, n, repeticiones);
        const Medicion tabulado = medir(fibonacciTabulado, n, repeticiones);
        if (ingenuo.resultado != esperados[i]
            || memorizado.resultado != esperados[i]
            || tabulado.resultado != esperados[i]) {
            cerr << "Error al calcular F(" << n << ").\n";
            return 1;
        }

        cout << left << setw(6) << n << setw(14) << ingenuo.resultado
             << setw(18) << ingenuo.microsegundos
             << setw(18) << memorizado.microsegundos
             << tabulado.microsegundos << '\n';
    }
    return 0;
}
