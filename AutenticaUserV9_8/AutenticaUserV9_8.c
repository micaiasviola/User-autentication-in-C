#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Para usar `exit()`
#include <locale.h>
#include <setjmp.h>
#include <conio.h>

#define MAX_USUARIOS 30
#define MAX_NOME 30
#define MAX_SENHA 20
#define MAX_VENDAS 200

int limpar_tela()
{
        system(

            "cls"); // Comando do Windows para limpar a tela
        return 1;
}

typedef struct
{ // estrutura com os parametros de cada usuario
        char nome[MAX_NOME];
        char senha[MAX_SENHA];
        char cargo[MAX_NOME];
        int id;

} estrutura;

/***VARIAVEIS GLOBAIS****/
int idUsuario = 0; // inicia o id sempre em 0 para comeÃÂ§ar a varredura
int idproduto = 0;
int ident = 0;
int excedido = 0;
char senhaADM[] = "1234";
char senhaAdmin[MAX_SENHA];
int position;
char adm[] = "adm";
char caixa[] = "caixa";
estrutura user[MAX_USUARIOS];

void limpabuffer() // funÃÂ§ÃÂ£o para limpar o buffer apos um endereÃÂ§o de memoria ser escrito, chamo apÃÂ³s os fgets
{
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
                // corpo vazio para limpar o endereÃÂ§o de memoria
        }
}

void lerUsuarios() // funÃÂ§ÃÂ£o que executa a leitura do banco de dados, armazena a quantidade de cadastros em idUsuario
{
        FILE *leitura = fopen("idusersenha.txt", "r");
        if (leitura == NULL)
        { // mensagem de erro caso o arquivo esteja vazio
                perror("Erro ao abrir arquivo");
                return;
        }

        idUsuario = 0;
        while (fgets(user[idUsuario].nome, MAX_NOME, leitura) != NULL)
        {
                user[idUsuario].nome[strcspn(user[idUsuario].nome, "\n")] = '\0';
                char *split = strtok(user[idUsuario].nome, "|"); // primeiro token a ser pego da tabela ÃÂ© o id, portanto split esta valendo o id no momento

                user[idUsuario].id = atoi(split); // funcao atoi() transforma numeros strings em numeros inteiros
                                                  // printf("%s\n", user[idUsuario].id);
                if (split != NULL)
                {

                        split = strtok(NULL, "|"); // agora split recebe o nome, pois foi chamado a funÃÂ§ÃÂ£o null dele, entÃÂ£o ele entende como 1token ÃÂ© nulo e apos a divisÃÂ£o ele ira armazenar o 2token que no caso ÃÂ© o nome
                        strcpy(user[idUsuario].nome, split);
                        // printf("%s\n", user[idUsuario].nome);
                        if (split != NULL)
                        {
                                split = strtok(NULL, "|"); // aqui o split ira ignorar o token atual (2) e ler a partir da proxima divisÃÂ£o, que no caso sera o 3token == senha
                                strncpy(user[idUsuario].senha, split, MAX_SENHA - 1);
                                // printf("%s\n", user[idUsuario].senha);
                                user[idUsuario].senha[MAX_SENHA - 1] = '\0'; // define o ultimo caractere de .senha como nulo

                                split = strtok(NULL, "|"); // ultimo token

                                // printf("%s\n", split);

                                strncpy(user[idUsuario].cargo, split, MAX_SENHA - 1);
                                user[idUsuario].cargo[MAX_SENHA - 1] = '\0';
                                idUsuario++;

                                if (idUsuario >= MAX_USUARIOS)
                                {
                                        excedido = 1;
                                        break;
                                }
                        }
                }
        }
        fclose(leitura);
}
#define MAX_LINHA 100
#define MAX_PRODUTO 100

typedef struct
{
        char nome[MAX_NOME];
        int id;
        float preco;
} structproduto;

structproduto produto[MAX_PRODUTO];

