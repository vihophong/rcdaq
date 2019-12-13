/* ************************ DO NOT REMOVE  ****************************** */
#include <iostream>
#include <fstream>
#include <pmonitor.h>
#include <TSystem.h>
#include <TROOT.h>
#include "pdecoder.h"
#include <dpp.h>
#include "libDataStruct.h"
#include "DataStruct_dict.h"

#define DGTZ_CLK_RES 10
#define V1730_EVENT_TYPE 1
#define V1740_EVENT_TYPE 2
#define TDC_EVENT_TYPE 3

#define V1730_N_MAX_BOARD 5
#define V1730_N_MAX_CH 16
#define N_MAX_INFO 9

#define TDC_BOARD_N 50
#define TDC_PACKET 10
#define TDC_N_CH 64



#define V1740_HDR 6
#define V1740_N_CH 64
#define V1740_N_BOARD 4
#define V1740_PACKET_0 100
#define V1740_PACKET_1 101
#define V1740_PACKET_2 102
#define V1740_PACKET_3 103
#define V1740_N_MAX_CH 64
#define NSBL 8
// 5000000 - 15 % memory = 1.2 Gb

/* ********************************************************************** */

#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <vector>
#include <map>
#include <TCanvas.h>

#define N_REFESH_WF 1000
#define N_MAX_WF_LENGTH 300

#define COUNTS_TO_DISPLAY 5000
#define COUNTS_TO_CLEAR 1000000
#define COUNTS_TO_CAL 500
#define MAX_N_SAMPLE 300

#define MAX_MAP_LENGTH 50

#define CH_THRESHOLD 100
//DSSDs constant
#define N_DSSD 4
#define N_STRIP_X 16
#define N_STRIP_Y 16


using namespace std;

int trgcnt_prev;
int its;
long long ts_prev;

//int dgtz_rate;
int dgtzcnt_current[V1740_N_CH*V1740_N_BOARD];
int dgtzcnt_prev[V1740_N_CH*V1740_N_BOARD];
long long dgtzts_prev[V1740_N_CH*V1740_N_BOARD];
int chthr[V1740_N_CH*V1740_N_BOARD];
TH1F *hwf1d[V1740_N_CH*V1740_N_BOARD];
TH2F *hwf2d[V1740_N_CH*V1740_N_BOARD];
TH2F *ht2d;
TH2F *he2d;
TH2F *he2dmax;
TH2F *hbl2d;
TH1F *hratedgtz;

TCanvas* c1;
Int_t c1updatetrgcnt;
Int_t hcleartrgcnt;



//! stuff for sorter
Double_t           le_dssd_thr[N_DSSD*(N_STRIP_X+N_STRIP_Y)];
Double_t           le_dssd_cal0[N_DSSD*(N_STRIP_X+N_STRIP_Y)];
Double_t           le_dssd_cal1[N_DSSD*(N_STRIP_X+N_STRIP_Y)];
Int_t              le_chtostrip[V1740_N_BOARD*V1740_N_CH];

TH1F *hle_z;
TH2F *hle_zx;
TH2F *hle_yz;

Double_t           he_dssd_thr[N_DSSD*(N_STRIP_X+N_STRIP_Y)];
Double_t           he_dssd_cal0[N_DSSD*(N_STRIP_X+N_STRIP_Y)];
Double_t           he_dssd_cal1[N_DSSD*(N_STRIP_X+N_STRIP_Y)];
Int_t              he_chtostrip[V1740_N_BOARD*V1740_N_CH];
TH1F *hhe_z;
TH2F *hhe_zx;
TH2F *hhe_yz;

Int_t striptoch[N_DSSD*(N_STRIP_X+N_STRIP_Y)];
Int_t striptoch_he[N_DSSD*(N_STRIP_X+N_STRIP_Y)];


void ReadConfigTable(char* configfile)
{
  ifstream inpf(configfile);
  if (inpf.fail()){
    cout<<"No Configuration table is given"<<endl;
    return;
  }

  cout<<"Start reading configuration table: "<<configfile<<endl;
  Int_t index,dssd_index,strip_index,dgtz_channel;
  Double_t threshold,cal0,cal1;
  Int_t mm=0;

  while (inpf.good()){
    inpf>>index>>dssd_index>>strip_index>>dgtz_channel>>threshold>>cal0>>cal1;
    cout<<index<<"-"<<dssd_index<<"-"<<strip_index<<"-"<<dgtz_channel<<"-"<<threshold<<"-"<<cal0<<"-"<<cal1<<endl;
    striptoch[index]=dgtz_channel;
    //dssd_thr[index]=threshold;
    //dssd_cal0[index]=cal0;
    //dssd_cal1[index]=cal1;
    le_dssd_thr[index]=threshold;
    le_dssd_cal0[index]=cal0;
    le_dssd_cal1[index]=cal1;

    mm++;
  }
  cout<<"Read "<<mm<<" line"<<endl;
  inpf.close();
}

