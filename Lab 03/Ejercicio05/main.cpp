#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <random>
#include <string>

using namespace std;

struct Producto
{
  string codigo;
  string nombre;
  float precio;
};

void merge(Producto arr[], int izq, int medio, int der, long long &comparaciones, long long &intercambios)
{
  int n1 = medio - izq + 1;
  int n2 = der - medio;

  Producto L[n1];
  Producto R[n2];

  for (int i = 0; i < n1; i++)
    L[i] = arr[izq + i];
  for (int j = 0; j < n2; j++)
    R[j] = arr[medio + 1 + j];

  int i = 0, j = 0, k = izq;

  while (i < n1 && j < n2)
  {
    comparaciones++;
    // Condición >= para ordenar de mayor a menor precio
    if (L[i].precio >= R[j].precio)
    {
      arr[k++] = L[i++];
    }
    else
    {
      arr[k++] = R[j++];
    }
    intercambios++;
  }

  while (i < n1)
    arr[k++] = L[i++];
  intercambios++;
  while (j < n2)
    arr[k++] = R[j++];
  intercambios++;
}

void mergeSort(Producto arr[], int izq, int der, long long &comparaciones, long long &intercambios)
{
  if (izq < der)
  {
    int medio = izq + (der - izq) / 2;
    mergeSort(arr, izq, medio, comparaciones, intercambios);
    mergeSort(arr, medio + 1, der, comparaciones, intercambios);
    merge(arr, izq, medio, der, comparaciones, intercambios);
  }
}

int partition(Producto arr[], int low, int high, long long &comparaciones, long long &intercambios)
{
  float pivote = arr[high].precio;
  int i = low - 1;

  for (int j = low; j <= high - 1; j++)
  {
    comparaciones++;
    // Condición >= para ordenar de mayor a menor precio
    if (arr[j].precio >= pivote)
    {
      i++;
      // Intercambio manual
      Producto temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
      intercambios++;
    }
  }
  Producto temp = arr[i + 1];
  arr[i + 1] = arr[high];
  arr[high] = temp;
  intercambios++;

  return i + 1;
}

void quickSort(Producto arr[], int low, int high, long long &comparaciones, long long &intercambios)
{
  if (low < high)
  {
    int pi = partition(arr, low, high, comparaciones, intercambios);
    quickSort(arr, low, pi - 1, comparaciones, intercambios);
    quickSort(arr, pi + 1, high, comparaciones, intercambios);
  }
}

// Función auxiliar para imprimir el arreglo
void imprimirProductos(Producto arr[], int n)
{
  cout << left << setw(6) << "N."
       << setw(10) << "Codigo"
       << setw(30) << "Producto"
       << right << setw(12) << "Precio" << "\n";
  cout << "\n";

  for (int i = 0; i < n; i++)
  {
    cout << left << setw(6) << i + 1
         << setw(10) << arr[i].codigo
         << setw(30) << arr[i].nombre
         << right << fixed << setprecision(2)
         << setw(12) << arr[i].precio << "\n";
  }
}

int main()
{
  ifstream archivo("productos.txt");

  if (!archivo.is_open())
  {
    cout << "Error: No se pudo abrir el archivo productos.txt" << endl;
    return 1;
  }

  // n es la cantidad de productos a leer del archivo, que es 100
  int n = 100;

  Producto productosMerge[n];
  Producto productosQuick[n];

  // Lectura del archivo
  for (int i = 0; i < n; i++)
  {
    archivo >> productosMerge[i].codigo >> productosMerge[i].nombre >> productosMerge[i].precio;
    productosQuick[i] = productosMerge[i];
  }
  archivo.close();

  long long comparacionesMerge = 0, intercambiosMerge = 0;
  long long comparacionesQuick = 0, intercambiosQuick = 0;

  auto inicioMerge = chrono::high_resolution_clock::now();
  mergeSort(productosMerge, 0, n - 1, comparacionesMerge, intercambiosMerge);
  auto finMerge = chrono::high_resolution_clock::now();

  auto inicioQuick = chrono::high_resolution_clock::now();
  quickSort(productosQuick, 0, n - 1, comparacionesQuick, intercambiosQuick);
  auto finQuick = chrono::high_resolution_clock::now();

  chrono::duration<double, milli> tiempoMerge = finMerge - inicioMerge;
  chrono::duration<double, milli> tiempoQuick = finQuick - inicioQuick;

  cout << "\nProductos ordenados con Mergesort\n";
  imprimirProductos(productosMerge, n);

  cout << "\nProductos ordenados con Quicksort\n";
  imprimirProductos(productosQuick, n);

  cout << "\nTiempos de ejecucion\n";
  cout << "Mergesort : " << tiempoMerge.count() << " ms\n";
  cout << "Quicksort : " << tiempoQuick.count() << " ms\n";
  cout << "\nMetricas\n";
  cout << "Mergesort: Comparaciones: " << comparacionesMerge << ", Intercambios: " << intercambiosMerge << "\n";
  cout << "Quicksort: Comparaciones: " << comparacionesQuick << ", Intercambios: " << intercambiosQuick << "\n";

  return 0;
}