void lerProduto()
{
        FILE *lerproduto = fopen("produtos.txt", "r");
        if (lerproduto == NULL)
        {
                perror("Erro ao abrir o arquivo");
                return;
        }

        char linha[MAX_LINHA];
        idproduto = 0;
        // LÃÂª a primeira linha do arquivo
        while (fgets(linha, sizeof(linha), lerproduto) != NULL)
        {
                // Remove o caractere de nova linha, se presente
                linha[strcspn(linha, "\n")] = '\0';
                char *parse = strtok(linha, "|");

                produto[idproduto].id = atoi(parse);
                // printf("%i|", produto[idproduto].id);
                if (parse != NULL)
                {

                        parse = strtok(NULL, "|");
                        strcpy(produto[idproduto].nome, parse);
                        // printf("%s|", produto[idproduto].nome);

                        if (parse != NULL)
                        {
                                parse = strtok(NULL, "|");
                                produto[idproduto].preco = (float)atof(parse);
                                // printf("%.2f\n", produto[idproduto].preco);
                                // printf("%i|", produto[idproduto].id);
                                // printf("%s|", produto[idproduto].nome);
                                // printf("%.2f\n", produto[idproduto].preco);
                                idproduto++;
                                if (idproduto >= MAX_PRODUTO)
                                {
                                        limpar_tela();
                                        printf("numero de produtos maximos cadastrados excedidos!");
                                        break;
                                }
                        }
                }
        }

        fclose(lerproduto);
}
void menuProdutos()
{
        // lerProduto();
        printf("\tMENU\n\n");

        for (int i = 0; i < idproduto; i++)
        {
                printf("\t%i|%s|%.2f\n", produto[i].id, produto[i].nome, produto[i].preco);
        }
}
void substituirProdutoNoArquivo(int id, structproduto NovoP)
{
        /*A RAZAO DESSA FUNÇÃO É PARA PODER APAGAR E RESSCREVER UMA NOVA LISTA DE PRODUTOS COM OS VALORES ANTERIORES POREM COM ALTERAÇÕES, PARA MUDAR O VALOR DE UM PRODUTO POR
        EXEMPLO. PRIMEIRO SE ABRE O ARQUIVO ORIGINAL PRODUTOS.TXT EM MOTO LEITURA E FAZ A VARREDURA DE TODAS AS LINHAS. CASO O PRODUTO A SER CADASTRADO JA FOR EXISTE, A FUNCAO
        CADASTRARPRODUTOS IRA ME RETORNAR O ID DO PRODUTO EM INT E ARMAZENARA O ID DA ESTRUTURA PRODUTO EM UMA NOVA ESTRUTURA DEFINIDA NESSA FUNCAO COMO NOVOP. APOS ISSO,
        DURANTE A VARREDURA NO LOOPING WHILE, OS VALORES DAS LINHAS SAO ARMAZENAS EM VARIAVEIS DE CONTROLE EXISTENTE. E SE O PRODUTO FOR EXISTENTE IREMOS ESCREVER NO NOVO ARQUIVO
        NA MESMA LINHA ORIGINAL AO ARQUIVO ANTERIOR, E POR FIM O ARQUIVO ORIGINAL SERA RESSCRITO EM UM ARQUIVO NOVO DE MESMO NOME E SERA APAGADO*/
        FILE *arquivo = fopen("produtos.txt", "r");
        FILE *arquivoTemp = fopen("temp.txt", "w");

        if (arquivo == NULL || arquivoTemp == NULL)
        {
                printf("Erro ao abrir o arquivo.\n");
                return;
        }

        char linha[256];
        while (fgets(linha, sizeof(linha), arquivo))
        {
                int idExistente;
                char nomeExistente[MAX_NOME];
                float precoExistente;

                // Ler os dados da linha atual
                sscanf(linha, "%d|%[^|]|%f", &idExistente, nomeExistente, &precoExistente);

                // Se for o produto que queremos alterar, gravar a linha com os novos dados
                if (idExistente == id)
                {
                        fprintf(arquivoTemp, "%d|%s|%.2f\n", NovoP.id, NovoP.nome, NovoP.preco);
                }
                else
                {
                        // Caso contrÃ¡rio, gravar a linha original
                        fprintf(arquivoTemp, "%s", linha);
                }
        }

        fclose(arquivo);
        fclose(arquivoTemp);

        // Substituir o arquivo original pelo arquivo temporÃ¡rio
        remove("produtos.txt");
        rename("temp.txt", "produtos.txt");

        printf("\nProduto atualizado com sucesso!\n");
}

