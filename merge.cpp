#include <iostream>
#include <vector>

using namespace std;

// Función para combinar dos subvectores ordenados en un solo vector ordenado
void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Crear vectores temporales
    vector<int> L(n1), R(n2);

    // Copiar datos a los vectores temporales L[] y R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Combinar los vectores temporales de nuevo en arr[l..r]
    int i = 0; // Índice inicial del primer subvector
    int j = 0; // Índice inicial del segundo subvector
    int k = l; // Índice inicial del subvector combinado
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copiar los elementos restantes de L[], si los hay
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copiar los elementos restantes de R[], si los hay
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Función principal que implementa el algoritmo de merge sort
void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        // Encuentra el punto medio del vector
        int m = l + (r - l) / 2;

        // Ordena primero y segundo subvectores
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // Combina los subvectores ordenados
        merge(arr, l, m, r);
    }
}

int main() {
    vector<int> arr = {12, 11, 13, 5, 6, 7};
    int arr_size = arr.size();

    cout << "Vector original:" << endl;
    for (int i = 0; i < arr_size; i++)
        cout << arr[i] << " ";
    cout << endl;

    mergeSort(arr, 0, arr_size - 1);

    cout << "Vector ordenado:" << endl;
    for (int i = 0; i < arr_size; i++)
        cout << arr[i] << " ";
    cout << endl;

    return 0;
}
