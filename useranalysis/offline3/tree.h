//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Oct  5 09:52:51 2017 by ROOT version 5.34/36
// from TTree tree/tree
// found on file: ../test.root
//////////////////////////////////////////////////////////

#ifndef tree_h
#define tree_h

#include "aidaclass.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <fstream>
#include <vector>
#define V1740_HDR 6
#define V1740_N_CH 64
#define V1740_N_BOARD 4
#define V1740_PACKET_0 100
#define V1740_PACKET_1 101
#define V1740_PACKET_2 102
#define V1740_PACKET_3 103
#define V1740_N_MAX_CH 64
#define NSBL 8
#define N_REFESH_WF 1000
#define N_MAX_WF_LENGTH 500

#define COUNTS_TO_DISPLAY 10000
#define COUNTS_TO_CAL 100
#define MAX_N_SAMPLE 300

#define MAX_MAP_LENGTH 50



// default constant

#define CH_THRESHOLD 50
//DSSDs constant
#define N_DSSD 4
#define N_STRIP_X 16
#define N_STRIP_Y 16



// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

void syncrecodata(AIDAClass* desdata,AIDAClass* srcdata){
    desdata->E=srcdata->E;
    desdata->EX=srcdata->EX;
    desdata->EY=srcdata->EY;
    desdata->x=srcdata->x;
    desdata->y=srcdata->y;
    desdata->z=srcdata->z;
    desdata->nx=srcdata->nx;
    desdata->ny=srcdata->ny;
    desdata->nz=srcdata->nz;
    desdata->T=srcdata->T;
    desdata->Tfast=srcdata->Tfast;
    desdata->ID=srcdata->ID;
}
void clearrecodata(AIDAClass* desdata){
    desdata->E=-9999.;
    desdata->EX=-9999.;
    desdata->EY=-9999.;
    desdata->x=-9999.;
    desdata->y=-9999.;
    desdata->z=-9999.;
    desdata->nx=-9999;
    desdata->ny=-9999;
    desdata->nz=-9999;
    desdata->T=0;
    desdata->Tfast=0;
    desdata->ID=0;
}

class tree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           evt[V1740_N_CH*V1740_N_BOARD];
   Double_t        dgtz_e[V1740_N_CH*V1740_N_BOARD];
   Double_t        dgtz_bl[V1740_N_CH*V1740_N_BOARD];
   Int_t           dgtz_ch[V1740_N_CH*V1740_N_BOARD];
   UShort_t        dgtz_nsample[V1740_N_CH*V1740_N_BOARD];
   ULong64_t       dgtz_ts[V1740_N_CH*V1740_N_BOARD];
   UShort_t        dgtz_waveform[V1740_N_CH*V1740_N_BOARD][300];
   UShort_t        dgtz_sample[V1740_N_CH*V1740_N_BOARD][300];

   // List of branches
   TBranch        *b_evt;   //!
   TBranch        *b_dgtz_e;   //!
   TBranch        *b_dgtz_bl;   //!
   TBranch        *b_dgtz_ch;   //!
   TBranch        *b_dgtz_nsample;   //!
   TBranch        *b_dgtz_ts;   //!
   TBranch        *b_dgtz_waveform;   //!
   TBranch        *b_dgtz_sample;   //!


   unsigned char fid;
   Double_t           dssd_adc[N_DSSD*(N_STRIP_X+N_STRIP_Y)];
   Double_t           dssd_e[N_DSSD*(N_STRIP_X+N_STRIP_Y)];

   Double_t           dssd_thr[N_DSSD*(N_STRIP_X+N_STRIP_Y)];
   Double_t           dssd_cal0[N_DSSD*(N_STRIP_X+N_STRIP_Y)];
   Double_t           dssd_cal1[N_DSSD*(N_STRIP_X+N_STRIP_Y)];

   Int_t           striptoch[N_DSSD*(N_STRIP_X+N_STRIP_Y)];
   Bool_t          flag_mapping;

   tree(TFile* infilef,char* treename,TTree *tree=0);
   virtual ~tree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   //virtual void     Loop(char* outfile);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   void ReadConfigTable(char* configfile);

   //! reconstruction function
   void Reconstruction();

   //! remember to clear reco data
   void ClearRecoData();

   unsigned short GetNRecoData(){return nrecodata;}
   void SetStripThreshold(Int_t strip,Double_t thr){dssd_thr[strip]=thr;}
   AIDAClass* GetRecoData(unsigned short n) {return recodata.at(n);}

   void SetID(unsigned char id){fid=id;}
private:
   void GetPosMax();
   void GetPosMaxIon();

   void AddRecoData(AIDAClass* data){
       recodata.push_back(data);
       nrecodata++;
   }
   unsigned short nrecodata;
   std::vector<AIDAClass*> recodata;//pulse
};

#endif
