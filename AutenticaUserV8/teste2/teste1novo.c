#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Para usar `exit()`
#include <locale.h>

#define MAX_USUARIOS 30
#define MAX_NOME 30
#define MAX_SENHA 20

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


                //printf("%s\n", split);

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

        if (strcmp(senhaAdmin, senhaADM) == 0) // compara senha de adm para acessar a função de cadastrar
        {
            estrutura novoUsuario; // preciso de uma variavel do tipo struct igual ao user para fazer a comparação dos dados inseridos e ver se o usuario existe ou não

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

                                    printf("Usuario cadastrado com sucesso!\n");
                                }
                                else
                                {
                                    printf("Erro ao ler a opção de cargo\n");
                                    limpabuffer(); // Limpar buffer após scanf
                                }
                            }
                            else
                            {
                                printf("Senha deve ter no mínimo 4 caracteres\n");
                            }
                        }
                        else
                        {
                            printf("Erro ao ler a senha\n");
                        }
                    }
                }
                else
                {
                    printf("Nome deve ter no mínimo 4 caracteres\n");
                }
            }
            else
            {
                printf("Erro ao ler o nome de usuario\n");
            }
        }
        else
        {
            printf("Senha incorreta!\n");
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
                ident = i;
                return 1;
            }
        }
        i++;
    }
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
        printf("3. Sair\n");
        printf("Escolha uma opcao: ");

        if (fgets(buffer, sizeof(buffer), stdin))
        {
            if (sscanf(buffer, "%d", &opcao) == 1 && opcao >= 1 && opcao <= 3)
            {
                return opcao;
            }
            else
            {
                printf("Opcao invalida. Tente novamente.\n");
            }
        }
        else
        {
            printf("Erro ao ler a entrada. Tente novamente.\n");
        }
    }
}

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
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
                printf("Nome: %s\n", user[ident].nome);
                printf("Senha: %s\n", user[ident].senha);
                printf("ID: %i\n", user[ident].id);
                printf("Cargo: %s\n", user[ident].cargo);
            }
            else
            {
                printf("Falha na autenticacao. Usuario ou senha incorretos.\n");
            }
            break;
        case 3:
            printf("Saindo...\n");
            break;
        default:
            // Não deve chegar aqui devido ao controle na função obterOpcao()
            break;
        }
    } while (opcao != 3);

    return 0;
}