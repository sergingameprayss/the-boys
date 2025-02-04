//header dos eventos contendo a struct evento e todas as funcoes que tratam os eventos
#ifndef EVENTOS
#define EVENTOS

struct evento {
  int heroi; //id do heroi do evento

  int local; //id do local do evento
             //pode ser base ou missao
};

struct distancia_base {
  int id; //id da base 
  int dist; //distancia dela ate a o local (missao)
};

//funcao que cria o evento dinamicamente com o id do heroi e local indicados;
struct evento *cria_ev (int heroi, int local);

//seta alguns eventos iniciais, como a chegada de cada herois em bases aletorias
//insere todas as missoes na lef em tempos aletorios de 0 ate fim do mundo
//insere na lef o evento fim do mundo o momento T_FIM_DO_MUNDO
void eventos_iniciais (struct fprio_t *lef);


//simula a lef, conforme o tipo do evento chama a funcao correspondente,
//faz isso ate que o relogio seja fim do mundo entao retorna
//retorna 1 em caso de sucesso e 0 se erro;
int simula_eventos(struct fprio_t *lef, struct mundo *m);

//funcao que trata a chegada do heroi em uma base, decidindo se o heroi 
//espera para ou desiste de entrar na base 
void chega (struct fprio_t *lef, struct mundo *m, struct evento *ev);

//funcao que coloca o heroi na fila de espera da base 
//insere na lef o evento avisa 
void espera(struct fprio_t *lef, struct mundo *m, struct evento *ev);

//funcao que faz o heroi nao esperar para entrar na base eir para outra aleatoria
//insere na lef o evento viaja
void desiste(struct fprio_t *lef, struct mundo *m, struct evento *ev);

//funcao que avisa o porteiro para que ele olhe se ha espaco na base 
//se houver, segue a politica fifo para entrada dos herois presentes na fila
//tirando eles da fila e colocando na base ate a lotacao
//insere na lef o evento entra
void avisa(struct fprio_t *lef, struct mundo *m, struct evento *ev);

//funcao que calcula o tempo de permanencia do heroi na base
//insere na lef o evento sai no tempo calculado
void entra(struct fprio_t *lef, struct mundo *m, struct evento *ev);

//funcao que retira o heroi da base
//insere na lef o evento viaja para uma base aleatoria
//insere na lef o evento avisa para avisar o porteiro que liberou espaco
void sai(struct fprio_t *lef, struct mundo *m, struct evento *ev);

//funcao que calcula a distancia e duracao da viagem do heroi da base 
//que ele esta ate a base destino
//insere na lef o evento chega no tempo pos duracao da viagem
void viaja(struct fprio_t *lef, struct mundo *m, struct evento *ev);

//funcao que troca o status do heroi para morto, retirando ele da base que estava
//insere na lef o evento avisa
void morre(struct fprio_t *lef, struct mundo *m, struct evento *ev);

//funcao que calcula as distancias das basea ate a missao, e escolhe a mais 
//proxima e capacitada para cumprir a missao (ter todas as habilidades requeridas), 
//se nao achar nenhuma base que cumpra a missao, remarca ela para o dia seguinte, 
//se achar, calcula o risco para cada heroi que participou e ou incrementa a 
//experiencia dele ou insere na lef o evento morre
void missao(struct fprio_t *lef, struct mundo *m, struct evento *ev);

//funcao que marca o fim da simulacao
//imprime:
//as estatisticas de cada heroi (id, vivo/morto, atributos...)
//as estatisticas de cada base (id, lotacao, fila maxima...)
//quantidade de eventos tratados
//quantidade de missoes cumpridas
//tentativas por missao
//taxa de mortalidade
void fim(struct mundo *m);

#endif