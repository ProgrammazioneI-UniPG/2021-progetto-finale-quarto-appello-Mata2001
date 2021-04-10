#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "gamelib.h"
#include <stdbool.h>

int numero_giocatori = 0;
int totale_quest = 0;
int totale_quest_da_finire = 0;
int conta_botole = 0;
struct Nodo* inizio_lista = NULL;
struct Stanza* stanza_inizio;
struct Giocatore giocatori[10];
static unsigned short quest_da_finire;
static struct Stanza *lista_stanze;
static char* Ottieni_Nome_Giocatore(struct Giocatore giocatore);
static char* Ottieni_Stato_Giocatore(struct Giocatore giocatore);
static void Genera_Array_Nomi(int *nomi_univoci);
static void Genera_Array_Stati(int *stati_validati);
static int genera_stato_stanza();
static void stampa_giocatori(int *stati_validati, int *nomi_univoci);
static void inizia_gioco();
static enum Nome_Giocatore Ottieni_colore_giocatore(int codice_colore);
static void avanza_personaggio();
static struct Stanza* crea_nuova_stanza();
static char *Ottieni_stanza_giocatore(struct Giocatore giocatore);
static void esegui_quest(int giocatore_corrente);
static void uccidi_astronauta(int giocatore_corrente);
static void sabotaggio (int giocatore_corrente);
static void usa_botola(int giocatore_corrente);
static void chiamata_emergenza(int giocatore_corrente, int totale_impostori_stanza, int totale_astronauti_stanza);
static void inserisciNodo(struct Stanza* stanza);
static struct Stanza* scegli_stanza_botola();
static struct Stanza* scegli_altra_stanza();
static int conta_stanze();

void imposta_gioco()
{
  time_t tempo;
  int turno_giocatore;

  printf("Quanti giocatori desideri impostare? Scelta compresa tra 4 e 10: ");
  do {
    scanf("%d", &numero_giocatori);
    if (numero_giocatori < 4 || numero_giocatori > 10) {
      printf("Numero non valido, si prega di scegliere un numero compreso tra 4 e 10\n");
    }
  } while (numero_giocatori < 4 || numero_giocatori > 10);
  do {
    printf("Di quanti turni deve essere la partita? Scegliere tra 5 e 10: ");
    scanf("%d", &turno_giocatore);
    if (turno_giocatore < 5 || turno_giocatore > 10) {
      printf("Numero non valido, si prega di scegliere un numero compreso tra 5 e 10\n");
    }
  } while (turno_giocatore <= 4 || turno_giocatore >= 10);
  do {
    printf("Inserisci il numero di quest che dovranno completare gli astronauti: ");
    scanf("%hd", &quest_da_finire);
    if (quest_da_finire < 1 || quest_da_finire > 10) {
      printf("Numero non valido, si prega di scegliere un numero compreso tra 1 e 10\n");
    }
  } while (quest_da_finire < 1 || quest_da_finire > 10);
  totale_quest_da_finire = quest_da_finire;
  totale_quest = 0;
  conta_botole = 0;

  int nomi_univoci[numero_giocatori];
  Genera_Array_Nomi(nomi_univoci);

  int stati_validati[numero_giocatori];
  Genera_Array_Stati(stati_validati);

  stanza_inizio = crea_nuova_stanza();
  stampa_giocatori(stati_validati, nomi_univoci);

  inizia_gioco();
}

// algoritmo che genera un array di interi univoci da usare come "nomi"
static void Genera_Array_Nomi(int *nomi_univoci)
{
  int nome_candidato;
    int flag;
    for (int i = 0; i < numero_giocatori; i++) {
      do {
        flag = 0;
        // genero il numero che corrisponde al numero in modo random
        nome_candidato = rand() % 10;
        for (int j = 0; j < i; j++) {
          if (nomi_univoci[j] == nome_candidato) {
            // il nome è presente, devo ricominciare :(
            flag = 1;
          }
        }
        if (flag == 0) {
          // il nome non è presente lo salvo nell'array
         nomi_univoci[i] = Ottieni_colore_giocatore(nome_candidato);
        }
      } while (flag == 1);
    }
  }

