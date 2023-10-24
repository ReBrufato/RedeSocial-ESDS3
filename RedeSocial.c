#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//defini��o de fun��es
typedef struct NO* ArvAVL;

//strutura da pessoa
typedef struct pessoa {
	int id;
	char cpf[12];
	char nome[200];
	char sobrenome[200];
} pessoa;

//estrutura que armazena um n�/�rvore
struct NO{
    pessoa pessoa;
    int altura;
    struct NO *esq;
    struct NO *dir;
};

//estrutura do grafo
typedef struct Grafo{
	int **adj; //ponteiro para um ponteiro de inteiros, que ir� referenciar cada vetor(linha) de inteiros da matriz
	int n; //n�mero m�ximo de elementos do grafo	
}Grafo;

//c�digos da �rvore AVL
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

//fun��es extras
void inicia_grafo(Grafo *g, int); //recebe um ponteiro para um grafo e o n�mero de elementos a ser setado
void adiciona_amizade(Grafo *grafo, ArvAVL *raiz);
void imprimi_amigos(Grafo *g, ArvAVL *raiz);
void exibe_id(ArvAVL *raiz, int);
void exibe_amigos_dos_amigos(Grafo *g, ArvAVL *raiz);

//vari�vel que controla o n�mero de pessoas cadastradas na �rvore
int id = 0;

//Fun��o principal
int main(){
	//inicializa o grafo com 100 posi��es
	Grafo *grafo;
	grafo = malloc(sizeof(Grafo));
	inicia_grafo(grafo, 100);
	
	//cria um ponteiro para um ponteiro para uma struct NO
    ArvAVL* avl;
	
	//raiz da arvore AVL
    avl = cria_ArvAVL(); 
	
	//vari�vel de controle do primeiro la�o
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
			
			//pega as informa��es de entrada para a pessoa a ser cadastrada
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

////////////////////////////////////////////////FUN��ES ARVORE AVL ////////////////////////////////////////////////


//cria um ponteiro para a raiz da �rvore, que foi alocada dinamicamente na mem�ria
ArvAVL* cria_ArvAVL(){
    ArvAVL* raiz = (ArvAVL*) malloc(sizeof(ArvAVL));
    if(raiz != NULL)
        *raiz = NULL;
    return raiz;
}

//destro� a �rvore limpando cada espa�o de mem�ria dos n�s criados
void libera_NO(struct NO* no){
    if(no == NULL)
        return;
    libera_NO(no->esq);
    libera_NO(no->dir);
    free(no);
    no = NULL;
}

//limpa a �rvore, e passa para a fun��o 'libera_NO' a raiz da �rvore
void libera_ArvAVL(ArvAVL* raiz){
    if(raiz == NULL)
        return;
    libera_NO(*raiz);
    free(raiz);
}

//retorna a altura de um n� em quest�o, se for nullo ele n�o ter� altura, que ser� tratado como '-1'
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

//verifica se a �rvore est� sem raiz, ou seja, se ela est� nula. No caso de sim significa que a �rvore n�o existe, ou seja,e st� vazia
int estaVazia_ArvAVL(ArvAVL *raiz){
    if(raiz == NULL)
        return 1;
    if(*raiz == NULL)
        return 1;
    return 0;
}

//retorna o n�mero de n�s da �rvore, executando a soma dos n�s da sub�rvore da esquerda e da direita.
//Dessa forma a conta � realizada de baixo para cima atrav�s da recurs�o, ou seja, no sentido das folhas para a raiz
int totalNO_ArvAVL(ArvAVL *raiz){
    if (raiz == NULL)
        return 0;
    if (*raiz == NULL)
        return 0;
    int alt_esq = totalNO_ArvAVL(&((*raiz)->esq));
    int alt_dir = totalNO_ArvAVL(&((*raiz)->dir));
    return(alt_esq + alt_dir + 1);
}

//executa uma fun��o recursiva que retorna a altura da sub�rvore maior, ou seja, a altura da �rvore
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

//exibe o valor e a altura de cada n�
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

//exibe o valor de cada n�, sua altura e o fator de balanceamento
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

//exibe o valor de cada n�
void posOrdem_ArvAVL(ArvAVL *raiz){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        posOrdem_ArvAVL(&((*raiz)->esq));
        posOrdem_ArvAVL(&((*raiz)->dir));
        printf("%d\n",(*raiz)->pessoa.id);
    }
}