void cadastrarProduto() {
    limpar_tela();

    printf("\tInsira o nome do produto que deseja cadastrar: ");
    structproduto NovoP;

    if (fgets(NovoP.nome, sizeof(NovoP.nome), stdin) != NULL) {
        NovoP.nome[strcspn(NovoP.nome, "\n")] = '\0';

        // Verificar se o produto já existe
        for (int i = 0; i < idproduto; i++) {
            if (strcmp(NovoP.nome, produto[i].nome) == 0) {
                printf("\tO item %s já está cadastrado! Deseja alterar o valor? (s) sim (n) nao: ", produto[i].nome);
                char resposta[10];

                if (fgets(resposta, sizeof(resposta), stdin)) {
                    resposta[strcspn(resposta, "\n")] = '\0';

                    if (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0) {
                        char novopreco[15]; // Buffer maior para o preco
                        float novoprecofloat;
                        printf("\tInsira o novo preco por kg: ");

                        if (fgets(novopreco, sizeof(novopreco), stdin) != NULL) {
                            novopreco[strcspn(novopreco, "\n")] = '\0';
                            novoprecofloat = strtof(novopreco, NULL);

                            produto[i].preco = novoprecofloat;
                            substituirProdutoNoArquivo(produto[i].id, produto[i]); // Substituir no arquivo passando id e produto como parametro
                            printf("\tProduto alterado com sucesso!\n");
                        }
                    } else {
                        printf("Usuário escolheu não. Cancelando a alteracao...\n");
                    }
                } else {
                    printf("Erro ao ler a entrada.\n");
                }
                return; // Parar a função após alterar o produto
            }
        }

        // Se o produto não existe, solicitar preco e confirmar o cadastro
        printf("\n\tInsira o preco por kg: ");
        char novopreco[15];
        if (fgets(novopreco, sizeof(novopreco), stdin) != NULL) {
            NovoP.preco = (float)atof(novopreco);
            if (NovoP.preco > 0) {
                NovoP.id = idproduto + 1; // Atribuir ID

                // Mostrar informações para confirmação
                printf("\n\tResumo do produto a ser cadastrado:\n");
                printf("\tNome: %s\n", NovoP.nome);
                printf("\tpreco por kg: R$%.2f\n", NovoP.preco);
                printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");

                char opcao[10];
                if (fgets(opcao, sizeof(opcao), stdin) != NULL) {
                    int escolha = atoi(opcao);

                    switch (escolha) {
                        case 1: // Confirmar
                            produto[idproduto] = NovoP;
                            idproduto++;

                            // Escrever no arquivo
                            FILE *escreveProduto = fopen("produtos.txt", "a");
                            if (escreveProduto != NULL) {
                                fprintf(escreveProduto, "%i|%s|%.2f\n", NovoP.id, NovoP.nome, NovoP.preco);
                                fclose(escreveProduto);
                                printf("\n\tProduto cadastrado com sucesso\n");
                            } else {
                                printf("Erro ao abrir o arquivo de produtos!\n");
                            }
                            break;

                        case 2: // Cancelar
                            printf("Cadastro do produto cancelado.\n");
                            break;

                        default:
                            printf("Opção inválida. Cadastro não realizado.\n");
                            break;
                    }
                } else {
                    printf("Erro ao ler a escolha. Cadastro não realizado.\n");
                }
            } else {
                printf("preco inválido...\n");
            }
        }
    } else {
        limpar_tela();
        printf("Erro ao ler a entrada. Tente novamente.\n");
    }
}
void cadastrarUsuario() // FUNCAO RESPONSAVEL PELO CADASTRO DE USUARIOS. VERIFICA AS CONDIÇÕES NECESSARIAS PARA O CADASTRO
{
        if (excedido == 1)
        {
                printf("Numero maximo de usuarios excedidos\n");
                return;
        }

        printf("\tDigite a senha de ADMINISTRADOR: ");
        if (fgets(senhaAdmin, MAX_SENHA, stdin) != NULL) // insere senha de adm
        {
                senhaAdmin[strcspn(senhaAdmin, "\n")] = '\0';
                limpar_tela();
                if (strcmp(senhaAdmin, senhaADM) == 0) // compara senha de adm para acessar a funÃÂ§ÃÂ£o de cadastrar
                {
                        estrutura novoUsuario; // preciso de uma variavel do tipo struct igual ao ser para fazer a comparaÃÂ§ÃÂ£o dos dados inseridos e ver se o usuario existe ou nÃ£o

                        printf("\tDigite o nome de usuario: ");
                        if (fgets(novoUsuario.nome, MAX_NOME, stdin) != NULL) // armazena nome atraves de fgets, definindo o tamanho do buffer como MAX_NOME, e o valor atraves do teclado (stdin)
                        {
                                novoUsuario.nome[strcspn(novoUsuario.nome, "\n")] = '\0'; // strcspn procura pela posiÃÂ§ÃÂ£o da string em que o \n se encontra e o substitui por \0

                                if (strlen(novoUsuario.nome) >= 4) // strlen() tamanho da variavel
                                {
                                        int usuarioExiste = 0;
                                        for (int i = 0; i < idUsuario; i++) // esse looping ira percorrer a quantidade de vezes e linhas que existem usuarios atraves da leitura feita na funÃÂ§ÃÂ£o lerusuarios armazenada em idUsuario
                                        {
                                                if (strcmp(user[i].nome, novoUsuario.nome) == 0) // se linha atual == novo nome entÃÂ£o usuario existe
                                                {
                                                        usuarioExiste = 1;
                                                        break;
                                                }
                                        }
                                        if (usuarioExiste) // condiÃÂ§ÃÂ£o de controle para usuario existente
                                        {
                                                limpar_tela();
                                                printf("Usuario existente\n");
                                        }
                                        else // caso o usuario nÃ£o existe ele ira para essa condiÃÂ§ÃÂ£o, no caso aqui sera feito o cadastro
                                        {
                                                printf("\tDigite a senha: ");
                                                if (fgets(novoUsuario.senha, MAX_SENHA, stdin) != NULL) // endereÃÂ§a valor de senha
                                                {
                                                        novoUsuario.senha[strcspn(novoUsuario.senha, "\n")] = '\0'; // retira a quebra de linha no fim da string
                                                        if (strlen(novoUsuario.senha) >= 4)                         // se senha for maior ou igual a 4 caracteres
                                                        {
                                                                printf("Digite 1 para adm ou 2 para caixa: ");
                                                                if (scanf("%d", &position) == 1)
                                                                {
                                                                        limpabuffer(); // Limpar buffer apÃÂ³s scanf

                                                                        if (position == 1)
                                                                        {
                                                                                strcpy(novoUsuario.cargo, adm);
                                                                        }
                                                                        else if (position == 2)
                                                                        {
                                                                                strcpy(novoUsuario.cargo, caixa);
                                                                        }
                                                                        else
                                                                        {
                                                                                limpar_tela();
                                                                                printf("OpÃÂ§ÃÂ£o invÃ¡lida. UsuÃ¡rio nÃ£o cadastrado.\n");
                                                                                return;
                                                                        }

                                                                        user[idUsuario] = novoUsuario; // endereÃÂ§a os valores .nome, .senha
                                                                        idUsuario++;
                                                                        FILE *escrevearquivo = fopen("idusersenha.txt", "a");
                                                                        if (escrevearquivo == NULL)
                                                                        {
                                                                                perror("Erro ao abrir arquivo");
                                                                                return;
                                                                        }
                                                                        fprintf(escrevearquivo, "%d|%s|%s|%s\n", idUsuario, user[idUsuario - 1].nome, user[idUsuario - 1].senha, user[idUsuario - 1].cargo);
                                                                        fclose(escrevearquivo);
                                                                        limpar_tela();
                                                                        printf("Usuario cadastrado com sucesso!\n");
                                                                        return;
                                                                }
                                                                else
                                                                {
                                                                        limpar_tela();
                                                                        printf("Erro ao ler a opÃÂ§ÃÂ£o de cargo\n");
                                                                        limpabuffer(); // Limpar buffer apÃÂ³s scanf
                                                                        return;
                                                                }
                                                        }
                                                        else
                                                        {
                                                                limpar_tela();
                                                                printf("Senha deve ter no minimo 4 caracteres\n");
                                                                return;
                                                        }
                                                }
                                                else
                                                {

                                                        return;
                                                }
                                        }
                                }
                                else
                                {
                                        limpar_tela();
                                        printf("Nome deve ter no minimo 4 caracteres\n");
                                        return;
                                }
                        }
                        else
                        {
                                limpar_tela();
                                printf("Erro ao ler o nome de usuario\n");
                                return;
                        }
                }
                else
                {
                        limpar_tela();
                        printf("Senha incorreta!\n");
                        return;
                }
        }
}

