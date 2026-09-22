#include <iostream>
#include <chrono>
#include <random>

using namespace std;

int partition(int arr[], int low, int high, long long &comparaciones, long long &intercambios)
{
  int pivote = arr[high];
  int i = low - 1;
  for (int j = low; j <= high - 1; j++)
  {
    comparaciones++;
    if (arr[j] < pivote)
    {
      i++;
      swap(arr[i], arr[j]);
      intercambios++;
    }
  }
  swap(arr[i + 1], arr[high]);
  intercambios++;
  return i + 1;
}

void quickSort(int arr[], int low, int high, long long &comparaciones, long long &intercambios)
{
  if (low < high)
  {
    int pi = partition(arr, low, high, comparaciones, intercambios);
    quickSort(arr, low, pi - 1, comparaciones, intercambios);
    quickSort(arr, pi + 1, high, comparaciones, intercambios);
  }
}

// Busqueda binaria
int busquedaBinaria(int arr[], int left, int right, int x, long long &comparaciones)
{
  while (left <= right)
  {
    int mid = left + (right - left) / 2;
    comparaciones++;
    if (arr[mid] == x)
    {
      return mid;
    }

    comparaciones++;
    if (arr[mid] < x)
    {
      left = mid + 1;
    }
    else
    {
      right = mid - 1;
    }
  }
  return -1;
}

// Busqueda secuencial
int busquedaSecuencial(int arr[], int n, int x, long long &comparaciones)
{
  for (int i = 0; i < n; i++)
  {
    comparaciones++;
    if (arr[i] == x)
    {
      return i;
    }
  }
  return -1;
}

int main()
{
  // Cantidad de elementos a generar
  int n = 50000;

  // Arreglo para quicksort y busqueda binaria
  int arr[n];

  // Arreglo para busqueda secuencial
  int arrDesordenado[n];

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dist(0, 999999);

  for (int i = 0; i < n; i++)
  {
    arr[i] = dist(gen);
    arrDesordenado[i] = arr[i];
  }

  int objetivo = arrDesordenado[n - 1];

  long long comparacionesSecuencial = 0;
  long long comparacionesOrdenamiento = 0;
  long long intercambiosOrdenamiento = 0;
  long long comparacionesBinaria = 0;

  auto inicioSecuencial = chrono::high_resolution_clock::now();
  busquedaSecuencial(arrDesordenado, n, objetivo, comparacionesSecuencial);
  auto finSecuencial = chrono::high_resolution_clock::now();

  auto inicioOrdenamiento = chrono::high_resolution_clock::now();
  quickSort(arr, 0, n - 1, comparacionesOrdenamiento, intercambiosOrdenamiento);
  auto finOrdenamiento = chrono::high_resolution_clock::now();

  auto inicioBinaria = chrono::high_resolution_clock::now();
  busquedaBinaria(arr, 0, n - 1, objetivo, comparacionesBinaria);
  auto finBinaria = chrono::high_resolution_clock::now();

  long long totalComparaciones = comparacionesOrdenamiento + comparacionesBinaria;
  long long totalIntercambios = intercambiosOrdenamiento;

  chrono::duration<double, milli> tiempoSecuencial = finSecuencial - inicioSecuencial;
  chrono::duration<double, milli> tiempoOrdenamiento = finOrdenamiento - inicioOrdenamiento;
  chrono::duration<double, milli> tiempoBinaria = finBinaria - inicioBinaria;
  chrono::duration<double, milli> tiempoEstrategiaBinaria = finBinaria - inicioOrdenamiento;

  cout << "Cantidad de elementos: " << n << endl;
  cout << "Elemento a buscar: " << objetivo << "\n"
       << endl;

  cout << "Estrategia 1: Busqueda Secuencial" << endl;
  cout << "Comparaciones                 : " << comparacionesSecuencial << endl;
  cout << "Tiempo                        : " << tiempoSecuencial.count() << " ms\n"
       << endl;

  cout << "Estrategia 2: Quicksort y Busqueda Binaria" << endl;
  cout << "Comparaciones de ordenamiento : " << comparacionesOrdenamiento << endl;
  cout << "Intercambios de ordenamiento  : " << intercambiosOrdenamiento << endl;
  cout << "Comparaciones de busqueda     : " << comparacionesBinaria << endl;
  cout << "Comparaciones totales         : " << totalComparaciones << endl;
  cout << "Intercambios totales          : " << totalIntercambios << endl;
  cout << "Tiempo de ordenamiento        : " << tiempoOrdenamiento.count() << " ms" << endl;
  cout << "Tiempo de busqueda binaria    : " << tiempoBinaria.count() << " ms" << endl;
  cout << "Tiempo total de estrategia    : " << tiempoEstrategiaBinaria.count() << " ms\n"
       << endl;

  return 0;
}