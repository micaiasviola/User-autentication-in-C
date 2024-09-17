#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Para usar `exit()`
#include <locale.h>
#include <setjmp.h>
#include <conio.h>
#include <time.h>

#define MAX_USUARIOS 30
#define MAX_NOME 30
#define MAX_SENHA 20
#define MAX_VENDAS 200
#define MAX_LINHA 100
#define MAX_PRODUTO 100

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
int idSessao = 0; // idSessaoifica o usuario que esta logado
int excedido = 0;
char senhaADM[] = "1234";
char senhaAdmin[MAX_SENHA];
int position;
char adm[] = "adm";
char caixa[] = "caixa";
estrutura user[MAX_USUARIOS];

void limpabuffer() // funcao para limpar o buffer apos um endereco de memoria ser escrito, chamo apÃÂ³s os fgets
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // corpo vazio para limpar o endereco de memoria
    }
}

void lerUsuarios() // funcao que executa a leitura do banco de dados, armazena a quantidade de cadastros em idUsuario
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
        char *split = strtok(user[idUsuario].nome, "|"); // primeiro token a ser pego da tabela apos o id, portanto split esta valendo o id no momento

        user[idUsuario].id = atoi(split); // funcao atoi() transforma numeros strings em numeros inteiros
                                          // printf("%s\n", user[idUsuario].id);
        if (split != NULL)
        {

            split = strtok(NULL, "|"); // agora split recebe o nome, pois foi chamado a funcao null dele, entÃÂ£o ele entende como 1token apos nulo e apos a divisao ele ira armazenar o 2token que no caso apos o nome
            strcpy(user[idUsuario].nome, split);
            // printf("%s\n", user[idUsuario].nome);
            if (split != NULL)
            {
                split = strtok(NULL, "|"); // aqui o split ira ignorar o token atual (2) e ler a partir da proxima divisao, que no caso sera o 3token == senha
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
    // ler a primeira linha do arquivo
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

    printf("\n\tProduto atualizado com sucesso!\n");
}

void cadastrarProduto()
{

    menuProdutos();
    structproduto NovoP;
    printf("\tInsira o nome do produto que deseja cadastrar: ");
    if (fgets(NovoP.nome, sizeof(NovoP.nome), stdin) != NULL)
    {
        NovoP.nome[strcspn(NovoP.nome, "\n")] = '\0';

        // Verificar se o produto já existe
        for (int i = 0; i < idproduto; i++)
        {
            if (strcmp(NovoP.nome, produto[i].nome) == 0)
            {
                printf("\tO item %s já está cadastrado! Deseja alterar o valor? (s) sim (n) nao: ", produto[i].nome);
                char resposta[10];

                if (fgets(resposta, sizeof(resposta), stdin))
                {
                    resposta[strcspn(resposta, "\n")] = '\0';

                    if (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0)
                    {
                        char novopreco[15]; // Buffer maior para o preco
                        float novoprecofloat;
                        printf("\tInsira o novo preco por kg: ");

                        if (fgets(novopreco, sizeof(novopreco), stdin) != NULL)
                        {
                            novopreco[strcspn(novopreco, "\n")] = '\0';
                            novoprecofloat = strtof(novopreco, NULL);

                            produto[i].preco = novoprecofloat;
                            if (produto[i].preco > 0)
                            {
                                printf("\n\tResumo do produto a ser cadastrado:\n");
                                printf("\tNome: %s\n", produto[i].nome);
                                printf("\tpreco por kg: R$%.2f\n", produto[i].preco);
                                printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");

                                char opcao1[10];
                                if (fgets(opcao1, sizeof(opcao1), stdin) != NULL)
                                {
                                    int escolha = atoi(opcao1);

                                    switch (escolha)
                                    {
                                    case 1:
                                        substituirProdutoNoArquivo(produto[i].id, produto[i]); // Substituir no arquivo passando id e produto como parametro
                                        getch();
                                        break;
                                    case 2: // Cancelar
                                        limpar_tela();
                                        printf("\tAlteracao do produto cancelada.\n");
                                        break;

                                    default:
                                        printf("Opção inválida. Cadastro não realizado.\n");
                                        break;
                                    }
                                }
                                else
                                {
                                    perror("Invalid");
                                    return;
                                }
                            }
                        }
                        else
                        {
                            printf("Usuário escolheu não. Cancelando a alteracao...\n");
                        }
                    }
                    else
                    {
                        printf("Erro ao ler a entrada.\n");
                    }
                    return; // Parar a função após alterar o produto
                }
            }
        }
        // Se o produto não existe, solicitar preco e confirmar o cadastro
        printf("\n\tInsira o preco por kg: ");
        char novopreco[15];
        if (fgets(novopreco, sizeof(novopreco), stdin) != NULL)
        {
            NovoP.preco = (float)atof(novopreco);
            if (NovoP.preco > 0)
            {
                NovoP.id = idproduto + 1; // Atribuir ID

                // Mostrar informações para confirmação
                printf("\n\tResumo do produto a ser cadastrado:\n");
                printf("\tNome: %s\n", NovoP.nome);
                printf("\tpreco por kg: R$%.2f\n", NovoP.preco);
                printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");

                char opcao[10];
                if (fgets(opcao, sizeof(opcao), stdin) != NULL)
                {
                    int escolha = atoi(opcao);

                    switch (escolha)
                    {
                    case 1: // Confirmar
                        produto[idproduto] = NovoP;
                        idproduto++;

                        // Escrever no arquivo
                        FILE *escreveProduto = fopen("produtos.txt", "a");
                        if (escreveProduto != NULL)
                        {
                            fprintf(escreveProduto, "%i|%s|%.2f\n", NovoP.id, NovoP.nome, NovoP.preco);
                            fclose(escreveProduto);
                            printf("\n\tProduto cadastrado com sucesso\n");
                        }
                        else
                        {
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
                }
                else
                {
                    printf("Erro ao ler a escolha. Cadastro não realizado.\n");
                }
            }
            else
            {
                printf("preco inválido...\n");
            }
        }
    }

    else
    {
        limpar_tela();
        printf("Erro ao ler a entrada. Tente novamente.\n");
    }
}

void excluirProduto()
{
    int idProduto;
    printf("\tDigite o ID do produto que deseja excluir: ");
    
    char idStr[10];
    if (fgets(idStr, sizeof(idStr), stdin) != NULL)
    {
        idProduto = atoi(idStr); // Converter a string para número

        int encontrado = -1;
        for (int i = 0; i < idproduto; i++)
        {
            if (produto[i].id == idProduto)
            {
                encontrado = i;
                break;
            }
        }

        if (encontrado != -1)
        {
            printf("\tProduto %s encontrado. Deseja realmente excluir? (s/n): ", produto[encontrado].nome);
            char resposta[5];
            if (fgets(resposta, sizeof(resposta), stdin))
            {
                resposta[strcspn(resposta, "\n")] = '\0';

                if (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0)
                {
                    // Remover o produto do array
                    for (int i = encontrado; i < idproduto - 1; i++)
                    {
                        produto[i] = produto[i + 1];
                    }
                    idproduto--; // Decrementa o total de produtos

                    // Reescrever o arquivo de produtos com IDs atualizados
                    FILE *arquivo = fopen("produtos.txt", "w");
                    if (arquivo != NULL)
                    {
                        for (int i = 0; i < idproduto; i++)
                        {
                            produto[i].id = i + 1; // Atualizar ID de cada produto
                            fprintf(arquivo, "%i|%s|%.2f\n", produto[i].id, produto[i].nome, produto[i].preco);
                        }
                        fclose(arquivo);
                        printf("\n\tProduto excluído e IDs atualizados com sucesso!\n");
                    }
                    else
                    {
                        printf("Erro ao abrir o arquivo de produtos.\n");
                    }
                }
                else
                {
                    printf("\tOperação de exclusão cancelada.\n");
                }
            }
            else
            {
                printf("\tErro ao ler a resposta. Tente novamente.\n");
            }
        }
        else
        {
            printf("\tProduto com ID %d não encontrado!\n", idProduto);
        }
    }
    else
    {
        printf("Erro ao ler a entrada.\n");
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
        if (strcmp(senhaAdmin, senhaADM) == 0) // compara senha de adm para acessar a funcao de cadastrar
        {
            estrutura novoUsuario; // preciso de uma variavel do tipo struct igual ao ser para fazer a compacao dos dados inseridos e ver se o usuario existe ou nao

            printf("\tDigite o nome de usuario: ");
            if (fgets(novoUsuario.nome, MAX_NOME, stdin) != NULL) // armazena nome atraves de fgets, definindo o tamanho do buffer como MAX_NOME, e o valor atraves do teclado (stdin)
            {
                novoUsuario.nome[strcspn(novoUsuario.nome, "\n")] = '\0'; // strcspn procura pela posicao da string em que o \n se encontra e o substitui por \0

                if (strlen(novoUsuario.nome) >= 4) // strlen() tamanho da variavel
                {
                    int usuarioExiste = 0;
                    for (int i = 0; i < idUsuario; i++) // esse looping ira percorrer a quantidade de vezes e linhas que existem usuarios atraves da leitura feita na funcao lerusuarios armazenada em idUsuario
                    {
                        if (strcmp(user[i].nome, novoUsuario.nome) == 0) // se linha atual == novo nome usuario existe
                        {
                            usuarioExiste = 1;
                            break;
                        }
                    }
                    if (usuarioExiste) // controle para usuario existente
                    {
                        limpar_tela();
                        printf("Usuario existente\n");
                    }
                    else // caso o usuario nao exista sera feito o cadastro
                    {
                        printf("\tDigite a senha: ");
                        if (fgets(novoUsuario.senha, MAX_SENHA, stdin) != NULL) //  valor de senha
                        {
                            novoUsuario.senha[strcspn(novoUsuario.senha, "\n")] = '\0'; // retira a quebra de linha no fim da string
                            if (strlen(novoUsuario.senha) >= 4)                         // se senha for maior ou igual a 4 caracteres
                            {
                                printf("Digite 1 para adm ou 2 para caixa: ");
                                if (scanf("%d", &position) == 1)
                                {
                                    limpabuffer(); // Limpar buffer scanf

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
                                        printf("OpÃÂ§ÃÂ£o invÃ¡lida. UsuÃ¡rio nao cadastrado.\n");
                                        return;
                                    }

                                    user[idUsuario] = novoUsuario; // atribui os novos valores no array user
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
                                    printf("Opcao invalida\n");
                                    limpabuffer(); 
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
                idSessao = i;
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
    char horariovenda[20];
} structvendas;

structvendas vendas[999];

int idvenda = 0;
void cadastrarVenda()
{
    float precokg, valorkg, x;
    char produtoid[MAX_PRODUTO];
    char pesokg[10];
    char opcao[10];
    char nomeproduto[MAX_NOME];
    int id;

    structvendas novavenda;
    limpar_tela();
    menuProdutos();

    printf("\n\n\tInforme o id do produto que deseja vender: ");
    if (fgets(produtoid, sizeof(produtoid), stdin) != NULL)
    {
        limpar_tela();
        id = atoi(produtoid);

        // Verifica se o ID é válido
        if (id > 0 && id <= MAX_VENDAS)
        {
            strcpy(nomeproduto, produto[id - 1].nome);
            precokg = produto[id - 1].preco;
            if (precokg > 0)
            {
                printf("\n\t%s R$%.2fKG.\n\t\n\tInsira a quantidade de KGs: ", produto[id - 1].nome, produto[id - 1].preco);
                if (fgets(pesokg, sizeof(pesokg), stdin) != NULL)
                {
                    pesokg[strcspn(pesokg, "\n")] = '\0';
                    valorkg = strtof(pesokg, NULL);

                    x = valorkg * precokg;

                    printf("\n\tValor a pagar: R$%.2f\n", x);
                    printf("\t1 - Confirma\n\t2 - Cancela\n\tEscolha: ");

                    // Lê a escolha do usuário
                    if (fgets(opcao, sizeof(opcao), stdin) != NULL)
                    {
                        int escolha = atoi(opcao);

                        switch (escolha)
                        {
                        case 1:
                        { // Confirma
                            FILE *relatorioVendas = fopen("relatorioVendas.txt", "r");
                            if (relatorioVendas != NULL)
                            {
                                idvenda = 0;
                                char buffer[MAX_PRODUTO];
                                while (fgets(buffer, sizeof(buffer), relatorioVendas) != NULL)
                                {
                                    idvenda++;
                                }
                                fclose(relatorioVendas);
                            }

                            // Recebe o horario atual para a venda
                            time_t agora;
                            time(&agora);

                            struct tm *local = localtime(&agora);

                            char buffer[80];
                            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local);

                            strcpy(novavenda.horariovenda, buffer);

                            // Atualiza a nova venda
                            novavenda.id = idvenda;
                            strcpy(novavenda.nome, user[idSessao].nome);
                            novavenda.preco = x;

                            // Armazena a venda no array de vendas
                            vendas[idvenda] = novavenda;

                            // Adiciona a nova venda ao arquivo
                            relatorioVendas = fopen("relatorioVendas.txt", "a");
                            if (relatorioVendas != NULL)
                            {
                                fprintf(relatorioVendas, "%i|%.2f|%s|%s\n", vendas[idvenda].id, vendas[idvenda].preco, vendas[idvenda].nome, vendas[idvenda].horariovenda);
                                fclose(relatorioVendas);
                            }
                            else
                            {
                                perror("ERRO AO ABRIR ARQUIVO");
                                return;
                            }

                            // Imprime a venda recém-adicionada
                            limpar_tela();
                            printf("\nVenda confirmada: ");
                            printf("ID %i - R$%.2f - %s\n", vendas[idvenda].id, vendas[idvenda].preco, vendas[idvenda].nome);
                            break;
                        }

                        case 2: // Cancela
                            printf("Venda cancelada.\n");
                            break;

                        default:
                            printf("Opção inválida. Venda não confirmada.\n");
                            break;
                        }
                    }
                    else
                    {
                        printf("ERRO DE LEITURA");
                    }

                    // Pausa para o usuário
                }
                else
                {
                    printf("ERRO DE LEITURA");

                    return;
                }
            }
            else
            {
                printf("Produto inexistente!");

                return;
            }
        }
        else
        {
            printf("ID do produto inválido!");

            return;
        }
    }
    else
    {
        printf("ERRO DE LEITURA");

        return;
    }
}
void relatorioVendasCaixa()
{
    FILE *relatorioVendas = fopen("relatorioVendas.txt", "r");
    if (relatorioVendas == NULL)
    {
        perror("ARQUIVO VAZIO");
        return;
    }

    int i = 0;
    char buffer[MAX_PRODUTO];
    printf("\n\t     CODIGO - VALOR - VENDEDOR - HORARIO\n");
    while (fgets(buffer, sizeof(buffer), relatorioVendas) != NULL)
    {
        sscanf(buffer, "%d|%f|%99[^|]|%19[^\n]", &vendas[i].id, &vendas[i].preco, vendas[i].nome, vendas[i].horariovenda);

        // Filtra apenas as vendas do caixa logado
        if (strcmp(vendas[i].nome, user[idSessao].nome) == 0)
        {

            printf("\n\t    ID %i - R$%.2f - %s - %s\n", vendas[i].id, vendas[i].preco, vendas[i].nome, vendas[i].horariovenda);
        }
        i++;
    }
    fclose(relatorioVendas);
}
void relatorioVendas()
{
    limpar_tela();
    FILE *relatorioVendas = fopen("relatorioVendas.txt", "r");
    if (relatorioVendas == NULL)
    {
        perror("ARQUIVO VAZIO");
        return;
    }
    // endereca as vendas toda vez que e chamado
    int i = 0;
    char buffer[MAX_PRODUTO];
    while (fgets(buffer, sizeof(buffer), relatorioVendas) != NULL)
    {
        sscanf(buffer, "%d|%f|%99[^|]|%19[^\n]", &vendas[i].id, &vendas[i].preco, vendas[i].nome, vendas[i].horariovenda);
        i++;
    }
    // limpabuffer();
    //  printf("%i|%.2f|%s\n", vendas[5].id, vendas[5].preco, vendas[5].nome);
    fclose(relatorioVendas);
    printf("\tCODIGO - VALOR - VENDEDOR - HORARIO\n");
    // Exibindo o relatório das vendas
    for (int j = 0; j < i; j++)
    {
        printf("\n\t    ID %i - R$%.2f - %s - %s\n", vendas[j].id, vendas[j].preco, vendas[j].nome, vendas[j].horariovenda);
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
        printf("Bem Vindo, %s, %s\n", user[idSessao].nome, user[idSessao].cargo);
        printf("\nMenu:\n");
        printf("1. Cadastrar/Alterar Produtos\n");
        printf("2. Efetuar Venda\n");
        printf("3. Relatorio de Vendas\n");
        printf("4. Sair\n");
        printf("5. Remover Produto\n");
        printf("Escolha uma opcao: ");

        if (fgets(bufferADM, sizeof(bufferADM), stdin))
        {
            if (sscanf(bufferADM, "%d", &opcaoADM) == 1 && opcaoADM >= 1 && opcaoADM <= 5)
            {
                switch (opcaoADM)
                {
                case 1:
                    limpar_tela();

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
                case 5:
                    menuProdutos();                               
                    excluirProduto();
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
        printf("Bem Vindo, %s, %s\n", user[idSessao].nome, user[idSessao].cargo);
        printf("\nMenu:\n");
        printf("1. Efetuar Venda\n");
        printf("2. Consultar Precos\n");
        printf("3. Relatorio de vendas\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");

        if (fgets(bufferCAIXA, sizeof(bufferCAIXA), stdin))
        {
            if (sscanf(bufferCAIXA, "%d", &opcaoCAIXA) == 1 && opcaoCAIXA >= 1 && opcaoCAIXA <= 4)
            {
                switch (opcaoCAIXA)
                {
                case 1:
                    printf("Efetuar Venda");
                    cadastrarVenda();
                    getch();
                    break;
                case 2:
                    printf("\n\tConsultar Precos\n");
                    menuProdutos();
                    getch();
                    break;
                case 3:
                    printf("\n\t     RELATORIO DE VENDAS\n");
                    relatorioVendasCaixa();
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
                if (strcmp(user[idSessao].cargo, adm) == 0)
                {
                    autenticaOpcao = 1;
                }
                else if (strcmp(user[idSessao].cargo, caixa) == 0)
                {
                    autenticaOpcao = 2;
                }
                printf("Bem Vindo, %s, %s\n", user[idSessao].nome, user[idSessao].cargo);

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
            // nao deve chegar aqui devido ao controle na funcao obterOpcao()
            break;
        }
    } while (opcao != 3);

    return 0;
}