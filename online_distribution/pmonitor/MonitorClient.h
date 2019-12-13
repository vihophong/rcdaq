#ifndef __MONITORCLIENT_H
#define __MONITORCLIENT_H


#include <map>
#include <iostream>
#include <string>

#define MONIPORT 1999

class TH1;
class TH2;

class MonitorClient
{
  public:
  static MonitorClient *instance(const char *host="localhost");

  virtual ~MonitorClient(){};

  virtual void setHost ( const char * host);

  //  virtual unsigned int n1Histos() const;
  //  virtual unsigned int n2Histos() const;

  virtual TH1 *getHisto1(const char *hname);
  virtual TH2 *getHisto2(const char *hname);

  const char *getHisto1Names();
  const char *getHisto2Names();

  virtual int updateAll();

  virtual int Reset(const char *hname) const;
  virtual int ResetAll() const;

  //  virtual void identify(std::ostream& os = std::cout) const;

 protected:

  MonitorClient(const char *hostname="localhost");

  static MonitorClient *__instance;

  std::string TheHost;

  std::map<const std::string, TH1*> Histo1;
  std::map<const std::string, TH2*> Histo2;


};

#endif