int autentica()
{
        lerUsuarios();
        char confirmaNome[MAX_NOME];
        char confirmaSenha[MAX_SENHA];

        printf("\tDigite o nome de usuario: ");
        fgets(confirmaNome, MAX_NOME, stdin);
        confirmaNome[strcspn(confirmaNome, "\n")] = '\0';

        printf("\tDigite a senha: ");
        fgets(confirmaSenha, MAX_SENHA, stdin);
        confirmaSenha[strcspn(confirmaSenha, "\n")] = '\0';

        int i = 0;
        while (i < idUsuario)
        {
                if (strcmp(confirmaNome, user[i].nome) == 0)
                {
                        if (strcmp(confirmaSenha, user[i].senha) == 0)
                        {
                                limpar_tela();
                                ident = i;
                                return 1;
                        }
                }
                i++;
        }
        limpar_tela();
        return 0;
}

int obterOpcao()
{
        int opcao = 0;
        char buffer[100];

        while (1)
        {

                printf("\nMenu:\n");
                printf("1. Cadastrar usuario\n");
                printf("2. Autenticar usuario\n");
                printf("3. LimparMenu\n");
                printf("4. Sair\n");
                printf("Escolha uma opcao: ");

                if (fgets(buffer, sizeof(buffer), stdin))
                {
                        if (sscanf(buffer, "%d", &opcao) == 1 && opcao >= 1 && opcao <= 4)
                        {
                                return opcao;
                        }
                        else
                        {
                                limpar_tela();
                                printf("Opcao invalida. Tente novamente.\n");
                        }
                }
                else
                {
                        limpar_tela();
                        printf("Erro ao ler a entrada. Tente novamente.\n");
                }
        }
}