void ReadConfigTableHighE(char* configfile)
{

  ifstream inpf(configfile);
  if (inpf.fail()){
    cout<<"No Configuration table is given"<<endl;
    return;
  }

  cout<<"Start reading configuration table: "<<configfile<<endl;
  Int_t index,dssd_index,strip_index,dgtz_channel;
  Double_t threshold,cal0,cal1;
  Int_t mm=0;

  while (inpf.good()){
    inpf>>index>>dssd_index>>strip_index>>dgtz_channel>>threshold>>cal0>>cal1;
    cout<<index<<"-"<<dssd_index<<"-"<<strip_index<<"-"<<dgtz_channel<<"-"<<threshold<<"-"<<cal0<<"-"<<cal1<<endl;
    striptoch_he[index]=dgtz_channel;
    //dssd_thr[index]=threshold;
    //dssd_cal0[index]=cal0;
    //dssd_cal1[index]=cal1;
    he_dssd_thr[index]=threshold;
    he_dssd_cal0[index]=cal0;
    he_dssd_cal1[index]=cal1;
    mm++;
  }
  cout<<"Read "<<mm<<" line"<<endl;
  inpf.close();
}

void ClearHist(){
    for (int i=0;i<V1740_N_CH*V1740_N_BOARD;i++){
        hwf1d[i]->Reset();
        hwf2d[i]->Reset();
    }
    ht2d->Reset();
    he2d->Reset();
    he2dmax->Reset();
    hbl2d->Reset();
    hratedgtz->Reset();
    hle_z->Reset();
    hle_zx->Reset();
    hle_yz->Reset();
    hhe_z->Reset();
    hhe_zx->Reset();
    hhe_yz->Reset();
}