// algoritmo che genera un array di interi univoci da usare come "stati"
static void Genera_Array_Stati(int *stati_validati)
{
  int stato_candidato;
  int numero_impostori = 0;
  do {
    for (int i = 0; i < numero_giocatori; i++) {
      stato_candidato = rand() % 2;
      if (numero_impostori < 3) {
        if (stato_candidato == 0) {
          stati_validati[i] = 0; // astronauta
        } else {
          numero_impostori++;
          stati_validati[i] = 1; // impostore
        }
      } else {
        // abbiamo raggiunto il numero massimo di impostori
        stati_validati[i] = 0; // astronauta
      }
    }
  } while (numero_impostori == 0); // almeno 1 impostore ci deve essere nel caso genera stati random fino a che non è presente almeno 1 impostore
}

// Ritorna la stringa con il nome del giocatore
static char *Ottieni_Nome_Giocatore(struct Giocatore giocatore)
{
  switch (giocatore.nome_giocatore) {
     case 0: return "Rosso";
     break;
     case 1: return "Blu";
     break;
     case 2: return "Giallo";
     break;
     case 3: return "Verde";
     break;
     case 4: return "Nero";
     break;
     case 5: return "Fucsia";
     break;
     case 6: return "Viola";
     break;
     case 7: return "Celeste";
     break;
     case 8: return "Bianco";
     break;
     case 9: return "Marrone";
     break;
  }
  return "";
}
static enum Nome_Giocatore Ottieni_colore_giocatore(int codice_colore)
{
  switch (codice_colore) {
     case 0: return rosso;
     break;
     case 1: return blu;
     break;
     case 2: return giallo;
     break;
     case 3: return verde;
     break;
     case 4: return nero;
     break;
     case 5: return fucsia;
     break;
     case 6: return viola;
     break;
     case 7: return celeste;
     break;
     case 8: return bianco;
     break;
     case 9: return marrone;
     break;
  }
  return rosso;
}

// Ritorna la stringa con lo stato del giocatore
static char *Ottieni_Stato_Giocatore(struct Giocatore giocatore)
{
  switch (giocatore.stato_giocatore) {
   case 0: return "Astronauta";
   break;
   case 1: return "Impostore";
   break;
   case 2: return "Assassinato";
   break;
   case 3: return "Defenestrato";
   break;
  }
  return "";
}

static void stampa_giocatori(int *stati_validati, int *nomi_univoci)
{
  for (int i = 0; i < numero_giocatori; i++) {
    struct Giocatore giocatore;
    giocatore.stanza_giocatore_momentanea = stanza_inizio;
    giocatore.nome_giocatore = nomi_univoci[i];
    giocatore.stato_giocatore = stati_validati[i];
    giocatori[i] = giocatore;

    printf("Nome: [%s]\n", Ottieni_Nome_Giocatore(giocatori[i]));
    printf("Stato: [%s]\n", Ottieni_Stato_Giocatore(giocatori[i]));
    printf("\n");
  }
}

// Generare in modo casuale lo stato delle stanze
static int genera_stato_stanza()
{
  int random_stanza;
  random_stanza = rand() % 100; // generare numeri casuali
  if (random_stanza < 25) {
    return botola;
  }
  else if (random_stanza >= 25 && random_stanza < 40) {
    return quest_doppia;
  }
  else if (random_stanza >= 40 && random_stanza < 70) {
    return quest_semplice;
  }
  else {
    return vuota;
  }
}

static void inizia_gioco()
{
  // messaggio dopo aver impostato il gioco.
  printf("Bene! hai impostato il gioco, ora verrai mandato nel menù principale!\n");
}

