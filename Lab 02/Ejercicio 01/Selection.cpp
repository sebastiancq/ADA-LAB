#include <iostream>
using namespace std;

void printArray(int arr[], int n, int paso) {
    cout << "Paso " << paso + 1 << ": ";
    for (int i=0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}

void selectionSort(int arr[], int n) {
    int paso = 0;
    printArray(arr, n, paso++);
    for (int i=0; i<n-1; i++) {
        int minIdx = i;
        for (int j=i+1; j<n; j++)
            if (arr[j] < arr[minIdx])
                minIdx = j;
        swap(arr[i], arr[minIdx]);
        printArray(arr, n, paso++);
    }
}

int main() {
    int arr[] = {45, 12, 89, 33, 7, 56, 21, 9};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    selectionSort(arr, n);
    return 0;
}