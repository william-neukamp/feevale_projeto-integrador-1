#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define END 999999
#define BUFFSIZE 512

// variavel global pra controlar linhas lidas do arquivo de log //
int lineCount = 0;

struct Log{
    char lineContext[BUFFSIZE];
    char timeStamp[15];
    char user[30];
    char data[BUFFSIZE];
    struct Log *next;
};

// 1 - cira node nulo //
struct Log* create(void);

// 2 - duas funcoes em um: adiciona no inicio ou no fim, vai via parametro //
struct Log* addLog(struct Log* head, int first);

// 3 - procura na linha do log completo, se tem retorna //
struct Log* removeLog(struct Log* head, char *str);

// 4 - mostra em tela as informacoes da lista //
void printLogs(struct Log* head);

// 5 - limpa lista //
struct Log* clearLogs(struct Log* head);

// 6 - procura e retorna um node caso encontre //
struct Log* searchLog(struct Log* head, char *str);

// 7 - preenche a informacao de um node da lista //
int fillNode(struct Log *node);

// 8 - quebra a string baseado no parâmetro //
char* split(char *str, int type);

int main()
{
    printf("Iniciando varredura, por favor, aguarde...\n");

    struct Log *head;
    start:
    // resetar consulta aos arquivos //
    lineCount = 0;
    while (1){
        // procura o log //        
        head = addLog(head, 0);        
        
        if (lineCount == END){
        	// imprime depois de checar logs //
        	printf("Varredura concluída. Imprimindo:\n");
        	printLogs(head);

        	// operacoes //
        	opc:
            printf("Opções disponíveis:\n");
            printf("1. Procurar na estrutura.\n");
            printf("2. Remover um item da estrutura.\n");
            printf("3. Limpar estrutura.\n");
            printf("4. Fazer varredura novamente.\n");
            printf("0. Sair.\n");

            __fpurge(stdin);
            int *tmp = malloc(sizeof(int));
            scanf("%ld", tmp);
            // 0 //
            if (*tmp == 0)
            	break;
            // 1 //
            if (*tmp == 1){
            	printf("Digite o que deseja procurar na estrutura: ");
            	__fpurge(stdin);
            	char aux[BUFFSIZE];
            	scanf("%[^\n]", aux);
            	struct Log *search = searchLog(head, aux);
            	if (search != NULL)
            		printf("Encontrado: %s", search->lineContext);
            	else
            		printf("Não encontrado");
            	printf("\n");
            	goto opc;
            }
            // 2 //
            if (*tmp == 2){
            	printf("Digite o que deseja remover na estrutura: ");
            	__fpurge(stdin);
            	char aux[BUFFSIZE];
            	scanf("%[^\n]", aux);
            	head = removeLog(head, aux);
            	printf("Atualizando logs existentes...\n");
            	printLogs(head);
            	printf("\n");
            	goto opc;
            }
            // 3 //
            if (*tmp == 3){
            	printf("Limpando estrutura...\n");
            	head = clearLogs(head);
            	printLogs(head);
            	printf("\n");
            	goto opc;
            }
            // 4 //
            if (*tmp == 4)
            	goto start;

            __fpurge(stdin);
        }
    }
}

// 1 //
struct Log* create(void){
    return NULL;
};

// 2 //
struct Log* addLog(struct Log* head, int first){
    struct Log *nodeAux = (struct Log*) malloc(sizeof(struct Log));
    if (!fillNode(nodeAux)){
         free(nodeAux);
         return head;
    }
    // inicio da lista //
    if (first){
        nodeAux->next = head;
        return nodeAux;
    }else{  // fim da lista //
        struct Log *nodeCount = NULL;
        nodeCount = head;
        // se for o 1 //
        if (head == NULL){
            nodeAux->next = NULL;
            head = nodeAux;
            return head;
        }
        // se for o 1 depois do head //
        if (head != NULL && head->next == NULL){
            nodeAux->next = NULL;
            head->next = nodeAux;
            return head;
        }
        while (nodeCount->next != NULL){
            nodeCount = nodeCount->next;
            if (nodeCount->next == NULL){
                nodeAux->next = NULL;
                nodeCount->next = nodeAux;
                nodeCount = head;
                return nodeCount;
            }
        }
    };
};

// 3 //
struct Log* removeLog(struct Log* head, char *str){
    struct Log *now = head;
    struct Log *old = NULL;
    char aux[BUFFSIZE];
    strcpy(aux, str);  
    while (now != NULL & strstr(now->lineContext, aux) == NULL){
    	printf("aux: %s\n", aux);
    	printf("node: %s\n", now->lineContext);
    	old = now;
    	now = now->next;
    }
    // nao encontrou nada //
    if (now == NULL){
        return head;
    }
    if (old == NULL){
        head = now->next;
    }else{
        old->next = now->next;
    }
    free(now);
    return head;
};

