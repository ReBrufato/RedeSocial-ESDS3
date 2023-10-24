#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//definição de funções
typedef struct NO* ArvAVL;

//strutura da pessoa
typedef struct pessoa {
	int id;
	char cpf[12];
	char nome[200];
	char sobrenome[200];
} pessoa;

//estrutura que armazena um nó/árvore
struct NO{
    pessoa pessoa;
    int altura;
    struct NO *esq;
    struct NO *dir;
};

//estrutura do grafo
typedef struct Grafo{
	int **adj; //ponteiro para um ponteiro de inteiros, que irá referenciar cada vetor(linha) de inteiros da matriz
	int n; //número máximo de elementos do grafo	
}Grafo;

//códigos da árvore AVL
ArvAVL* cria_ArvAVL();
void libera_ArvAVL(ArvAVL *raiz);
int insere_ArvAVL(ArvAVL *raiz, char*, char*, char*);
int remove_ArvAVL(ArvAVL *raiz, int valor);
int estaVazia_ArvAVL(ArvAVL *raiz);
int altura_ArvAVL(ArvAVL *raiz);
int totalNO_ArvAVL(ArvAVL *raiz);
int consulta_ArvAVL(ArvAVL *raiz, int valor);
void preOrdem_ArvAVL(ArvAVL *raiz);
void emOrdem_ArvAVL(ArvAVL *raiz);
void posOrdem_ArvAVL(ArvAVL *raiz);

//funções extras
void inicia_grafo(Grafo *g, int); //recebe um ponteiro para um grafo e o número de elementos a ser setado
void adiciona_amizade(Grafo *grafo, ArvAVL *raiz);
void imprimi_amigos(Grafo *g, ArvAVL *raiz);
void exibe_id(ArvAVL *raiz, int);
void exibe_amigos_dos_amigos(Grafo *g, ArvAVL *raiz);

//variável que controla o número de pessoas cadastradas na árvore
int id = 0;

//Função principal
int main(){
	//inicializa o grafo com 100 posições
	Grafo *grafo;
	grafo = malloc(sizeof(Grafo));
	inicia_grafo(grafo, 100);
	
	//cria um ponteiro para um ponteiro para uma struct NO
    ArvAVL* avl;
	
	//raiz da arvore AVL
    avl = cria_ArvAVL(); 
	
	//variável de controle do primeiro laço
	int opcao=0;
	
	//menu principal
	while(opcao!=6){
		printf("-------CADASTRO DE PESSOAS------- \n");
		printf("1 - Cadastrar Pessoa \n");
		printf("2 - Imprimir pessoas cadastradas \n");
		printf("3 - Adicionar amizades\n");
		printf("4 - Imprimir amigos \n");
		printf("5 - Exibir amigos de amigos \n");
		printf("6 - Sair \n");
		
		//seleciona a entrada
		printf("Escolha a opcao desejada: ");
		scanf("%d", &opcao);
		printf("\n");
		
		//Cadastra Pessoa
		if (opcao == 1){
			int res;
			char CPF[12];
			char nome[200];
			char sobrenome[200];
			
			//pega as informações de entrada para a pessoa a ser cadastrada
			//CPF
			fflush(stdin);
			printf("Digite o CPF da pessoa : ");
			gets(CPF);
			//testa de o CPF foi digitado da forma correta
			while(strlen(CPF) < 11){
				printf("Numeros de caracteres invalidos! O CPF devera apresentar 11 digitos\n");
				fflush(stdin);
				printf("Digite o CPF da pessoa : ");
				gets(CPF);
			}
			
			//nome
			fflush(stdin);
			printf("Digite o nome da pessoa : ");
			gets(nome);
			
			//sobrenome
			fflush(stdin);
			printf("Digite o sobrenome da pessoa : ");
			gets(sobrenome);
			
			//verifica se o cadastro ocorreu de forma correta
			res = insere_ArvAVL(avl, nome, sobrenome, CPF);
			
			//incrementa id
			if(res == 1){
				id++;
				printf("\nCadastro concluido com sucesso!\n\n");
			}else{
				printf("Falha no cadastro!\n");
			}
			
		}
		
		//exibe oas pessoas cadastradas em ordem crescente de ID
		else if (opcao == 2){
			emOrdem_ArvAVL(avl);
		}
		
		//cria amizades
		else if (opcao == 3){
			adiciona_amizade(grafo, avl);
		}
		
		//imprimi amigos de uma pessoa 
		else if (opcao == 4){
			imprimi_amigos(grafo, avl);
		}
		
		//imprimi amigos dos amigos de uma pessoa
		else if (opcao == 5){
			exibe_amigos_dos_amigos(grafo, avl);
		}
		
		
		else if (opcao == 6){
			printf("Obrigado, fim do programa! \n");
		}
		
		else{
			printf("OPCAO INVALIDA, TENTE NOVAMENTE! \n");
		}
	}
}

