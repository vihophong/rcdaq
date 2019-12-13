#ifndef LUPOREADOUT_H
#define LUPOREADOUT_H


#include "lupoLib.h"
#include "stdio.h"
#include "caenvmeA32.h"
#include "QString"
//For share memory
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#define SHMKEY 95

typedef struct SHM_LUPO_S{
  uint32_t pid;
  uint32_t timestampMSB;
  uint32_t timestampLSB;
  uint32_t trgCnt;
  int ana_flag;
  char com_message[500];
  uint32_t com_flag;
  char filename[1000];
} SHM_LUPO;
class lupoReadout
{
public:
     lupoReadout();
    ~lupoReadout();

    void readoutCycle();

    void resetLUPO();

    void printStop();

    void connect();

    void disconnect();
    int shmid;

//signals:
//    void endOfReadOutCycle();

private:
    int BHandle;
    lupoLib* lupo;
    SHM_LUPO* shmp;
    unsigned int triggerCounter;
    unsigned int receivedTrigger;
    long long timestamp;
    FILE* fTSLupo;
    uint64_t CurrentTime, PrevRateTime, ElapsedTime;
    QString output;
    unsigned int deadtimeC;
    QString saveDirectory;
    QString saveFileName;
    bool saveFile;

    int ti;    
};

#endif // LUPOREADOUT_H
