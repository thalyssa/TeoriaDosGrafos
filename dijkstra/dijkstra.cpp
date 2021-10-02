#include <iostream>
#include <limits.h>
#include <vector>

#define MSG "nome_programa -f <arquivo> -i <vertice-inicial>\n\
-f <arquivo>  Arquivo de entrada.\n\
-i            Vértice inicial\n\n"

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

int distanciaMin(int dist[], bool sptSet[], int tamanho)
{
    int min = INT_MAX, indice_min;
    for (int v = 0; v < tamanho; v++)
        if (!sptSet[v] && dist[v] <= min)
            min = dist[v], indice_min = v;
    return indice_min;
}

int * dijkstra(int ** grafo, int origem, int grafo_tamanho, int dist[])
{
    bool sptSet[grafo_tamanho];
    for (int i = 0; i < grafo_tamanho; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
    dist[origem] = 0;

    for (int cont = 0; cont < grafo_tamanho - 1; cont++) {
        int u = distanciaMin(dist, sptSet, grafo_tamanho);
        sptSet[u] = true;
        for (int v = 0; v < grafo_tamanho; v++)
            if (!sptSet[v] && grafo[u][v] && dist[u] != INT_MAX
            && dist[u] + grafo[u][v] < dist[v])
                dist[v] = dist[u] + grafo[u][v];
    }
    return dist;
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << MSG;
        return 1;
    }

    int num_vertices = 0, num_arestas = 0;
    FILE * arquivo_entrada;
    int inicio = -1;

    for (int i=1; i < argc; i++) {
        std::string argumento (argv[i]);
        if (argumento.compare("-f") == 0) {
            arquivo_entrada = fopen(argv[i + 1], "r");
            fscanf(arquivo_entrada, "%d %d\n", &num_vertices, &num_arestas);
        }
        if (argumento.compare("-i") == 0)
            inicio = atoi(argv[i+1]);
    }

    if (inicio == -1) {
        std::cout << MSG;
        return 1;
    }

    Grafo grafo;
    Aresta * arestas[num_arestas];
    if (num_vertices != 0 && num_arestas != 0) {
        for (int j=0; j < num_vertices; j++) {
            Vertice v(j);
            grafo.push_back(v);
        }
        for (int k=0; k < num_arestas; k++) {
            int origem, destino, peso, n;
            char linha[128];
            fgets(linha, 128, arquivo_entrada);
            n = sscanf(linha, "%d %d %d", &origem, &destino, &peso);
            if (n < 3) peso = 1;
            Aresta * e = new Aresta(origem, destino, peso);
            arestas[k] = e;
            grafo.at(e->origem).arestas.push_back(e);
        }
    }
    else std::cout << "Grafo inválido.\n";

    int ** matriz_arestas;
    matriz_arestas = new int * [num_vertices];
    for (int b=0; b < num_vertices; b++)
        matriz_arestas[b] = new int [num_vertices];
    for (int l=0; l < num_arestas; l++)
        matriz_arestas[arestas[l]->origem][arestas[l]->destino] = arestas[l]->peso;

    int distancias[num_vertices];
    dijkstra(matriz_arestas, inicio, num_vertices, distancias);

    for (int i=0; i < num_vertices; i++)
        std::cout << i << ":" << distancias[i] << " ";
    std::cout << "\n";

    return 0;
}