typedef struct
{
        int id;
        char nome[100];
        float preco;
} structvendas;
 structvendas vendas[999];
int idvenda = 0;
void cadastrarVenda() {
    float precokg, valorkg, x;
    char produtoid[MAX_PRODUTO];
    char pesokg[10];
    char opcao[10];
    int id;

    structvendas novavenda;
    menuProdutos();

    printf("\n\n\tInforme o id do produto que deseja vender: \n");
    if (fgets(produtoid, sizeof(produtoid), stdin) != NULL) {
        limpar_tela();
        id = atoi(produtoid);

        // Verifica se o ID é válido
        if (id > 0 && id <= MAX_VENDAS) {
            precokg = produto[id - 1].preco;
            if (precokg > 0) {
                printf("\t\nInsira o peso em KILOGRAMAS, EX: 3,25\n\t");
                if (fgets(pesokg, sizeof(pesokg), stdin) != NULL) {
                    pesokg[strcspn(pesokg, "\n")] = '\0';
                    valorkg = strtof(pesokg, NULL);

                    x = valorkg * precokg;

                    printf("\tValor a pagar: R$%.2f\n", x);
                    printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");

                    // Lê a escolha do usuário
                    if (fgets(opcao, sizeof(opcao), stdin) != NULL) {
                        int escolha = atoi(opcao);

                        switch (escolha) {
                            case 1: {  // Confirma
                                FILE *relatorioVendas = fopen("relatorioVendas.txt", "r");
                                if (relatorioVendas != NULL) {
                                    idvenda = 0;
                                    char buffer[MAX_PRODUTO];
                                    while (fgets(buffer, sizeof(buffer), relatorioVendas) != NULL) {
                                        idvenda++;
                                    }
                                    fclose(relatorioVendas);
                                }

                                // Atualiza a nova venda
                                novavenda.id = idvenda;
                                strcpy(novavenda.nome, user[ident].nome);  // Certifique-se de que user e ident estão definidos
                                novavenda.preco = x;

                                // Armazena a venda no array
                                vendas[idvenda] = novavenda;

                                // Adiciona a nova venda ao arquivo
                                relatorioVendas = fopen("relatorioVendas.txt", "a");
                                if (relatorioVendas != NULL) {
                                    fprintf(relatorioVendas, "%i|%.2f|%s\n", vendas[idvenda].id, vendas[idvenda].preco, vendas[idvenda].nome);
                                    fclose(relatorioVendas);
                                } else {
                                    perror("ERRO AO ABRIR ARQUIVO");
                                    return;
                                }

                                // Imprime a venda recém-adicionada
                                printf("\nVenda confirmada:\n");
                                printf("%i|%.2f|%s\n", vendas[idvenda].id, vendas[idvenda].preco, vendas[idvenda].nome);
                                break;
                            }

                            case 2:  // Cancela
                                printf("Venda cancelada.\n");
                                break;

                            default:
                                printf("Opção inválida. Venda não confirmada.\n");
                                break;
                        }
                    } else {
                        printf("ERRO DE LEITURA");
                    }

                    // Pausa para o usuário
                    getchar();  // Substitua por getch() se estiver disponível
                } else {
                    printf("ERRO DE LEITURA");
                    getchar();  // Substitua por getch() se estiver disponível
                    return;
                }
            } else {
                printf("Produto inexistente!");
                getchar();  // Substitua por getch() se estiver disponível
                return;
            }
        } else {
            printf("ID do produto inválido!");
            getchar();  // Substitua por getch() se estiver disponível
            return;
        }
    } else {
        printf("ERRO DE LEITURA");
        getchar();  // Substitua por getch() se estiver disponível
        return;
    }
}

