#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "PointH/instructions.h"
#include "PointH/utils.h"


int main(int argc, char * argv[]) {
 
  Instruction TPI = {0, 0, 0, 0,0};
 
   int FIFO_fdW = open(argv[1], O_WRONLY);
   int FIFO_fdR = open(argv[2], O_RDONLY);

   do{
     TPI[0] = lirePipe(FIFO_fdR);
     if (TPI[0] == -1) break;  // Arrêt du service demandé
     TPI[1] = lirePipe(FIFO_fdR);
     TPI[2] = lirePipe(FIFO_fdR);
  
      // L'opération
            TPI[3] = TPI[1] - TPI[2];
  
       ecrirePipe(FIFO_fdW, TPI[3]);
       ecrirePipe(FIFO_fdW, TPI[4]);
   
    }while(1==1);
 
   close(FIFO_fdR);
   close(FIFO_fdW);
   printf(" Arrêt subService\n");

}
