//
// fileEventIterator   mlp 4/19/1997
//
// this iterator reads events froma data file. 


#include <fileEventiterator.h>
#include <stddef.h>
#include <string.h>
//#include <sys/types.h>
#include <unistd.h>

// there are two similar constructors, one with just the
// filename, the other with an additional status value
// which is non-zero on return if anything goes wrong. 


fileEventiterator::~fileEventiterator()
{
     if (fp != NULL) fclose (fp);
     if (thefilename != NULL) delete thefilename;
     if (bp != NULL ) delete [] bp;
     if (bptr != NULL ) delete bptr;
}  


fileEventiterator::fileEventiterator(const char *filename)
{
  fp = fopen (filename,"r");
  bptr = 0;
  bp = 0;
  allocatedsize = 0;
  if (fp != NULL) 
    {
      thefilename = new char[strlen(filename)+1];
      strcpy (thefilename, filename);
      last_read_status = 0;
      current_index = 0;
    }
  else
    last_read_status = 1;

}  

fileEventiterator::fileEventiterator(const char *filename, int &status)
{
  fp = fopen (filename,"r");
  bptr = 0;
  bp = 0;
  allocatedsize = 0;
  if (fp != NULL) 
    {
      thefilename = new char[strlen(filename)+1];
      strcpy (thefilename, filename);
      status = 0;
      last_read_status = 0;
      current_index = 0;
    }
  else
    {
      status = 1;
      last_read_status = 1;
    }
}  


void  
fileEventiterator::identify (std::ostream &os) const
{ 
  os << getIdTag() << std::endl;

};

const char *  
fileEventiterator::getIdTag () const
{ 
  sprintf (idline, " -- fileEventiterator reading from %s", thefilename);
  return idline;
};



// and, finally, the only non-constructor member function to
// retrieve events from the iterator.

Event *
fileEventiterator::getNextEvent()
{
  Event *evt = 0;


  // if we had a read error before, we just return
  if (last_read_status) return NULL;

  // see if we have a buffer to read
  if (bptr == 0) 
    {
      if ( (last_read_status = read_next_buffer()) !=0 )
	{
	  return NULL;
	}
    }

  while (last_read_status == 0)
    {
      if (bptr) evt =  bptr->getEvent();
      if (evt) return evt;

      last_read_status = read_next_buffer();
    }

  return NULL;

}

// -----------------------------------------------------
// this is a private function to read the next buffer
// if needed. 

int fileEventiterator::read_next_buffer()
{
  int status;
  int ip = 8192;

  if (bptr) 
    {
      delete bptr;
      bptr = 0;
    }

  // set the pointer to char to the destination buffer
  char *cp = (char *) initialbuffer;

  // read the first record
  status = fread ( cp, 8192, 1, fp);

  // error of EoF?
  if ( feof(fp) || ferror(fp) ) return -1;

  // get the length into a dedicated variable
  if (initialbuffer[1] == BUFFERMARKER || initialbuffer[1]== GZBUFFERMARKER ) 
      buffer_size = initialbuffer[0];
  else
    buffer_size = buffer::i4swap(initialbuffer[0]);
  COUT << "buffer size: " << buffer_size << std::endl;

  int i;
  if (bp) 
    {
      if  (buffer_size > allocatedsize*4)
	{
	  delete [] bp;
	  i = (buffer_size +BUFFERBLOCKSIZE-1) /BUFFERBLOCKSIZE;
	  allocatedsize = i * BUFFERBLOCKSIZE/4;
	  bp = new PHDWORD[allocatedsize];
	}
    }
  else
    {
      i = (buffer_size +BUFFERBLOCKSIZE-1) /BUFFERBLOCKSIZE;
      allocatedsize = i * BUFFERBLOCKSIZE/4;
      bp = new PHDWORD[allocatedsize];
      //      COUT << "allocating buffer, size is " << allocatedsize <<std::endl;
    }
  for (i = 0; i<BUFFERBLOCKSIZE/4; i++ ) bp[i] = initialbuffer[i];

  cp = (char *) bp;

  // and update the destination buffer pointer
  cp += BUFFERBLOCKSIZE;

  PHDWORD read_so_far =  BUFFERBLOCKSIZE;

  // now we read records until the whole buffer is read 
  while ( ip < buffer_size)
    {
      //   COUT << "ip is " << ip << std::endl;
      // read the next record
      status = fread ( cp, BUFFERBLOCKSIZE, 1, fp);
      if ( feof(fp) || ferror(fp) ) 
	{
	  COUT << "error in buffer, salvaging" << std::endl;
	  bp[0] = read_so_far; 
	  break;
	}

      // update the pointer and byte count
      cp += BUFFERBLOCKSIZE;
      ip += BUFFERBLOCKSIZE;
      read_so_far += BUFFERBLOCKSIZE;
    }

  // and initialize the current_index to be the first event
  if ( initialbuffer[1]== GZBUFFERMARKER || buffer::i4swap(initialbuffer[1])== GZBUFFERMARKER )
    {
      bptr = new gzbuffer ( bp, allocatedsize );
    }
  else
    {
      bptr = new buffer ( bp, allocatedsize );
    }
  return 0;
}