// 4 //
void printLogs(struct Log* head){
  struct Log *nodeCount = NULL;
    for (nodeCount = head; nodeCount != NULL; nodeCount = nodeCount->next){
        printf("Hora: %s \n", nodeCount->timeStamp);
        printf("Usuario: %s \n", nodeCount->user);
        printf("Linha: %s \n", nodeCount->data);
        printf("Linha inteira: %s\n", nodeCount->lineContext);
        printf("-------------------------------------------------------------------------\n");
        if (nodeCount->next == NULL){
            return;
        }
    }
    printf("Lista vazia.\n");
};

// 5 //
struct Log* clearLogs(struct Log* head){
    struct Log *now = head;
    struct Log *old = NULL;
    while (now != NULL){
        old = now->next;
        free(now);
        now = old;
    }
    return NULL;
};

// 6 //
struct Log* searchLog(struct Log* head, char *str){
    struct Log *nodeCount;
    char aux[BUFFSIZE];
    strcpy(aux, str);
    for (nodeCount = head; nodeCount != NULL; nodeCount = nodeCount->next){
        if (strstr(nodeCount->lineContext, aux) != NULL){
            return nodeCount;
        }
    }
    return NULL;
};

// 7 //
int fillNode(struct Log *node){
    FILE *file;
    char line[99];
    //char *pline;
    //rever seg fault //
    char *pline = malloc(sizeof(char) * BUFFSIZE);

    // abre arquivo //
    //file = fopen("E:\\teste.txt", "r"); // WINDOWS
    file = fopen("/var/log/syslog", "r"); // LINUX

    // Se houve erro na abertura
    if (file == NULL){
        printf("Problemas na abertura do arquivo\n");
        return 0;
    }

    // le as linhas (512 bytes/caracteres) //
    int selfCount = 0;
    while (!feof(file)){
        // passa pro ponteiro a informação de ate 512 bytes/caracteres //
        fgets(pline, BUFFSIZE, file);

        selfCount++;

        // testa se prossegue ou nao - so cria node de fato se for de conexao ou desconexao de rede //
        // caiu a rede //
        if (strstr(pline, "link disconnected") != NULL && selfCount > lineCount){
            strcpy(node->lineContext, pline);               // linha inteira //
            strcpy(node->timeStamp, split(pline, 1));       // data/hora     //
            strcpy(node->user, split(pline, 2));            // user/host     //
            strcpy(node->data, split(pline, 3));            // info log      //
            // contador pra guardar ultima linha do arquivo gravada //
            lineCount = selfCount;
            // retorna node pra insercao //
            return 1;
        }
        // voltou a rede //
        if (strstr(pline, "link connected") != NULL && selfCount > lineCount){
            strcpy(node->lineContext, pline);               // linha inteira //
            strcpy(node->timeStamp, split(pline, 1));       // data/hora     //
            strcpy(node->user, split(pline, 2));            // user/host     //
            strcpy(node->data, split(pline, 3));            // info log      //
            // contador pra guardar ultima linha do arquivo gravada //
            lineCount = selfCount;
            // retorna node pra insercao //
            return 1;
        }
    }

    // encerra loop infinito que procura no log as infos de conexao de desconexao //
    lineCount = END;
    // fecha buffer -- possivel memory leak, pois o return esta sempre antes.  //
    fclose(file);
};

// 8 //
char* split(char *str, int type){
    // 1 -  timestamp
    // 2 -  user
    // 3 -  data
    char *aux;
    char lstr[BUFFSIZE];	// TEM QUE SER DO TAMANHO DO BUFFER!!!
    strcpy(lstr, &*str);
    // data/hora //
    if (type == 1){
        char tmp[15];
        for (int i = 0; i <= 14; i++){
        	tmp[i] = lstr[i];
        }
        char *result = tmp;
        return result;
    };
    // user/host //
    if (type == 2){
        aux = strtok(lstr, " ");
        char tmp[BUFFSIZE] = "";
        int i = 1;
        while (aux != NULL && i <= 4){
            if (i >= 4)
                strcat(tmp, aux);
            aux = strtok(NULL, " ");
            ++i;
        }
        char *result = tmp;
        return result;
    }
    // info do log //
    if (type == 3){
        aux = strtok(lstr, " ");
        char tmp[BUFFSIZE] = "";
        int i = 1;
        while (aux != NULL){
            if (i >= 5)
                strcat(tmp, aux);
            aux = strtok(NULL, " ");
            ++i;
        }
        char *result = tmp;
        return result;
    }
    char *result = "NOT FOUND";
    return result;
};