////////////////////////////////////////////////FUNÇÕES ARVORE AVL ////////////////////////////////////////////////


//cria um ponteiro para a raiz da árvore, que foi alocada dinamicamente na memória
ArvAVL* cria_ArvAVL(){
    ArvAVL* raiz = (ArvAVL*) malloc(sizeof(ArvAVL));
    if(raiz != NULL)
        *raiz = NULL;
    return raiz;
}

//destroí a árvore limpando cada espaço de memória dos nós criados
void libera_NO(struct NO* no){
    if(no == NULL)
        return;
    libera_NO(no->esq);
    libera_NO(no->dir);
    free(no);
    no = NULL;
}

//limpa a árvore, e passa para a função 'libera_NO' a raiz da árvore
void libera_ArvAVL(ArvAVL* raiz){
    if(raiz == NULL)
        return;
    libera_NO(*raiz);
    free(raiz);
}

//retorna a altura de um nó em questão, se for nullo ele não terá altura, que será tratado como '-1'
int altura_NO(struct NO* no){
    if(no == NULL)
        return -1;
    else
    return no->altura;
}


int fatorBalanceamento_NO(struct NO* no){
    return labs(altura_NO(no->esq) - altura_NO(no->dir));
}

//retorna o maior inteiro
int maior(int x, int y){
    if(x > y)
        return x;
    else
        return y;
}

//verifica se a árvore está sem raiz, ou seja, se ela está nula. No caso de sim significa que a árvore não existe, ou seja,e stá vazia
int estaVazia_ArvAVL(ArvAVL *raiz){
    if(raiz == NULL)
        return 1;
    if(*raiz == NULL)
        return 1;
    return 0;
}

//retorna o número de nós da árvore, executando a soma dos nós da subárvore da esquerda e da direita.
//Dessa forma a conta é realizada de baixo para cima através da recursão, ou seja, no sentido das folhas para a raiz
int totalNO_ArvAVL(ArvAVL *raiz){
    if (raiz == NULL)
        return 0;
    if (*raiz == NULL)
        return 0;
    int alt_esq = totalNO_ArvAVL(&((*raiz)->esq));
    int alt_dir = totalNO_ArvAVL(&((*raiz)->dir));
    return(alt_esq + alt_dir + 1);
}

//executa uma função recursiva que retorna a altura da subárvore maior, ou seja, a altura da árvore
int altura_ArvAVL(ArvAVL *raiz){
    if (raiz == NULL)
        return 0;
    if (*raiz == NULL)
        return 0;
    int alt_esq = altura_ArvAVL(&((*raiz)->esq));
    int alt_dir = altura_ArvAVL(&((*raiz)->dir));
    if (alt_esq > alt_dir)
        return (alt_esq + 1);
    else
        return(alt_dir + 1);
}

//exibe o valor e a altura de cada nó
void preOrdem_ArvAVL(ArvAVL *raiz){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        //printf("%d\n",(*raiz)->info);
        //printf("No %d: %d\n",(*raiz)->info,fatorBalanceamento_NO(*raiz));
        printf("No %d: %d\n",(*raiz)->pessoa.id,altura_NO(*raiz));
        preOrdem_ArvAVL(&((*raiz)->esq));
        preOrdem_ArvAVL(&((*raiz)->dir));
    }
}

