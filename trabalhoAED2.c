#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TAMANHO_FILA 50
#define MAX_MEDICOS 30
#define MAX_SALAS 50
#define MAX_PACIENTE 50
#include <ctype.h>
#define MAX_CONSULTAS 50

// estruturas
typedef struct
{
    int id;
    char nome[50];
    char especialidade[50];
    int prioridade; // de 1 a 5
    int idade;
    float peso;
    float altura;
    char telefone[20];
    char sintomas[500];
    char medicacao[500];
    int retorno; // 0 = primeira consulta, 1 = retorno
} Paciente;

typedef struct
{
    int id;
    char nome[50];
    char especialidade[50]; // Refer ncia   especialidade
    int ocup;
    int horas_trabalhadas;
} Medico;

typedef struct
{
    int idSala;
    Paciente *paciente;
    Medico *medico;
    int ocup;
    int horas_ocupadas; // Horas ocupadas na sala
} Sala;

typedef struct
{
    int dia;
    int pacienteId;
    int medicoId;
    char medicoEsp[50];
    char especialidadePaciente[50];
    char especialidadeMedico[50];
    int salaId;
    int horario;
    int retorno;
} Consulta;

typedef struct
{
    Paciente fila[TAMANHO_FILA];
    int inicio;  //  ndice do primeiro elemento
    int fim;     //  ndice do pr ximo elemento dispon vel
    int tamanho; // N mero atual de elementos na fila
} Fila;

void bubble_sort(Paciente *h, int tam);
void trocar(Paciente *a, Paciente *b);
void print_heap(Paciente *h, int tam);

void inicializarFila(Fila *fila)
{
    fila->inicio = 0;
    fila->fim = 0;
    fila->tamanho = 0;
}

int filaVazia(Fila *fila)
{
    return fila->tamanho == 0;
}

int filaCheia(Fila *fila)
{
    return fila->tamanho == TAMANHO_FILA;
}

void adicionarPaciente(Fila *fila, Paciente paciente)
{
    if (filaCheia(fila))
    {
        printf("A fila est  cheia! N o   poss vel adicionar mais pacientes.\n");
        return;
    }
    fila->fila[fila->fim] = paciente;           // Adiciona o paciente na posi  o 'fim'
    fila->fim = (fila->fim + 1) % TAMANHO_FILA; // Move o  ndice de forma circular
    fila->tamanho++;                            // Incrementa o tamanho da fila
}

int removerPaciente(Fila *fila)
{
    if (filaVazia(fila))
    {
        printf("Erro: a fila est  vazia.\n");
        return 0;
    }

    fila->inicio = (fila->inicio + 1) % TAMANHO_FILA; // Move o  ndice de forma circular
    fila->tamanho--;                                  // Decrementa o tamanho da fila
    return 1;
}

void exibirFila(Fila *fila)
{
    if (filaVazia(fila))
    {
        printf("A fila est  vazia.\n");
        return;
    }
    printf("Fila de pacientes:\n");
    int i = fila->inicio;
    for (int count = 0; count < fila->tamanho; count++)
    {
        printf("ID: %d, Nome: %s, Prioridade: %d\n",
               fila->fila[i].id,
               fila->fila[i].nome,
               fila->fila[i].prioridade);
        i = (i + 1) % TAMANHO_FILA; // Move para o pr ximo  ndice de forma circular
    }
}

void trocar(Paciente *a, Paciente *b)
{
    Paciente temp = *a;
    *a = *b;
    *b = temp;
}

// Fun  o para ajustar a heap subindo o elemento
void subir_heap(Paciente *h, int i)
{
    if (i <= 0)
        return; // J    a raiz

    int pai = (i - 1) / 2;

    if ((h + pai)->prioridade < (h + i)->prioridade)
    {
        trocar(&h[pai], &h[i]);
        subir_heap(h, pai); // Chama recursivamente para o pai
    }
}

// Função para inserir um paciente na MaxHeap
void insertHeap(Paciente *heap, int *tam, int id, int prioridade) {
    if (*tam == TAMANHO_FILA) {
        printf("Erro: Heap cheia!\n");
        return;
    }

    heap[*tam].id = id;
    heap[*tam].prioridade = prioridade;
    (*tam)++;

    subir_heap(heap, *tam - 1);
}

