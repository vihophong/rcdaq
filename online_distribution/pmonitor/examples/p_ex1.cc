
#include "pmonitor.h"
#include <TH1.h>


int init_done = 0;

TH1F *h;  // the one histogram


int pinit()
{
  if (init_done) return 1;
  init_done = 1;
  h = new TH1F("H1", "Channel 0", 101,-49.5,49.5);
  return 0;
}

int process_event (Event * e)
{
  Packet *p = e->getPacket(1003);
  if (p)
    {
      h->Fill (p->iValue(0));
      delete p;
    }
  return 0;
}