void gioca()
{

  int numero_turni[10];
  int turno_corrente = 0;
  int giocatore_corrente;
  bool puo_giocare;
  bool fine_partita = false;
  int giocatore = 1;
  int giocatori_morti_stanza = 0;
  int totale_impostori_stanza = 0;
  int totale_astronauti_stanza = 0;

  for (int i = 0; i < numero_giocatori; i++) {
    numero_turni[i] = 0;
  }
  do {
    turno_corrente++;
    for (int i = 0; i < numero_giocatori; i++) {
      puo_giocare = false;
      do {
        giocatore_corrente = rand () % numero_giocatori;
        if (numero_turni[giocatore_corrente] < turno_corrente) {
          puo_giocare = true;
        }
      } while (puo_giocare == false);

      if (fine_partita == false) {
        totale_impostori_stanza = 0;
        totale_astronauti_stanza = 0;
        giocatori_morti_stanza = 0;
        numero_turni[giocatore_corrente]++;
        if (giocatori[giocatore_corrente].stato_giocatore != 3 && giocatori[giocatore_corrente].stato_giocatore != 2) {
          printf("Il giocatore %s è il %d a cominciare\n", Ottieni_Nome_Giocatore(giocatori[giocatore_corrente]), giocatore++);
          printf("Il tipo della stanza è: %s\n", Ottieni_stanza_giocatore(giocatori[giocatore_corrente]));
          printf("In questa stanza sono presenti:\n");
          for (int j = 0; j < numero_giocatori; j++) {
            if (giocatori[giocatore_corrente].stanza_giocatore_momentanea == giocatori[j].stanza_giocatore_momentanea) {
              printf("%s-", Ottieni_Nome_Giocatore(giocatori[j]));
            }
            if (giocatori[j].stato_giocatore == 2) {
              giocatori_morti_stanza++;
            }
            if (giocatori[j].stato_giocatore == 0) {
              totale_astronauti_stanza++;
            }
            if (giocatori[j].stato_giocatore == 1) {
              totale_impostori_stanza++;
            }
          }
          printf("\n");

          int sceltaUtente;
          // definisco la variabile scelta_ammessa per verificare che l'input dell'utente sia corretto (tra i valori accettati)
          bool scelta_ammessa = false;
          do {
            do {
              printf("\n");
              printf("Che cosa vuoi fare?\n");
              printf("Premere [1] per avanzare\n");
              if (giocatori[giocatore_corrente].stato_giocatore == 0 && (giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza == 1 || giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza == 2)) {
                printf("Premere [2] per eseguire la quest\n");
              }
              if (giocatori[giocatore_corrente].stanza_giocatore_momentanea->emergenza_chiamata == false && giocatori_morti_stanza > 0) {
                printf("Premere [3] per utilizzare la chiamata di emergenza\n");
              }
              if (giocatori[giocatore_corrente].stato_giocatore == 1) {
                printf("Premere [4] per uccidere un astronauta\n");
                if (giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza == 1 || giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza == 2) {
                  printf("Premere [5] per sabotare\n");
                }
                // permettiamo di utilizzare la botola solo se il numero di stanze è maggiore di 1
                if (giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza == 3 && conta_stanze() > 1) {
                  printf("Premere [6] per utilizzare la botola\n");
                }
              }

              scanf("%d", &sceltaUtente);
            } while(sceltaUtente <= 0 || sceltaUtente >= 7);

            if (sceltaUtente == 1) {
              avanza_personaggio(giocatore_corrente);
              scelta_ammessa = true;
            }
            if (sceltaUtente == 2) {
              if (giocatori[giocatore_corrente].stato_giocatore == 0 && (giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza == 1 || giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza == 2)) {
                esegui_quest(giocatore_corrente);
                scelta_ammessa = true;
              }
            }
            if (sceltaUtente == 3 && giocatori[giocatore_corrente].stanza_giocatore_momentanea->emergenza_chiamata == false && giocatori_morti_stanza > 0) {
              chiamata_emergenza(giocatore_corrente, totale_impostori_stanza, totale_astronauti_stanza);
              scelta_ammessa = true;
            }
            if (sceltaUtente == 4 && giocatori[giocatore_corrente].stato_giocatore == 1) {
              uccidi_astronauta(giocatore_corrente);
              scelta_ammessa = true;
            }
            if (sceltaUtente == 5 && (giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza == 1 || giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza == 2) && giocatori[giocatore_corrente].stato_giocatore == 1) {
              sabotaggio(giocatore_corrente);
              scelta_ammessa = true;
            }
            if (sceltaUtente == 6 && giocatori[giocatore_corrente].stato_giocatore == 1 && giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza == 3 && conta_stanze() > 1) {
              usa_botola(giocatore_corrente);
              scelta_ammessa = true;
            }
            if (totale_quest >= totale_quest_da_finire) {
              printf("***************************\n");
              printf("* Vincono gli Astronauti! *\n");
              printf("***************************\n");
              fine_partita = true;
            }
            int totale_astronauti = 0;
              for (int j = 0; j < numero_giocatori; j++) {
                if (giocatori[j].stato_giocatore == 0) {
                  totale_astronauti++;
                }
              }
              if (totale_astronauti == 0) {
                printf("**************************\n");
                printf("* Vincono gli Impostori! *\n");
                printf("**************************\n");
                fine_partita = true;
              }
          } while (scelta_ammessa == false); // verifico se l'input è valido
        }
      }
    }
  } while (fine_partita == false);
  termina_gioco();
}