//exibe o valor de cada nó, sua altura e o fator de balanceamento
void emOrdem_ArvAVL(ArvAVL *raiz){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        emOrdem_ArvAVL(&((*raiz)->esq));
        
        printf("Posicao na arvore: %d \n",(*raiz)->pessoa.id);
        printf("CPF: %s \n",(*raiz)->pessoa.cpf);
        printf("Nome: %s \n",(*raiz)->pessoa.nome);
        printf("Sobrenome: %s \n",(*raiz)->pessoa.sobrenome);
        printf("\n");
        emOrdem_ArvAVL(&((*raiz)->dir));
    }
}

//exibe o valor de cada nó
void posOrdem_ArvAVL(ArvAVL *raiz){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        posOrdem_ArvAVL(&((*raiz)->esq));
        posOrdem_ArvAVL(&((*raiz)->dir));
        printf("%d\n",(*raiz)->pessoa.id);
    }
}


//procura um valor na árvore. Se o valor for encontrado é retornado '1', se não é retornado '0'. Se o valor for maior do que o do nó atual segue-se para a direita dele (pois à direita
//só tem números maiores), se não segue-se à esquerda, pois ao contrário da direita, a esquerda só tem números menores
int consulta_ArvAVL(ArvAVL *raiz, int valor){
    if(raiz == NULL)
        return 0;
    struct NO* atual = *raiz;
    while(atual != NULL){
        if(valor == atual->pessoa.id){
            return 1;
        }
        if(valor > atual->pessoa.id)
            atual = atual->dir;
        else
            atual = atual->esq;
    }
    return 0;
}

//rotação simples à direita. É realizada quando o nó filho(B) da subarvore em que ocorreu a inserção em relação ao nó que está desbalanceado (A)
//tem o mesmo sinal do nó desbalanceado.A rotação é realizada no sentido direito
//o nó 'B' apontará par aonde o 'A' está apontando na direita
//o lado esquerdo do nó 'A' vai apontar para o direito do nó 'B'
//o nó direito do 'B' vai apontar para o 'A'
//depois são atualizadas as alturas de cada nó
void RotacaoLL(ArvAVL *A){//LL
    printf("RotacaoLL\n");
    struct NO *B;
    B = (*A)->esq;
    (*A)->esq = B->dir;
    B->dir = *A;
    (*A)->altura = maior(altura_NO((*A)->esq),altura_NO((*A)->dir)) + 1;
    B->altura = maior(altura_NO(B->esq),(*A)->altura) + 1;
    *A = B;
}

//rotação simples à esquerda. A rotação é par ao lado esquerdo, nesse caso a ideia é a mesma do item anterior, só muda o sentido da rotação, ou seja,
//o nó que recebe o esquerdo de outro vai receber o direito e vice-versa
void RotacaoRR(ArvAVL *A){//RR
    printf("RotacaoRR\n");
    struct NO *B;
    B = (*A)->dir;
    (*A)->dir = B->esq;
    B->esq = (*A);
    (*A)->altura = maior(altura_NO((*A)->esq),altura_NO((*A)->dir)) + 1;
    B->altura = maior(altura_NO(B->dir),(*A)->altura) + 1;
    (*A) = B;
}

//rotação dupla à direita. Dessa forma é necessária uma rotação à esquerda e outra a direita. Se isso fosse realizado em uma única função 
//seria necessário a útilização de outro ponteiro 'C' para referenciação, porém nesse caso existem duas função que estão sendo utilizadas para otimizar o código
void RotacaoLR(ArvAVL *A){//LR
    RotacaoRR(&(*A)->esq);
    RotacaoLL(A);
}

//rotação dupla á esquerda. primeiro é realizada uma rotação à direita seguida de outra à esquerda
void RotacaoRL(ArvAVL *A){//RL
    RotacaoLL(&(*A)->dir);
    RotacaoRR(A);
}

