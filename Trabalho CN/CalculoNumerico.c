/*
 * EXERCÍCIO PROGRAMA DE CÁLCULO NUMÉRICO - ENGENHARIA DE TELECOMUNICAÇÕES
 * PROFESSOR: GLAUBER CINTRA                ENTREGA: 12/08/2024
 * INTEGRANTES (ORDEM ALFABÉTICA):          
 * Debora Sales Costa 
 * Douglas Cartaxo Farias                         
 * Júlia Lucena de Saboya 
 * Vladimir Rodaly Joseph             
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

/* Função que resolve um sistema linear usando o método de Gauss-Jordan */
void metodo_jordan(double **matriz, int n) {
    double pivo, multiplicador;
    int i, j, k;

    // Percorre cada linha da matriz
    for (i = 0; i < n; i++) {
        pivo = matriz[i][i];  // Seleciona o pivô, que é o elemento diagonal principal

        // Normaliza a linha, dividindo todos os elementos pelo pivô
        for (j = 0; j < n + 1; j++) {
            matriz[i][j] /= pivo;
        }

        // Elimina os elementos abaixo e acima do pivô na mesma coluna
        for (k = 0; k < n; k++) {
            if (k != i) {
                multiplicador = matriz[k][i];

                for (j = 0; j < n + 1; j++) {
                    matriz[k][j] -= multiplicador * matriz[i][j];
                }
            }
        }
    }
}

/* Função principal para ler o sistema linear de um arquivo e resolver usando o método de Gauss-Jordan */
int resolverSistemaLinear() {
    FILE *arquivo;
    int tamanhoMatriz, linha, coluna;
    double **matriz;

    // Abre o arquivo com o sistema linear
    arquivo = fopen("sistema.txt", "r");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Lê o tamanho da matriz
    fscanf(arquivo, "%d", &tamanhoMatriz);

    // Aloca memória para a matriz
    matriz = (double **) malloc(tamanhoMatriz * sizeof(double *));
    for (linha = 0; linha < tamanhoMatriz; linha++) {
        matriz[linha] = (double *) malloc((tamanhoMatriz + 1) * sizeof(double));
    }

    // Lê os valores da matriz a partir do arquivo
    for (linha = 0; linha < tamanhoMatriz; linha++) {
        for (coluna = 0; coluna < tamanhoMatriz + 1; coluna++) {
            fscanf(arquivo, "%lf", &matriz[linha][coluna]);
        }
    }

    fclose(arquivo);

    // Resolve o sistema linear utilizando o método de Gauss-Jordan
    metodo_jordan(matriz, tamanhoMatriz);

    // Exibe a matriz resultante após a aplicação do método de Gauss-Jordan
    printf("Matriz diagonal:\n");
    for (linha = 0; linha < tamanhoMatriz; linha++) {
        for (coluna = 0; coluna < tamanhoMatriz + 1; coluna++) {
            printf("%lf ", matriz[linha][coluna]);
        }
        printf("\n");
    }

    // Verifica se o sistema tem solução ou se é incompatível
    if (matriz[tamanhoMatriz - 1][tamanhoMatriz - 1] == 0) {
        printf("Sistema incompatível.\n");
    } else {
        printf("Solução do sistema:\n");

        for (linha = 0; linha < tamanhoMatriz; linha++) {
            printf("x%d = %lf\n", linha + 1, matriz[linha][tamanhoMatriz]);
        }
    }

    // Libera a memória alocada para a matriz
    for (linha = 0; linha < tamanhoMatriz; linha++) {
        free(matriz[linha]);
    }

    free(matriz);
    
    return 0;
}

/* Função para calcular o valor de uma equação algébrica dado um valor de x */
double calcularValorEquacao(double coeficientes[], int grau, double x) {
    double resultado = 0;

    // Calcula o valor da equação para o valor dado de x
    for (int i = 0; i <= grau; i++) {
        resultado += coeficientes[i] * pow(x, grau - i);
    }

    return resultado;
}

/* Função para verificar se um intervalo contém uma quantidade ímpar de raízes */
int verificarIntervaloRaizesImpares(double coeficientes[], int grau, double limiteInferior, double limiteSuperior) {
    return (calcularValorEquacao(coeficientes, grau, limiteInferior) * calcularValorEquacao(coeficientes, grau, limiteSuperior) < 0);
}

