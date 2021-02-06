//Julio Hebert Silva
//Tomaz Miranda de Oliveira
#include "substituicao.h"

unsigned Valor_S(int tam_page){
	int s, tmp;
	int page;
	/* Derivar o valor de s: */
	tmp = tam_page;
	s = 0;
	while (tmp>1) {
		tmp = tmp>>1;
		s++;
	}
    return s;
}

int LRU(Lista_se *lista){     
    if(lista->ultimo != NULL){
        Celula *tmp = lista->ultimo;
        lista->ultimo = tmp->ant;
        lista->ultimo->prox = NULL;
        int addr = tmp->addr;
        lista->tamanho--;
        free(tmp);
        return addr;
    }
    return 0;
}

int NRU(Hash tab){ 
    int i = 0;
    Dados* escolha = NULL;
    Dados* tmp;
    for( i = 0 ; i < tam; i++){
        if(tab[i] != NULL){
            if(tab[i]->prox == NULL){
                if(escolha == NULL){
                    escolha = tab[i];
                }else if(tab[i]->referenciada == 0 && tab[i]->alterada == 0){
                    escolha = tab[i];
                    return escolha->addr;
                }else if(tab[i]->referenciada == 0 && tab[i]->alterada == 1){
                    if(escolha->referenciada == 1){
                        escolha = tab[i];
                    }
                }else if(tab[i]->referenciada == 1 && tab[i]->alterada == 0){
                    if(escolha->referenciada == 1 && escolha->alterada == 1){
                        escolha = tab[i];
                    }
                }
            }else{
                tmp = tab[i];
                do{
                    if(escolha == NULL){
                        escolha = tmp;
                    }else if(tmp->referenciada == 0 && tmp->alterada == 0){
                        escolha = tmp;
                        return escolha->addr;
                    }else if(tmp->referenciada == 0 && tmp->alterada == 1){
                        if(escolha->referenciada == 1){
                            escolha = tmp;
                        }
                    }else if(tmp->referenciada == 1 && tmp->alterada == 0){
                        if(escolha->referenciada == 1 && escolha->alterada == 1){
                            escolha = tmp;
                        }
                    }
                    tmp = tmp->prox;
                }while(tmp != NULL);
            }
        }
    }
    return escolha->addr;
}

void zerarR( Hash tab, int tempo){
    for(int i = 0; i < tam ; i++){
        Dados* tmp = tab[i];
        if(tmp != NULL){
            do{
                if(tempo - tmp->acesso > 50){
                    tmp->referenciada = 0;
                }
                tmp = tmp->prox;
            }while(tmp!=NULL);
        }
    }
}

int segunda_Chance(Hash tab, int *lista, int **index, int qnt_page){
    do{
        int chave = funcaoHash(lista[**index]);
        if(tab[chave]->addr == lista[**index]){
            if(tab[chave]->referenciada == 1){
                tab[chave]->referenciada = 0;
            }else{
                return tab[chave]->addr;
            }
        }else if(tab[chave]->prox !=NULL){
            Dados* tmp = tab[chave]->prox;
            while(tmp != NULL){
                if( tmp->addr == lista[**index]){
                    if(tmp->referenciada == 1){
                        tmp->referenciada = 0;
                        break;
                    }else{
                        return tmp->addr;
                    }
                }else{
                    tmp = tmp->prox;
                }
            }
        }
        **index = **index +1;
        if( **index >= qnt_page){
            **index = 0;
        }
    }while(1);
    return 0;
}

void inicializaLista(Lista_se *lista){   
	lista->prim = NULL;
	lista->ultimo = NULL;
	lista->tamanho = 0;
}

int insere(Lista_se *lista, int addr){	   
	if(lista->prim ==  NULL){
		Celula *c = (Celula *) malloc(sizeof(Celula));
		c->addr = addr;
		c->prox = NULL;
        c->ant = NULL;
		lista->prim = c;
		lista->ultimo = c;
		lista->tamanho++;
        return 1;
	}else{
		Celula *c = (Celula *) malloc(sizeof(Celula));
		c->addr = addr;
		c->prox = lista->prim;
        c->ant = NULL ;
        lista->prim->ant = c;
        lista->prim = c ;
		lista->tamanho++;        
        return 1;
	}
    return 0;
}

int remove_celula(Lista_se *lista, int addr){				
	Celula *tmp = lista->prim;
	Celula *aux = lista->prim;
	if(lista->prim !=  NULL){
        while(tmp!=NULL && tmp->addr!=addr){
            aux=tmp;
            tmp =tmp->prox;
        }
        if(tmp==NULL){ // nao existe addr na lista
            //printf("ERRO: addr INEXISTENTE\n");
            return 0;
        }else{            
			if(tmp->prox == NULL){
				lista->ultimo = aux;
                aux->prox = NULL;
            }else if(tmp->ant == NULL){
                lista->prim = tmp->prox;
                if(lista->prim != NULL){
                    lista->prim->ant = NULL;
                }
            }else{
                aux->prox = tmp->prox;
                tmp->prox->ant = aux;
            }
			free(tmp);
			lista->tamanho--;
			return 1;
		}
	}else{ // lista vazia
        return 1;
    }
}

void saida(char *alg, char *nome_file, int tam_mem, int tam_page, int lidas_page, int suja_page, int tempo){
        printf("Arquivo de estrada: %s\n", nome_file);
        printf("\nTamanho da memoria: %d KB\n", tam_mem);
        printf("Tamanho das paginas: %d KB\n", tam_page);
        printf("Tecnica de reposicao: %s\n", alg);
        printf("\nPaginas lidas: %d\n", lidas_page);
        printf("Paginas escritas: %d\n", suja_page);
        printf("Chamadas de pagina: %d\n", tempo);
}

