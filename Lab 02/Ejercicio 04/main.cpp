#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

void ordenarArreglo(vector<int>& arr, string tipo) {
    int n = arr.size();
    if (tipo == "insertion") {
        for (int i = 1; i < n; i++) {
            int key = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    } else if (tipo == "selection") {
        for (int i = 0; i < n - 1; i++) {
            int minIdx = i;
            for (int j = i + 1; j < n; j++) {
                if (arr[j] < arr[minIdx]) {
                    minIdx = j;
                }
            }
            swap(arr[i], arr[minIdx]);
        }
    } else {
        cout << "Tipo de ordenamiento no valido." << endl;
    }
}

void evaluarRendimiento(int n, string tipo) {
    vector<int> arr(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100000);

    for (int i = 0; i < n; i++) {
        arr[i] = dis(gen);
    }

    // chrono::high_resolution_clock::time_point inicio = chrono::high_resolution_clock::now();
    auto inicio = high_resolution_clock::now();
    ordenarArreglo(arr, tipo);

    //chrono::high_resolution_clock::time_point fin = chrono::high_resolution_clock::now();
    auto fin = high_resolution_clock::now();

    //chrono::milliseconds duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio);
    auto duracion = duration_cast<milliseconds>(fin - inicio);
    cout << "Tiempo para " << n << " elementos (" << tipo << "): " 
         << duracion.count() << " ms" << endl;
}

int main() {
    vector<int> tamanos = {1000, 5000, 10000};
    
    for (int n : tamanos) {
        evaluarRendimiento(n, "insertion");
        evaluarRendimiento(n, "selection");
    }
    
    return 0;
}