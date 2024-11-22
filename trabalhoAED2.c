#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    int id;
    char nome[50];
    char especialidade[50]; // qual a especialidade que o paciente vai precisar
    int prioridade; // de 1 a 5
    int idade;
    float peso;
    float altura;
    char telefone[20];
    char sintomas[500];
    char medicacao[500];
} Paciente;

typedef struct 
{
    int id; // para poder identificar melhor cada médico
    char nome[50];
    char especialidade[30];
    int horas_trabalhadas;
} Medico;

typedef struct 
{
    char nome[50];
    int qatMed;
    Medico *medicos;
} Especialidade;

typedef struct 
{
    char horario[20];
    Especialidade *especialidades;
    Medico *medico;
    Paciente *paciente;
    int qatSala;
} Sala;

#define TAMANHO_FILA 10


void calcularPrioridade(Paciente *paciente) {
    paciente->prioridade = rand() % 5 + 1;
}

// fila de pacientes
void criarFila(int *inicio, int *fim) {
    *inicio = 0;
    *fim = 0;
}

int filaVazia(int inicio, int fim) {
    return inicio == fim;
}

int filaCheia(int inicio, int fim) {
    return (fim + 1) % TAMANHO_FILA == inicio;
}

void addFila(Paciente fila[], int *inicio, int *fim, Paciente paciente) {
    if (filaCheia(*inicio, *fim)) {
        printf("A fila está cheia!\n");
        return;
    }
    fila[*fim] = paciente;
    *fim = (*fim + 1) % TAMANHO_FILA;
}

Paciente removeFila(Paciente fila[], int *inicio, int *fim) {
    if (filaVazia(*inicio, *fim)) {
        printf("Erro: a fila está vazia.\n");
        Paciente vazio = {0}; // Retorna um paciente vazio em caso de erro
        return vazio;
    }
    Paciente paciente = fila[*inicio];
    *inicio = (*inicio + 1) % TAMANHO_FILA;
    return paciente;
}


void ordenarFilaPorPrioridade(Paciente fila[], int inicio, int fim) {
    int tamanho = (fim - inicio + TAMANHO_FILA) % TAMANHO_FILA; 
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            if (fila[j].prioridade < fila[j + 1].prioridade) {
                Paciente temp = fila[j];
                fila[j] = fila[j + 1];
                fila[j + 1] = temp;
            }
        }
    }
}

int main() {

    Paciente fila[TAMANHO_FILA];
    int inicio = 0, fim = 0;

    Paciente paciente1 = {1, "João", "Cardiologia", 0, 45, 80.5, 1.75, "123456789", "Dor no peito", "Aspirina"};
    Paciente paciente2 = {2, "Maria", "Neurologia", 0, 30, 70.2, 1.68, "987654321", "Dores de cabeça", "Paracetamol"};
    
    calcularPrioridade(&paciente1); 
    calcularPrioridade(&paciente2); 

    addFila(fila, &inicio, &fim, paciente1);
    addFila(fila, &inicio, &fim, paciente2);


    ordenarFilaPorPrioridade(fila, inicio, fim);

    for (int i = inicio; i != fim; i = (i + 1) % TAMANHO_FILA) {
        printf("Paciente ID: %d, Prioridade: %d, Nome: %s\n", fila[i].id, fila[i].prioridade, fila[i].nome);
    }

    return 0;
}