// funzione per la stanza dove il giocatore andrà
static char *Ottieni_stanza_giocatore(struct Giocatore giocatore)
{
  switch (giocatore.stanza_giocatore_momentanea->tipo_stanza) {
     case 0: return "Vuota";
     break;
     case 1: return "Quest Semplice";
     break;
     case 2: return "Quest Doppia";
     break;
     case 3: return "Botola";
     break;
  }
  return "";
}

// funzione per l'avanzamento del giocatore
static void avanza_personaggio(int numero_giocatore)
{
  int sceltaUtente;

  do {
    printf("Che cosa vuoi fare?\n");
    printf("Premere [1] per andare avanti\n");
    printf("Premere [2] per andare a destra\n");
    printf("Premere [3] per andare a sinistra\n");
    printf("Premere [4] per rimanere fermo\n");
    scanf("%d", &sceltaUtente);
    printf("\n");
  } while(sceltaUtente <= 0 || sceltaUtente > 4);

  if (sceltaUtente == 1) {
    // azione: AVANTI
    struct Stanza* prossima_stanza = giocatori[numero_giocatore].stanza_giocatore_momentanea->avanti;
    if (prossima_stanza != NULL) {
      giocatori[numero_giocatore].stanza_giocatore_momentanea = prossima_stanza;
    } else {
        struct Stanza* nuova_stanza = crea_nuova_stanza();
        nuova_stanza->stanza_precedente = giocatori[numero_giocatore].stanza_giocatore_momentanea;
        giocatori[numero_giocatore].stanza_giocatore_momentanea->avanti = nuova_stanza;
        giocatori[numero_giocatore].stanza_giocatore_momentanea = nuova_stanza;
      }
  }

  if (sceltaUtente == 2) {
    // azione: DESTRA
    struct Stanza* prossima_stanza = giocatori[numero_giocatore].stanza_giocatore_momentanea->destra;
    if (prossima_stanza != NULL) {
      giocatori[numero_giocatore].stanza_giocatore_momentanea = prossima_stanza;
    } else {
        struct Stanza* nuova_stanza = crea_nuova_stanza();
        nuova_stanza->stanza_precedente = giocatori[numero_giocatore].stanza_giocatore_momentanea;
        giocatori[numero_giocatore].stanza_giocatore_momentanea->destra = nuova_stanza;
        giocatori[numero_giocatore].stanza_giocatore_momentanea = nuova_stanza;
      }
  }

  if (sceltaUtente == 3) {
    // azione: SINISTRA
    struct Stanza* prossima_stanza = giocatori[numero_giocatore].stanza_giocatore_momentanea->sinistra;
    if (prossima_stanza != NULL) {
      giocatori[numero_giocatore].stanza_giocatore_momentanea = prossima_stanza;
    } else {
        struct Stanza* nuova_stanza = crea_nuova_stanza();
        nuova_stanza->stanza_precedente = giocatori[numero_giocatore].stanza_giocatore_momentanea;
        giocatori[numero_giocatore].stanza_giocatore_momentanea->sinistra = nuova_stanza;
        giocatori[numero_giocatore].stanza_giocatore_momentanea = nuova_stanza;
      }
  }
}

// funzione di utilità per generare una nuova stanza
static struct Stanza* crea_nuova_stanza()
{
  struct Stanza* nuova_stanza;
    nuova_stanza = (struct Stanza*) malloc (sizeof(struct Stanza));
    nuova_stanza->avanti = NULL;
    nuova_stanza->destra = NULL;
    nuova_stanza->sinistra = NULL;
    nuova_stanza->stanza_precedente = NULL;
    nuova_stanza->tipo_stanza = genera_stato_stanza();
    nuova_stanza->emergenza_chiamata = false;
    if (nuova_stanza->tipo_stanza == 3) {
      // la stanza di tipo Botola, incremento il contatore delle botole
      conta_botole++;
    }
    inserisciNodo(nuova_stanza);
    return nuova_stanza;
}

