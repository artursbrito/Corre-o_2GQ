/*
 * UNICAP - Programacao Estruturada - 2o GQ (Junho/2026)
 * Solucao das 5 questoes + main de teste
 */
#include <stdio.h>
#include <string.h>
#include <math.h>

#define TAM_CONTAS 50
#define TAM_OPERACOES 10000

struct Conta {
    int id;
    char cliente[40];
    double saldo;
    double limite;
};

struct Operacao {
    int id;
    int idConta;
    char tipo;      /* 'D', 'S' ou 'T' */
    double valor;   /* positivo = credito, negativo = debito */
};

/* ---------- Auxiliares ---------- */
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Ja existe na prova; implementada aqui so para o programa compilar */
int buscaId(struct Conta v[], int tam, int x) {
    int i;
    for (i = 0; i < tam; i++) {
        if (v[i].id == x) {
            return i;
        }
    }
    return -1;
}

/* ---------- Questao 1 ---------- */
void cadastraConta(struct Conta v[], int *tam) {
    struct Conta nova;

    if (*tam >= TAM_CONTAS) {
        printf("Vetor cheio! Nao e possivel cadastrar.\n");
        return;
    }

    printf("ID da conta: ");
    scanf("%d", &nova.id);
    limparBuffer();

    if (buscaId(v, *tam, nova.id) != -1) {
        printf("ID duplicado! Cadastro cancelado.\n");
        return;
    }

    printf("Nome do cliente: ");
    fgets(nova.cliente, 40, stdin);
    nova.cliente[strcspn(nova.cliente, "\n")] = '\0';

    printf("Limite: ");
    scanf("%lf", &nova.limite);
    limparBuffer();

    if (nova.limite < 0) {
        printf("Limite negativo! Cadastro cancelado.\n");
        return;
    }

    nova.saldo = 0;           /* conta nova comeca com saldo zero */
    v[*tam] = nova;
    (*tam)++;
    printf("Conta cadastrada com sucesso!\n");
}

/* ---------- Questao 2 ---------- */
/* Pre-condicao: o vetor esta ordenado pelo nome do cliente (ordem crescente) */
int buscaBinariaNome(struct Conta v[], int tam, char x[]) {
    int inicio = 0, fim = tam - 1, meio, cmp;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        cmp = strcmp(x, v[meio].cliente);
        if (cmp == 0) {
            return meio;
        } else if (cmp < 0) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }
    return -1;
}

/* ---------- Questao 3 ---------- */
void filtraDevedores(struct Conta vContas[], int tamConta,
                     struct Conta vDevedores[], int *tamDevedores) {
    int i;

    *tamDevedores = 0;
    for (i = 0; i < tamConta; i++) {
        if (vContas[i].saldo < 0) {
            vDevedores[*tamDevedores] = vContas[i];
            (*tamDevedores)++;
        }
    }
}

/* ---------- Questao 4 ---------- */
int realizarSaque(struct Conta vContas[], int tamConta,
                  struct Operacao vOp[], int *tamOp) {
    int id, pos;
    double valor;

    printf("ID da conta: ");
    scanf("%d", &id);
    printf("Valor do saque: ");
    scanf("%lf", &valor);
    limparBuffer();

    pos = buscaId(vContas, tamConta, id);
    if (pos == -1) {
        printf("Conta nao encontrada!\n");
        return 0;
    }
    if (valor <= 0) {
        printf("Valor invalido!\n");
        return 0;
    }
    if (vContas[pos].saldo + vContas[pos].limite < valor) {
        printf("Saldo + limite insuficiente!\n");
        return 0;
    }
    if (*tamOp >= TAM_OPERACOES) {
        printf("Vetor de operacoes cheio!\n");
        return 0;
    }

    vContas[pos].saldo -= valor;

    vOp[*tamOp].id = *tamOp + 1;
    vOp[*tamOp].idConta = id;
    vOp[*tamOp].tipo = 'S';
    vOp[*tamOp].valor = -valor;
    (*tamOp)++;

    printf("Saque realizado!\n");
    return 1;
}

/* ---------- Questao 5 ---------- */
void imprimeValor(double valor) {
    printf("%9.2f%c", fabs(valor), valor >= 0 ? 'C' : 'D');
}

const char *nomeTipo(char tipo) {
    if (tipo == 'D') return "Deposito";
    if (tipo == 'S') return "Saque";
    return "Transferencia";
}

