#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
 #include <fcntl.h>

#include "DirC/PointH/utils.h"
#include "DirC/PointH/chargerProg.h"
#include "DirC/PointH/chargerFichierProg.h"
#include "DirC/PointH/demandeService.h"
#include "DirC/PointH/startStopServices.h"
#include "DirServices/listServices.h"


int main(int argc, char* argv[]) {

  int nbInst = 0;
  
  // Chargement du programme depuis un fichier

  int fp = -1;

  if(argc != 2) { printf(" Usage : %s <fichier programme>\n",argv[0]); exit(-1);}
  if ((fp = open(argv[1],O_RDONLY)) < 0) { 
     printf(" Problème à l'ouverture de %s <fichier programme>\n",argv[1]);  
     exit(-1);
  }

 nbInst = lireNbrInst(fp);
 printf(" Nbr Instr : %d \n",nbInst);

  Instruction TPI[nbInst];   
  
   chargerFichierProg(fp, nbInst, TPI);
  
   for(int n = 0; n < nbInst; n++) affInst(TPI[n]);

 
 printf("+++++ Programme de calculs chargé +++++++++ \n");

   // Création du tableau des pipes nommés qui seront ouverts lors du lancement des services
   int FIFO_fdR[NbServ];
   int FIFO_fdW[NbServ];
   startService(NbServ, ServDir, listServices, FIFO_fdR, FIFO_fdW);

printf("+++++ Lancement des services effectué +++++++++ \n");

   for(int i = 0; i <nbInst; i++) {
            if (TPI[i][5] == 0) {  
               if(fork() != 0) {  // un fils va être crée pour que le père n'attende pas 
                                       // Il faudra transférer le résultat vers le père
                                       // Donc, il faut trouver une autre solution
                   printf(" Le fils traitre l'instruction i : %d  <-- ",i); affInst(TPI[i]);
                   ecrirePipe(FIFO_fdW[TPI[i][0]-1], TPI[i][0]);
                   ecrirePipe(FIFO_fdW[TPI[i][0]-1], TPI[i][1]);
                   ecrirePipe(FIFO_fdW[TPI[i][0]-1], TPI[i][2]);
              	   TPI[i][3] =  lirePipe(FIFO_fdR[TPI[i][0]-1]);              
		   TPI[i][4] =  lirePipe(FIFO_fdR[TPI[i][0]-1]);
                   printf(" Le fils a reçu le résultat de l'instruction i : %d  --> ",i); affInst(TPI[i]);
                       exit(1);
               }
            } else {  // Le père attend le résultat
                 printf(" Le père traitre l'instruction i : %d  <-- ",i); affInst(TPI[i]);
                 ecrirePipe(FIFO_fdW[TPI[i][0]-1], TPI[i][0]);
                 ecrirePipe(FIFO_fdW[TPI[i][0]-1], TPI[i][1]);
                 ecrirePipe(FIFO_fdW[TPI[i][0]-1], TPI[i][2]);
            	 TPI[i][3] =  lirePipe(FIFO_fdR[TPI[i][0]-1]);              
		 TPI[i][4] =  lirePipe(FIFO_fdR[TPI[i][0]-1]);
                 printf(" Le père a reçu le résultat de l'instruction i : %d   --> ",i); affInst(TPI[i]);
             }
   }
  printf(" ++++++ Calculs terminées ++++++ \n");
  
  stopService(NbServ, listServices,FIFO_fdR, FIFO_fdW);
  printf(" ++++++ Serveurs arrêtés ++++++ \n");
 
  exit(0);
}
