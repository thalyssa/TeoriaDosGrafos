#include <iostream>
#include <queue>
#include <algorithm>

#define MSG "nome_programa -f <arquivo> -i <vertice-inicial>\n\
-f <arquivo>  Arquivo de entrada.\n\n"

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
    Vertice() { peso = 1; visitado = false; }
    Vertice(int peso_) { peso = peso_; visitado = false; }
    int peso;
    std::vector<Aresta*> arestas;
    bool visitado;
};

typedef std::vector<Vertice> Grafo;

void ordenarArestas(Aresta * arestas[], int num_arestas) {
    for (int j = num_arestas; j > 0; j--) {
        for (int i = 0; i < j-1; i++) {
            if (arestas[i + 1]->peso < arestas[i]->peso) {
                Aresta * tmp = arestas[i + 1];
                arestas[i + 1] = arestas[i];
                arestas[i] = tmp;
            }
        }
    }
}

bool existeCicloVertice(int ** grafo, int vertice, bool incluso[], bool visitado[], int num_vertices, int pai) {
    visitado[vertice] = true;
    for (int a=0; a < num_vertices; a++) {
        if (!incluso[a] || grafo[vertice][a] < 0) continue;
        if (!visitado[a]) {
            if (existeCicloVertice(grafo, a, incluso, visitado, num_vertices, vertice))
                return true;
        }
        else if (a != pai) return true;
    }
    return false;
}

bool existeCicloAresta(int ** grafo, Aresta ** mst_arestas, int num_arestas, int num_vertices) {
    bool incluso_vertice[num_vertices];
    for (int i=0; i < num_vertices; i++) incluso_vertice[i] = false;

    bool * visitado_vertice = new bool[num_vertices];
    for (int i=0; i < num_vertices; i++) visitado_vertice[i] = false;

    for (int i=0; i < num_arestas; i++) {
        incluso_vertice[mst_arestas[i]->origem] = true;
        incluso_vertice[mst_arestas[i]->destino] = true; }

    for (int i=0; i < num_vertices; i++) {
        if (incluso_vertice[i] && !visitado_vertice[i]) {
            if (existeCicloVertice(grafo, i, incluso_vertice, visitado_vertice, num_vertices, -1))
                return true;
        }
    }

    return false;
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << MSG;
        return 1;
    }

    int num_vertices = 0, num_arestas = 0;
    FILE * arquivo_entrada;

    for (int i=1; i < argc; i++) {
        std::string argumento (argv[i]);
        if (argumento.compare("-f") == 0) {
            arquivo_entrada = fopen(argv[i + 1], "r");
            fscanf(arquivo_entrada, "%d %d\n", &num_vertices, &num_arestas);
        }
    }

    Aresta * arestas[num_arestas];
    Grafo grafo;

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
    else {
        std::cout << "Grafo inválido.\n";
        return 1;
    }

    int ** matriz_arestas;
    matriz_arestas = new int * [num_vertices];
    for (int b=0; b < num_vertices; b++)
        matriz_arestas[b] = new int [num_vertices];
    for (int a=0; a < num_vertices; a++)
        for (int b=0; b < num_vertices; b++)
            matriz_arestas[a][b] = -1;

    ordenarArestas(arestas, num_arestas);

    Aresta * mst_arestas[num_vertices - 1];
    for (int i=0; i < num_vertices - 1; i++) mst_arestas[i] = nullptr;

    int j = 0;
    int sum = 0;
    for (int i=0; i < num_arestas; i++) {

        mst_arestas[j] = arestas[i];
        matriz_arestas[arestas[i]->origem][arestas[i]->destino] = arestas[i]->peso;
        matriz_arestas[arestas[i]->destino][arestas[i]->origem] = arestas[i]->peso;

        if (existeCicloAresta(matriz_arestas, mst_arestas, j, num_vertices)) {
            mst_arestas[j] = nullptr;
            matriz_arestas[arestas[i]->origem][arestas[i]->destino] = -1;
            matriz_arestas[arestas[i]->destino][arestas[i]->origem] = -1;
        }

        else {
            std::cout << '(' << mst_arestas[j]->origem << ',' << mst_arestas[j]->destino << ')' << " ";
            sum += mst_arestas[j]->peso;
            j += 1;
        }
    }

    std::cout << '\n' << sum << '\n';

    return 0;
}
