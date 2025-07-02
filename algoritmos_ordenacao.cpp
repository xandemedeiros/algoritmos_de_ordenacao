#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <map>
#include <functional>
#include <numeric>

using namespace std;
using namespace chrono;

struct Metricas {
    long long comparacoes = 0;
    long long trocas = 0;
};


// Bubble Sort
void bubbleSort(vector<int>& vetor, Metricas& m) {
    int n = vetor.size();
    bool houve_troca;
    for (int i = 0; i < n - 1; i++) {
        houve_troca = false;
        for (int j = 0; j < n - i - 1; j++) {
            m.comparacoes++;
            if (vetor[j] > vetor[j + 1]) {
                swap(vetor[j], vetor[j + 1]);
                m.trocas++;
                houve_troca = true;
            }
        }
        if (!houve_troca) break;
    }
}

// Insertion Sort
void insertionSort(vector<int>& vetor, Metricas& m) {
    int n = vetor.size();
    for (int i = 1; i < n; i++) {
        int chave = vetor[i];
        int j = i - 1;
        while (j >= 0) {
            m.comparacoes++;
            if (vetor[j] > chave) {
                vetor[j + 1] = vetor[j];
                m.trocas++;
                j--;
            } else {
                break;
            }
        }
        vetor[j + 1] = chave;
    }
}

// Selection Sort
void selectionSort(vector<int>& vetor, Metricas& m) {
    int n = vetor.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            m.comparacoes++;
            if (vetor[j] < vetor[min_idx]) min_idx = j;
        }
        if (i != min_idx) {
            swap(vetor[i], vetor[min_idx]);
            m.trocas++;
        }
    }
}

// Merge Sort
void merge(vector<int>& vetor, int inicio, int meio, int fim, Metricas& m) {
    vector<int> esq(vetor.begin() + inicio, vetor.begin() + meio + 1);
    vector<int> dir(vetor.begin() + meio + 1, vetor.begin() + fim + 1);
    int i = 0, j = 0, k = inicio;

    while (i < esq.size() && j < dir.size()) {
        m.comparacoes++;
        if (esq[i] <= dir[j]) vetor[k++] = esq[i++];
        else vetor[k++] = dir[j++];
        m.trocas++; // Contabiliza cópia de elemento.
    }
    while (i < esq.size()) { vetor[k++] = esq[i++]; m.trocas++; }
    while (j < dir.size()) { vetor[k++] = dir[j++]; m.trocas++; }
}

void mergeSort(vector<int>& vetor, int inicio, int fim, Metricas& m) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;
        mergeSort(vetor, inicio, meio, m);
        mergeSort(vetor, meio + 1, fim, m);
        merge(vetor, inicio, meio, fim, m);
    }
}

// Quick Sort
int particiona(vector<int>& vetor, int baixo, int alto, Metricas& m) {
    int pivo = vetor[alto];
    int i = (baixo - 1);
    for (int j = baixo; j < alto; j++) {
        m.comparacoes++;
        if (vetor[j] < pivo) {
            i++;
            swap(vetor[i], vetor[j]);
            m.trocas++;
        }
    }
    swap(vetor[i + 1], vetor[alto]);
    m.trocas++;
    return (i + 1);
}

void quickSort(vector<int>& vetor, int baixo, int alto, Metricas& m) {
    if (baixo < alto) {
        int pi = particiona(vetor, baixo, alto, m);
        quickSort(vetor, baixo, pi - 1, m);
        quickSort(vetor, pi + 1, alto, m);
    }
}

vector<int> gerarDados(int tamanho, const string& tipo) {
    vector<int> dados(tamanho);
    iota(dados.begin(), dados.end(), 1);
    default_random_engine rng(42);

    if (tipo == "aleatorio") {
        shuffle(dados.begin(), dados.end(), rng);
    } else if (tipo == "inverso") {
        reverse(dados.begin(), dados.end());
    } else if (tipo == "quase_ordenado") {
        shuffle(dados.begin(), dados.begin() + tamanho / 10, rng);
    }
    return dados;
}

void compararAlgoritmos(int tamanho, const string& tipo_dado) { 
    map<string, function<void(vector<int>&, Metricas&)>> algoritmos = {
        {"BubbleSort",    [](vector<int>& v, Metricas& m){ bubbleSort(v, m); }},
        {"InsertionSort", [](vector<int>& v, Metricas& m){ insertionSort(v, m); }},
        {"SelectionSort", [](vector<int>& v, Metricas& m){ selectionSort(v, m); }},
        {"MergeSort",     [](vector<int>& v, Metricas& m){ mergeSort(v, 0, v.size() - 1, m); }},
        {"QuickSort",     [](vector<int>& v, Metricas& m){ quickSort(v, 0, v.size() - 1, m); }}
    };

    for (const auto& par : algoritmos) {
        string nome_algoritmo = par.first;
        function<void(vector<int>&, Metricas&)> func_algoritmo = par.second;

        vector<int> vetor = gerarDados(tamanho, tipo_dado);
        Metricas m;

        auto inicio = high_resolution_clock::now();
        func_algoritmo(vetor, m);
        auto fim = high_resolution_clock::now();

        auto tempo_ms = duration_cast<milliseconds>(fim - inicio).count();

        cout << nome_algoritmo << ", " << tipo_dado << ", " << tamanho << ", "
             << tempo_ms << ", " << m.comparacoes << ", " << m.trocas << endl;
    }
}

int main() {
    vector<int> tamanhos_vetor = {1000, 5000, 10000};
    vector<string> tipos_dados = {"aleatorio", "quase_ordenado", "inverso"};

    cout << "Aqui está a forma de que será demonstrado os dados:" << endl << "Algoritmo, Tipo, Tamanho, Tempo(ms), Comparacoes, Trocas" << endl;

    for (const auto& tipo : tipos_dados) {
        for (int tamanho : tamanhos_vetor) {
            cout << endl << "Executando testes para tipo: " << tipo << ", tamanho: " << tamanho << endl;
            compararAlgoritmos(tamanho, tipo);
        }
    }

    cout << endl << "Todos os testes foram concluídos" << endl;

    return 0;
}
