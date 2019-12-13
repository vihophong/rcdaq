
#include "myanalysis.cc"
#include <stdint.h>

int CFD_delay=1;//in Tclk unit, 1 Tclk=2ns
double CFD_fraction=0.5;
double LED_threshold=100;
double LED_threshold_LED=100;
double LED_threshold_740=20;
double LED_threshold_LED_740=20;
int gateOffset=10;
int shortGate=20;
int longGate=150;
int nBaseline=16;
int minVarBaseline=100; //criteria for baseline determination
int mode_selection=2;


//! dont care about the following code
uint16_t seperateint(int inint,bool islsb){
  if (islsb) return (uint16_t) (inint&0xFFFF);
  else return (uint16_t) (inint>>16);
}

NIGIRIHit* data;

int init_done = 0;
int pinit()
{
  if (init_done) return 1;
  init_done = 1;
  gROOT->ProcessLine(".L libDataStruct.so");
  data = new NIGIRIHit;
  Init();
  return 0;
}

int process_event (Event * e)
{  
  Packet *p1[V1730_N_MAX_BOARD];
  for(int pk=0;pk<V1730_N_MAX_BOARD;pk++){
    p1[pk]= e->getPacket(pk);
    if (p1[pk])
      {            
        int* temp;
        int* gg;
        gg=(int*) p1[pk]->getIntArray(temp);
        int size=p1[pk]->getPadding();
        //! content
        int k=V1740_HDR+V1740_N_MAX_CH; 
        for (int i=0;i<V1730_N_MAX_CH;i++){
          //! header
          data->Clear();
          data->evt_type = V1730_EVENT_TYPE;
          data->b = pk;//for sorter
          data->evt = gg[2]+1;//this event start from 0
          data->overrange = (Char_t) gg[1];//intepret as channel(group) mask
          UInt_t tslsb = (UInt_t)gg[5];
          UInt_t tsmsb = (UInt_t)gg[4];
          data->ts = (((ULong64_t)tsmsb<<32)&0xFFFF00000000)|(ULong64_t)tslsb;//resolution is 16 ns!
          data->ch = i;//for sorter

          int nsample = gg[i+V1740_HDR];
          data->nsample = nsample;		  
          UShort_t WaveLine[nsample];
          int ispl = 0;	  
	  /*
	  for(int i=0;i<nsample/2;i++){
	    WaveLine[i*2] = (UShort_t)seperateint(gg[k],true);
	    data->pulse.push_back(gg[k]&0xFFFF); 
	    WaveLine[i*2+1]=(UShort_t)seperateint(gg[k],false);
	    data->pulse.push_back(WaveLine[i*2+1]);
	    k++;
	  }	  	  	  
	  */
          for (int j=0;j<nsample/2+nsample%2;j++){
            if (ispl<nsample) {	      
                WaveLine[ispl]=gg[k]&0xFFFF;
                data->pulse.push_back(gg[k]&0xFFFF);
            }
            ispl++;
            if (ispl<nsample) {
                WaveLine[ispl]=(gg[k]>>16)&0xFFFF;
                data->pulse.push_back((gg[k]>>16)&0xFFFF);
            }
            ispl++;
            k++;
          }


          if (nsample>NSBL){
              dpp *oj=new dpp(nsample,WaveLine);
              oj->baselineMean(nBaseline,minVarBaseline);
              double timeData = 0;
              int cShort,cLong;

              if (mode_selection==0) {
                  timeData=oj->led(LED_threshold_LED);
              }else if (mode_selection==1) {
                  timeData=oj->ledWithCorr(LED_threshold);
              }else if (mode_selection==2){
                  oj->makeCFD(LED_threshold,CFD_delay,CFD_fraction);
                  timeData=oj->cfdFast();
              }else if (mode_selection==3){
                  oj->makeCFD(LED_threshold,CFD_delay,CFD_fraction);
                  timeData=oj->cfdSlow();
              }

              oj->chargeInter(cShort,cLong,timeData,gateOffset,shortGate,longGate);
              data->cshort = cShort;
              data->clong = cLong;
	      if (data->b==2&&(data->ch==11||data->ch==12)) data->clong=oj->maxAdcPos(N_MAX_WF_LENGTH)-oj->bL;
              //cout<<cLong<<endl;
              data->baseline = oj->bL;
              data->finets = timeData;
              delete oj;
          }

          //! Do sort!
          ProcessEvent(data);
        }//end while
	delete p1[pk];
      }  
  }//loop all packet


  //! v1740 packet
  Packet *p1740=e->getPacket(V1740_PACKET);
  if (p1740)
    {
      int* temp;
      int* gg;
      gg=(int*) p1740->getIntArray(temp);
      int size=p1740->getPadding();
      //! content
      int k=V1740_HDR+V1740_N_MAX_CH; 
      for (int i=0;i<V1740_N_CH;i++){
        //! header
        data->Clear();
        data->evt_type = V1740_EVENT_TYPE;
        data->b = V1740_BOARD_N+i/V1730_N_MAX_CH;//for sorter
        data->evt = gg[2]+1;//this event start from 0
        data->overrange = (Char_t) gg[1];//intepret as channel(group) mask
        UInt_t tslsb = (UInt_t)gg[5];
        UInt_t tsmsb = (UInt_t)gg[4];
        data->ts = (((ULong64_t)tsmsb<<32)&0xFFFF00000000)|(ULong64_t)tslsb;//resolution is 16 ns!
        data->ch = i%V1730_N_MAX_CH;//for sorter

        int nsample = gg[i+V1740_HDR];
        data->nsample = nsample;
        UShort_t WaveLine[nsample];
	
        int ispl = 0;
        for (int j=0;j<nsample/2+nsample%2;j++){
          if (ispl<nsample) {
              WaveLine[ispl]=gg[k]&0xFFFF;
              data->pulse.push_back(gg[k]&0xFFFF);

          }
          ispl++;
          if (ispl<nsample) {
              WaveLine[ispl]=(gg[k]>>16)&0xFFFF;
              data->pulse.push_back((gg[k]>>16)&0xFFFF);
          }
          ispl++;
          k++;
        }		

        if (nsample>NSBL){
            dpp *oj=new dpp(nsample,WaveLine);
            oj->baselineMean(nBaseline,minVarBaseline);
            double timeData = 0;
            int cShort,cLong;

            if (mode_selection==0) {
                timeData=oj->led(LED_threshold_LED_740);
            }else if (mode_selection==1) {
                timeData=oj->ledWithCorr(LED_threshold_740);
            }else if (mode_selection==2){
                oj->makeCFD(LED_threshold_740,CFD_delay,CFD_fraction);
                timeData=oj->cfdFast();
            }else if (mode_selection==3){
                oj->makeCFD(LED_threshold_740,CFD_delay,CFD_fraction);
                timeData=oj->cfdSlow();
            }

            //oj->chargeInter(cShort,cLong,timeData,gateOffset,shortGate,longGate);
            data->cshort = 0;
            data->clong = oj->bL - oj->minAdcNeg(N_MAX_WF_LENGTH);
            //cout<<cLong<<endl;
            data->baseline = oj->bL;
            data->finets = timeData;
            delete oj;
        }
	ProcessEvent(data);

	/*
	  if (gg[2]==1000) {
	    for (int ii=0;ii<2;ii++) cout<<i<<" "<<nsample<<" "<<ii<<" "<<WaveLine[ii]<<endl;
	    for (int ii=nsample-3;ii<nsample;ii++) cout<<i<<" "<<nsample<<" "<<ii<<" "<<WaveLine[ii]<<endl;
	  }
	*/
      }
       delete p1740;
  }

  //! tdc packet
  Packet *tdcp=e->getPacket(TDC_PACKET);
  if(tdcp){
      int* temp;
      int* gg;
      gg=(int*) tdcp->getIntArray(temp);
      int size=tdcp->getPadding();
      data->Clear();
      data->evt_type = TDC_EVENT_TYPE;
      data->b = TDC_BOARD_N;
      data->ch = 0;
      UInt_t tslsb = (UInt_t)gg[3];
      UInt_t tsmsb = (UInt_t)gg[2];
      data->ts = (((ULong64_t)tsmsb<<32)&0xFFFF00000000)|(ULong64_t)tslsb;//resolution is 16 ns!
      data->clong= gg[1];//daq counter
      data->evt = gg[0];
      //! more tdc data here!
      int nwordtdc = 0;
      //cout<<"EVENT BEGIN"<<endl;

      bool flag_trailer = false;
      bool flag_endofv1190 = false;
      int nwordqdc = 0;
      for (int i=4;i<size;i++){
          if(!flag_trailer){
              if ((gg[i]&0xF8000000)>>27==0x00){//! tdc measurement word
                  UShort_t ch =(UShort_t)((gg[i]&0x3F80000)>>19);
                  Int_t t = (Int_t)(gg[i]&0x7FFFF);
                  data->AddTDCHit(ch,t);
                  //cout<<"tdc ch "<<std::dec<<ch<<" - tdc t "<<t<<std::dec<<endl;
              }
              nwordtdc++;
          }
          //! when global trailer met
          if((gg[i]&0xF8000000)>>27==0x10){
              //cout<<"Global trailer met"<<gg[i]<<endl;
              flag_trailer = true;
              //break;
          }
          if((gg[i]&0xF8000000)>>27==0x12){
              //cout<<"Separation bit met"<<gg[i]<<endl;
              flag_endofv1190 = true;
              //break;
          }
          if (flag_endofv1190&&(gg[i]&0xF8000000)>>27!=0x12){
              //! read qdc
              //cout<<"qdc bit"<<std::hex<<((gg[i]&0x7000000)>>24)<<std::dec<<endl;

              if (((gg[i]&0x7000000)>>24)==0x2){//Header met
                  int qdcnmemorisech=(int)((gg[i]&0x3F00)>>8);
                  //cout<<qdcnmemorisech<<endl;
              }else if(((gg[i]&0x7000000)>>24)==0x0){
                  int qdcch_lsb =(gg[i]&0xF0000)>>16;
                  int qdcch_msb =(gg[i]&0x100000)>>20;
                  int qdcch = qdcch_msb*16+qdcch_lsb;
                  int qdcdata =gg[i]&0xFFF;
                  data->AddQDCHit(qdcch,qdcdata);
                  //cout<<qdcch<<"-"<<qdcdata<<endl;
              }else if (((gg[i]&0x7000000)>>24)==0x4){//end of qdc block met
                  int qdcevtcounter = gg[i]&0xFFFFFF;
                  //cout<<qdcevtcounter<<endl;
              }else{
                  cout<<"Something wrong with QDC?"<<endl;
              }
              nwordqdc++;
          }
      }      
      //cout<<"EVENT END "<<nwordqdc<<endl;

      ProcessEvent(data);
      delete tdcp;
  }
  return 0;
}

int pclose(){
    CloseMe();
    cout<<"Finish!"<<endl;
    return 0;
}

