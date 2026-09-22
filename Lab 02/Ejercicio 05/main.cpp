#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Libro
{
    string id;
    string titulo;
    float calificacion;
    string autor;
    int paginas;
};

vector<Libro> cargarDatos(const string &archivo)
{
    vector<Libro> datos;
    ifstream file(archivo);
    Libro temp;
    while (file >> temp.id >> temp.titulo >> temp.calificacion >> temp.autor >> temp.paginas)
    {
        datos.push_back(temp);
    }
    return datos;
}

void exportarDatos(const vector<Libro> &arr, const string &archivo)
{
    ofstream file(archivo);
    file << "ID\tTitulo\tCalificacion\tAutor\tPaginas\n";
    for (const auto &l : arr)
    {
        file << l.id << "\t" << l.titulo << "\t" << l.calificacion << "\t"
             << l.autor << "\t" << l.paginas << "\n";
    }
}


void insertionSortDesc(vector<Libro> &arr, int &comparaciones, int &intercambios)
{
    int n = arr.size();
    for (int i = 1; i < n; i++)
    {
        Libro key = arr[i];
        int j = i - 1;

        while (j >= 0)
        {
            comparaciones++;
            if (arr[j].calificacion < key.calificacion)
            {
                arr[j + 1] = arr[j];
                intercambios++;
                j--;
            }
            else
            {
                break;
            }
        }
        arr[j + 1] = key;
    }
}


void selectionSortDesc(vector<Libro> &arr, int &comparaciones, int &intercambios)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
    {
        int maxIdx = i;
        for (int j = i + 1; j < n; j++)
        {
            comparaciones++;
            if (arr[j].calificacion > arr[maxIdx].calificacion)
            {
                maxIdx = j;
            }
        }
        if (maxIdx != i)
        {
            swap(arr[i], arr[maxIdx]);
            intercambios++;
        }
    }
}

int main()
{
    vector<Libro> datos = cargarDatos("libros.txt");

    if (datos.empty())
    {
        cout << "Error: No se encontro libros.txt o esta vacio." << endl;
        return 1;
    }

    vector<Libro> copiaInsertion = datos;
    vector<Libro> copiaSelection = datos;

    int compIns = 0, interIns = 0;
    int compSel = 0, interSel = 0;

    auto inicioIns = high_resolution_clock::now();
    insertionSortDesc(copiaInsertion, compIns, interIns);
    auto finIns = high_resolution_clock::now();
    auto duracionIns = duration_cast<microseconds>(finIns - inicioIns);

    auto inicioSel = high_resolution_clock::now();
    selectionSortDesc(copiaSelection, compSel, interSel);
    auto finSel = high_resolution_clock::now();
    auto duracionSel = duration_cast<microseconds>(finSel - inicioSel);

    cout << "Insertion Sort" << endl;
    cout << "Tiempo de ejecucion: " << duracionIns.count() << " microsegundos" << endl;
    cout << "Comparaciones: " << compIns << " Intercambios: " << interIns << endl;

    cout << "\nSelection Sort" << endl;
    cout << "Tiempo de ejecucion: " << duracionSel.count() << " microsegundos" << endl;
    cout << "Comparaciones: " << compSel << " Intercambios: " << interSel << endl;

    exportarDatos(copiaInsertion, "libros_ordenados_insertion.csv");
    exportarDatos(copiaSelection, "libros_ordenados_selection.csv");

    cout << "\nDatos exportados exitosamente a:" << endl;
    cout << "libros_ordenados_insertion.csv" << endl;
    cout << "libros_ordenados_selection.csv" << endl;

    return 0;
}