//insere um novo nó na árvore
int insere_ArvAVL(ArvAVL *raiz, char* nome, char* sobrenome, char* cpf){
    int res;
    if(*raiz == NULL){
        struct NO *novo;
        novo = (struct NO*)malloc(sizeof(struct NO));
        if(novo == NULL)
            return 0; 

        //dados de cada pessoa
		novo->pessoa.id = id;
		strcpy(novo->pessoa.cpf, cpf);
		strcpy(novo->pessoa.nome, nome);
		strcpy(novo->pessoa.sobrenome, sobrenome);
		
        novo->altura = 0;
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo;
       
        return 1;
    }
	
	//verifica se o valor do nó atual é maior do que o nó em questão, inserindo um novo nó e realizando a/as rotações necessárias 
	//caso seja necessário, com base na função 'fatorBalanceamento'
    struct NO *atual = *raiz;
    if(id < atual->pessoa.id){
        if((res = insere_ArvAVL(&(atual->esq), nome, sobrenome, cpf)) == 1){
            if(fatorBalanceamento_NO(atual) >= 2){
                if(id < (*raiz)->esq->pessoa.id ){
                    RotacaoLL(raiz);
                }else{
                    RotacaoLR(raiz);
                }
            }
        }
        
    //possui a mesma ideia da parte de cima, porém realiza a operação se o valor atual é menor do que o nó em questão
    }else{
        if(id > atual->pessoa.id){
            if((res = insere_ArvAVL(&(atual->dir), nome, sobrenome, cpf)) == 1){
                if(fatorBalanceamento_NO(atual) >= 2){
                    if((*raiz)->dir->pessoa.id < id){
                        RotacaoRR(raiz);
                    }else{
                        RotacaoRL(raiz);
                    }
                }
            }
        }else{
            printf("Valor duplicado!!\n");
            return 0;
        }
    }

    atual->altura = maior(altura_NO(atual->esq),altura_NO(atual->dir)) + 1;

    return res;
}

