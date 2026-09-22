#include <iostream>
#include <chrono>
#include <iomanip>
#include <random>
#include <string>

using namespace std;

int partitionFixed(float arr[], int low, int high, int &comparaciones, int &intercambios)
{
  float pivote = arr[high];
  int i = low - 1;

  for (int j = low; j < high; j++)
  {
    comparaciones++;
    if (arr[j] < pivote)
    {
      i++;
      float temporal = arr[i];
      arr[i] = arr[j];
      arr[j] = temporal;
      intercambios++;
    }
  }

  float temporal = arr[i + 1];
  arr[i + 1] = arr[high];
  arr[high] = temporal;
  intercambios++;
  return i + 1;
}

int partitionRandom(float arr[], int low, int high, int &comparaciones, int &intercambios)
{
  static random_device rd;
  static mt19937 generador(rd());
  uniform_int_distribution<int> distribucion(low, high);
  int indicePivote = distribucion(generador);

  float temporal = arr[indicePivote];
  arr[indicePivote] = arr[high];
  arr[high] = temporal;
  intercambios++;

  float pivote = arr[high];
  int i = low - 1;
  for (int j = low; j < high; j++)
  {
    comparaciones++;
    if (arr[j] < pivote)
    {
      i++;
      temporal = arr[i];
      arr[i] = arr[j];
      arr[j] = temporal;
      intercambios++;
    }
  }

  temporal = arr[i + 1];
  arr[i + 1] = arr[high];
  arr[high] = temporal;
  intercambios++;
  return i + 1;
}

void quickSort(float arr[], int low, int high, bool usarAleatorio, int &comparaciones, int &intercambios)
{
  if (low < high)
  {
    int pi;
    if (usarAleatorio)
      pi = partitionRandom(arr, low, high, comparaciones, intercambios);
    else
      pi = partitionFixed(arr, low, high, comparaciones, intercambios);

    quickSort(arr, low, pi - 1, usarAleatorio, comparaciones, intercambios);
    quickSort(arr, pi + 1, high, usarAleatorio, comparaciones, intercambios);
  }
}

void mostrarResultado(string nombreTipo, float datosFijo[], float datosAleatorio[], int n)
{
  int compFijo = 0;
  int compAleatorio = 0;
  int intercambiosFijo = 0;
  int intercambiosAleatorio = 0;

  auto inicioFijo = chrono::high_resolution_clock::now();
  quickSort(datosFijo, 0, n - 1, false, compFijo, intercambiosFijo);
  auto finFijo = chrono::high_resolution_clock::now();

  auto inicioAleatorio = chrono::high_resolution_clock::now();
  quickSort(datosAleatorio, 0, n - 1, true, compAleatorio, intercambiosAleatorio);
  auto finAleatorio = chrono::high_resolution_clock::now();

  chrono::duration<double, milli> tiempoFijo = finFijo - inicioFijo;
  chrono::duration<double, milli> tiempoAleatorio = finAleatorio - inicioAleatorio;

  cout << endl;
  cout << "Caso de entrada " << nombreTipo << "\n";
  cout << left << setw(22) << ""
       << right << setw(18) << "Pivote fijo"
       << setw(22) << "Pivote aleatorio" << "\n\n";
  cout << left << setw(22) << "Comparaciones"
       << right << setw(18) << compFijo
       << setw(22) << compAleatorio << "\n";
  cout << left << setw(22) << "Intercambios"
       << right << setw(18) << intercambiosFijo
       << setw(22) << intercambiosAleatorio << "\n";
  cout << left << setw(22) << "Tiempo (ms)"
       << right << fixed << setprecision(4)
       << setw(18) << tiempoFijo.count()
       << setw(22) << tiempoAleatorio.count() << "\n";
}

void ejecutarAscendente(float datosFijo[], float datosAleatorio[], int n)
{
  for (int i = 0; i < n; i++)
  {
    datosFijo[i] = i * 1.5f;
    datosAleatorio[i] = datosFijo[i];
  }
  mostrarResultado("Ascendente", datosFijo, datosAleatorio, n);
}

void ejecutarDescendente(float datosFijo[], float datosAleatorio[], int n)
{
  for (int i = 0; i < n; i++)
  {
    datosFijo[i] = (n - i) * 1.5f;
    datosAleatorio[i] = datosFijo[i];
  }
  mostrarResultado("Descendente", datosFijo, datosAleatorio, n);
}

void ejecutarAleatorio(float datosFijo[], float datosAleatorio[], int n)
{
  static random_device rd;
  static mt19937 generador(rd());
  uniform_real_distribution<float> distribucion(0.0f, 1000000.0f);

  for (int i = 0; i < n; i++)
  {
    datosFijo[i] = distribucion(generador);
    datosAleatorio[i] = datosFijo[i];
  }
  mostrarResultado("Aleatorio", datosFijo, datosAleatorio, n);
}

int main()
{
  int n = 10000;
  float datosFijo[n];
  float datosAleatorio[n];

  ejecutarAscendente(datosFijo, datosAleatorio, n);
  ejecutarDescendente(datosFijo, datosAleatorio, n);
  ejecutarAleatorio(datosFijo, datosAleatorio, n);

  return 0;
}
