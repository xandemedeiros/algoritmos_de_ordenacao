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
    for (int i = 0; i < n - 1; i++) { // for que vai percorrer a lista 
        houve_troca = false;  // caso otimizado para caso a lista já esteja ordenada.
        for (int j = 0; j < n - i - 1; j++) {
            m.comparacoes++;
            if (vetor[j] > vetor[j + 1]) { // checagem de desordenamento.
                swap(vetor[j], vetor[j + 1]); // caso desordenados, realizam o swap e os elementos adjacentes são trocados.
                m.trocas++;
                houve_troca = true;
            }
        }
        if (!houve_troca) break; // break para caso não houver trocas, que vai parar o loop pois se não houver trocas a lista está ordenada. 
    }
}

// Insertion Sort
void insertionSort(vector<int>& vetor, Metricas& m) {
    int n = vetor.size();
    for (int i = 1; i < n; i++) { // for que vai percorrer a lista a partir do segundo elemento.
        int chave = vetor[i]; // elemento que será inserido na parte ordenada.
        int j = i - 1; // último elemento da parte já ordenada.
        while (j >= 0) { // loop while que vai deslocar os maiores elementos para a direita
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
    for (int i = 0; i < n - 1; i++) { // for que percorre a lista para encontrar a posição do menor elemento
        int min_idx = i; // index criado para armazenar o menor elemento, de inicio assumir que o menor elemento é o atual
        for (int j = i + 1; j < n; j++) { // for que vai comparar o elemento atual com os outros, caso encontre um menor, substitui o index por esse novo valor 
            m.comparacoes++;
            if (vetor[j] < vetor[min_idx]) min_idx = j;
        }
        if (i != min_idx) {
            swap(vetor[i], vetor[min_idx]); // realiza a troca do elemento atual com o menor encotrado
            m.trocas++;
        }
    }
}

// Merge Sort
void merge(vector<int>& vetor, int inicio, int meio, int fim, Metricas& m) {
    vector<int> esq(vetor.begin() + inicio, vetor.begin() + meio + 1); // criação da sublista da esquerda
    vector<int> dir(vetor.begin() + meio + 1, vetor.begin() + fim + 1); // criação da sublista da direita
    int i = 0, j = 0, k = inicio;

    while (i < esq.size() && j < dir.size()) { // while para intercalar elementos das duas sublistas
        m.comparacoes++;
        if (esq[i] <= dir[j]) vetor[k++] = esq[i++]; // escolhe o menor e copia para o vetor original
        else vetor[k++] = dir[j++];
        m.trocas++;
    }
    while (i < esq.size()) { vetor[k++] = esq[i++]; m.trocas++; } // copia os elementos que restaram da sublista da esquerda
    while (j < dir.size()) { vetor[k++] = dir[j++]; m.trocas++; } // copia os elementos que restaram da sublista da direita
}

void mergeSort(vector<int>& vetor, int inicio, int fim, Metricas& m) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2; // calculo de ponto médio para realizar a divisão da lista
        mergeSort(vetor, inicio, meio, m); // ordena a primeira metade
        mergeSort(vetor, meio + 1, fim, m); // ordena a segunda metade
        merge(vetor, inicio, meio, fim, m); // combina as duas metades já ordenadas
    }
}

// Quick Sort
int particiona(vector<int>& vetor, int baixo, int alto, Metricas& m) {
    int pivo = vetor[alto]; // definindo o ultimo elemento como um pivô para otimizar o quick sort
    int i = (baixo - 1); // índice para restrear a posição do último elemento menor que o pivô
    for (int j = baixo; j < alto; j++) {  // for que vai comparar os elementos com o pivô
        m.comparacoes++;
        if (vetor[j] < pivo) {
            i++;
            swap(vetor[i], vetor[j]); // troca o elemento menor para a parte esquerda
            m.trocas++;
        }
    }
    swap(vetor[i + 1], vetor[alto]); // coloca o pivô na posição final, realizando mais uma troca
    m.trocas++;
    return (i + 1);
}

void quickSort(vector<int>& vetor, int baixo, int alto, Metricas& m) {
    if (baixo < alto) {
        int pi = particiona(vetor, baixo, alto, m); // parte a lista em torno do pivô
        quickSort(vetor, baixo, pi - 1, m); // ordena a sublista da esquerda do pivô
        quickSort(vetor, pi + 1, alto, m); // ordena a sublista da direita do pivô
    }
}

vector<int> gerarDados(int tamanho, const string& tipo) {
    vector<int> dados(tamanho);
    iota(dados.begin(), dados.end(), 1); // preenchimento do vetor com valores sequênciais
    default_random_engine rng(42);  // "semente" fixa para garantir testes reproduziveis

    if (tipo == "aleatorio") {
        shuffle(dados.begin(), dados.end(), rng); // shuffle que vai embaralhar o vetor para ter dados aleatórios
    } else if (tipo == "inverso") {
        reverse(dados.begin(), dados.end()); // inverte o vetor para obter os dados inversamente ordenado
    } else if (tipo == "quase_ordenado") {
        shuffle(dados.begin(), dados.begin() + tamanho / 10, rng); // embaralha uma fração dos dados para obter os dados QUASE ordenados 
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

        auto inicio = high_resolution_clock::now(); // inicio de execução de tempo
        func_algoritmo(vetor, m); // execução do algoritmo de ordenação
        auto fim = high_resolution_clock::now(); // final de execução de tempo

        auto tempo_ms = duration_cast<milliseconds>(fim - inicio).count();

        cout << nome_algoritmo << ", " << tipo_dado << ", " << tamanho << ", "
             << tempo_ms << ", " << m.comparacoes << ", " << m.trocas << endl; // imprime os resultados formatados
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
