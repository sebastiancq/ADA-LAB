#include <iostream>
#include <vector>

using namespace std;

void selectionSortFlotantes(vector<float> &arr)
{
    int n = arr.size();
    int comparaciones = 0;
    int intercambios = 0;

    for (int i = 0; i < n - 1; i++)
    {
        int minIdx = i;
        for (int j = i + 1; j < n; j++)
        {
            comparaciones++;
            if (arr[j] < arr[minIdx])
            {
                minIdx = j;
            }
        }
        if (minIdx != i)
        {
            swap(arr[i], arr[minIdx]);
            intercambios++;
        }
    }

    cout << "Comparaciones realizadas: " << comparaciones << endl;
    cout << "Intercambios realizados: " << intercambios << endl;
}

int main()
{
    vector<float> arr = {
        12.5, 3.14, 7.8, 1.1, 9.99, 4.5, 6.28, 2.71, 8.0, 5.5,
        11.11, 10.0, 0.99, 15.2, 13.7, 18.4, 2.2, 7.1, 19.8, 6.6,
        3.3, 14.9, 8.8, 22.4, 1.5, 16.7, 9.4, 17.3, 5.9, 12.1,
        20.0, 4.1, 10.8, 18.9, 7.6, 2.9, 11.4, 21.7};

    cout << "Arreglo original: ";
    for (float num : arr)
    {
        cout << num << " ";
    }
    cout << endl;

    selectionSortFlotantes(arr);

    cout << "Arreglo ordenado: ";
    for (float num : arr)
    {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}