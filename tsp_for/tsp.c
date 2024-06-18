#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

//#define DEBUG

//função para resolver o problema do Caixeiro Viajante usando backtracking
int tsp(int matrix[51][51], int cities, int distance, int initial, int current,
        int ok[51], int total, int shortest) {

    int i, d;

    //se todas as cidades foram visitadas, retorna a distância total mais a distância de volta ao início
    if (total == cities)
        return distance + matrix[current][initial];

    //se a distância acumulada já é maior que a menor distância encontrada, retorna a menor distância
    if (shortest < distance)
        return shortest;

    //itera sobre todas as cidades para encontrar o próximo passo
    for (i = 1; i <= cities; i++)
        if (i != current && !ok[i]) { //verifica se a cidade não é a atual e se não foi visitada
            ok[i] = 1; //marca a cidade como visitada
            //chama recursivamente a função para a próxima cidade
            d = tsp(matrix, cities, distance + matrix[current][i], initial, i,
                    ok, total + 1, shortest);
            {
                //atualiza a menor distância se a distância encontrada for menor
                if (d < shortest){
                    shortest = d;
                }
            }
            ok[i] = 0; //marca a cidade como não visitada para outras permutações
        }
    return shortest; //retorna a menor distância encontrada
}

int main(int argc, char *argv[]) {

    int c, e; //número de cidades e número de arestas
    int i, j, w, v; //variáveis auxiliares
    int matrix[51][51], ok[51], D; //matriz de adjacência, vetor de cidades visitadas e menor distância
    int k; //variável auxiliar

    //inicializa a matriz de adjacência e o vetor de cidades visitadas com zeros
    memset(matrix, 0, sizeof(matrix));
    memset(ok, 0, sizeof(ok));

    FILE *file;
    //abre o arquivo de entrada "tsp.in"
    if ((file = fopen("tsp.in", "r")) == NULL) {
        perror("tsp.in");
        exit(1);
    }

    //le o número de cidades e arestas do arquivo
    if(fscanf(file, "%d%d", &c, &e)!= 2){
        printf("Error reading values from the file.\n");
        fclose(file); // Fecha o arquivo antes de sair
        return 1; // Sai do programa com um código de erro
    };

    //se a flag DEBUG estiver definida, imprime informações de debug
#ifdef DEBUG
    printf("size: %d\nedges: %d\n", c, e);
    fflush(stdout);
#endif

    //le as arestas do arquivo e preenche a matriz de adjacência
    for (k = 0; k < e; k++) {
        if(fscanf(file, "%d%d%d", &i, &j, &w) != 3){
            printf("Error on reading values from file");
        }
        matrix[i][j] = w;
        matrix[j][i] = w;
    }

    D = 1000000; //inicializa a menor distância com um valor alto
    #pragma omp parallel for private(ok) shared(D)
    //itera sobre cada cidade como ponto inicial do TSP
    for (v = 1; v <= c; v++) {

#ifdef DEBUG
        printf("running: %d\n", v);
        fflush(stdout);
#endif
        memset(ok, 0, sizeof(ok)); //reinicializa o vetor de cidades visitadas
        ok[v] = 1; //marca a cidade inicial como visitada
        int d = tsp(matrix, c, 0, v, v, ok, 1, D); //chama a função TSP
        #pragma omp critical
        {
            if (d < D){ //atualiza a menor distância se a distância encontrada for menor
                D = d;
            }
        }
    }
    fclose(file); //fecha o arquivo de entrada

    //abre o arquivo de saída "tsp.out" para escrita
    if ((file = fopen("tsp.out", "w")) == NULL) {
        perror("tsp.out");
        exit(1);
    }
    //escreve a menor distância encontrada no arquivo de saída
    fprintf(file, "%d\n", D);
    fclose(file); //fecha o arquivo de saída

    return EXIT_SUCCESS; //retorna sucesso
}
