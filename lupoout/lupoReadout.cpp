#include "lupoReadout.h"
#include "QString"
#include "stdlib.h"
long get_time()
{
    long time_ms;
#ifdef WIN32
    struct _timeb timebuffer;
    _ftime( &timebuffer );
    time_ms = (long)timebuffer.time * 1000 + (long)timebuffer.millitm;
#else
    struct timeval t1;
    struct timezone tz;
    gettimeofday(&t1, &tz);
    time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
#endif
    return time_ms;
}

lupoReadout::lupoReadout()
{
}

lupoReadout::~lupoReadout()
{    
}

void lupoReadout::resetLUPO()
{    
    //ui->label->setText("Aquiring data...");
    printf("Aquiring data\n");
    lupo->clearFIFO();
    //lupo->resetTimeStamp();
    lupo->clearTriggerCounter();
    ti=0;
    deadtimeC=0;
    triggerCounter=0;
    receivedTrigger=0;
    timestamp=0;
    PrevRateTime = get_time();
    output="";       
}
void lupoReadout::readoutCycle()
{
    while(1){
        if (shmp->com_flag==1){ //start enter loop
            if(strncmp(shmp->com_message,"close",3)==0){
                fclose(fTSLupo);
                shmp->com_flag = 0;
                printf("Closed files!\n");
            }else if(strncmp(shmp->com_message,"start",3)==0){
                fTSLupo=fopen(shmp->filename, "w");
                if(fTSLupo==NULL)
                {
                  perror("open data file error!");
                  fTSLupo=NULL;
                  break;
                }else{
                    saveFile=true;
                }
                printf("OPEN FILE: %s\n",shmp->filename);
                resetLUPO();
                shmp->com_flag = 0;
                //enter readout cycle
                while (1){
                    CurrentTime = get_time();
                    ElapsedTime = CurrentTime - PrevRateTime; // milliseconds
                    if (ElapsedTime > 1000) {
                      system("clear");
                      ti++;
                      printf("         LUPO READOUT         \n");
                      printf("Current file name = %s \n",shmp->filename);
                      printf("Tiktok = %i \n",ti);
                      PrevRateTime = CurrentTime;
                      printf("Trigger Counter = %i\n",triggerCounter);
                      printf("Received Trigger = %i\n",receivedTrigger);
                      printf("Dead Events = %i\n",deadtimeC);
                      printf("Current timestamp=%lli\n",timestamp);
                    }

                    unsigned int prev_TC;
                    prev_TC=triggerCounter;
                    triggerCounter=lupo->readTriggerCounter();
                    if (triggerCounter>prev_TC){
                        //unsigned long timeStampLupo=((unsigned long)lupo->readTimeStampMSB32())<<32|((unsigned long)lupo->readTimeStampLSB32());
                        unsigned int BinHeader[3];
                        size_t s;
                        if ((triggerCounter-prev_TC)>1){
                            for (unsigned int i=prev_TC+1;i<triggerCounter;i++){
                                BinHeader[0] =i;//Number of sample
                                BinHeader[1] = lupo->readTimeStampMSB32();
                                BinHeader[2] = lupo->readTimeStampLSB32();
                                timestamp=(long long) BinHeader[1]<<32 | BinHeader[2];
                                //Write header and check for consistency
                                if (saveFile==true){
                                    //s=fwrite(BinHeader,sizeof(unsigned int),3,fTSLupo);
                                    fprintf(fTSLupo,"%i %i %lli\n",triggerCounter,triggerCounter,timestamp);
                                    fflush(fTSLupo);
                                    //if(s!=3){
                                    //    printf("Error writing header, return %i\n",s);
                                    //}
                                }

                            }
                            deadtimeC+=triggerCounter-prev_TC-1;
                        }
                        BinHeader[0] =triggerCounter;//Number of sample
                        BinHeader[1] = lupo->readTimeStampMSB32();
                        BinHeader[2] = lupo->readTimeStampLSB32();

                        shmp->trgCnt=triggerCounter;
                        shmp->timestampMSB=BinHeader[1];
                        shmp->timestampLSB=BinHeader[2];
                        timestamp=(long long) BinHeader[1]<<32 | BinHeader[2];
                        //Write header and check for consistency
                        if (saveFile==true){
                            fprintf(fTSLupo,"%i %i %lli\n",triggerCounter,triggerCounter,timestamp);
                            fflush(fTSLupo);
                            //if(s!=3){
                            //    printf("Error writing header, return %i\n",s);
                            //}
                            //fflush(fTSLupo);
                        }
                        //fprintf(fTSLupo,"%lu %lu\n",triggerCounter,timeStampLupo); //1us
                        lupo->clearFIFO();//1 us
                        //lupo->sendPulse(1);
                        receivedTrigger++;
                    }
                    if(strncmp(shmp->com_message,"stop",3)==0){
                        printStop();
                        shmp->com_flag=0;
                        break;
                    }
                }//end of readout cycle
                //
            }//if start
        }//if comflag
    }

}

void lupoReadout::printStop()
{
    printf("LUPO Daq stopped\n");
}

void lupoReadout::connect()
{
    lupo->setLupoAddress(0x111000);
    lupo->lupoConnect(1,0);
    if (lupo->getConnectStatus()==1) {
        printf("Connected!\n");
    }else{
        printf("Error!\n");
        return;
    }
    //also open share memory
    if ((shmid = shmget(SHMKEY, sizeof(struct SHM_LUPO_S), 0600)) != -1)
    {
         shmp = (struct SHM_LUPO_S *)shmat(shmid, 0, 0);
         shmp->ana_flag=0;
         shmp->timestampMSB=0;
         shmp->timestampLSB=0;
         shmp->trgCnt=0;
         shmp->com_flag=0;
         printf("Sucessfully attached to share memory with shmid=%i, size of %i\n-----------------\n",shmid,(int)sizeof(struct SHM_LUPO_S));
    }else if ((shmid = shmget(SHMKEY, sizeof(struct SHM_LUPO_S), IPC_CREAT|0600)) != -1)
    {
        shmp = (struct SHM_LUPO_S *)shmat(shmid, 0, 0);
        printf("Sucessfully created share memory with shmid=%i, size of %i\n-----------------\n",shmid,(int)sizeof(struct SHM_LUPO_S));
    }else{
        printf("Can not attached to share memory!\n");
    }
    shmp->pid=getpid();
}

void lupoReadout::disconnect()
{
    release_nbbqvio(BHandle);
    printf("Disconnected\n");
}

