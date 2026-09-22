#include <iostream>
using namespace std;

void printArray(int arr[], int n, int paso) {
    cout << "Paso "<< paso + 1 << ": ";
    for (int i=0; i<n; i++)
        cout << arr[i] << " ";
    cout << endl;
}

void insertionSort(int arr[], int n) {
    int paso = 0;
    printArray(arr, n, paso++);
    for (int i=1; i<n; i++) {
        int key = arr[i];
        int j=i-1;
        while (j>=0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
        printArray(arr, n, paso++);
    }
}

int main() {
    int arr[] = {45, 12, 89, 33, 7, 56, 21, 9};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    insertionSort(arr, n);
    return 0;
}