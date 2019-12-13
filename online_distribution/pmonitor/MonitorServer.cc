#include "MonitorServer.h"

#include <iostream>
#include <TH1.h>
#include <TH2.h>

#include <TMessage.h>
#include <TServerSocket.h>
#include <TSocket.h>

using namespace std;

MonitorServer *MonitorServer::__instance = NULL;
int MonitorServer::verbosity = 0;

pthread_mutex_t MonitorServer::mutex;

MonitorServer *MonitorServer::instance()
{
  if (__instance)
    {
      return __instance;
    }
  __instance = new MonitorServer();
  return __instance;
}

MonitorServer::MonitorServer()
{
  pthread_mutex_init(&mutex, NULL);
  //  pthread_mutex_lock(&mutex);


  int status  = pthread_create(&serverthreadid, 0, ServerThread, (void *) 0);
  if (status ) 
    {
      serverthreadid=0;
      cout << __FILE__ << " " << __LINE__ << " Error in starting thread" << endl;
    }

}

void * MonitorServer::ServerThread(void *arg)
{

  if ( verbosity) 
  cout << __FILE__ << " " << __LINE__ << " ServerThread started"<< endl;

  TServerSocket *ss =  new TServerSocket(MONIPORT, kTRUE);
  if (!ss->IsValid())
    {
      cout << __FILE__ << " " << __LINE__ << " ServerThread failed to open moonitoring port"<< endl;
      delete ss;

      return 0;

    }


  while (1)
    {
      
      TSocket *s0 = ss->Accept();
      if (!s0)
	{
	  cout << __FILE__ << " " << __LINE__ << " ServerThread failed to accept"<< endl;
	  delete ss;
	  return 0;
	}
      
      if (verbosity)
	{
	  TInetAddress adr = s0->GetInetAddress();
	  cout << "got connection from " << endl;
	  adr.Print();
      
	  cout << __FILE__ << " " << __LINE__ << " in MonServer::ServerThread "<< endl;
      
	}
      pthread_mutex_lock(&mutex);
      HandleConnection(s0);
      pthread_mutex_unlock(&mutex);
      delete s0;
    }
}

int MonitorServer::HandleConnection(TSocket *s0)
{

  MonitorServer *ms = MonitorServer::instance();

  TMessage *mess = 0;
  TMessage outgoing(kMESS_OBJECT);
  int cont=1;
  while (cont)
    {
      
      s0->Recv(mess);
      if (! mess)
        {
          cout << "Broken Connection, closing socket" << endl;
          break;
        }

      if (mess->What() == kMESS_STRING)
        {
          char str[64];
          mess->ReadString(str, 64);
          delete mess;
          mess = 0;

          if (!strcmp(str, "Finished"))
            {
	      cont = 0;
            }

          else if (!strcmp(str, "Ack"))
            {
              continue;
            }
          else if (!strcmp(str, "getTH1"))
            {

	      int nbytes = s0->Recv(mess);
	      char hn[64];
	      mess->ReadString(hn, 64);
	      delete mess;
	      mess = 0;

	      TH1 *histo = ms->getHisto1(hn);
              if (histo)
                {
                  outgoing.Reset();
                  outgoing.WriteObject(histo);
                  s0->Send(outgoing);
                  outgoing.Reset();
                }
              else
                {
                  s0->Send("UnknownHisto");
                }
	    }

	  else if (!strcmp(str, "getTH2"))
            {
	      
	      int nbytes = s0->Recv(mess);
	      char hn[64];
	      mess->ReadString(hn, 64);
	      delete mess;
	      mess = 0;

	      TH2 *histo = ms->getHisto2(hn);
              if (histo)
                {
                  outgoing.Reset();
                  outgoing.WriteObject(histo);
                  s0->Send(outgoing);
                  outgoing.Reset();
                }
              else
                {
		  s0->Send("UnknownHisto");
                }
	    }


	  else if (!strcmp(str, "Reset"))
            {
	      
	      int nbytes = s0->Recv(mess);
	      char hn[64];
	      mess->ReadString(hn, 64);
	      delete mess;
	      mess = 0;

	      int status = ms->Reset(hn);

	      if ( status) 
		{
		  s0->Send("UnknownHisto");
		}
	      else
		{
		  s0->Send("Ack");
		}

	    }


	  else if (!strcmp(str, "ResetAll"))
            {
	      
	      int status = ms->ResetAll();

	      if ( status) 
		{
		  s0->Send("Error");
		}
	      else
		{
		  s0->Send("Ack");
		}

	    }

	  else if (!strcmp(str, "Histo1List"))
	    {
	      if ( ms->n1Histos()) 
		{
		  string out = ms->getHistoName1(0);
		  for (unsigned int i = 1; i < ms->n1Histos(); i++)
		    {
		      string x = "\n";
		      out += x;
		      out += ms->getHistoName1(i);
		    }
		  s0->Send(out.c_str());
		}
	      else
		{
		  s0->Send(" ");
		}
	    }

	  else if (!strcmp(str, "Histo2List"))
	    {
	      if ( ms->n2Histos()) 
		{
		  string out = ms->getHistoName2(0);
		  for (unsigned int i = 1; i < ms->n2Histos(); i++)
		    {
		      string x = "\n";
		      out += x;
		      out += ms->getHistoName2(i);
		    }
		  s0->Send(out.c_str());
		}
	      else
		{
		  s0->Send(" ");
		}
	    }


	}

    }
      
  return 0;
}
  