/* Função para calcular uma raiz aproximada usando o Método da Bisseção */
double calcularRaizPorBissecao(double coeficientes[], int grau, double limiteInferior, double limiteSuperior, double erroTolerado, int maximoIteracoes) {
    double aproximacaoRaiz;
    int iteracoes = 0;

    // Aplica o Método da Bisseção para encontrar uma raiz aproximada
    do {
        aproximacaoRaiz = (limiteInferior + limiteSuperior) / 2;
        if (calcularValorEquacao(coeficientes, grau, limiteInferior) * calcularValorEquacao(coeficientes, grau, aproximacaoRaiz) < 0) {
            limiteSuperior = aproximacaoRaiz;
        } else {
            limiteInferior = aproximacaoRaiz;
        }
        iteracoes++;
    } while (fabs(limiteSuperior - limiteInferior) > erroTolerado && iteracoes < maximoIteracoes);

    return aproximacaoRaiz;
}

/* Função principal para resolver uma equação algébrica */
int resolverEquacaoAlgebrica() {
    int grau;
    printf("Digite o grau da equacao: ");
    scanf("%d", &grau);

    double coeficientes[grau + 1];
    printf("Digite os coeficientes da equacao, em ordem decrescente de grau:\n");
    for (int i = 0; i <= grau; i++) {
        scanf("%lf", &coeficientes[i]);
    }

    // Verificar se an > 0 e a0 ≠ 0, conforme requerido para o método da bisseção
    if (coeficientes[grau] <= 0 || coeficientes[0] == 0) {
        printf("Os coeficientes an e a0 nao atendem as condicoes an > 0 e a0 ≠ 0.\n");
        return 1;
    }

    // Calcular os intervalos onde se encontram as raízes reais negativas e positivas
    double limiteInferior = -1, limiteSuperior = 1;
    while (verificarIntervaloRaizesImpares(coeficientes, grau, limiteInferior, limiteSuperior)) {
        limiteInferior *= 2;
        limiteSuperior *= 2;
    }
    printf("Intervalo das raizes reais negativas: (%.2lf, %.2lf)\n", limiteInferior, limiteSuperior);

    limiteInferior = -1, limiteSuperior = 1;
    while (verificarIntervaloRaizesImpares(coeficientes, grau, limiteInferior, limiteSuperior)) {
        limiteInferior = limiteInferior * 2 - 1;
        limiteSuperior = limiteSuperior * 2 + 1;
    }
    printf("Intervalo das raizes reais positivas: (%.2lf, %.2lf)\n", limiteInferior, limiteSuperior);

    // Solicitar ao usuário que informe um intervalo para verificar a quantidade de raízes
    double intervaloInferior, intervaloSuperior;
    printf("Digite um intervalo para verificar a quantidade de raizes: ");
    scanf("%lf %lf", &intervaloInferior, &intervaloSuperior);

    // Verificar se o intervalo contém uma quantidade ímpar de raízes e calcular uma raiz aproximada
    if (verificarIntervaloRaizesImpares(coeficientes, grau, intervaloInferior, intervaloSuperior)) {
        double raizAproximada = calcularRaizPorBissecao(coeficientes, grau, intervaloInferior, intervaloSuperior, 1e-8, 1000);
        printf("Uma raiz aproximada no intervalo (%.2lf, %.2lf) é: %.8lf\n", intervaloInferior, intervaloSuperior, raizAproximada);
    } else {
        printf("O numero de raizes no intervalo (%.2lf, %.2lf) é par.\n", intervaloInferior, intervaloSuperior);
    }

    return 0;
}

/* Método auxiliar para imprimir valores na base hexadecimal */
void imprimirValorHexadecimal(int valor) {
    switch (valor) {
    case 10: printf("A");
        break;
    case 11: printf("B");
        break;
    case 12: printf("C");
        break;
    case 13: printf("D");
        break;
    case 14: printf("E");
        break;
    case 15: printf("F");
        break;
    default: printf("%d", valor);
        break;
    }
}

/* Função para converter um número decimal para a base binária */
void converterParaBinario(int parteInteira, double parteFracionaria) {
    int resultadoInteiro[10], indiceInteiro = 0, indiceFracionario = 0;
    double auxiliarFracionaria = parteFracionaria, resultadoFracionario[20];

    // Converte a parte inteira para binário
    while (parteInteira >= 2) {
        resultadoInteiro[indiceInteiro] = parteInteira % 2;
        parteInteira = (parteInteira - parteInteira % 2) / 2;
        indiceInteiro++;
    }
    resultadoInteiro[indiceInteiro] = parteInteira;

    // Converte a parte fracionária para binário
    while (auxiliarFracionaria > 0 && indiceFracionario < 20) {
        double parteInteiraTemp;
        auxiliarFracionaria = modf(auxiliarFracionaria * 2, &parteInteiraTemp);
        resultadoFracionario[indiceFracionario] = parteInteiraTemp;
        indiceFracionario++;
    }

    // Imprime o número binário completo (parte inteira e fracionária)
    printf("Binário: ");
    for (parteInteira = indiceInteiro; parteInteira >= 0; parteInteira--)
        printf("%d", resultadoInteiro[parteInteira]);
    if (parteFracionaria > 0) {
        printf(".");
        for (parteInteira = 0; parteInteira < indiceFracionario; parteInteira++)
            printf("%d", (int)resultadoFracionario[parteInteira]);
    }
    printf("\n");
}

