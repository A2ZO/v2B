#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "PointH/instructions.h"
#include "PointH/utils.h"
#include "PointH/startStopServices.h"

void startService( int NbServ, char * dirServ, char *listServices[], int FIFO_fdR[],  int FIFO_fdW[] ) {
   char *myCommand[4];
   myCommand[0] = (char *)malloc(20*sizeof(char));
   myCommand[1] = (char *)malloc(40*sizeof(char));
   myCommand[2] = (char *)malloc(40*sizeof(char));
   myCommand[3] = NULL;
  
   // Lancement des services

   for(int i=0; i<NbServ; i++){
       // Service i à lancer
        strcpy(myCommand[0], dirServ);   strcat(myCommand[0],"/");       strcat(myCommand[0],listServices[i]); 
        strcpy(myCommand[1], "./");    strcat(myCommand[1],listServices[i]);  strcat(myCommand[1],"1.fifo"); 
        strcpy(myCommand[2], "./");    strcat(myCommand[2],listServices[i]);  strcat(myCommand[2],"2.fifo"); 
        myCommand[3]= NULL;

         // Ouverture du pipe nommé associée et ouverture en lecture/écriture

	if (mkfifo(myCommand[1] , 0666) == -1) {
            if (errno == EEXIST) {} //printf("Le pipe nommé %s existe déjà\n",myCommand[1] );
            else {
               printf("Erreur inconnue lors de la création du pipe nommé %s\n",myCommand[1] );
               exit(EXIT_FAILURE);
            }
        }
	if (mkfifo(myCommand[2] , 0666) == -1) {
            if (errno == EEXIST) {} //printf("Le pipe nommé %s existe déjà\n",myCommand[2] );
            else {
               printf("Erreur inconnue lors de la création du pipe nommé %s\n",myCommand[2] );
               exit(EXIT_FAILURE);
            }
       } 
  
// Lancement du service
        if(fork() != 0) {  execv(myCommand[0], myCommand); }
          
        printf(" Serveur %d lancé avec la FIFO : %s et %s\n",i,myCommand[1],myCommand[2]);
 
    	FIFO_fdR[i] = open(myCommand[1], O_RDONLY);
        FIFO_fdW[i] = open(myCommand[2], O_WRONLY);
      //   sleep(1);
     
   }
}

void stopService( int NbServ, char *listServices[],int FIFO_fdR[],  int FIFO_fdW[]) {
  for(int i = 0; i<NbServ;i++)   {

    // Arrêt des serveurs
      ecrirePipe(FIFO_fdW[i], -1);

   // Fermeture des pipes
      close(FIFO_fdR[i]);
      close(FIFO_fdW[i]);

    // Suppression des pipes nommés
        char P1[40], P2[40];
   
        strcpy(P1, "./");    strcat(P1,listServices[i]);  strcat(P1,"1.fifo"); 
        strcpy(P2, "./");    strcat(P2,listServices[i]);  strcat(P2,"2.fifo"); 
        unlink(P1);
        unlink(P2);
  }
}