void relatorioVendas() {
    FILE *relatorioVendas = fopen("relatorioVendas.txt", "r");
    if (relatorioVendas == NULL) {
        perror("ARQUIVO VAZIO");
        return;
    }

    int i = 0;
    char buffer[MAX_PRODUTO];
    while (fgets(buffer, sizeof(buffer), relatorioVendas) != NULL) {
        sscanf(buffer, "%d|%f|%99[^\n]", &vendas[i].id, &vendas[i].preco, vendas[i].nome);
        i++;
    }
   // printf("%i|%.2f|%s\n", vendas[5].id, vendas[5].preco, vendas[5].nome);
    fclose(relatorioVendas);

    // Exibindo o relatório das vendas
    for (int j = 0; j < i; j++) {
        printf("%i - %.2f - %s\n", vendas[j].id, vendas[j].preco, vendas[j].nome);
    }
}
int menuADM()
{
        lerProduto();
        int opcaoADM = 0;
        char bufferADM[9];
        while (1)
        {
                limpar_tela();
                printf("Bem Vindo, %s, %s\n", user[ident].nome, user[ident].cargo);
                printf("\nMenu:\n");
                printf("1. Cadastrar/Alterar Produtos\n");
                printf("2. Efetuar Venda\n");
                printf("3. Relatorio de Vendas\n");
                printf("4. Sair\n");
                printf("Escolha uma opcao: ");

                if (fgets(bufferADM, sizeof(bufferADM), stdin))
                {
                        if (sscanf(bufferADM, "%d", &opcaoADM) == 1 && opcaoADM >= 1 && opcaoADM <= 4)
                        {
                                switch (opcaoADM)
                                {
                                case 1:
                                        limpar_tela();
                                        printf("CADASTRAR/ALTERAR PRODUTOS\n");
                                        cadastrarProduto();

                                        break;
                                case 2:
                                        // limpabuffer();
                                        printf("\tEfetuar Venda\n");
                                        cadastrarVenda();
                                        break;
                                case 3:
                                        relatorioVendas();

                                        getch();
                                        break;
                                case 4:
                                        printf("Saindo...");
                                        getch();
                                        limpar_tela();
                                        return 0;
                                        break;
                                }
                        }
                        else
                        {
                                limpar_tela();
                                printf("Opcao invalida. Tente novamente.\n");
                        }
                }
                else
                {
                        limpar_tela();
                        printf("Erro ao ler a entrada. Tente novamente.\n");
                }
        }
}