/* Função para converter um número decimal para a base hexadecimal */
void converterParaHexadecimal(int parteInteira, double parteFracionaria) {
    int resultadoInteiro[10], indiceInteiro = 0, indiceFracionario = 0;
    double auxiliarFracionaria = parteFracionaria, resultadoFracionario[20];

    // Converte a parte inteira para hexadecimal
    while (parteInteira >= 16) {
        resultadoInteiro[indiceInteiro] = parteInteira % 16;
        parteInteira = (parteInteira - parteInteira % 16) / 16;
        indiceInteiro++;
    }
    resultadoInteiro[indiceInteiro] = parteInteira;

    // Converte a parte fracionária para hexadecimal
    while (auxiliarFracionaria > 0 && indiceFracionario < 20) {
        double parteInteiraTemp;
        auxiliarFracionaria = modf(auxiliarFracionaria * 16, &parteInteiraTemp);
        resultadoFracionario[indiceFracionario] = parteInteiraTemp;
        indiceFracionario++;
    }

    // Imprime o número hexadecimal completo (parte inteira e fracionária)
    printf("Hexadecimal: ");
    for (parteInteira = indiceInteiro; parteInteira >= 0; parteInteira--)
        imprimirValorHexadecimal(resultadoInteiro[parteInteira]);
    if (parteFracionaria > 0) {
        printf(".");
        for (parteInteira = 0; parteInteira < indiceFracionario; parteInteira++)
            imprimirValorHexadecimal((int)resultadoFracionario[parteInteira]);
    }
    printf("\n");
}

/* Função para converter um número decimal para a base octal */
void converterParaOctal(int parteInteira, double parteFracionaria) {
    int resultadoInteiro[10], indiceInteiro = 0, indiceFracionario = 0;
    double auxiliarFracionaria = parteFracionaria, resultadoFracionario[20];

    // Converte a parte inteira para octal
    while (parteInteira >= 8) {
        resultadoInteiro[indiceInteiro] = parteInteira % 8;
        parteInteira = (parteInteira - parteInteira % 8) / 8;
        indiceInteiro++;
    }
    resultadoInteiro[indiceInteiro] = parteInteira;

    // Converte a parte fracionária para octal
    while (auxiliarFracionaria > 0 && indiceFracionario < 20) {
        double parteInteiraTemp;
        auxiliarFracionaria = modf(auxiliarFracionaria * 8, &parteInteiraTemp);
        resultadoFracionario[indiceFracionario] = parteInteiraTemp;
        indiceFracionario++;
    }

    // Imprime o número octal completo (parte inteira e fracionária)
    printf("Octal: ");
    for (parteInteira = indiceInteiro; parteInteira >= 0; parteInteira--)
        printf("%d", resultadoInteiro[parteInteira]);
    if (parteFracionaria > 0) {
        printf(".");
        for (parteInteira = 0; parteInteira < indiceFracionario; parteInteira++)
            printf("%d", (int)resultadoFracionario[parteInteira]);
    }
    printf("\n");
}

/* Função principal do programa */
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");  // Configura o locale para o sistema
    char escolhaUsuario;

    // Loop principal do programa para o menu de opções
    do {
        printf("\n====Menu Principal=====\n");
        printf("\n========================================\n");
        printf("C - Conversão: \n");
        printf("S - Sistema Linear: \n");
        printf("E - Equação Algébrica: \n");
        printf("F - Finalizar: \n");
        scanf(" %c", &escolhaUsuario);
        printf("\n========================================\n");

        switch (escolhaUsuario) {
            double numero, parteInteira, parteFracionaria;
            case 'C':
                // Conversão de bases (Binário, Octal, Hexadecimal)
                printf("Digite o número a ser convertido(use virgula):  ");
                scanf(" %lf", &numero);
                parteFracionaria = modf(numero, &parteInteira);
                converterParaBinario(parteInteira, parteFracionaria);
                converterParaOctal(parteInteira, parteFracionaria);
                converterParaHexadecimal(parteInteira, parteFracionaria);
                printf("\n");
                break;
            case 'S':
                // Resolver sistema linear
                resolverSistemaLinear();
                break;
            case 'E':
                // Resolver equação algébrica
                resolverEquacaoAlgebrica();
                break;
            case 'F':
                // Finalizar o programa
                printf("Finalizando a aplicação...\n");
                return 0;
            default:
                printf("Comando inválido!\n");
                break;
        }
    } while (1);  // O loop continua até o usuário escolher 'F' para finalizar
}