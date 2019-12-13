#ifndef __MONITORSERVER_H
#define __MONITORSERVER_H


#include <map>
#include <iostream>
#include <string>
#include <pthread.h>

#define MONIPORT 1999

class TH1;
class TH2;
class TSocket;

class MonitorServer
{
  public:
  static MonitorServer *instance();
  static int HandleConnection(TSocket*);

  virtual ~MonitorServer(){};

  virtual int registerHisto(TH1 *h1d);
  virtual int unregisterHisto(TH1 *h1d);

  virtual int registerHisto(TH2 *h2d);
  virtual int unregisterHisto(TH2 *h2d);
  virtual pthread_t getServerThreadId() const { return serverthreadid;};


  virtual const char *getHistoName1(const unsigned int ihisto) const;
  virtual const char *getHistoName2(const unsigned int ihisto) const;

  virtual unsigned int n1Histos() const {return Histo1.size();}
  virtual unsigned int n2Histos() const {return Histo2.size();}

  virtual TH1 *getHisto1(const std::string &hname) const;
  virtual TH2 *getHisto2(const std::string &hname) const;

  int Reset(const char *hname) const;
  int ResetAll() const;

  virtual void identify(std::ostream& os = std::cout) const;
  virtual void setVerbosity ( const int i) { verbosity = i;};
  virtual int getVerbosity () const {return verbosity;};

 protected:

  MonitorServer();


  static MonitorServer *__instance;
  std::map<const std::string, TH1*> Histo1;
  std::map<const std::string, TH2*> Histo2;

  static void* ServerThread (void *arg);

#ifndef __CINT__
  static pthread_mutex_t mutex;
  pthread_t serverthreadid;
#endif

  static int verbosity;

};

#endif
