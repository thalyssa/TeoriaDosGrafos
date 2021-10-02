#include <iostream>
#include <limits.h>
#include <queue>
#include <string.h>

#define MSG "nome_programa -f <arquivo> -i <vertice-inicial>\n\
-f <arquivo>  Arquivo de entrada.\n\
-i            Vértice inicial\n\
-l            Vértice final\n\n"

class Aresta {
public:
    Aresta(int origem_, int destino_, int peso_) { origem = origem_; destino = destino_; peso = peso_; }
    Aresta(const Aresta &a) { origem = a.origem; destino = a.destino; peso = a.peso; }
    int origem;
    int destino;
    int peso;
};

class Vertice {
public:
    Vertice() {peso = 1; visitado = false; }
    Vertice(int peso_) { peso = peso_; visitado = false; }
    int peso;
    std::vector<Aresta*> arestas;
    bool visitado;
};

typedef std::vector<Vertice> Grafo;

bool bfs(int ** grafo, int inicio, int destino, int pai[], int grafo_tamanho) {
    bool visitado[grafo_tamanho];
    memset(visitado, 0, sizeof(visitado));
    std::queue<int> q;
    q.push(inicio);
    visitado[inicio] = true;
    pai[inicio] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < grafo_tamanho; v++) {
            if (visitado[v] == false && grafo[u][v] > 0) {
                if (v == destino) {
                    pai[v] = u;
                    return true;
                }
                q.push(v);
                pai[v] = u;
                visitado[v] = true;
            }
        }
    }

    return false;
}

int fordFk(int ** grafo, int inicio, int saida, int matrix_tamanho) {
    int u, v;
    int ** r_grafo;

    r_grafo = new int * [matrix_tamanho];
    for (int b=0; b < matrix_tamanho; b++)
        r_grafo[b] = new int [matrix_tamanho];
    for (u = 0; u < matrix_tamanho; u++)
        for (v = 0; v < matrix_tamanho; v++)
            r_grafo[u][v] = grafo[u][v];

        int pai[matrix_tamanho];
        int max_flow = 0;

        while (bfs(r_grafo, inicio, saida, pai, matrix_tamanho)) {
            int path_flow = INT_MAX;
            for (v = saida; v != inicio; v = pai[v]) {
                u = pai[v];
                path_flow = std::min(path_flow, r_grafo[u][v]);
            }

            for (v = saida; v != inicio; v = pai[v]) {
                u = pai[v];
                r_grafo[u][v] -= path_flow;
                r_grafo[v][u] += path_flow;
            }

            max_flow += path_flow;
        }

        return max_flow;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << MSG;
        return 1;
    }

    int num_vertices = 0, num_arestas = 0;
    FILE * arquivo_entrada;
    int inicio = -1, fim = -1;

    for (int i=1; i < argc; i++) {
        std::string argumento (argv[i]);

        if (argumento.compare("-f") == 0) {
            arquivo_entrada = fopen(argv[i + 1], "r");
            fscanf(arquivo_entrada, "%d %d\n", &num_vertices, &num_arestas);
        }
        if (argumento.compare("-i") == 0)
            inicio = atoi(argv[i+1]);
        if (argumento.compare("-l") == 0)
            fim = atoi(argv[i+1]);
    }

    if (inicio == -1) {
        std::cout << MSG;
        return 1;
    }

    if (fim == -1) {
        std::cout << MSG;
        return 1;
    }

    Grafo grafo;
    Aresta * arestas[num_arestas];
    if (num_vertices != 0 && num_arestas != 0) {

        for (int j=0; j < num_vertices; j++) {
            Vertice v(j); // j é o índice, e o índice +1 é o número do vértice no arquivo de entrada.
            grafo.push_back(v);
        }

        for (int k=0; k < num_arestas; k++) {
            int origem, destino, peso, n;
            char linha[128];
            fgets(linha, 128, arquivo_entrada);
            n = sscanf(linha, "%d %d %d", &origem, &destino, &peso);
            if (n < 3) peso = 1;
            Aresta * pAresta = new Aresta(origem, destino, peso);
            arestas[k] = pAresta;
            grafo.at(pAresta->origem).arestas.push_back(pAresta);
        }
    }
    else std::cout << "Grafo lido inválido.";

    int ** matriz_arestas;
    matriz_arestas = new int * [num_vertices];
    for (int b=0; b < num_vertices; b++)
        matriz_arestas[b] = new int [num_vertices];
    for (int l=0; l < num_arestas; l++)
        matriz_arestas[arestas[l]->origem][arestas[l]->destino] = arestas[l]->peso;

    std::cout << fordFk(matriz_arestas, inicio, fim, num_vertices);

    return 0;
}