// funzione per l'incremento della quest
static void esegui_quest(int giocatore_corrente)
{
  // la quest viene aumentata di 1 perchè la stanza è "quest semplice"
  if (giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza == 1) {
    totale_quest++;
  }
  // la questa viene aumentata di 2 perchè la stanza è "quest doppia"
  if (giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza == 2) {
    totale_quest = totale_quest + 2;
  }
  giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza = 0;
}

static void uccidi_astronauta(int giocatore_corrente)
{
  bool presenza_giocatore = false;
  int sceltaUtente;
  char nome[7]; // 7 è il numero massimo di caratteri dei colori utilizzati nel gioco
  int tot_probabilita = 0;

  do {
    printf("Quale giocatore vuoi uccidere?\n");
    scanf("%s", nome);
    for (int i = 0; i < numero_giocatori; i++) {
      // verifico se il nome inserito dall'utente coincide con uno di quelli presenti nella  stanza
      if (strcmp(nome, Ottieni_Nome_Giocatore(giocatori[i])) == 0 && giocatori[i].stato_giocatore == 0 && giocatori[i].stanza_giocatore_momentanea == giocatori[giocatore_corrente].stanza_giocatore_momentanea) {
        giocatori[i].stato_giocatore = 2;
        presenza_giocatore = true;
        printf("Il giocatore %s è stato assassinato\n", nome);
        printf("\n");
      }
    }
    if (presenza_giocatore == false) {
      printf("Non posso assassinare il giocatore\n");
    }
  } while (presenza_giocatore == false);

  if (presenza_giocatore == true) {
    // il codice che segue è l'algoritmo con la logica per determinare la probabilità che il giocatore venga Defenestrato
    // la logica è:
    // 50% di probabilità in più per ogni altro astronauta presente nella stanza
    // 20% di probabilità in più per ogni astronauta presente nella stanza precedente
    for (int j = 0; j < numero_giocatori; j++) {
      if (giocatori[j].stato_giocatore == 0) { // se il giocatore è ASTRONAUTA
        if (giocatori[giocatore_corrente].stanza_giocatore_momentanea == giocatori[j].stanza_giocatore_momentanea) {
          tot_probabilita += 50;
        }
        if (giocatori[giocatore_corrente].stanza_giocatore_momentanea->stanza_precedente == giocatori[j].stanza_giocatore_momentanea) {
          tot_probabilita += 20;
        }
      }
    }

    if (tot_probabilita >= 70) {
      // cambio lo stato del giocare in defenestrato!
      giocatori[giocatore_corrente].stato_giocatore = 3;
      printf("Sei stato defenestrato!\n");
    }
  }
}

// funzione per il cambio dello stato della stanza (da quest "semplice" o "doppia") in "vuota"
static void sabotaggio (int giocatore_corrente)
{
  giocatori[giocatore_corrente].stanza_giocatore_momentanea->tipo_stanza = 0;
}

// la funzione  chiamata di emergenza viene invocata quando nella stanza c'è un astronauta assassinato
static void chiamata_emergenza(int giocatore_corrente, int totale_impostori_stanza, int totale_astronauti_stanza)
{
  // calcolo le probabilità e le assegno a due variabili intere
  int percentuale_impostori = 30 + (20 * totale_astronauti_stanza) - (30 * totale_impostori_stanza);
  int percentuale_astronauti = 30 + (20 * totale_impostori_stanza ) - (30 * totale_astronauti_stanza);
  int eliminazione_giocatore;
  bool giocatore_eliminato = false;

  do {
    eliminazione_giocatore = rand () % numero_giocatori;
    // se gli impostori sono in percentuale maggiore rispetto agli astronauti e un giocatore preso a caso è di tipo Impostore
    // oppure se
    // se gli astronati sono in percentuale maggiore rispetto agli impostori e il giocatore preso a caso è di tipo Astronauta
    if ((percentuale_impostori > percentuale_astronauti && giocatori[eliminazione_giocatore].stato_giocatore == 1) || (percentuale_impostori <= percentuale_astronauti && giocatori[eliminazione_giocatore].stato_giocatore == 0)) {
      giocatore_eliminato = true;
      // Defenestro il giocatore precedentemente scelto casualmente
      giocatori[eliminazione_giocatore].stato_giocatore = 3;
      // Annoto che la chiamata di emergenza per questa stanza è gia stata chiamata
      giocatori[giocatore_corrente].stanza_giocatore_momentanea->emergenza_chiamata = true;
      printf("Il giocatore %s è stato eliminato\n", Ottieni_Nome_Giocatore(giocatori[eliminazione_giocatore]));
    }
  } while(giocatore_eliminato == false);
}

