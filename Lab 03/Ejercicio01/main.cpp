#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>

using namespace std;

struct Estudiante
{
  string codigo;
  string nombre;
  float promedio_ponderado;
};

void merge(Estudiante arr[], int izq, int medio, int der, long long &comparaciones, long long &intercambios)
{
  int n1 = medio - izq + 1;
  int n2 = der - medio;

  Estudiante L[n1];
  Estudiante R[n2];

  for (int i = 0; i < n1; i++)
    L[i] = arr[izq + i];
  for (int j = 0; j < n2; j++)
    R[j] = arr[medio + 1 + j];

  int i = 0, j = 0, k = izq;

  while (i < n1 && j < n2)
  {
    comparaciones++;
    if (L[i].promedio_ponderado <= R[j].promedio_ponderado)
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
  {
    arr[k++] = L[i++];
    intercambios++;
  }
  while (j < n2)
  {
    arr[k++] = R[j++];
    intercambios++;
  }
}

void mergeSort(Estudiante arr[], int izq, int der, long long &comparaciones, long long &intercambios)
{
  if (izq < der)
  {
    int medio = izq + (der - izq) / 2;

    mergeSort(arr, izq, medio, comparaciones, intercambios);
    mergeSort(arr, medio + 1, der, comparaciones, intercambios);
    merge(arr, izq, medio, der, comparaciones, intercambios);
  }
}

void imprimirEstudiantes(Estudiante arr[], int n)
{
  cout << "\nLista de Estudiantes Ordenada por Promedio Ponderado\n";
  cout << left << setw(6) << "N."
       << setw(15) << "Codigo"
       << setw(25) << "Nombre"
       << right << setw(10) << "Promedio" << "\n";
  cout << "\n";

  for (int i = 0; i < n; i++)
  {
    cout << left << setw(6) << i + 1
         << setw(15) << arr[i].codigo
         << setw(25) << arr[i].nombre
         << right << fixed << setprecision(2)
         << setw(10) << arr[i].promedio_ponderado << "\n";
  }
}

int main()
{
  int n;
  cout << "Ingrese la cantidad de estudiantes a registrar: ";
  cin >> n;

  if (n < 1)
  {
    cout << "La cantidad debe ser mayor que cero.\n";
    return 1;
  }

  Estudiante arr[n];

  for (int i = 0; i < n; i++)
  {
    cout << "\nDatos del Estudiante " << i + 1 << "\n";
    cout << "Codigo: ";
    cin >> arr[i].codigo;

    cout << "Nombre: ";
    cin >> arr[i].nombre;

    cout << "Promedio ponderado: ";
    cin >> arr[i].promedio_ponderado;
  }

  auto inicioOrdenamiento = chrono::high_resolution_clock::now();
  long long comparaciones = 0;
  long long intercambios = 0;
  mergeSort(arr, 0, n - 1, comparaciones, intercambios);
  auto finOrdenamiento = chrono::high_resolution_clock::now();
  chrono::duration<double, milli> tiempoOrdenamiento = finOrdenamiento - inicioOrdenamiento;

  imprimirEstudiantes(arr, n);

  cout << "\nMetricas de Merge Sort\n";
  cout << "Comparaciones: " << comparaciones << "\n";
  cout << "Intercambios: " << intercambios << "\n";
  cout << "Tiempo: " << tiempoOrdenamiento.count() << " ms\n";

  return 0;
}