int menuCAIXA()
{
        int opcaoCAIXA = 0;
        char bufferCAIXA[9];
        while (1)
        {
                limpar_tela();
                printf("Bem Vindo, %s, %s\n", user[ident].nome, user[ident].cargo);
                printf("\nMenu:\n");
                printf("1. Efetuar Venda\n");
                printf("2. Consultar Precos\n");
                printf("3. Sair\n");
                printf("Escolha uma opcao: ");

                if (fgets(bufferCAIXA, sizeof(bufferCAIXA), stdin))
                {
                        if (sscanf(bufferCAIXA, "%d", &opcaoCAIXA) == 1 && opcaoCAIXA >= 1 && opcaoCAIXA <= 3)
                        {
                                switch (opcaoCAIXA)
                                {
                                case 1:
                                        printf("Efetuar Venda");

                                        getch();
                                        break;
                                case 2:
                                        printf("Consultar Precos");
                                        getch();
                                        break;
                                case 3:
                                        printf("Saindo...");
                                        getch();
                                        limpar_tela();
                                        return 0;
                                        break;
                                }
                        }
                        else
                        {
                                limpar_tela();
                                printf("Opcao invalida. Tente novamente.\n");
                        }
                }
                else
                {
                        limpar_tela();
                        printf("Erro ao ler a entrada. Tente novamente.\n");
                }
        }
}

int main()
{
        setlocale(LC_ALL, "Portuguese");
        int opcao;
        menuProdutos();
        lerUsuarios();
        lerProduto();
        limpar_tela();
        do
        {
                opcao = obterOpcao();

                switch (opcao)
                {
                case 1:
                        cadastrarUsuario();
                        break;
                case 2:
                        if (autentica() == 1)
                        {
                                int autenticaOpcao = 0;
                                limpar_tela();
                                if (strcmp(user[ident].cargo, adm) == 0)
                                {
                                        autenticaOpcao = 1;
                                }
                                else if (strcmp(user[ident].cargo, caixa) == 0)
                                {
                                        autenticaOpcao = 2;
                                }
                                printf("Bem Vindo, %s, %s\n", user[ident].nome, user[ident].cargo);

                                switch (autenticaOpcao)
                                {
                                case 1:
                                        printf("ADMINISTRADOR");
                                        menuADM();
                                        break;

                                case 2:
                                        printf("CAIXA");
                                        menuCAIXA();
                                        break;
                                }
                        }
                        else
                        {
                                limpar_tela();
                                printf("Falha na autenticacao. Usuario ou senha incorretos.\n");
                        }
                        break;
                case 3:
                        limpar_tela();
                        main();
                        break;
                case 4:

                        printf("Saindo...\n");
                        return 0;
                        break;
                default:
                        // nÃ£o deve chegar aqui devido ao controle na funÃÂ§ÃÂ£o obterOpcao()
                        break;
                }
        } while (opcao != 3);

        return 0;
}
