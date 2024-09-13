#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Para usar `exit()`
#include <locale.h>
#include <setjmp.h>
#include <conio.h>

#define MAX_USUARIOS 30
#define MAX_NOME 30
#define MAX_SENHA 20

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
int idUsuario = 0; // inicia o id sempre em 0 para começar a varredura
int ident = 0;
int excedido = 0;
char senhaADM[] = "1234";
char senhaAdmin[MAX_SENHA];
int position;
char adm[] = "adm";
char caixa[] = "caixa";
estrutura user[MAX_USUARIOS];

void limpabuffer() // função para limpar o buffer apos um endereço de memoria ser escrito, chamo após os fgets
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // corpo vazio para limpar o endereço de memoria
    }
}

void lerUsuarios() // função que executa a leitura do banco de dados, armazena a quantidade de cadastros em idUsuario
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
        char *split = strtok(user[idUsuario].nome, "|"); // primeiro token a ser pego da tabela é o id, portanto split esta valendo o id no momento

        user[idUsuario].id = atoi(split); // funcao atoi() transforma numeros strings em numeros inteiros
                                          // printf("%s\n", user[idUsuario].id);
        if (split != NULL)
        {

            split = strtok(NULL, "|"); // agora split recebe o nome, pois foi chamado a função null dele, então ele entende como 1token é nulo e apos a divisão ele ira armazenar o 2token que no caso é o nome
            strcpy(user[idUsuario].nome, split);
            // printf("%s\n", user[idUsuario].nome);
            if (split != NULL)
            {
                split = strtok(NULL, "|"); // aqui o split ira ignorar o token atual (2) e ler a partir da proxima divisão, que no caso sera o 3token == senha
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
int idproduto = 0;
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
    // Lê a primeira linha do arquivo
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
                printf("%i|", produto[idproduto].id);
                printf("%s|", produto[idproduto].nome);
                printf("%.2f\n", produto[idproduto].preco);
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
void cadastrarProduto()
{
    

    while (1)
    {
        int opcao;
        char buffer[MAX_PRODUTO];
        // limpar_tela();

        printf("\nMenu:\n");
        printf("1. Cadastrar novo produto\n");
        printf("2. Alterar produto\n");
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");

        if (fgets(buffer, sizeof(buffer), stdin))
        {
            if (sscanf(buffer, "%d", &opcao) == 1 && opcao >= 1 && opcao <= 3)
            {
                switch (opcao)
                {
                case 1:
                    printf("Insira o nome do produto que seja cadastrar\n");
                    lerProduto();
                    
                    getch();
                    break;
                case 2:
                    printf("Consultar Preços");
                    getch();
                    break;
                case 3:
                    printf("Saindo...");
                    getch();
                    limpar_tela();
                    return;
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

void cadastrarUsuario() // função responsavel pelo cadastro de usuarios, verifica as condições para cadastro
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
        if (strcmp(senhaAdmin, senhaADM) == 0) // compara senha de adm para acessar a função de cadastrar
        {
            estrutura novoUsuario; // preciso de uma variavel do tipo struct igual ao ser para fazer a comparação dos dados inseridos e ver se o usuario existe ou não

            printf("\tDigite o nome de usuario: ");
            if (fgets(novoUsuario.nome, MAX_NOME, stdin) != NULL) // armazena nome atraves de fgets, definindo o tamanho do buffer como MAX_NOME, e o valor atraves do teclado (stdin)
            {
                novoUsuario.nome[strcspn(novoUsuario.nome, "\n")] = '\0'; // strcspn procura pela posição da string em que o \n se encontra e o substitui por \0

                if (strlen(novoUsuario.nome) >= 4) // strlen() tamanho da variavel
                {
                    int usuarioExiste = 0;
                    for (int i = 0; i < idUsuario; i++) // esse looping ira percorrer a quantidade de vezes e linhas que existem usuarios atraves da leitura feita na função lerusuarios armazenada em idUsuario
                    {
                        if (strcmp(user[i].nome, novoUsuario.nome) == 0) // se linha atual == novo nome então usuario existe
                        {
                            usuarioExiste = 1;
                            break;
                        }
                    }
                    if (usuarioExiste) // condição de controle para usuario existente
                    {
                        limpar_tela();
                        printf("Usuario existente\n");
                    }
                    else // caso o usuario não existe ele ira para essa condição, no caso aqui sera feito o cadastro
                    {
                        printf("\tDigite a senha: ");
                        if (fgets(novoUsuario.senha, MAX_SENHA, stdin) != NULL) // endereça valor de senha
                        {
                            novoUsuario.senha[strcspn(novoUsuario.senha, "\n")] = '\0'; // retira a quebra de linha no fim da string
                            if (strlen(novoUsuario.senha) >= 4)                         // se senha for maior ou igual a 4 caracteres
                            {
                                printf("Digite 1 para adm ou 2 para caixa: ");
                                if (scanf("%d", &position) == 1)
                                {
                                    limpabuffer(); // Limpar buffer após scanf

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
                                        printf("Opção inválida. Usuário não cadastrado.\n");
                                        return;
                                    }

                                    user[idUsuario] = novoUsuario; // endereça os valores .nome, .senha
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
                                    printf("Erro ao ler a opção de cargo\n");
                                    limpabuffer(); // Limpar buffer após scanf
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

int menuADM()
{
    int opcaoADM = 0;
    char bufferADM[9];
    while (1)
    {
        limpar_tela();
        printf("Bem Vindo, %s, %s\n", user[ident].nome, user[ident].cargo);
        printf("\nMenu:\n");
        printf("1. Cadastrar/Alterar Produtos\n");
        printf("2. Vendas recentes\n");
        printf("3. Menu\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");

        if (fgets(bufferADM, sizeof(bufferADM), stdin))
        {
            if (sscanf(bufferADM, "%d", &opcaoADM) == 1 && opcaoADM >= 1 && opcaoADM <= 4)
            {
                switch (opcaoADM)
                {
                case 1:
                    printf("CADASTRAR/ALTERAR PRODUTOS\n");
                    lerProduto();
                    cadastrarProduto();
                    getch();
                    break;
                case 2:
                    printf("VENDAS");
                    getch();
                    break;
                case 3:

                    printf("MENU\n");
                    lerProduto();
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
        printf("2. Consultar Preços\n");
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
                    printf("Consultar Preços");
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
    setlocale(LC_ALL, "");
    int opcao;
    
    lerUsuarios();

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
            // Não deve chegar aqui devido ao controle na função obterOpcao()
            break;
        }
    } while (opcao != 3);

    return 0;
}