// Fun  o para ajustar a heap descendo o elemento
void descer_heap(int n, int i, Paciente *h)
{
    int maior = i;             // Inicializa como raiz
    int filho_esq = 2 * i + 1; //  ndice do filho   esquerda
    int filho_dir = 2 * i + 2; //  ndice do filho   direita

    // Verifica se o filho   esquerda existe e   maior que a raiz
    if (filho_esq < n && (h + filho_esq)->prioridade > (h + maior)->prioridade)
    {
        maior = filho_esq;
    }

    // Verifica se o filho   direita existe e   maior que o maior atual
    if (filho_dir < n && (h + filho_dir)->prioridade > (h + maior)->prioridade)
    {
        maior = filho_dir;
    }

    // Se o maior n o for a raiz, troca e continua descendo
    if (maior != i)
    {
        trocar(&h[i], &h[maior]);
        descer_heap(n, maior, h); // Chama recursivamente
    }
}

void remocao_heap(Paciente *h, int *tam)
{
    if (*tam <= 0)
    {
        printf("Heap vazia, nada para remover.\n");
        return;
    }

    // Substitui a raiz pelo  ltimo elemento
    trocar(&h[0], &h[*tam - 1]);

    // Reduz o tamanho da heap
    (*tam)--;

    // Ajusta a heap para manter as propriedades
    descer_heap(*tam, 0, h);
}

void construir_heap(Paciente *h, int tam)
{
    for (int i = 1; i < tam; i++)
    {
        subir_heap(h, i);
    }
}

void print_heap(Paciente *h, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        printf("Paciente %d: Prioridade = %d\n", h[i].id, h[i].prioridade);
    }
}

void calcularPrioridade(Paciente *paciente)
{
    paciente->prioridade = rand() % 20; // Gera n mero entre 0 e 5
}

int lerPacientes(FILE *arquivo, Paciente *pacientes, int totalPacientes)
{
    // Inicializar todos os pacientes para evitar lixo de mem ria
    for (int i = 0; i < totalPacientes; i++)
    {
        pacientes[i].id = 0;
        strcpy(pacientes[i].nome, "");
        strcpy(pacientes[i].especialidade, "");
        pacientes[i].peso = 0.0;
        pacientes[i].altura = 0.0;
        strcpy(pacientes[i].medicacao, "");
        strcpy(pacientes[i].telefone, "");
        strcpy(pacientes[i].sintomas, "");
        pacientes[i].prioridade = 0; // Inicializa a prioridade como 0
    }

    rewind(arquivo); // Voltar ao in cio do arquivo
    char linha[1000];
    int i = 0; // Contador de pacientes lidos
    int pacientesSection = 0;

    while (fgets(linha, sizeof(linha), arquivo))
    {
        // Detectar a se  o de pacientes
        if (strncmp(linha, "[Pacientes]", 11) == 0)
        {
            pacientesSection = 1; // Entrou na se  o de pacientes
            continue;
        }

        // Interromper ao encontrar uma nova se  o
        if (pacientesSection && linha[0] == '[')
        {
            break;
        }

        // Ignorar linhas vazias ou espa os em branco
        if (pacientesSection && linha[0] != '\n' && !isspace(linha[0]) && i < totalPacientes)
        {
            // Ler os dados do paciente
            if (sscanf(linha, "%d %49s %49s %f %f %49s %49s %[^\n]",
                       &pacientes[i].id,           // ID do paciente
                       pacientes[i].nome,          // Nome do paciente
                       pacientes[i].especialidade, // Especialidade m dica
                       &pacientes[i].peso,         // Peso
                       &pacientes[i].altura,       // Altura
                       pacientes[i].medicacao,     // Medicamento
                       pacientes[i].telefone,      // Contato
                       pacientes[i].sintomas) == 8)
            {                                      // Sintomas ( ltimo campo)
                calcularPrioridade(&pacientes[i]); // Calcula a prioridade
                i++;                               // Incrementa o contador de pacientes lidos
            }

            // Interrompe se atingir o limite de pacientes
            if (i >= totalPacientes)
            {
                break;
            }
        }
    }

    // Imprime os pacientes lidos para depura  o

    return i; // Retorna o n mero de pacientes lidos
}

