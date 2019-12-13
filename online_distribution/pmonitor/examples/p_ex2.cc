
#include <iostream>
#include "pmonitor.h"
#include <TH1.h>
#include <TH2.h>

using namespace std;

int init_done = 0;

TH1F *h1; 
TH2F *h2; 


int fill_h1;
int fill_h2;

int pinit()
{

  if (init_done) return 1;
  init_done = 1;

  fill_h1 = fill_h2 = 1;
      
  h1 = new TH1F("H1", "Channel 0", 101,-49.5,49.5);
      
  h2 = new TH2F("H2", "Channel 0 vs 1"
		, 64,-49.5,49.5
		, 64,-490.5,490.5
		);
  return 0;
}

int process_event (Event * e)
{
  //  static int i = 0;
  Packet *p = e->getPacket(1003);
  if (p)
    {
      if (fill_h1) h1->Fill (p->iValue(0));
      if (fill_h2) h2->Fill (p->iValue(0), p->iValue(1) );
      delete p;
    }
  return 0;
}


void h1enable()
{
  fill_h1 = 1;
}
void h1disable()
{
  fill_h1 = 0;
}

void h2enable()
{
  fill_h2 = 1;
}
void h2disable()
{
  fill_h2 = 0;
}
void h1status()
{
  if (fill_h1) cout << "H1 is enabled" << endl;
  else cout << "H1 is disabled" << endl;
}

void h2status()
{
  if (fill_h2) cout << "H2 is enabled" << endl;
  else cout << "H2 is disabled" << endl;
}