void extrato(struct Conta vContas[], int tamConta,
             struct Operacao vOp[], int *tamOp) {
    int idConta, pos, i, n = 0, inicio, k;
    int idx[TAM_OPERACOES];      /* posicoes das operacoes dessa conta */
    double saldo = 0;

    printf("ID da conta: ");
    scanf("%d", &idConta);
    limparBuffer();

    pos = buscaId(vContas, tamConta, idConta);
    if (pos == -1) {
        printf("Conta nao encontrada!\n");
        return;
    }

    /* 1) guarda so as operacoes dessa conta (mesma ideia da questao 3) */
    for (i = 0; i < *tamOp; i++) {
        if (vOp[i].idConta == idConta) {
            idx[n] = i;
            n++;
        }
    }

    /* 2) so as 5 ultimas; as anteriores apenas atualizam o saldo anterior */
    inicio = (n > 5) ? n - 5 : 0;
    for (k = 0; k < inicio; k++) {
        saldo += vOp[idx[k]].valor;
    }

    printf("\n+--------------------------------------------------+\n");
    printf("| C O N T A  N.%03d\n", idConta);
    printf("| CLIENTE: %s\n", vContas[pos].cliente);
    printf("+--------------------------------------------------+\n");
    printf("| ID   | OPERACAO        |     VALOR |     SALDO\n");
    printf("| ---- | Saldo Anterior  |           | ");
    imprimeValor(saldo);
    printf("\n");

    for (k = inicio; k < n; k++) {
        saldo += vOp[idx[k]].valor;
        printf("| %04d | %-15s | ", vOp[idx[k]].id, nomeTipo(vOp[idx[k]].tipo));
        imprimeValor(vOp[idx[k]].valor);
        printf(" | ");
        imprimeValor(saldo);
        printf("\n");
    }
    printf("+--------------------------------------------------+\n");
    printf("| Saldo Atual: ");
    imprimeValor(saldo);
    printf("\n+--------------------------------------------------+\n");
}

/* ---------- Extra, so para testar (nao faz parte da prova) ---------- */
void realizarDeposito(struct Conta vContas[], int tamConta,
                      struct Operacao vOp[], int *tamOp) {
    int id, pos;
    double valor;

    printf("ID da conta: ");
    scanf("%d", &id);
    printf("Valor do deposito: ");
    scanf("%lf", &valor);
    limparBuffer();

    pos = buscaId(vContas, tamConta, id);
    if (pos == -1 || valor <= 0 || *tamOp >= TAM_OPERACOES) {
        printf("Deposito nao realizado!\n");
        return;
    }
    vContas[pos].saldo += valor;
    vOp[*tamOp].id = *tamOp + 1;
    vOp[*tamOp].idConta = id;
    vOp[*tamOp].tipo = 'D';
    vOp[*tamOp].valor = valor;
    (*tamOp)++;
    printf("Deposito realizado!\n");
}

void listaDevedores(struct Conta vContas[], int tamConta) {
    struct Conta devedores[TAM_CONTAS];
    int tamDev, i;

    filtraDevedores(vContas, tamConta, devedores, &tamDev);
    printf("Devedores: %d\n", tamDev);
    for (i = 0; i < tamDev; i++) {
        printf("ID %d | %s | saldo %.2f\n",
               devedores[i].id, devedores[i].cliente, devedores[i].saldo);
    }
}

/* ---------- Programa principal ---------- */
int main(void) {
    struct Conta contas[TAM_CONTAS];
    struct Operacao ops[TAM_OPERACOES];
    int tamContas = 0, tamOps = 0;
    int opcao, pos;
    char nome[40];

    do {
        printf("\n1-Cadastrar conta  2-Buscar por nome  3-Devedores\n");
        printf("4-Sacar  5-Extrato  6-Depositar (teste)  0-Sair\nOpcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: cadastraConta(contas, &tamContas); break;
            case 2:
                printf("Nome (vetor deve estar em ordem alfabetica): ");
                fgets(nome, 40, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                pos = buscaBinariaNome(contas, tamContas, nome);
                if (pos == -1) printf("Nome nao encontrado.\n");
                else printf("Encontrado na posicao %d (conta %d)\n", pos, contas[pos].id);
                break;
            case 3: listaDevedores(contas, tamContas); break;
            case 4: realizarSaque(contas, tamContas, ops, &tamOps); break;
            case 5: extrato(contas, tamContas, ops, &tamOps); break;
            case 6: realizarDeposito(contas, tamContas, ops, &tamOps); break;
            case 0: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}