int MonitorServer::registerHisto(TH1 *h1d)
{
  // does this thing exist already?
  map<const string, TH1 *>::const_iterator histoiter = Histo1.find(h1d->GetName());
  if (histoiter != Histo1.end())
    {
      cout << "Histogram " <<  h1d->GetName() << " already registered" << endl;
      return -1;
    }

  Histo1[h1d->GetName()] = h1d;
  return 0;
}

int MonitorServer::registerHisto(TH2 *h2d)
{
  // does this thing exist already?
  map<const string, TH2 *>::const_iterator histoiter = Histo2.find(h2d->GetName());
  if (histoiter != Histo2.end())
    {
      cout << "Histogram " <<  h2d->GetName() << " already registered" << endl;
      return -1;
    }

  Histo2[h2d->GetName()] = h2d;
  return 0;
}

int MonitorServer::unregisterHisto(TH1 *h1d)
{
  // does this thing exist already?
  map<const string, TH1 *>::const_iterator histoiter = Histo1.find(h1d->GetName());
  if (histoiter != Histo1.end())
    {
      cout << "unregistering Histogram " <<  h1d->GetName() << endl;
      Histo1.erase(h1d->GetName());
    }
  return 0;
}

int MonitorServer::unregisterHisto(TH2 *h2d)
{
  // does this thing exist already?
  map<const string, TH2 *>::const_iterator histoiter = Histo2.find(h2d->GetName());
  if (histoiter != Histo2.end())
    {
      cout << "unregistering Histogram " <<  h2d->GetName() << endl;
      Histo2.erase(h2d->GetName() );
    }
  return 0;
}





TH1 *MonitorServer::getHisto1(const string &hname) const
{
  map<const string, TH1 *>::const_iterator histoiter = Histo1.find(hname);
  if (histoiter != Histo1.end())
    {
      return histoiter->second;
    }
  //  cout << "Histogram " << hname << " not found" << endl;
  return 0;
  
}

TH2 *MonitorServer::getHisto2(const string &hname) const
{
  map<const string, TH2 *>::const_iterator histoiter = Histo2.find(hname);
  if (histoiter != Histo2.end())
    {
      return histoiter->second;
    }
  //  cout << "Histogram " << hname << " not found" << endl;
  return 0;
  
  return NULL;
}

const char *MonitorServer::getHistoName1(const unsigned int ihisto) const
{
  map<const string, TH1 *>::const_iterator histoiter = Histo1.begin();
  advance ( histoiter, ihisto);
  if (histoiter != Histo1.end())
    {
      return histoiter->first.c_str();
    }
  cout << "Histogram " << ihisto << " not found" << endl;
  return 0;
  
}

const char *MonitorServer::getHistoName2(const unsigned int ihisto) const
{
  map<const string, TH2 *>::const_iterator histoiter = Histo2.begin();
  advance ( histoiter, ihisto);
  if (histoiter != Histo2.end())
    {
      return histoiter->first.c_str();
    }
  cout << "Histogram " << ihisto << " not found" << endl;
  return 0;
  
}



int MonitorServer::Reset(const char *hname) const
{
 
  TH1 *h1;
  TH2 *h2;

  if ( (h1 = getHisto1(hname)) )
    {
      h1->Reset();
      return 0;
    }

  if ( (h2 = getHisto2(hname)) )
    {
      h2->Reset();
      return 0;
    }

  return -1;
}


int MonitorServer::ResetAll() const
{
  map<const string, TH1 *>::const_iterator histoiter = Histo1.begin();
  for (; histoiter != Histo1.end(); ++histoiter)
    {
      histoiter->second->Reset();
    }
  
  map<const string, TH2 *>::const_iterator histoiter2 = Histo2.begin();
  for (; histoiter2 != Histo2.end(); ++histoiter2)
    {
      histoiter2->second->Reset();
    }
  
  return 0;
  
}




void MonitorServer::identify(std::ostream& os ) const
{
  
  os << "MonitorServer with the following histograms:"  << endl;

  map<const string, TH1 *>::const_iterator histoiter1 = Histo1.begin();
  for ( ; histoiter1 != Histo1.end(); ++histoiter1)
    {
      os << " 1d:  " << histoiter1->first << endl;
    }

  map<const string, TH2 *>::const_iterator histoiter2 = Histo2.begin();
  for ( ; histoiter2 != Histo2.end(); ++histoiter2)
    {
      os << " 2d:  " << histoiter2->first << endl;
    }
  os << endl;

}
