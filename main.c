#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

int main ()
{
  int sceltaUtente;
  // flag per controllare se il gioco è stato impostato
  int setup_gioco = 0;
  printf("**************************\n");
  printf("****TRANNOI, IL GIOCO!****\n");
  printf("**************************\n");
  printf("\n");
  
  do {
    printf("Menù principale!\n");
    printf("Premere [1] per impostare il gioco\n");
    printf("Premere [2] per giocare\n");
    printf("Premere [3] per terminare il gioco\n");
    printf("\n");

    printf("Seleziona un'azione:\n");
    scanf("%d", &sceltaUtente);

    switch (sceltaUtente){
      case 1:
        imposta_gioco();
        setup_gioco = 1;
      break;

      case 2:
        if (setup_gioco == 1) {
          gioca();
        } else {
          printf("Prima di giocare è necessario impostare il gioco!\n");
        }
      break;

      case 3:
        termina_gioco();
      break;

      default:
        printf("Il valore inserito non è valido, inserire un valore compreso tra 1 e 3\n");
    }
  } while (sceltaUtente != 3);
}