//procura um valor na �rvore. Se o valor for encontrado � retornado '1', se n�o � retornado '0'. Se o valor for maior do que o do n� atual segue-se para a direita dele (pois � direita
//s� tem n�meros maiores), se n�o segue-se � esquerda, pois ao contr�rio da direita, a esquerda s� tem n�meros menores
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

//rota��o simples � direita. � realizada quando o n� filho(B) da subarvore em que ocorreu a inser��o em rela��o ao n� que est� desbalanceado (A)
//tem o mesmo sinal do n� desbalanceado.A rota��o � realizada no sentido direito
//o n� 'B' apontar� par aonde o 'A' est� apontando na direita
//o lado esquerdo do n� 'A' vai apontar para o direito do n� 'B'
//o n� direito do 'B' vai apontar para o 'A'
//depois s�o atualizadas as alturas de cada n�
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

//rota��o simples � esquerda. A rota��o � par ao lado esquerdo, nesse caso a ideia � a mesma do item anterior, s� muda o sentido da rota��o, ou seja,
//o n� que recebe o esquerdo de outro vai receber o direito e vice-versa
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

//rota��o dupla � direita. Dessa forma � necess�ria uma rota��o � esquerda e outra a direita. Se isso fosse realizado em uma �nica fun��o 
//seria necess�rio a �tiliza��o de outro ponteiro 'C' para referencia��o, por�m nesse caso existem duas fun��o que est�o sendo utilizadas para otimizar o c�digo
void RotacaoLR(ArvAVL *A){//LR
    RotacaoRR(&(*A)->esq);
    RotacaoLL(A);
}

//rota��o dupla � esquerda. primeiro � realizada uma rota��o � direita seguida de outra � esquerda
void RotacaoRL(ArvAVL *A){//RL
    RotacaoLL(&(*A)->dir);
    RotacaoRR(A);
}

//insere um novo n� na �rvore
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
	
	//verifica se o valor do n� atual � maior do que o n� em quest�o, inserindo um novo n� e realizando a/as rota��es necess�rias 
	//caso seja necess�rio, com base na fun��o 'fatorBalanceamento'
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
        
    //possui a mesma ideia da parte de cima, por�m realiza a opera��o se o valor atual � menor do que o n� em quest�o
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

