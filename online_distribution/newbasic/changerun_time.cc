#include <A_Event.h>
#include <packet.h>
#include <Cframe.h>
#include <framePackets.h>
#include <dataBlock.h>

#include <frameRoutines.h>
#include <frameHdr.h>

//#include <PHTimeStamp.h>





#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fileEventiterator.h>
#include <testEventiterator.h>
//#include <ddEventiterator.h>
//#include <dd_factory.h>
#include <phenixTypes.h>
#include <oBuffer.h>
#include <oEvent.h>
#include <stdio.h>

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#define DDEVENTITERATOR 1
#define FILEEVENTITERATOR 2
#define TESTEVENTITERATOR 3
#define DDPOOL 4
#define DFILE 5
#define DNULL 6




class X_Event : public A_Event 
{

public:
  // constructors and destructors
  X_Event( PHDWORD *, const int runnumber, const time_t tt);
  X_Event( int *, const int runnumber, const time_t tt);
  ~X_Event();

   // info & debug utils
   
};

X_Event::X_Event (PHDWORD *data, const int runnumber, const time_t tt)
  : A_Event(data)
{ 
  EventData->run_number=runnumber;
  PHTimeStamp p(tt);
  phtime_t *xx =  (phtime_t *) &EventData->date;
  *xx = p.getBinaryTime();
}

 

X_Event::X_Event (int *data, const int runnumber, const time_t tt)
  : A_Event(data)
{ 
  EventData->run_number=runnumber;
  PHTimeStamp p(tt);
  phtime_t *xx =  (phtime_t *) &EventData->date;
  *xx = p.getBinaryTime();
}


X_Event::~X_Event ()
{ }




#if defined(SunOS) || defined(Linux) || defined(OSF1)
void sig_handler(int);
#else
void sig_handler(...);
#endif


void exitmsg()
{
  COUT << "** usage: changerun_time infile outfile newrunnumber [newtime] " << std::endl;
  COUT << "   if newtime is omitted then time(0)  -- now -- is used." << std::endl;

  exit(0);
}

void exithelp()
{
  COUT <<  std::endl;

}

// The global pointer to the Eventiterator (we must be able to 
// get at it in the signal handler) 
Eventiterator *it;



int 
main(int argc, char *argv[])
{
  int c;
  int status = 0;

  int eventnr = 0;

  extern char *optarg;
  extern int optind;

  PHDWORD  *buffer;
  oBuffer *ob;
  FILE *fp;
  int buffer_size = 2000000;

  if (argc <3) exitmsg();


  while ((c = getopt(argc, argv, "s:d:n:w:vhi")) != EOF)
    {
      switch (c) 
	{
	  // the -s (source type) switch
	case 'h':
	  exithelp();
	  break;

	default:
	  break;
	}
    }



  // install some handlers for the most common signals
  signal(SIGKILL, sig_handler);
  signal(SIGTERM, sig_handler);
  signal(SIGINT,  sig_handler);

  // see if we can open the file

  if (argc <= optind + 2 ) exitmsg();


  it = new fileEventiterator(argv[optind], status);
       

  if (status)
    {
      delete it;
      COUT << "Could not open input stream" << std::endl;
      exit(1);
    }

  fp = fopen (argv[optind+1], "w");
  if (!fp) 
    {
      COUT << "Could not open file: " <<  argv[optind+1] << std::endl;
      exit (1);
    }

  buffer = new PHDWORD [buffer_size];
  
  ob = new oBuffer (fp, buffer, buffer_size);


  int newrunnumber;
  time_t newtime; 
  sscanf(argv[optind + 2 ]    , "%d", &newrunnumber ); 

  
  if (argc == optind +4  ) // yes we have a time
    {
      sscanf(argv[optind + 3 ]    , "%d", &newtime);
    }
  else
    {
      newtime = time(0);
    }
  
  COUT << "Using run number " << newrunnumber << " and time " << ctime(&newtime) << std::endl;

	

  // ok. now go through the events

  Event *evt;
  X_Event *nevt;

  int *eb;
  int nw;
  int j;
  while (  ( evt = it->getNextEvent())  )
    {


      eb = new int [evt->getEvtLength() +100];
      evt->Copy (eb, evt->getEvtLength() +100, &nw);
      nevt = new X_Event(eb, newrunnumber, newtime);

      ob->addEvent(nevt);


      eventnr++;

      delete evt;
      delete nevt;
      delete [] eb;

    }
  delete it;


  delete ob;
  fclose(fp);
      
  return 0;
}
	

#if defined(SunOS) || defined(Linux) || defined(OSF1)
void sig_handler(int i)
#else
  void sig_handler(...)
#endif
{
  COUT << "sig_handler: signal seen " << std::endl;
  if (it) delete it;
  exit(0);
}


  
