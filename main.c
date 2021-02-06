//Julio Hebert Silva
//Tomaz Miranda de Oliveira
#include "substituicao.h"

int main(int argc, char **argv){
    clock_t Ticks[2];
    Ticks[0] = clock();
    char ta_bugando_alg[100];
    char nome_file[100];
    int tam_page, tam_mem;
    if(! valor_entrada(argc, argv, ta_bugando_alg, nome_file, &tam_page, &tam_mem)){
        return 0;
    }else{
        int qnt_page = tam_mem / tam_page; 
        unsigned addr;
        char rw[1];
        FILE *file = fopen(nome_file, "r");  
        if(file != NULL){
            char alg[100];
            if( strcmp( "nru", ta_bugando_alg ) == 0 ){
                printf("\n\nExecutando simulador...\n\n");
                strcpy( alg, "nru");
                Hash tab;
                int suja_page = 0;
                int lidas_page = 0;
                int tempo = -1;
                int tempo2 = -1;
                inicializaHash(tab);
                unsigned s = Valor_S(tam_page);
                while(!feof(file)){
                    fscanf(file,"%x %s", &addr, rw);
                    rw[1] = '\0';
                    int page = addr >> s; // arrumando o endereço segundo especificação 
                    tempo++;
                    tempo2++;
                    insereHash(tab, alg, page, tempo, rw, &lidas_page, qnt_page, &suja_page, NULL, 0, NULL);   
                    if(tempo2 == 100){ // a cada 100 linhas resetar valor de R psuja_page as paginas que a diferença do tempo de ultimo acesso - tempo seja maior que 50
                        zerarR(tab, tempo);
                        tempo2 = 0;
                    }            
                }
                saida(alg, nome_file, tam_mem, tam_page, lidas_page, suja_page, tempo);
            }else if( strcmp( ta_bugando_alg, "lru") == 0){
                printf("\n\nExecutando simulador...\n\n");
                strcpy( alg, "lru");
                Hash tab;
                int suja_page = 0;
                int lidas_page = 0;
                int tempo = -1;
                inicializaHash(tab);
                unsigned s = Valor_S(tam_page);
                Lista_se lista;
                inicializaLista(&lista);
                while(!feof(file)){
                    fscanf(file,"%x %s", &addr, rw);
                    rw[1] = '\0';
                    int page = addr >> s; // arrumando o endereço segundo especificação 
                    tempo++;
                    insereHash(tab, alg, page, tempo, rw, &lidas_page, qnt_page, &suja_page, NULL, 0, &lista);           
                }
                saida(alg, nome_file, tam_mem, tam_page, lidas_page, suja_page, tempo);
            }else if( strcmp( ta_bugando_alg, "segunda_chance") == 0){
                printf("\n\nExecutando simulador...\n\n");
                int* list = (int*)malloc(qnt_page * sizeof(int));
                int index = 0;
                strcpy( alg, "segunda_chance");
                Hash tab;
                int suja_page = 0;
                int lidas_page = 0;
                int tempo = -1;
                inicializaHash(tab);
                unsigned s = Valor_S(tam_page);
                while(!feof(file)){
                    fscanf(file,"%x %s", &addr, rw);
                    rw[1] = '\0';
                    int page = addr >> s; // arrumando o endereço segundo especificação 
                    tempo++;
                    insereHash(tab, alg, page, tempo, rw, &lidas_page, qnt_page, &suja_page, list, &index, NULL); 
                }
                saida(alg, nome_file, tam_mem, tam_page, lidas_page, suja_page, tempo);
            }
            fclose(file);
        }else{
            printf("\nErro ao abrir aquivo %s\n", nome_file);
            exit(1);
        }
    }
    Ticks[1] = clock();
    double Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %g ms.\n\n", Tempo);
    return 0;
}-