void Init(){       
    c1=new TCanvas("c1","c1",900,900);
    c1updatetrgcnt=0;
    hcleartrgcnt=0;
    //dgtz_rate=0;
    for (int i=0;i<V1740_N_CH*V1740_N_BOARD;i++){
        hwf1d[i] = new TH1F (Form("wf1d_%d",i),Form("Waveform 1d v1740 %d",i), N_MAX_WF_LENGTH, 0,N_MAX_WF_LENGTH );
        hwf2d[i] = new TH2F (Form("wf2d_%d",i),Form("Waveform 2d v1740 %d",i), N_MAX_WF_LENGTH, 0,N_MAX_WF_LENGTH, 2500, 0,5000 );
    }
    for (int i=0;i<V1740_N_CH*V1740_N_BOARD;i++){
        dgtzcnt_prev[i] = 0;
        dgtzts_prev[i] = 0;
        dgtzcnt_current[i] = 0;
        chthr[i]=CH_THRESHOLD;
    }
    ht2d = new TH2F("t2d","Time Spectra 2D",V1740_N_CH*V1740_N_BOARD,0,V1740_N_CH*V1740_N_BOARD,500,0,500);
    he2d = new TH2F("e2d","Energy Spectra 2D",V1740_N_CH*V1740_N_BOARD,0,V1740_N_CH*V1740_N_BOARD,2000,0,4000);
    he2dmax = new TH2F("e2dmax","Energy Spectra 2D nobl",V1740_N_CH*V1740_N_BOARD,0,V1740_N_CH*V1740_N_BOARD,2000,0,2000);
    hbl2d = new TH2F("bl2d","Baseline Spectra 2D",V1740_N_CH*V1740_N_BOARD,0,V1740_N_CH*V1740_N_BOARD,2500,0,5000);
    hratedgtz = new TH1F("chrate","chrate",V1740_N_CH*V1740_N_BOARD,0,V1740_N_CH*V1740_N_BOARD);

    //! sorter
    hle_z=new TH1F("hle_z","low energy z",N_DSSD,0,N_DSSD);
    hle_zx=new TH2F("hle_zx","low energy z vs x (top view)",N_STRIP_X,0,N_STRIP_X,N_DSSD,0,N_DSSD);
    hle_yz=new TH2F("hle_yz","low energy y vs z (side view)",N_DSSD,0,N_DSSD,N_STRIP_Y,0,N_STRIP_Y);
    hhe_z=new TH1F("hhe_z","implantation profile z",N_DSSD,0,N_DSSD);
    hhe_zx=new TH2F("hhe_zx","high energy z vs x (top view)",N_STRIP_X,0,N_STRIP_X,N_DSSD,0,N_DSSD);
    hhe_yz=new TH2F("hhe_yz","high energy y vs z (side view)",N_DSSD,0,N_DSSD,N_STRIP_Y,0,N_STRIP_Y);

    for (int i=0;i<N_DSSD*(N_STRIP_X+N_STRIP_Y);i++){
        le_dssd_thr[i]=CH_THRESHOLD;
        le_dssd_cal0[i]=0.;
        le_dssd_cal1[i]=1.;

        he_dssd_thr[i]=CH_THRESHOLD;
        he_dssd_cal0[i]=0.;
        he_dssd_cal1[i]=1.;
    }
    //! default mapping
    Int_t strip=0;
    for (int z=0;z<N_DSSD;z++){
        for (int xy=0;xy<N_STRIP_X+N_STRIP_Y;xy++){
            strip=z*(N_STRIP_X+N_STRIP_Y)+xy;
            if (xy<N_STRIP_X){
                striptoch[strip]=z*N_STRIP_X+xy;
		striptoch_he[strip]=z*N_STRIP_X+xy;
            }else{//Y strips
	      if ((xy-N_STRIP_X)<8){// correct mapping
		  striptoch[strip]=z*N_STRIP_Y+N_STRIP_X/2-1-xy+N_STRIP_X+V1740_N_CH;
		  striptoch_he[strip]=z*N_STRIP_Y+N_STRIP_X/2-1-xy+N_STRIP_X+V1740_N_CH;
	      }else{
		  striptoch[strip]=z*N_STRIP_Y+xy-N_STRIP_X+V1740_N_CH;
		  striptoch_he[strip]=z*N_STRIP_Y+xy-N_STRIP_X+V1740_N_CH;
	      }
            }

        }
    }
    //! read mapping from file
    ReadConfigTable("dssdconfiglowe.txt");
    ReadConfigTableHighE("dssdconfighighe.txt");
   
    for (int i=0;i<N_DSSD*(N_STRIP_X+N_STRIP_Y);i++){
        le_chtostrip[striptoch[i]]=i;
        he_chtostrip[striptoch_he[i]]=i;
    }

    c1->Divide(3,3);
    c1->cd(1);
    gPad->SetGridy(2);
    hhe_zx->Draw("colz");
    hhe_zx->GetYaxis()->SetNdivisions(4);
    c1->cd(2);
    gPad->SetGridx(2);
    hhe_yz->Draw("colz");
    hhe_yz->GetXaxis()->SetNdivisions(4);
    c1->cd(3);
    hhe_z->Draw();
    hhe_z->GetXaxis()->SetNdivisions(4);
    c1->cd(4);
    gPad->SetGridy(2);
    hle_zx->Draw("colz");
    hle_zx->GetYaxis()->SetNdivisions(4);
    c1->cd(5);
    gPad->SetGridx(2);
    hle_yz->Draw("colz");
    hle_yz->GetXaxis()->SetNdivisions(4);
    c1->cd(6);
    hle_z->Draw();
    hle_z->GetXaxis()->SetNdivisions(4);
    c1->cd(7);
    he2d->Draw("colz");
    c1->cd(8);
    hratedgtz->Draw();
    c1->cd(9);
    ht2d->Draw("colz");
}

