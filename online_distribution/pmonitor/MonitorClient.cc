#include "MonitorClient.h"

#include <iostream>
#include <TH1.h>
#include <TH2.h>

#include <TMessage.h>
#include <TSocket.h>

using namespace std;

MonitorClient *MonitorClient::__instance = NULL;

MonitorClient *MonitorClient::instance( const char *host)
{
  if (__instance)
    {
      return __instance;
    }
  __instance = new MonitorClient(host);
  return __instance;
}

MonitorClient::MonitorClient(const char *host)
{
  TheHost = host;

}


void MonitorClient::setHost(const char *host)
{
  TheHost = host;
}


TH1 *MonitorClient::getHisto1(const char *hname)
{

  TH1 *hist = 0;
  

  TSocket sock(TheHost.c_str(), MONIPORT);
  TMessage *mess;
  sock.Send("getTH1"); // tell server that we request a TH1

  sock.Send(hname);

  sock.Recv(mess);
  if (! mess)  // if server is not up mess is NULL
    {
      cout <<  "Server not running on " << TheHost << endl;
      sock.Close();
      return 0;
    }

  if (mess->What() == kMESS_STRING)
    { 
      char str[64];
      mess->ReadString(str, 64);
      delete mess;
      
      if (!strcmp(str, "UnknownHisto"))
	{
	  sock.Send("Finished");          // tell server we are finished
	}
    }

  else if (mess->What() == kMESS_OBJECT)
    {
      TH1 *histo = (TH1 *) mess->ReadObjectAny(mess->GetClass());
      delete mess;
      
      map<const string, TH1 *>::const_iterator histoiter = Histo1.find(hname);
      if (histoiter != Histo1.end())
	{
	  hist=Histo1[hname];
	  hist->Reset();
	  hist->Add (histo);
	  
	}
      else
	{
	  hist = (TH1 *) histo->Clone(histo->GetName());
	  delete histo;
	  Histo1[hist->GetName()] = hist;
	}
      
    }
  sock.Send("Finished");          // tell server we are finished
  sock.Close();
  return hist;
 
}

TH2 *MonitorClient::getHisto2(const char *hname)
{

  TH2 *hist = 0;


  TSocket sock(TheHost.c_str(), MONIPORT);
  TMessage *mess;
  sock.Send("getTH2"); // tell server that we request a TH1

  sock.Send(hname);

  sock.Recv(mess);
  if (! mess)  // if server is not up mess is NULL
    {
      cout <<  "Server not running on " << TheHost << endl;
      sock.Close();
      return 0;
    }

  if (mess->What() == kMESS_STRING)
    { 
      char str[64];
      mess->ReadString(str, 64);
      delete mess;
      
      if (!strcmp(str, "UnknownHisto"))
	{
	  sock.Send("Finished");          // tell server we are finished
	  //  sock.Close();
	  //return 0;
	}
    }

  else if (mess->What() == kMESS_OBJECT)
    {
      TH2 *histo = (TH2 *) mess->ReadObjectAny(mess->GetClass());
      delete mess;

      map<const string, TH2 *>::const_iterator histoiter = Histo2.find(hname);
      if (histoiter != Histo2.end())
	{
	  hist=Histo2[hname];
	  hist->Reset();
	  hist->Add (histo);
	  
	}
      else
	{
	  hist = (TH2 *) histo->Clone(histo->GetName());
	  delete histo;
	  Histo2[hist->GetName()] = hist;
	}
      
    }

  sock.Send("Finished");          // tell server we are finished
  sock.Close();
  return hist;
}


const char *MonitorClient::getHisto1Names()
{
  static char str[8192*4];

  TSocket sock(TheHost.c_str(), MONIPORT);
  TMessage *mess;
  sock.Send("Histo1List"); // we want the list of all TH1's names

  sock.Recv(mess);
  if (! mess)  // if server is not up mess is NULL
    {
      cout <<  "Server not running on " << TheHost << endl;
      sock.Close();
      return 0;
    }

  if (mess->What() == kMESS_STRING)
    { 
      mess->ReadString(str, 8192*4);
      delete mess;
      sock.Send("Finished");          // tell server we are finished
      sock.Close();
      return str;
    }

  sock.Send("Finished");          // tell server we are finished
  sock.Close();
  return 0;
 
}

const char *MonitorClient::getHisto2Names()
{
  static char str[8192*4];

  TSocket sock(TheHost.c_str(), MONIPORT);
  TMessage *mess;
  sock.Send("Histo2List"); // we want the list of all TH1's names

  sock.Recv(mess);
  if (! mess)  // if server is not up mess is NULL
    {
      cout <<  "Server not running on " << TheHost << endl;
      sock.Close();
      return 0;
    }

  if (mess->What() == kMESS_STRING)
    { 
      mess->ReadString(str, 8192*4);
      delete mess;
      sock.Send("Finished");          // tell server we are finished
      sock.Close();
      return str;
    }

  sock.Send("Finished");          // tell server we are finished
  sock.Close();
  return 0;
 
}


int MonitorClient::Reset(const char *hname) const
{

  int status =-1; // bad return return status unless we change it
  
  TSocket sock(TheHost.c_str(), MONIPORT);
  TMessage *mess;
  sock.Send("Reset"); // tell server that we request a Reset

  sock.Send(hname);

  sock.Recv(mess);
  if (! mess)  // if server is not up mess is NULL
    {
      cout <<  "Server not running on " << TheHost << endl;
      sock.Close();
      return status;
    }

  if (mess->What() == kMESS_STRING)
    { 
      char str[64];
      mess->ReadString(str, 64);
      delete mess;
      
      if (!strcmp(str, "Ack"))
	{
	  status =0;
	}

    }

  sock.Send("Finished");          // tell server we are finished
  sock.Close();
  return status;
 
}

int MonitorClient::ResetAll() const
{

  int status =-1; // bad return return status unless we change it
  
  TSocket sock(TheHost.c_str(), MONIPORT);
  TMessage *mess;
  sock.Send("ResetAll"); // tell server that we request a ResetAll

  sock.Recv(mess);
  if (! mess)  // if server is not up mess is NULL
    {
      cout <<  "Server not running on " << TheHost << endl;
      sock.Close();
      return status;
    }

  if (mess->What() == kMESS_STRING)
    { 
      char str[64];
      mess->ReadString(str, 64);
      delete mess;
      
      if (!strcmp(str, "Ack"))
	{
	  status =0;
	}

    }

  sock.Send("Finished");          // tell server we are finished
  sock.Close();
  return status;
 
}


int MonitorClient::updateAll()
{


  map<const string, TH1 *>::const_iterator histoiter = Histo1.begin();
  for ( ; histoiter != Histo1.end(); ++histoiter)
    {

      getHisto1(histoiter->first.c_str());
      
    }

  map<const string, TH2 *>::const_iterator histoiter2 = Histo2.begin();
  for ( ; histoiter2 != Histo2.end(); ++histoiter2)
    {

      getHisto2(histoiter2->first.c_str());
      
    }
  return 0;
}