int lerMedicos(FILE *arquivo, Medico *medicos, int totalMedicos)
{
    // Inicializar todos os m dicos
    for (int i = 0; i < totalMedicos; i++)
    {
        medicos[i].id = 0;
        strcpy(medicos[i].nome, "");
        strcpy(medicos[i].especialidade, "");
        medicos[i].horas_trabalhadas = 0;
    }

    rewind(arquivo); // Voltar ao in cio do arquivo
    char linha[1000];
    int i = 0; // Contador de m dicos lidos
    int medicosSection = 0;

    while (fgets(linha, sizeof(linha), arquivo))
    {
        // Detectar a se  o de m dicos
        if (strncmp(linha, "[Medicos]", 9) == 0)
        {
            medicosSection = 1; // Entrou na se  o de m dicos
            continue;
        }

        // Interromper ao encontrar uma nova se  o
        if (medicosSection && linha[0] == '[')
        {
            break;
        }

        // Ignorar linhas vazias ou espa os em branco
        if (medicosSection && linha[0] != '\n' && !isspace(linha[0]) && i < totalMedicos)
        {
            // L  os dados do m dico
            if (sscanf(linha, "%d %49s %49s",
                       &medicos[i].id,
                       medicos[i].nome,
                       medicos[i].especialidade) == 3)
            {
                medicos[i].horas_trabalhadas = 0; // Inicializa horas trabalhadas
                i++;                              // Incrementa o contador de m dicos lidos
            }

            // Para se atingir o limite de m dicos
            if (i >= totalMedicos)
            {
                break;
            }
        }
    }

    return i; // Retorna o n mero de m dicos lidos
}

#include <ctype.h> // Para usar isspace

int lerSalas(FILE *arquivo, Sala *salas, int totalSalas)
{
    for (int i = 0; i < totalSalas; i++)
    {
        salas[i].idSala = 0;
        salas[i].horas_ocupadas = 0;
        salas[i].medico = NULL;
        salas[i].paciente = NULL;
        salas[i].ocup = 0;
    }

    rewind(arquivo); // Voltar ao in cio do arquivo
    char linha[1000];
    int i = 0;
    int salasSection = 0;

    while (fgets(linha, sizeof(linha), arquivo))
    {
        // Encontrar a se  o de salas
        if (strncmp(linha, "[Salas]", 7) == 0)
        {
            salasSection = 1; // Encontrou a se  o de salas
            continue;
        }

        // Se estamos na se  o de salas e n o atingimos o total de salas
        if (salasSection && i < totalSalas)
        {
            // Ignorar linhas em branco ou com apenas espa os
            if (isspace(linha[0]))
            {
                continue;
            }

            // L  o ID da sala da linha
            int idSala;
            if (sscanf(linha, "%d", &idSala) == 1)
            {                                // Certifica-se de que um n mero v lido foi lido
                salas[i].idSala = idSala;    // Atribui o ID lido   sala
                salas[i].paciente = NULL;    // Inicializa o ponteiro do paciente como NULL
                salas[i].medico = NULL;      // Inicializa o ponteiro do m dico como NULL
                salas[i].horas_ocupadas = 8; // Inicializa as horas ocupadas como 8
                i++;                         // Avan a para a pr xima sala
            }

            // Se atingiu o limite de salas, interrompe
            if (i >= totalSalas)
            {
                break;
            }
        }
    }

    // Imprime as salas lidas
    printf("\nSalas:\n");
    for (int j = 0; j < i; j++)
    {
        printf("ID Sala: %d, Horas Ocupadas: %d\n", salas[j].idSala, salas[j].horas_ocupadas);
    }
    return i;
}

