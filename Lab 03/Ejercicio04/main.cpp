#include <iostream>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

// --- Insertion Sort ---
void insertionSort(int arr[], int n, long long &comparaciones, long long &intercambios)
{
  for (int i = 1; i < n; i++)
  {
    int key = arr[i];
    int j = i - 1;
    while (j >= 0)
    {
      comparaciones++;
      if (arr[j] <= key)
        break;
      arr[j + 1] = arr[j];
      intercambios++;
      j = j - 1;
    }
    arr[j + 1] = key;
    intercambios++;
  }
}

// --- Selection Sort ---
void selectionSort(int arr[], int n, long long &comparaciones, long long &intercambios)
{
  for (int i = 0; i < n - 1; i++)
  {
    int min_idx = i;
    for (int j = i + 1; j < n; j++)
    {
      comparaciones++;
      if (arr[j] < arr[min_idx])
        min_idx = j;
    }
    if (min_idx != i)
    {
      swap(arr[min_idx], arr[i]);
      intercambios++;
    }
  }
}

// --- Mergesort ---
void merge(int arr[], int izq, int medio, int der, long long &comparaciones, long long &intercambios)
{
  int n1 = medio - izq + 1;
  int n2 = der - medio;
  int L[10000];
  int R[10000];
  for (int i = 0; i < n1; i++)
    L[i] = arr[izq + i];
  for (int j = 0; j < n2; j++)
    R[j] = arr[medio + 1 + j];
  int i = 0, j = 0, k = izq;
  while (i < n1 && j < n2)
  {
    comparaciones++;
    if (L[i] <= R[j])
      arr[k++] = L[i++];
    else
      arr[k++] = R[j++];
    intercambios++;
  }
  while (i < n1)
    arr[k++] = L[i++];
  intercambios++;
  while (j < n2)
    arr[k++] = R[j++];
  intercambios++;
}

void mergeSort(int arr[], int izq, int der, long long &comparaciones, long long &intercambios)
{
  if (izq < der)
  {
    int medio = izq + (der - izq) / 2;
    mergeSort(arr, izq, medio, comparaciones, intercambios);
    mergeSort(arr, medio + 1, der, comparaciones, intercambios);
    merge(arr, izq, medio, der, comparaciones, intercambios);
  }
}

// --- Quicksort ---
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

int main()
{
  int n = 10000;

  // Generar un arreglo original con 10,000 elementos aleatorios
  int original[n];
  random_device rd;
  mt19937 generador(rd());
  uniform_int_distribution<int> distribucion(0, 99999);

  for (int i = 0; i < n; i++)
  {
    original[i] = distribucion(generador);
  }

  // Crear copias exactas para garantizar que todos ordenen los mismos datos
  int arrMerge[n];
  int arrQuick[n];
  int arrInsert[n];
  int arrSelect[n];
  for (int i = 0; i < n; i++)
  {
    arrMerge[i] = original[i];
    arrQuick[i] = original[i];
    arrInsert[i] = original[i];
    arrSelect[i] = original[i];
  }

  cout << "Cantidad de elementos: " << n << endl;

  long long comparacionesMerge = 0, intercambiosMerge = 0;
  long long comparacionesQuick = 0, intercambiosQuick = 0;
  long long comparacionesInsert = 0, intercambiosInsert = 0;
  long long comparacionesSelect = 0, intercambiosSelect = 0;

  // Medir tiempo de Mergesort
  auto startMergeSort = high_resolution_clock::now();
  mergeSort(arrMerge, 0, n - 1, comparacionesMerge, intercambiosMerge);
  auto stopMergeSort = high_resolution_clock::now();
  duration<double, milli> duration1 = stopMergeSort - startMergeSort;
  cout << "Tiempo de Mergesort     : " << duration1.count() << " milisegundos." << endl;

  // Medir tiempo de Quicksort
  auto start2 = high_resolution_clock::now();
  quickSort(arrQuick, 0, n - 1, comparacionesQuick, intercambiosQuick);
  auto stop2 = high_resolution_clock::now();
  duration<double, milli> duration2 = stop2 - start2;
  cout << "Tiempo de Quicksort     : " << duration2.count() << " milisegundos." << endl;

  // Medir tiempo de Insertion Sort
  auto start3 = high_resolution_clock::now();
  insertionSort(arrInsert, n, comparacionesInsert, intercambiosInsert);
  auto stop3 = high_resolution_clock::now();
  duration<double, milli> duration3 = stop3 - start3;
  cout << "Tiempo de Insertion Sort: " << duration3.count() << " milisegundos." << endl;

  // Medir tiempo de Selection Sort
  auto start4 = high_resolution_clock::now();
  selectionSort(arrSelect, n, comparacionesSelect, intercambiosSelect);
  auto stop4 = high_resolution_clock::now();
  duration<double, milli> duration4 = stop4 - start4;
  cout << "Tiempo de Selection Sort: " << duration4.count() << " milisegundos." << endl;

  cout << "\nMetricas" << endl;
  cout << "Mergesort: Comparaciones: " << comparacionesMerge << ", Intercambios: " << intercambiosMerge << endl;
  cout << "Quicksort: Comparaciones: " << comparacionesQuick << ", Intercambios: " << intercambiosQuick << endl;
  cout << "Insertion: Comparaciones: " << comparacionesInsert << ", Intercambios: " << intercambiosInsert << endl;
  cout << "Selection: Comparaciones: " << comparacionesSelect << ", Intercambios: " << intercambiosSelect << endl;

  return 0;
}