//fução que procura o menor nó com relação ao nó que foi passado. Como a árvore está balanceada e sabendo que a cada elemento à esquerda do nó atual o campo 'valor'
//diminui, o menor nó será o último, ou seja, o que apontar para NULL
struct NO* procuraMenor(struct NO* atual){
    struct NO *no1 = atual;
    struct NO *no2 = atual->esq;
    while(no2 != NULL){
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

//remove o nó que tiver um valor procurado
int remove_ArvAVL(ArvAVL *raiz, int valor){
	if(*raiz == NULL){//valor não existe
	    printf("valor nao existe!!\n");
	    return 0;
	}
	
	//se o valor for menor do que o procurado, segue-se pela árvora à esquerda, que tem apenas números menores
    int res;
	if(valor < (*raiz)->pessoa.id){
	    if((res = remove_ArvAVL(&(*raiz)->esq,valor)) == 1){
            if(fatorBalanceamento_NO(*raiz) >= 2){
                if(altura_NO((*raiz)->dir->esq) <= altura_NO((*raiz)->dir->dir))
                    RotacaoRR(raiz);
                else
                    RotacaoRL(raiz);
            }
	    }
	}
	
	//se o valor for maior do que o procurado, segue-se pela árvora à direita, que tem apenas números maiores
	if((*raiz)->pessoa.id < valor){
	    if((res = remove_ArvAVL(&(*raiz)->dir, valor)) == 1){
            if(fatorBalanceamento_NO(*raiz) >= 2){
                if(altura_NO((*raiz)->esq->dir) <= altura_NO((*raiz)->esq->esq) )
                    RotacaoLL(raiz);
                else
                    RotacaoLR(raiz);
            }
	    }
	}
	
	//sse o valor for igual ao procurado...
	if((*raiz)->pessoa.id == valor){
		//analisa de o nó em questão sem ao menos um filho, se tiver ele é limpado da memória
	    if(((*raiz)->esq == NULL || (*raiz)->dir == NULL)){
			struct NO *oldNode = (*raiz);
			if((*raiz)->esq != NULL)
                *raiz = (*raiz)->esq;
            else
                *raiz = (*raiz)->dir;
			free(oldNode);
		//caso não haja nenhum filho
		}else { 
			struct NO* temp = procuraMenor((*raiz)->dir);
			(*raiz)->pessoa.id = temp->pessoa.id;
			remove_ArvAVL(&(*raiz)->dir, (*raiz)->pessoa.id);
            if(fatorBalanceamento_NO(*raiz) >= 2){
				if(altura_NO((*raiz)->esq->dir) <= altura_NO((*raiz)->esq->esq))
					RotacaoLL(raiz);
				else
					RotacaoLR(raiz);
			}
		}
		if (*raiz != NULL)
            (*raiz)->altura = maior(altura_NO((*raiz)->esq),altura_NO((*raiz)->dir)) + 1;
		return 1;
	}

	(*raiz)->altura = maior(altura_NO((*raiz)->esq),altura_NO((*raiz)->dir)) + 1;

	return res;
}

////////////////////////////////////////////////FUNÇÕES EXTRAS ////////////////////////////////////////////////

//inicializa um grafo - reservar um espaço na memória equivamente ao número 'n' passado
void inicia_grafo(Grafo *g, int n){
	//variáveis que 
	int i, j;
	g->n = n;
	g->adj = (int**)malloc(n * sizeof(int *));
	//percorre cada vetor de inteiros que será apontado por 'adj'
	for(i=0; i<n ; i++){
		g->adj[i] = (int*)malloc(n * sizeof(int)); //aloca um espaço na memória para cada vetopr de inteiros igual ao número de elementos
	}
	//percorre cada posição de cada vetor de inteiros, preenchendo com '0'
	for(i=0; i<n ; i++){
		for(j=0; j<n; j++){
			g->adj[i][j] = 0;
		}
	}
}

//função para criar as amizades - recebe um ponteiro para o grafo inicializado
void adiciona_amizade(Grafo *g, ArvAVL *raiz){
	int j;
	int id;
	int n_amigos; //numero de amigos da pessoa
	int id_amigos[100];
	int id_temp; //armazena o ID do amigo para verificar se ele existe na árvore, antes de coloca-lo no vetor de amigos que serão adicionados
	int verifica; //recebe o retorno da função 'consulta_ArvAVL'
	int numero_nos; //recebe o núemero de nós existentes na árvore com a função 'totalNO_ArvAVL'
	
	//verifica se a raiz passada é nula
	if(raiz == NULL){
		printf("Erro!");
		system("pause");
        return;
	}
	
	//recebe o id da pessoa que terá amigos adicionados
	fflush(stdin);
	printf("Digite o ID da pessoa que quer adicionar amizades \n");
	scanf("%d", &id);
	verifica = consulta_ArvAVL(raiz, id);
	if(verifica == 0){
		printf("\nID nao corresponde a nenhuma pessoa cadastrada! \n\n");
		system("pause");
		system("cls");
		return;
	}
	
	printf("Digite o numero de amigos que a pessoa com ID %d tera... \n", id);
	scanf("%d", &n_amigos);
	//verifica se o número de pessoas adicionadas existe na árvore
	numero_nos = totalNO_ArvAVL(raiz);
	if(numero_nos <= n_amigos){
		printf("\nErro! nao e possivel adicionar %d amigos, pois existem %d pessoas a mais cadastradas! \n\n", n_amigos, numero_nos-1);
		system("pause");
		system("cls");
		return;
	}
		
	for(j=0; j<n_amigos; j++){
		printf("Digite o ID do amigo %d: ", j+1);
		scanf("%d", &id_temp);
		
		verifica = consulta_ArvAVL(raiz, id_temp);
		if(verifica == 0){
			printf("\nID nao corresponde a nenhuma pessoa cadastrada! \n\n");
			system("pause");
			system("cls");
			return;
		//verifica se o id do amigo a ser adicionado é da pesosa que receberá amizades
		}else if(id_temp == id){
			printf("\nErro, nao e possivel alguem ser amigo de si mesmo! \n\n");
			system("pause");
			system("cls");
			return;
		}else{ //se o ID do amigo existir, ele será adicionado no vetor que contém os ID's das pessoas que serão adicionadas como amigos
			id_amigos[j] = id_temp;
		}
	}
	
	//adiciona amizades no grafo
	for(j=0; j<n_amigos;j++){
		g->adj[id][id_amigos[j]] = 1; //adiciona um nos vértices que representma amizade
		
		g->adj[id_amigos[j]][id] = 1;//adiciona simetria na matriz de adjacências
	}
	
	printf("Cadastro de amigos concluido com sucesso... \n\n");
	system("pause");
	system("cls");
	
}

//função para imprimir amigos de alguém
void imprimi_amigos(Grafo *g, ArvAVL *raiz){
	int id;
	int j;
	int numero_nos; //recebe o número de nós da árvore
	
	//verifica se há pessoas cadastradas na árvore
	numero_nos = totalNO_ArvAVL(raiz);
	if(numero_nos > 0){
		//recebe o id da pessoa para listar seus amigos
		fflush(stdin);
		printf("Qual e o id da pessoa que deseja listar os amigos?\n");
		scanf("%d", &id);
		
		//percorre todas as posições do grafo em uma linha específica, procurando por amizades
		for(j=0; j<100; j++){
			//se houver amizade...
			if(g->adj[id][j] == 1){
				exibe_id(raiz,j); //passa a raiz da árvore e o id do amigo que será impresso
			}
		}
		
	}else{
		printf("Nao e possivel listar amigos, pois nao ha pessoas cadastradas!");
		system("pause");
		system("cls");
	}
	
}

//função que exibe os dados da pessoa de acordo com o id recebido
void exibe_id(ArvAVL *raiz, int id_amigo){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        exibe_id(&((*raiz)->esq), id_amigo);
        
        //exibe apenas a informação do amigo recebido
        if((*raiz)->pessoa.id == id_amigo){
	        printf("Posicao: %d \n",(*raiz)->pessoa.id);
	        printf("CPF: %s \n",(*raiz)->pessoa.cpf);
	        printf("Nome: %s \n",(*raiz)->pessoa.nome);
	        printf("Sobrenome: %s \n",(*raiz)->pessoa.sobrenome);
	        printf("\n");
        }
        
        exibe_id(&((*raiz)->dir), id_amigo);
    }
}

//função que exibe amigos dos amigos 
void exibe_amigos_dos_amigos(Grafo *g, ArvAVL *raiz){
	int id_pessoa;
	int p_amigo; //auxiliar para percorrer a linha no grafo da pessoa especificada
	int s_amigo;
	int n_resultados=0;
	bool teste=false; //verificca se a mensagem da linha 641 já foi exibida, para que não seja exibida a cada iteração do 2° 'for'
	
	//recebe o id da pessoa
	fflush(stdin);
	printf("Quer saber os amigos dos amigos de qual pessoa? (Digite o ID):\n");
	scanf("%d", &id_pessoa);
	
	//percorre cada elemento do grafo para achar os amigos dessa pessoa
	for(p_amigo=0; p_amigo<100; p_amigo++){
		//verifica se existe amizade
		if(g->adj[id_pessoa][p_amigo] == 1){
			teste=true;
			//verifica amizades do amigo
			for(s_amigo=0; s_amigo<100; s_amigo++){
				//verifica se existe amizade do segundo amigo
				if(g->adj[p_amigo][s_amigo] == 1){
					if(teste == true){
						printf("AMIGOS DA PESSOA COM ID %d ...\n", p_amigo);
					}
					exibe_id(raiz,s_amigo);
					n_resultados++;
					teste=false;
				}
			}
		}
	}
	if(n_resultados > 0){
		printf("\nEXISTEM %d AMIGOS DE AMIGOS DESSA PESSOA!\n", n_resultados);
	}else{
		printf("\nOS AMIGOS DESSA PESSOA NAO POSSUEM AMIZADES!\n");
	}
}