void verificarFalta(Paciente *paciente, Paciente *h, int *tam) {
    // Gera um n�mero aleat�rio entre 1 e 100

    if (paciente == NULL || h == NULL || *tam <= 0) {
        printf("Erro: Paciente ou lista de pacientes inv�lida.\n");
        return;
    }

    int randNum = rand() % 100 + 1;

    if (randNum <= 5) {  // 5% de chance de faltar
        printf("Paciente %d faltou!\n", paciente->id);

        // Reduz a prioridade do paciente
        paciente->prioridade -= 1; // Exemplo de redu��o de 5 na prioridade

        // Reorganiza a heap para refletir a mudan�a de prioridade
        bubble_sort(h, *tam);
    } else {
        printf("Paciente %d compareceu!\n", paciente->id);
    }
}



void bubble_sort(Paciente *h, int tam)
{
    int trocou;

    for (int i = 0; i < tam - 1; i++)
    {
        trocou = 0;

        for (int j = 0; j < tam - 1 - i; j++)
        {
            if (h[j].prioridade < h[j + 1].prioridade)
            {
                trocar(&h[j], &h[j + 1]);
                trocou = 1;
            }
        }

        if (!trocou)
        {
            break;
        }
    }
}

void remover_paciente(Paciente *pacientes, int *num_pacientes, int index) {
    if (index < 0 || index >= *num_pacientes) {
        printf("�ndice inv�lido para remo��o.\n");
        return;
    }

    printf("Removendo paciente: %s com prioridade %d\n",
           pacientes[index].nome, pacientes[index].prioridade);

    // Desloca os pacientes para preencher a lacuna
    for (int i = index; i < (*num_pacientes) - 1; i++) {
        pacientes[i] = pacientes[i + 1];
    }

    // Decrementa o n�mero total de pacientes
    (*num_pacientes)--;

    // Ordena os pacientes restantes por prioridade
    bubble_sort(pacientes, *num_pacientes);
}

void gerar_consulta(Paciente *pacientes, Medico *medicos, Sala *salas, int num_pacientes, int num_salas,
                    int num_medicos, Consulta *consultas, int *qtd_consultas, Fila *filaretorno) {
    int horario_medicos[MAX_MEDICOS][17][7] = {0}; // Disponibilidade dos médicos
    int horario_salas[MAX_SALAS][17][7] = {0};    // Disponibilidade das salas
    const char *dias_semana[] = {"Segunda-feira", "Terça-feira", "Quarta-feira",
                                 "Quinta-feira", "Sexta-feira", "Sábado", "Domingo"};

    int consultas_dia = 0; // Contador de consultas realizadas no dia

    Paciente aux;
    construir_heap(pacientes, num_pacientes);
    print_heap(pacientes, num_pacientes);

    for (int dia = 0; dia < 7; dia++)
    {
        printf("\n=== Consultas do dia: %s ===\n", dias_semana[dia]);
        int hora = 8;

        int erro = 0;
        int sala = 0;
        while (hora <= 16 && pacientes[1].prioridade != 0)
        {
            erro = 0;
            
                for (int idM = 0; idM < num_medicos; idM++) {

                if (!medicos[idM].ocup && strcmp(medicos[idM].especialidade, pacientes[0].especialidade) == 0)
                {
                    // Cria a consulta
                    Consulta newConsulta = {
                        .pacienteId = pacientes[0].id,
                        .medicoId = medicos[idM].id,
                        .medicoEsp = "",
                        .salaId = salas[sala].idSala,
                        .horario = hora,
                        .retorno = 30};

                    strncpy(newConsulta.medicoEsp, medicos[idM].especialidade, sizeof(newConsulta.medicoEsp) - 1);

                    // Atualiza consultas
                    consultas[*qtd_consultas] = newConsulta;
                    (*qtd_consultas)++;
                    medicos[idM].horas_trabalhadas++;

                    medicos[idM].ocup = 1;

                    printf("Consulta agendada: Paciente ID: %d, SALA ID: %d, MEDICO ID: %d, "
                        "HORARIO: %02d:00, ESPECIALIDADE: %s\n",
                        newConsulta.pacienteId, newConsulta.salaId, newConsulta.medicoId,
                        newConsulta.horario, medicos[idM].especialidade);

                    sala++;

                    if (sala >= 9) {
                        sala = 0;
                        hora++;
                        for (int idM = 0; idM < num_medicos; idM++) medicos[idM].ocup = 0;
                    }

                    remocao_heap(pacientes, &num_pacientes);
                    break;
                }
                if (num_medicos - 1 == idM && num_pacientes > 0 && pacientes[1].prioridade != 0)
                {
                    aux = pacientes[0];
                    aux.prioridade = pacientes[1].prioridade - 1;
                    insertHeap(pacientes, &num_pacientes, aux.id, aux.prioridade);
                    remocao_heap(pacientes, &num_pacientes);
                }
            }
        }  
    }

    printf("\nRetornos:\n");
    int mud = 0;

    printf("\n=== Consultas do dia: %s ===\n", dias_semana[(consultas[0].retorno + consultas[0].dia) % 7]);
    for (int i = 0; i < *qtd_consultas; i++)
    {
        printf("Consulta agendada: Paciente ID: %d, SALA ID: %d, MEDICO ID: %d, "
                        "HORARIO: %02d:00, ESPECIALIDADE: %s\n",
                        consultas[i].pacienteId, consultas[i].salaId, consultas[i].medicoId,
                        consultas[i].horario, consultas[i].medicoEsp);
    }
    
}