//fu��o que procura o menor n� com rela��o ao n� que foi passado. Como a �rvore est� balanceada e sabendo que a cada elemento � esquerda do n� atual o campo 'valor'
//diminui, o menor n� ser� o �ltimo, ou seja, o que apontar para NULL
struct NO* procuraMenor(struct NO* atual){
    struct NO *no1 = atual;
    struct NO *no2 = atual->esq;
    while(no2 != NULL){
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

//remove o n� que tiver um valor procurado
int remove_ArvAVL(ArvAVL *raiz, int valor){
	if(*raiz == NULL){//valor n�o existe
	    printf("valor nao existe!!\n");
	    return 0;
	}
	
	//se o valor for menor do que o procurado, segue-se pela �rvora � esquerda, que tem apenas n�meros menores
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
	
	//se o valor for maior do que o procurado, segue-se pela �rvora � direita, que tem apenas n�meros maiores
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
		//analisa de o n� em quest�o sem ao menos um filho, se tiver ele � limpado da mem�ria
	    if(((*raiz)->esq == NULL || (*raiz)->dir == NULL)){
			struct NO *oldNode = (*raiz);
			if((*raiz)->esq != NULL)
                *raiz = (*raiz)->esq;
            else
                *raiz = (*raiz)->dir;
			free(oldNode);
		//caso n�o haja nenhum filho
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

////////////////////////////////////////////////FUN��ES EXTRAS ////////////////////////////////////////////////

//inicializa um grafo - reservar um espa�o na mem�ria equivamente ao n�mero 'n' passado
void inicia_grafo(Grafo *g, int n){
	//vari�veis que 
	int i, j;
	g->n = n;
	g->adj = (int**)malloc(n * sizeof(int *));
	//percorre cada vetor de inteiros que ser� apontado por 'adj'
	for(i=0; i<n ; i++){
		g->adj[i] = (int*)malloc(n * sizeof(int)); //aloca um espa�o na mem�ria para cada vetopr de inteiros igual ao n�mero de elementos
	}
	//percorre cada posi��o de cada vetor de inteiros, preenchendo com '0'
	for(i=0; i<n ; i++){
		for(j=0; j<n; j++){
			g->adj[i][j] = 0;
		}
	}
}

//fun��o para criar as amizades - recebe um ponteiro para o grafo inicializado
void adiciona_amizade(Grafo *g, ArvAVL *raiz){
	int j;
	int id;
	int n_amigos; //numero de amigos da pessoa
	int id_amigos[100];
	int id_temp; //armazena o ID do amigo para verificar se ele existe na �rvore, antes de coloca-lo no vetor de amigos que ser�o adicionados
	int verifica; //recebe o retorno da fun��o 'consulta_ArvAVL'
	int numero_nos; //recebe o n�emero de n�s existentes na �rvore com a fun��o 'totalNO_ArvAVL'
	
	//verifica se a raiz passada � nula
	if(raiz == NULL){
		printf("Erro!");
		system("pause");
        return;
	}
	
	//recebe o id da pessoa que ter� amigos adicionados
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
	//verifica se o n�mero de pessoas adicionadas existe na �rvore
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
		//verifica se o id do amigo a ser adicionado � da pesosa que receber� amizades
		}else if(id_temp == id){
			printf("\nErro, nao e possivel alguem ser amigo de si mesmo! \n\n");
			system("pause");
			system("cls");
			return;
		}else{ //se o ID do amigo existir, ele ser� adicionado no vetor que cont�m os ID's das pessoas que ser�o adicionadas como amigos
			id_amigos[j] = id_temp;
		}
	}
	
	//adiciona amizades no grafo
	for(j=0; j<n_amigos;j++){
		g->adj[id][id_amigos[j]] = 1; //adiciona um nos v�rtices que representma amizade
		
		g->adj[id_amigos[j]][id] = 1;//adiciona simetria na matriz de adjac�ncias
	}
	
	printf("Cadastro de amigos concluido com sucesso... \n\n");
	system("pause");
	system("cls");
	
}

//fun��o para imprimir amigos de algu�m
void imprimi_amigos(Grafo *g, ArvAVL *raiz){
	int id;
	int j;
	int numero_nos; //recebe o n�mero de n�s da �rvore
	
	//verifica se h� pessoas cadastradas na �rvore
	numero_nos = totalNO_ArvAVL(raiz);
	if(numero_nos > 0){
		//recebe o id da pessoa para listar seus amigos
		fflush(stdin);
		printf("Qual e o id da pessoa que deseja listar os amigos?\n");
		scanf("%d", &id);
		
		//percorre todas as posi��es do grafo em uma linha espec�fica, procurando por amizades
		for(j=0; j<100; j++){
			//se houver amizade...
			if(g->adj[id][j] == 1){
				exibe_id(raiz,j); //passa a raiz da �rvore e o id do amigo que ser� impresso
			}
		}
		
	}else{
		printf("Nao e possivel listar amigos, pois nao ha pessoas cadastradas!");
		system("pause");
		system("cls");
	}
	
}

//fun��o que exibe os dados da pessoa de acordo com o id recebido
void exibe_id(ArvAVL *raiz, int id_amigo){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        exibe_id(&((*raiz)->esq), id_amigo);
        
        //exibe apenas a informa��o do amigo recebido
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

//fun��o que exibe amigos dos amigos 
void exibe_amigos_dos_amigos(Grafo *g, ArvAVL *raiz){
	int id_pessoa;
	int p_amigo; //auxiliar para percorrer a linha no grafo da pessoa especificada
	int s_amigo;
	int n_resultados=0;
	bool teste=false; //verificca se a mensagem da linha 641 j� foi exibida, para que n�o seja exibida a cada itera��o do 2� 'for'
	
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

