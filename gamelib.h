#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum Stato_giocatore {astronauta, impostore, assassinato, defenestrato};
enum Nome_Giocatore {rosso=0, blu=1, giallo=2, verde=3, nero=4, fucsia=5, viola=6, celeste=7, bianco=8, marrone=9};
enum Tipo_stanza {vuota, quest_semplice, quest_doppia, botola};
enum boolean { true, false};

 struct Stanza {
  struct Stanza *avanti;
  struct Stanza *destra;
  struct Stanza *sinistra;
  struct Stanza *stanza_precedente;
  enum  Tipo_stanza tipo_stanza;
  enum boolean emergenza_chiamata;
};

 struct Giocatore {
  struct Stanza *stanza_giocatore_momentanea;
  enum Stato_giocatore stato_giocatore;
  enum Nome_Giocatore nome_giocatore;
};

struct Nodo {
  struct Stanza* stanza;
  struct Nodo* prossimo;
};


void imposta_gioco();
void gioca();
void termina_gioco();
