#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_PRODUTOS 100
#define ARQUIVO_CSV "produtos.csv"
#define ARQUIVO_DOACAO "doacoes.csv"
#define ARQUIVO_FLUXO_CAIXA "fluxo_caixa.csv"


//Estrutura do cadastro de produto
typedef struct {
    int id;
    char nome[50];
    float preco_por_kg;
    char categoria[30];
    float preco_fornecedor;
    float quantidade;
} Produto;

// Nome das institui��es para doa��o
const char* instituicoes[] = {
    "Nova Esperan�a", 
    "Vida Feliz", 
    "A��o Esperan�a", 
    "Caminho do Bem"
};

// Estrutura para armazenar dados do fluxo de caixa
typedef struct {
    char descricao[50];
    float valor;
    char tipo[10];
} Transacao;


// Estrutura para armazenar as doa��es
typedef struct {
    char nome_produto[50];
    float quantidade_doada;
    int instituicao_id;
} Doacao;

// Fun��o para salvar doa��es no arquivo csv
void salvar_doacoes(Doacao doacoes[], int total_doacoes) {
    FILE *file = fopen(ARQUIVO_DOACAO, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de doa��es.\n");
        return;
    }

    for (int i = 0; i < total_doacoes; i++) {
        fprintf(file, "%s,%.2f,%d\n", 
                doacoes[i].nome_produto, 
                doacoes[i].quantidade_doada, 
                doacoes[i].instituicao_id);
    }

    fclose(file);
}

Produto produtos[MAX_PRODUTOS];
int total_produtos = 0;

// Fun��o para as categorias dos produtos
const char *categorias[] = {"Fruta", "Verdura", "Org�nico", "Legume"};

// Fun��o para limpar a tela
void limpar_tela() {
    system("cls||clear");
}

// Fun��o para limpar o buffer
void limpar_buffer() {
    while (getchar() != '\n');
}

// Fun��o para entrada de n�mero inteiro
int entrada_inteira_valida() {
    int numero;
    while (scanf("%d", &numero) != 1) {
        printf("Entrada inv�lida. Digite um n�mero inteiro: ");
        limpar_buffer();
    }
    limpar_buffer();
    return numero;
}

// Fun��o para entrada de n�mero decimal
float entrada_float_valida() {
    float numero;
    while (scanf("%f", &numero) != 1) {
        printf("Entrada inv�lida. Digite um n�mero decimal: ");
        limpar_buffer();
    }
    limpar_buffer();
    return numero;
}

// Fun��o para salvar as transa��es do fluxo de caixa no arquivo csv
void salvar_transacao(const char *descricao, float valor, const char *tipo) {
    FILE *file = fopen(ARQUIVO_FLUXO_CAIXA, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de fluxo de caixa.\n");
        return;
    }

    fprintf(file, "%s,%.2f,%s\n", descricao, valor, tipo);
    fclose(file);
}