// funzione che inserisce una nuova struct Stanza all'interno della lista di stanze
static void inserisciNodo(struct Stanza* stanza)
{
 // creare uno spazio in memoria dinamica contenente la struttura nuovo_nodo
 struct Nodo* nuovo_nodo = (struct Nodo*) malloc(sizeof(struct Nodo));
 nuovo_nodo->stanza = stanza;
 nuovo_nodo->prossimo = NULL;

 if (inizio_lista == NULL){
   inizio_lista = nuovo_nodo;
 } else {
    struct Nodo *ptr = inizio_lista; // variabile d'appoggio per scorrere la lista
    while (ptr->prossimo != NULL) {
      ptr = ptr->prossimo;
    }
    ptr->prossimo = nuovo_nodo;
  }
}

//funzione creata per ritornare una stanza botola tra quelle gia esistenti
static struct Stanza* scegli_stanza_botola()
{
  struct Nodo *ptr = inizio_lista; // creata una variabile d'appoggio per scorrere la lista e farla partire dall'inizio
  int contatore_botole = 0;
  while(ptr != NULL) {
    if (ptr->stanza->tipo_stanza == 3) {
      // la stanza è di tipo botola quindi incremento il contatore
      contatore_botole++;
      int random_stanza = rand() % 100;
      if (random_stanza >= 40 || contatore_botole == conta_botole) {
        return ptr->stanza;
      }
    }
    ptr = ptr->prossimo;
  }
  return NULL;
}

// funzione creata per ritornare una stanza casuale (non botola)
static struct Stanza* scegli_altra_stanza()
{
  struct Nodo *ptr = inizio_lista;
  struct Nodo *ptr_ultima_stanza;
  while(ptr != NULL) {
    if (ptr->stanza->tipo_stanza != 3) { //controllo che non sia di tipo botola
      ptr_ultima_stanza = ptr;
      int random_stanza = rand() % 100;
      if (random_stanza >= 60) {
        return ptr->stanza;
      }
    }
    ptr = ptr->prossimo;
  }
  return ptr_ultima_stanza->stanza;
}

// funzione chiamata dall'impostore se la stanza in cui si trova è di tipo "botola"
static void usa_botola(int giocatore_corrente)
{
  struct Stanza* stanza_scelta = NULL;
  if (conta_botole > 1) {
    //chiamo la funzione scegli_stanza_botola per scegliere una stanza di tipo "botola"
    stanza_scelta = scegli_stanza_botola();
  }
  if (stanza_scelta == NULL) {
    //chiamo la funzione scegli_altra_stanza per scegliere una stanza casuale che non sia "botola"
    stanza_scelta = scegli_altra_stanza();
  }
  if (stanza_scelta != NULL) {
    giocatori[giocatore_corrente].stanza_giocatore_momentanea->stanza_precedente = giocatori[giocatore_corrente].stanza_giocatore_momentanea;
    giocatori[giocatore_corrente].stanza_giocatore_momentanea = stanza_scelta;
    printf("Sei stato spostato in %s", Ottieni_stanza_giocatore(giocatori[giocatore_corrente]));
  }
}

// funzione che ritorna un intero con il numero delle stanze
static int conta_stanze()
{
  int totale = 0;
  if (inizio_lista != NULL) {
    struct Nodo *ptr = inizio_lista;
    while (ptr != NULL) {
      totale++;
      ptr = ptr->prossimo;
    }
  }
  return totale;
}

// funzione per deallocare la memoria scorrendo le stanze della lista
void termina_gioco()
{
  if (inizio_lista != NULL) {
    struct Nodo *ptr = inizio_lista;

    while (ptr != NULL) {
      free(ptr->stanza);
      ptr = ptr->prossimo;
      free(inizio_lista);
      inizio_lista = ptr;
    }
  }
}