int valor_entrada(int argc, char **argv, char* alg, char* nome_file, int *tam_page, int *tam_mem){ 
    if(argc < 5){
		printf("Erro. Numero de argumentos insuficientes\n");
		return 0;
	}
	strcpy( alg, argv[1]);
    if(strcmp(alg, "lru") && strcmp(alg, "nru") && strcmp(alg, "segunda_chance")){
        printf("Erro: Algoritimos deve ser lru ou nru ou segunda_chance\n");
        return 0;
    }

	strcpy( nome_file, argv[2] );

	*tam_page = atoi( argv[3] );
	if(*tam_page >=2 && *tam_page <= 64){
		*tam_page = *tam_page ; //* 1024;
	}else{
		printf("Erro: O tamanho da pagina deve ser um valor entre 2 e 64.\n");
		return 0;
	}

	*tam_mem = atoi( argv[4] );
	if(*tam_mem >= 128 && *tam_mem <= 16384 ){
		*tam_mem = *tam_mem ; //* 1024;
	}else{
		printf("Erro: O tamanho da memoria fisica deve ser um valor entre 128 e 16384.\n");
		return 0;
	}
    return 1;
}

void inicializaHash(Hash tab){
    int i;
    for(i = 0; i < tam; i++){
        tab[i] = NULL;
    }
}

int funcaoHash(int addr){ 
  return (addr % tam);
}

void removeHash(Hash tab, int addr, int **suja_page){
    int pos = funcaoHash(addr);
    int ex = addr;    
    if(tab[pos]->addr == ex){
        if(tab[pos]->prox == NULL){
            if(tab[pos]->alterada == 1){
                **suja_page = **suja_page +1;
            }
            tab[pos] = NULL;
            return;
        }
        if(tab[pos]->prox != NULL){
            if(tab[pos]->alterada == 1){
                **suja_page = **suja_page +1;
            }
            Dados *tmp = tab[pos];
            tab[pos] = tab[pos]->prox;
            free(tmp);
            return;
        }
    }else{
        if(tab[pos]->addr != ex){
            if(tab[pos]->prox == NULL){ // nao encontrou ... algum erro ??? , achou a pos na hash mas lista nula
                return;
            }else{
                Dados* ant = NULL;
                Dados* aux = tab[pos]->prox;
                while(aux->prox != NULL  && aux->addr != ex){
                    ant = aux;
                    aux = aux->prox;
                }
                if(aux->addr != ex){ // nao encontrou ... algum erro ??? , achou a pos na hash mas nao encontrou na lista
                    return;
                }else{
                    if(ant == NULL){
                        tab[pos]->prox = aux->prox;
                    }else{
                        ant->prox = aux->prox;
                    }
                    if(aux->alterada == 1){
                        **suja_page = **suja_page +1;
                    }
                    aux = NULL;
                    free(aux);
                }
            }
        }
    }   
}

void insereHash(Hash tab, char *alg, int addr, int tempo, char *rw, int *lidas_page, int qnt_page, int *suja_page, int *list, int *index, Lista_se *lista){ 
    int i = 0;
    int chave = funcaoHash(addr);
    Dados* aux = tab[chave];
    if(aux!=NULL)
        if(aux->addr!=addr)
            do{
                aux=aux->prox;
            }while(aux!=NULL && aux->addr!=addr);
    if(aux == NULL){
        if(qnt_page <= *lidas_page){
            if(strcmp( alg, "nru") == 0){
                removeHash(tab, NRU(tab), &suja_page); 
            }else if(strcmp( alg, "segunda_chance") == 0){
                removeHash(tab, segunda_Chance(tab, list, &index, qnt_page), &suja_page); 
            }else if(strcmp( alg, "lru" ) == 0){
                removeHash(tab, LRU(lista), &suja_page); 
            }
        }
        if(strcmp( alg, "segunda_chance") == 0){
            list[*index] = addr;
            *index = *index +1;
            if(*index>=qnt_page){
                *index =0 ;
            }
        }else if( strcmp ( alg, "lru") == 0){
            if(! insere(lista, addr)){
                printf("ERRO AO ADD NA LISTA\n");
                exit(1);
            }
        }
        aux = (Dados*)malloc(sizeof(Dados));
        *lidas_page = *lidas_page +1;
        aux->addr = addr;
        aux->prox = tab[chave];
        if( strcmp( rw, "W") == 0){
            aux->alterada = 1;
            aux->referenciada = 1;  suja_page++;
            aux->acesso = tempo;
        }else if( strcmp( rw, "R") == 0){
            aux->referenciada = 1;
            aux->acesso = tempo;
        }
        tab[chave] = aux;
    }else if(aux->addr == addr){
        if( strcmp ( alg, "lru") == 0){
            if(remove_celula(lista, addr)){
                if(!insere(lista, addr)){
                    printf("ERRO AO ADD NA LISTA\n");
                    exit(1);
                }
            }else{
                printf("ERRO AO REMOVER DA LISTA\n");
                exit(1);
            }
        }
        if( strcmp( rw, "W") == 0){
            aux->alterada = 1;
            aux->referenciada = 1;  suja_page++;
            aux->acesso = tempo;
        }else if( strcmp( rw, "R") == 0){
            aux->referenciada = 1;
            aux->acesso = tempo;
        }
    }
}