// Main
int main()
{
    srand(time(NULL));

    // Criar ou abrir o arquivo para salvar a saída
    FILE *resultado = fopen("resultado.txt", "w");
    if (!resultado) return perror("Erro ao criar arquivo"), 1;

    // Redirecionar stdout para o arquivo
    freopen("resultado.txt", "w", stdout);

    Medico medicos[MAX_MEDICOS];
    Sala salas[MAX_SALAS];
    Paciente pacientes[MAX_PACIENTE];
    Consulta consulta[MAX_PACIENTE] = {0};
    int numPacientes = 0, numMedicos = 0, numSalas = 0;

    // Abrir o arquivo
    FILE *arquivo = fopen("entrada.txt", "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Ler os pacientes do arquivo
    numPacientes = lerPacientes(arquivo, pacientes, MAX_PACIENTE);

    // Imprimir pacientes
    printf("Pacientes:\n");
    for (int i = 0; i < numPacientes; i++)
    {
        if (pacientes[i].id == 0)
        {
            break; // Interrompe se encontrar um ID vazio
        }
        printf("ID: %d, Nome: %s, Especialidade: %s, Peso: %.2f, Altura: %.2f, Medica  o: %s, Telefone: %s, Sintomas: %s, Prioridade:%d\n",
               pacientes[i].id, pacientes[i].nome, pacientes[i].especialidade,
               pacientes[i].peso, pacientes[i].altura, pacientes[i].medicacao,
               pacientes[i].telefone, pacientes[i].sintomas, pacientes[i].prioridade);
    }

    // Ler os m dicos do arquivo
    numMedicos = lerMedicos(arquivo, medicos, MAX_MEDICOS);

    // Imprimir m dicos
    printf("\nM dicos:\n");
    for (int i = 0; i < numMedicos; i++)
    {
        if (medicos[i].id == 0)
        {
            break; // Interrompe se encontrar um ID vazio
        }
        printf("ID: %d, Nome: %s, Especialidade (ID): %s, Horas Trabalhadas: %d\n",
               medicos[i].id, medicos[i].nome, medicos[i].especialidade,
               medicos[i].horas_trabalhadas);

        medicos[i].ocup = 0;
    }

    // Ler as salas do arquivo
    numSalas = lerSalas(arquivo, salas, MAX_SALAS);

    // Imprimir salas
    printf("\nSalas:\n");
    Fila filaPacientes;
    inicializarFila(&filaPacientes);

    adicionarPaciente(&filaPacientes, pacientes[0]);

    exibirFila(&filaPacientes);

    int qtd_consultas = 0;

    bubble_sort(pacientes, numPacientes);
    gerar_consulta(pacientes, medicos, salas, numPacientes, numSalas, numMedicos, consulta, &qtd_consultas, &filaPacientes);

    fclose(arquivo);

    return 0;
}