#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
using namespace std;
using namespace std::chrono;
using std::cout;
void mergeSecuencial(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0; 
    int j = 0; 
    int k = l; 
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;}
            else {
            arr[k] = R[j];
            j++; }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeParalelo(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    thread t3([&arr, &L, &n1, &R, &n2, &l]() {
        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                arr[k] = L[i];
                i++;
            }else {
                arr[k] = R[j];
                j++;}
            k++;
        }
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }
    });
    thread t4([&arr, &L, &n1, &R, &n2, &l, &m, &r]() {
        int i = n1 - 1, j = n2 - 1, k = r;
        while (i >= 0 && j >= 0) {
            if (L[i] >= R[j]) {
                arr[k] = L[i];
                i--;
            }else {
                arr[k] = R[j];
                j--;}
            k--;
        }
        while (j >= 0) {
            arr[k] = R[j];
            j--;
            k--;
        }
    });
    t3.join();
    t4.join();
}

void mergeSortSecuencial
(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortSecuencial
        (arr, l, m);
        mergeSortSecuencial
        (arr, m + 1, r);
        mergeSecuencial(arr, l, m, r);
    }
}

void mergeSortParallel(vector<int>& arr, int l, int r, int profundidad) {
    if (l < r) {
        if (profundidad <= 0) {
            mergeSortSecuencial
            (arr, l, r);
            return;
        }
        
        int m = l + (r - l) / 2;
        thread t1(mergeSortParallel, ref(arr), l, m, profundidad - 1);
        thread t2(mergeSortParallel, ref(arr), m + 1, r, profundidad - 1);
        t1.join();
        t2.join();

        mergeParalelo(arr, l, m, r);
    }
}

int main() {
    vector<int> arr;
    ifstream file("array.txt");
    string line;
    if (file.is_open()) {
        while (getline(file, line, ',')) {
            int num;
            stringstream ss(line);
            ss >> num;
            arr.push_back(num);
        }
        file.close();
    } 
    int arr_size = arr.size();
    // Inicia el conteo de tiempo .
    auto inicioSecuencial = high_resolution_clock::now();
    mergeSortSecuencial(arr, 0, arr_size - 1);
     // Finaliza el conteo después de ejecutar el algoritmo.
    auto finalizaSecuencial = high_resolution_clock::now();
     // Se calcula la duración total de la ejecución restando los tiempos de inicio y finalización.
    auto duracionSecuencial = duration_cast<microseconds>(finalizaSecuencial - inicioSecuencial);

    vector<int> arr_parallel = arr;  

    auto iniciaParalela = high_resolution_clock::now();
    mergeSortParallel(arr_parallel, 0, arr_size - 1, 2);
    auto finalizaParalela = high_resolution_clock::now();
    auto duracionParalela = duration_cast<microseconds>(finalizaParalela - iniciaParalela);

    cout << "Tiempo secuencial: " << duracionSecuencial.count() << " microseconds" << endl;
    cout << "Tiempo paralelo: " << duracionParalela.count() << " microseconds" << endl;
    double speedup = static_cast<double>(duracionSecuencial.count()) / duracionParalela.count();
    cout<<"Speedup: "<<speedup<<endl;
    int num_threads=4;
    double effiency = (speedup/num_threads)*100;
    cout<<"Eficiencia: "<<effiency<<"%"<<endl;
    return 0;
}