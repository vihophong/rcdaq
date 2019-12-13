// -*- c++ -*-
#ifndef __MSGQUEUE_H__
#define __MSGQUEUE_H__

#include <cstddef>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>
#include <cerrno>
#include <iostream>

// 

class msgqueue {

public:
  // destructor
  virtual ~msgqueue();

  msgqueue ( char * token, const int id, 
	     int &status, const int inew =0);

  int receive( struct msgbuf * structure, int size);  
  int receive_nowait( struct msgbuf * structure, int size);
  int send( struct msgbuf * structure, int size);
  int wait_for_ping();
  int send_ping();

protected:
  int tokenid;
  int msgid;
  int wasnew;
};

#endif /* __MSGQUEUE_H__ */
