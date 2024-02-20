#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::chrono;
// Función para combinar dos subvectores ordenados en un solo vector ordenado (secuencial)
void mergeSequential(vector<int>& arr, int l, int m, int r) {
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

// Función para realizar la fusión de dos subvectores ordenados en paralelo
void mergeParallel(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Crear vectores temporales
    vector<int> L(n1), R(n2);

    // Copiar datos a los vectores temporales L[] y R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Threads para fusionar los dos subvectores en paralelo
    thread t1([&arr, &L, &n1, &R, &n2, &l]() {
        int i = 0, j = 0, k = l;
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

        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }
    });

    thread t2([&arr, &L, &n1, &R, &n2, &l, &m, &r]() {
        int i = n1 - 1, j = n2 - 1, k = r;
        while (i >= 0 && j >= 0) {
            if (L[i] >= R[j]) {
                arr[k] = L[i];
                i--;
            }
            else {
                arr[k] = R[j];
                j--;
            }
            k--;
        }

        while (j >= 0) {
            arr[k] = R[j];
            j--;
            k--;
        }
    });

    // Esperar a que ambos hilos terminen
    t1.join();
    t2.join();
}

// Función principal que implementa el algoritmo de merge sort de manera secuencial
void mergeSortSequential(vector<int>& arr, int l, int r) {
    if (l < r) {
        // Encuentra el punto medio del vector
        int m = l + (r - l) / 2;

        // Ordena primero y segundo subvectores
        mergeSortSequential(arr, l, m);
        mergeSortSequential(arr, m + 1, r);

        // Combina los subvectores ordenados
        mergeSequential(arr, l, m, r);
    }
}

// Función principal que implementa el algoritmo de merge sort con paralelismo
void mergeSortParallel(vector<int>& arr, int l, int r, int depth) {
    if (l < r) {
        if (depth <= 0) {
            mergeSortSequential(arr, l, r);
            return;
        }
        
        // Encuentra el punto medio del vector
        int m = l + (r - l) / 2;

        // Ordena primero y segundo subvectores en paralelo
        thread t1(mergeSortParallel, ref(arr), l, m, depth - 1);
        thread t2(mergeSortParallel, ref(arr), m + 1, r, depth - 1);

        // Espera a que ambos hilos terminen
        t1.join();
        t2.join();

        // Combina los subvectores ordenados en paralelo
        mergeParallel(arr, l, m, r);
    }
}

int main() {
    vector<int> arr = {12, 11, 13, 5, 6, 7,43,54,631,65,34,12,3,4,6,7,3,14,634,6,3,1,355,785,5,3,5,3,234,243,67,
    46,752,542,768,53,767,45,76,234,768,343,863,234,65,788,54,54,23,3455,6677,88,99,2000};
    int arr_size = arr.size();

    cout << "Vector original:" << endl;
    for (int i = 0; i < arr_size; i++)
        cout << arr[i] << " ";
    cout << endl;

    auto startSequential = high_resolution_clock::now();
    mergeSortSequential(arr, 0, arr_size - 1);
    auto stopSequential = high_resolution_clock::now();
    auto durationSequential = duration_cast<microseconds>(stopSequential - startSequential);

    cout << "Vector ordenado secuencialmente:" << endl;
    for (int i = 0; i < arr_size; i++)
        cout << arr[i] << " ";
    cout << endl;

    // Vector original para el merge sort paralelo
    vector<int> arr_parallel = {};  

    auto startParallel = high_resolution_clock::now();
    mergeSortParallel(arr_parallel, 0, arr_size - 1, 2);
    auto stopParallel = high_resolution_clock::now();
    auto durationParallel = duration_cast<microseconds>(stopParallel - startParallel);

    cout << "Vector ordenado con paralelismo:" << endl;
    for (int i = 0; i < arr_size; i++)
        cout << arr_parallel[i] << " ";
    cout << endl;

    cout << "Tiempo secuencial: " << durationSequential.count() << " microseconds" << endl;
    cout << "Tiempo paralelo: " << durationParallel.count() << " microseconds" << endl;

    return 0;
}