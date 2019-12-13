#include "lupoReadout.h"
#include "lupoLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{

    int linkn,boardn,address,command;
    if (argc==5){
        linkn = atoi(argv[1]);
        boardn = atoi(argv[2]);
        address = (unsigned int)strtol(argv[3],NULL,16);
        command = atoi(argv[4]);
    }else{
      printf("Usage ./lupo link_number board_number hexaddress command(1/2)\n");
      return 0;
    }

    lupoLib* lupo = new lupoLib(address);
    lupo->lupoConnect(linkn,boardn);

    if (lupo->getConnectStatus()==1) {
        if (command==1||command==2)lupo->sendPulse(command);
        else if (command==3) lupo->send2718Pulse();
        printf("Sent a signal to the output %i\n",command);
    }else{
        printf("Error!\n");
    }
    return 0;

}