// Fun��o para carregar produtos do arquivo csv
void carregar_produtos() {
    FILE *file = fopen(ARQUIVO_CSV, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    total_produtos = 0;
    while (fscanf(file, "%d,%49[^,],%f,%29[^,],%f,%f\n", 
              &produtos[total_produtos].id, 
              produtos[total_produtos].nome,
              &produtos[total_produtos].preco_por_kg, 
              produtos[total_produtos].categoria, 
              &produtos[total_produtos].quantidade,
              &produtos[total_produtos].preco_fornecedor) == 6) {
    total_produtos++;
}

    fclose(file);
}

// Fun��o para salvar produtos no arquivo csv
void salvar_produtos() {
    FILE *file = fopen(ARQUIVO_CSV, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar os produtos.\n");
        return;
    }

    for (int i = 0; i < total_produtos; i++) {
        fprintf(file, "%d,%s,%.2f,%s,%.2f,%.2f\n", 
        produtos[i].id, 
        produtos[i].nome,
        produtos[i].preco_por_kg, 
        produtos[i].categoria, 
        produtos[i].quantidade,
		produtos[i].preco_fornecedor);  
}

    fclose(file);
}

// Fun��o para mostrar o fluxo de caixa no terminal
void registrar_fluxo_caixa() {
    FILE *file = fopen(ARQUIVO_FLUXO_CAIXA, "r");
    if (file == NULL) {
        printf("Nenhum registro encontrado no fluxo de caixa.\n");
        return;
    }

    char descricao[50];
    float valor;
    char tipo[10];
    float saldo = 0.0;

    printf("\n                \033[1;34m  === Fluxo de Caixa ===  \033[0m               \n");
    printf("+-------------------------------+----------+------------+\n");
    printf("| Descri��o                     | Valor    | Tipo       |\n");
    printf("+-------------------------------+----------+------------+\n");

    while (fscanf(file, " %49[^,],%f,%9s\n", descricao, &valor, tipo) == 3) {
        printf("| %-29s | %-8.2f | %-10s |\n", descricao, valor, tipo);
        if (strcmp(tipo, "Entrada") == 0) {
            saldo += valor;
        } else {
            saldo -= valor;
        }
    }

    printf("+-------------------------------+----------+------------+\n");
    printf("Saldo Atual: R$ %.2f\n\n", saldo);
    fclose(file);
}

// Fun��o para listar produtos
void listar_produtos() {
    printf("Lista de Produtos:\n\n");
    printf("%-5s %-25s %-12s %-20s %-12s\n", "ID", "Nome", "Pre�o/kg", "Categoria", "Quantidade");
    
    for (int i = 0; i < total_produtos; i++) {
        printf("%-5d %-25s %-12.2f %-20s %-12.2f\n", 
               produtos[i].id, produtos[i].nome, produtos[i].preco_por_kg, produtos[i].categoria, produtos[i].quantidade);
    }
}

// Fun��o para listar doa��es no terminal
void listar_doacoes() {
    FILE *file = fopen(ARQUIVO_DOACAO, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de doa��es.\n");
        return;
    }

    char nome_produto[50];
    float quantidade_doada;
    int instituicao_id;

    printf("Relat�rio de Doa��es:\n");
    printf("+-------------------------+-------------------+--------------------+\n");
    printf("| Produto                 | Quantidade (kg)   | Institui��o        |\n");
    printf("+-------------------------+-------------------+--------------------+\n");

    int found_any = 0;
    while (fscanf(file, " %49[^,],%f,%d\n", nome_produto, &quantidade_doada, &instituicao_id) == 3) {
        // Usa o nome da institui��o ao inv�s do ID
        const char* instituicao_nome = instituicoes[instituicao_id - 1];
        printf("| %-23s | %-17.2f | %-18s |\n", nome_produto, quantidade_doada, instituicao_nome);
        found_any = 1;
    }

    if (!found_any) {
        printf("| Nenhuma doa��o registrada no sistema.                           |\n");
    }

    printf("+-------------------------+-------------------+--------------------+\n");

    fclose(file);
}

// Fun��o para cadastrar produto
void cadastrar_produto() {
    if (total_produtos >= MAX_PRODUTOS) {
        printf("Limite m�ximo de produtos alcan�ado.\n\n");
        return;
    }

    Produto p;
    p.id = total_produtos + 1;
    printf("Digite o nome do produto: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = 0;
    printf("Digite o pre�o por kg do produto: ");
    p.preco_por_kg = entrada_float_valida();
    
    printf("Digite o pre�o do fornecedor (por kg): ");
    p.preco_fornecedor = entrada_float_valida();


    printf("Escolha a categoria:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s\n", i + 1, categorias[i]);
    }
    int opcao_categoria = entrada_inteira_valida();
    strcpy(p.categoria, categorias[opcao_categoria - 1]);

    printf("Digite a quantidade dispon�vel (em kg): ");
    p.quantidade = entrada_float_valida();
    salvar_transacao("Compra de Estoque", p.quantidade * p.preco_fornecedor, "Sa�da");

    produtos[total_produtos++] = p;
    salvar_produtos();
    printf("Produto cadastrado com sucesso!\n\n");
}


// Fun��o para listar os produtos com um tabela organizada diferencial , mostrando pre�o do fornecedor.
void listar_produtos_com_moldura_admin() {
    printf("+----+---------------------------+--------------+---------------------+------------------+------------------+\n");
    printf("| ID | Nome                      | Pre�o/kg     | Categoria           | Quantidade       | Pre�o Fornecedor |\n");
    printf("+----+---------------------------+--------------+---------------------+------------------+------------------+\n");
    
    for (int i = 0; i < total_produtos; i++) {
        printf("| %-2d | %-25s | %-12.2f | %-19s | %-16.2f | %-16.2f |\n", 
               produtos[i].id, produtos[i].nome, produtos[i].preco_por_kg, produtos[i].categoria, 
               produtos[i].quantidade,  produtos[i].preco_fornecedor);
    }

    printf("+----+---------------------------+--------------+---------------------+------------------+------------------+\n");
}


// Fun��o para alterar dados de um produto
void alterar_produto() {
    int id_produto;

    listar_produtos_com_moldura_admin();
    printf("\nDigite o ID do produto que deseja alterar: ");
    id_produto = entrada_inteira_valida();

    if (id_produto < 1 || id_produto > total_produtos) {
        printf("Produto n�o encontrado!\n\n");
        return;
    }

    Produto *p = &produtos[id_produto - 1];

    printf("Alterando informa��es do produto: %s\n", p->nome);
    printf("1. Nome\n2. Pre�o por kg\n3. Categoria\n4. Quantidade\n5. Pre�o do fornecedor\n");
    printf("Escolha o campo que deseja alterar (1-5): ");
    int opcao = entrada_inteira_valida();

    switch (opcao) {
        case 1:
            printf("Digite o novo nome: ");
            fgets(p->nome, sizeof(p->nome), stdin);
            p->nome[strcspn(p->nome, "\n")] = 0;
            break;
        case 2:
            printf("Digite o novo pre�o por kg: ");
            p->preco_por_kg = entrada_float_valida();
            break;
        case 3:
            printf("Escolha a nova categoria:\n");
            for (int i = 0; i < 4; i++) {
                printf("%d. %s\n", i + 1, categorias[i]);
            }
            int nova_categoria = entrada_inteira_valida();
            strcpy(p->categoria, categorias[nova_categoria - 1]);
            break;
        case 4:
            printf("Digite a nova quantidade (em kg): ");
            p->quantidade = entrada_float_valida();
            break;
        case 5:
            printf("Digite o novo pre�o do fornecedor: ");
            p->preco_fornecedor = entrada_float_valida();
            break;
        default:
            printf("Op��o inv�lida!\n\n");
            return;
    }

    salvar_produtos();
    printf("Informa��es do produto atualizadas com sucesso!\n\n");
}



// Fun��o para mostrar a tabela de produtos ( sem pre�o do fornecedor )
void listar_produtos_com_moldura() {
    printf("+----+---------------------------+--------------+---------------------+------------------+\n");
    printf("| ID | Nome                      | Pre�o/kg     | Categoria           | Quantidade       |\n");
    printf("+----+---------------------------+--------------+---------------------+------------------+\n");
    
    for (int i = 0; i < total_produtos; i++) {
        printf("| %-2d | %-25s | %-12.2f | %-19s | %-16.2f |\n", 
               produtos[i].id, produtos[i].nome, produtos[i].preco_por_kg, produtos[i].categoria, 
               produtos[i].quantidade);
    }

    printf("+----+---------------------------+--------------+---------------------+------------------+\n");
}



// Modifica��o na fun��o de realizar a compra para salvar as doa��es
void realizar_compra() {
    int id_produto;
    float quantidade_desejada;
    char continuar_compra;
    char finalizar_compra;
    char doar_mais;
    float peso_total = 0.0;
    float valor_total = 0.0;

    Doacao doacoes[MAX_PRODUTOS];
    int total_doacoes = 0;

    // Funb��o para armazenar os produtos comprados e suas quantidades
    typedef struct {
        Produto produto;
        float quantidade_comprada;
    } Compra;

    Compra compras[MAX_PRODUTOS];
    int total_compras = 0;

    do {
        listar_produtos_com_moldura();  
        printf("\nDigite o ID do produto que deseja comprar: ");
        id_produto = entrada_inteira_valida();

        if (id_produto < 1 || id_produto > total_produtos) {
            printf("\033[1;31mProduto n�o encontrado!\033[0m\n\n");
            continue;  
        }

        Produto *p = &produtos[id_produto - 1];

        printf("Digite a quantidade desejada (em kg): ");
        quantidade_desejada = entrada_float_valida();

        if (quantidade_desejada > p->quantidade) {
            printf("Quantidade indispon�vel! Estoque atual: %.2f kg\n\n", p->quantidade);
        } else {
            p->quantidade -= quantidade_desejada;
            salvar_produtos();

            // Adiciona o produto � lista de compras
            compras[total_compras].produto = *p;
            compras[total_compras].quantidade_comprada = quantidade_desejada;
            total_compras++;

            // Atualiza o peso total e o valor total
            peso_total += quantidade_desejada;
            valor_total += quantidade_desejada * p->preco_por_kg;

            printf("\nProduto adicionado ao carrinho.\n\n");
        }

        // Pergunta se o cliente deseja continuar comprando
        printf("Deseja continuar comprando? (S/N): ");
        scanf(" %c", &continuar_compra);
        limpar_buffer(); 

    } while (continuar_compra == 'S' || continuar_compra == 's');

    // Exibe o resumo da compra (quando o cliente parar de comprar)
    if (total_compras > 0) {
        printf("\n=== Resumo da Compra ===\n");
        printf("+----+---------------------------+--------------+---------------------+--------------+--------------+\n");
        printf("| ID | Nome                      | Quantidade   | Pre�o/kg            | Total (R$)   | Peso (kg)    |\n");
        printf("+----+---------------------------+--------------+---------------------+--------------+--------------+\n");

        for (int i = 0; i < total_compras; i++) {
            printf("| %-2d | %-25s | %-12.2f | %-12.2f        | %-12.2f | %-12.2f |\n", 
                   i + 1, compras[i].produto.nome, compras[i].quantidade_comprada, 
                   compras[i].produto.preco_por_kg, compras[i].quantidade_comprada * compras[i].produto.preco_por_kg, 
                   compras[i].quantidade_comprada);
        }

        printf("+----+---------------------------+--------------+---------------------+--------------+--------------+\n");

        // Exibe o peso total e o valor total
        printf("\nPeso Total: %.2f kg\n", peso_total);
        printf("Valor Total: R$ %.2f\n\n", valor_total);
    }

    // Pergunta se o cliente deseja finalizar a compra ou abandonar
    printf("Deseja finalizar a compra ou abandonar?\n");
    printf("1. Finalizar\n");
    printf("2. Abandonar\n");
    printf("Digite sua op��o: ");
    scanf(" %c", &finalizar_compra);
    limpar_buffer();  

    if (finalizar_compra == '1') {
        printf("\n\033[1;32mCompra finalizada com sucesso!\033[0m\n\n");
        salvar_transacao("Venda de Produtos", valor_total, "Entrada");


        // Pergunta se o cliente deseja doar
        printf("Deseja doar algum produto? (S/N): ");
        scanf(" %c", &doar_mais);
        limpar_buffer();

        if (doar_mais == 'S' || doar_mais == 's') {
            // Exibe as institui��es fict�cias
            printf("\nEscolha uma institui��o para doa��o:\n");
			for (int i = 0; i < 4; i++) {
    		printf("%d. %s\n", i + 1, instituicoes[i]);
			}
			int instituicao_escolhida;
			printf("Digite a op��o: ");
			instituicao_escolhida = entrada_inteira_valida();

			printf("\nVoc� escolheu a Institui��o %s. Vamos agora escolher os produtos para doa��o.\n", instituicoes[instituicao_escolhida - 1]);


            do {
                // Exibe os produtos comprados pelo cliente
                printf("\n=== Produtos Comprados ===\n");
                printf("+----+---------------------------+--------------+--------------+\n");
                printf("| ID | Nome                      | Quantidade   | Pre�o/kg     |\n");
                printf("+----+---------------------------+--------------+--------------+\n");

                for (int i = 0; i < total_compras; i++) {
                    printf("| %-2d | %-25s | %-12.2f | %-12.2f |              \n", 
                           i + 1, compras[i].produto.nome, 
                           compras[i].quantidade_comprada, compras[i].produto.preco_por_kg);
                }

                printf("+----+---------------------------+--------------+--------------+\n");

                // O cliente pode escolher o produto e a quantidade para doa��o
                int id_produto_doacao;
                printf("\nDigite o ID do produto que deseja doar: ");
                id_produto_doacao = entrada_inteira_valida();

                if (id_produto_doacao < 1 || id_produto_doacao > total_compras) {
                    printf("\033[1;31mProduto n�o encontrado!\033[0m\n\n");
                    continue;
                }

                float quantidade_doada;
                printf("Digite a quantidade desejada para doa��o (kg): ");
                quantidade_doada = entrada_float_valida();

                if (quantidade_doada <= compras[id_produto_doacao - 1].quantidade_comprada) {
                    doacoes[total_doacoes].quantidade_doada = quantidade_doada;
                    doacoes[total_doacoes].instituicao_id = instituicao_escolhida;
                    strcpy(doacoes[total_doacoes].nome_produto, compras[id_produto_doacao - 1].produto.nome);

                    total_doacoes++;

                    compras[id_produto_doacao - 1].quantidade_comprada -= quantidade_doada;
                    printf("\nProduto doado com sucesso!\n");

                } else {
                    printf("Quantidade excedente! Voc� n�o possui essa quantidade em sua compra.\n");
                }

                printf("Deseja doar outro produto? (S/N): ");
                scanf(" %c", &doar_mais);
                limpar_buffer();

            } while (doar_mais == 'S' || doar_mais == 's');

            // Salva as doa��es no arquivo csv
            salvar_doacoes(doacoes, total_doacoes);
        }
    }
}

// Fun��o para Admin ou Cliente
int autenticar_usuario() {
    int opcao;
    printf("\033[1;32m+-------------------------------+\n");
    printf("|        SEMEANDO SA�DE         |\n");
    printf("+-------------------------------+\033[0m\n");
    printf("\033[1;32m+-------------------------------+\033[0m\n");
    printf("\033[1;32m|                               |\033[0m\n");
    printf("\033[1;32m|\033[0m  Escolha seu tipo de \033[1;36musu�rio\033[0m: \033[1;32m|\033[0m\n");
    printf("\033[1;32m|_______________________________|\033[0m\n");
       printf("\033[1;32m|                               |\033[0m\n");
    printf("\033[1;32m|\033[0m  \033[1;36m1. Administrador   \033[0m          \033[1;32m|\033[0m\n");
    printf("\033[1;32m|\033[0m  \033[1;36m2. Cliente  \033[0m                 \033[1;32m|\033[0m\n");
    printf("\033[1;32m|\033[0m \033[1;31m 0. Sair           \033[0m           \033[1;32m|\033[0m\n");
    printf("\033[1;32m+-------------------------------+\033[0m\n");
    printf("Digite a op��o: ");
    opcao = entrada_inteira_valida();

    if (opcao == 1) {
        char usuario[20], senha[20];
        printf("Login (admin): ");
        fgets(usuario, sizeof(usuario), stdin);
        usuario[strcspn(usuario, "\n")] = 0;

        printf("Senha: ");
        fgets(senha, sizeof(senha), stdin);
        senha[strcspn(senha, "\n")] = 0;

        if (strcmp(usuario, "admin") == 0 && strcmp(senha, "123") == 0) {
            return 1;
        } else {
            printf("\033[1;31m\nUsu�rio ou senha incorretos.\033[0m\n\n");
            system("pause");
            system("cls");
            return autenticar_usuario();
        }
    } else if (opcao == 2) {
        return 0;
    } else if (opcao == 0) {
        printf("Saindo...\n\n");
        exit(0);
    } else {
        printf("Op��o inv�lida! Tente novamente.\n\n");
        return autenticar_usuario();
    }
}


// Fun��o principal (main)

int main() {
	 setlocale(LC_ALL, "portuguese");
    int opcao;
    int admin = autenticar_usuario();

    carregar_produtos();

    do {
        limpar_tela();
        printf("\033[1;34m");
        printf("+--------------------------+\n");
        printf("| Sistema de Gerenciamento |\n");
        printf("+--------------------------+\n ");
        printf("\033[0m\n");
        printf("\033[1;32m+--------------------------+\033[0m\n");
        printf("\033[1;32m|\033[0m1. Listar Produtos        \033[1;32m|\033[0m\n");

        if (admin) {
   		   
            printf("\033[1;32m|\033[0m2. Cadastrar Produto      \033[1;32m|\033[0m\n");
            printf("\033[1;32m|\033[0m3. Alterar                \033[1;32m|\033[0m\n");
            printf("\033[1;32m|\033[0m4. Relat�rio de Doa��es   \033[1;32m|\033[0m\n");
            printf("\033[1;32m|\033[0m5. Fluxo de Caixa         \033[1;32m|\033[0m\n");
            
        } else {
            printf("\033[1;32m|\033[0m2. Realizar Compra \033[1;32m       |\033[0m\n");
        }

        printf("\033[1;32m|\033[0m\033[1;31m0. Sair\033[0m                   \033[1;32m|\033[0m\n");
        printf("\033[1;32m+--------------------------+\033[0m\n\n");
        printf("Digite sua op��o: ");
        opcao = entrada_inteira_valida();

        limpar_tela();
        
        // Op��es para Admin e Cliente.

        switch (opcao) {
            case 1:
				 if (admin) {
                    listar_produtos_com_moldura_admin();
				} else { 
	                listar_produtos_com_moldura();
                }
                break;
            case 2:
                if (admin) {
                    cadastrar_produto();
                } else {
                    realizar_compra();
                }
                break;
            case 3:
                if (admin) {
                    alterar_produto();
                } else {
                    printf("Op��o inv�lida!\n\n");
                }
                break;
            case 4:
                if (admin) {
     			 	listar_doacoes();
				 } else {
                    printf("Op��o inv�lida!\n\n");
                }
                break;
            case 5:
                if (admin) {
     			 	registrar_fluxo_caixa();
				 } else {
                    printf("Op��o inv�lida!\n\n");
                }
                break;
            
            case 0:
                printf("Saindo...\n\n");
                break;
            default:	
                printf("Op��o inv�lida!\n\n");
        }

        printf("Pressione Enter para continuar...");
        limpar_buffer();

    } while (opcao != 0);

    return 0;
}