void ProcessEvent(NIGIRI* data){
    if (data->evt_type==V1740_EVENT_TYPE){
        int b = data->b;
        c1updatetrgcnt++;
        hcleartrgcnt++;

        //! for constructor
        Int_t le_ndssdhits[N_DSSD];
        Int_t he_ndssdhits[N_DSSD];
        memset(le_ndssdhits,0,sizeof(le_ndssdhits));
        memset(he_ndssdhits,0,sizeof(he_ndssdhits));

        Int_t maxz_he=-1;
        for (int i=0;i<V1740_N_CH;i++){
            NIGIRIHit* chdata=data->GetHit(i);
            int ch  = b*V1740_N_CH + chdata->ch;
            //!fill in waveform and energy
            if (chdata->clong>0) {
                ht2d->Fill(ch,chdata->finets);
                //he2d->Fill(ch,chdata->clong);
		he2dmax->Fill(ch,chdata->clong+chdata->baseline);
                hbl2d->Fill(ch,chdata->baseline);
            }
            int itcnt=0;
            for (std::vector<UShort_t>::iterator it = chdata->pulse.begin() ; it != chdata->pulse.end(); ++it){
                if (itcnt<N_MAX_WF_LENGTH){
		  //if (chdata->clong>chthr[ch]) hwf1d[ch]->SetBinContent(itcnt+1,*it);
		    hwf1d[ch]->SetBinContent(itcnt+1,*it);		  
                    hwf2d[ch]->Fill(itcnt,*it);
                }
                itcnt++;
            }

            dgtzcnt_current[ch]++;
            /*
            //! digitizer counter rate meter
            if (ch<2*V1740_N_CH) {//le
                Int_t str=le_chtostrip[ch];
                if (chdata->clong>le_dssd_thr[str]) {
                    dgtzcnt_current[ch]++;
                }
            }else{//he
                Int_t str=he_chtostrip[ch-2*V1740_N_CH];
                if (chdata->clong>he_dssd_thr[str]) {
                    dgtzcnt_current[ch]++;
                }
            }
            */
            if (dgtzcnt_current[ch]!=dgtzcnt_prev[ch]&&dgtzcnt_current[ch]%COUNTS_TO_CAL==0&&dgtzcnt_current[ch]>0){
                if (dgtzcnt_prev[ch]>0){
                    double rate=((double)(dgtzcnt_current[ch]-dgtzcnt_prev[ch]))/((double)(data->ts-dgtzts_prev[ch]))/DGTZ_CLK_RES*1e9;
                    //cout<<ch<<"-"<<rate<<"-"<<ch<<chdata->clong<<"-"<<dgtzcnt_current[ch]<<endl;
                    //cout<<"update!"<<ch<<"-"<<dgtzcnt_current[ch]-dgtzcnt_prev[ch]<<"-"<<data->ts-dgtzts_prev[ch]<<endl;
                    if (rate>0) hratedgtz->SetBinContent(ch+1,rate);
                }
                dgtzts_prev[ch] = data->ts;
                dgtzcnt_prev[ch] = dgtzcnt_current[ch];
            }

            //! sorter
            if (ch<2*V1740_N_CH) {//low e
                Int_t str=le_chtostrip[ch];
                Int_t z=str/(N_STRIP_X+N_STRIP_Y);
                Int_t xy=str%(N_STRIP_X+N_STRIP_Y);
                Double_t e=chdata->clong*le_dssd_cal1[str]+le_dssd_cal0[str];
                if (chdata->clong>le_dssd_thr[str]){
                    if (xy<N_STRIP_X){
                        hle_zx->Fill(xy,z);
                        le_ndssdhits[z]++;                        
                    }else{
                        hle_yz->Fill(z,xy-N_STRIP_X);
                    }
                }
		if (chdata->clong>0) {
		   he2d->Fill(ch,e);
		}
            }else{
                Int_t str=he_chtostrip[ch-2*V1740_N_CH];
                Int_t z=str/(N_STRIP_X+N_STRIP_Y);
                Int_t xy=str%(N_STRIP_X+N_STRIP_Y);
                Double_t e=chdata->clong*he_dssd_cal1[str]+he_dssd_cal0[str];
                if (chdata->clong>he_dssd_thr[str]){
                    if (xy<N_STRIP_X){
                        hhe_zx->Fill(xy,z);
                        he_ndssdhits[z]++;
                        maxz_he=z;
                    }else{
                        hhe_yz->Fill(z,xy-N_STRIP_X);
                    }
                }
		if (chdata->clong>0) {
		   he2d->Fill(ch,e);
		}
            }
        }//for all ch

        for (int iz=0;iz<N_DSSD;iz++){
            if (le_ndssdhits[iz]>0) hle_z->Fill(iz);
            //if (he_ndssdhits[iz]>0) hhe_z->Fill(iz);
        }
        hhe_z->Fill(maxz_he);
    }

    if (c1updatetrgcnt>COUNTS_TO_DISPLAY){
        for (int i=0;i<9;i++){
            c1->cd(i+1)->Modified();
            c1->cd(i+1)->Update();
        }
        c1updatetrgcnt=0;
    }
    /*
    if (hcleartrgcnt>COUNTS_TO_CLEAR){
        ClearHist();
        hcleartrgcnt=0;
    }
    */
}

void CloseMe(){
    TFile* f=new TFile("outhist.root","RECREATE");
    for (int i=0;i<V1740_N_CH*V1740_N_BOARD;i++){
        hwf1d[i]->Write();
        hwf2d[i]->Write();
    }
    ht2d->Write();
    he2d->Write();
    he2dmax->Write();
    hbl2d->Write();
    hratedgtz->Write();
    hle_z->Write();
    hle_zx->Write();
    hle_yz->Write();
    hhe_z->Write();
    hhe_zx->Write();
    hhe_yz->Write();
    f->Close();
    ClearHist();
    cout<<"Histograms saved and cleared!"<<endl;
}




