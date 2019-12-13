#include <buffer.h>
#include <A_Event.h>
#include <Cframe.h>
#include <stdio.h>

// the constructor first ----------------

buffer::buffer (){}

buffer::buffer (PHDWORD *array , const int length )
{

  bptr =  (buffer_ptr) array;
  data_ptr = &(bptr->data[0]);
  max_length = length;
  current_index = 0;

  if (bptr->ID != BUFFERMARKER) 
  {
    //    COUT << " will swap the buffer " << std::endl;
    int id = i4swap(bptr->ID);
    if (id != BUFFERMARKER) 
      {
	COUT << " wrong buffer" << std::endl;
	return;
      }
    if ( buffer_swap())  COUT << "problem in buffer swap" << std::endl;
  }
  buffer_size = array[0];
  COUT << " buffer sequence " << bptr->Bufseq<< std::endl;
}


buffer::~buffer()
{}

int buffer::buffer_swap()
{
 
  int i,  evtindex, frameindex;
  evtdata_ptr evtptr;
  PHDWORD *frameptr;

  //   swap the buffer header
  bptr->Length =  i4swap ( bptr->Length);
  bptr->ID =  i4swap ( bptr->ID);
  bptr->Bufseq =  i4swap ( bptr->Bufseq);
  bptr->Runnr =  i4swap ( bptr->Runnr);

  evtindex = 0;

  while (evtindex < bptr->Length - BUFFERHEADERLENGTH)
    {
      // COUT << "evt index " << evtindex << std::endl;

      // map event header on data
      evtptr = (  evtdata_ptr ) &bptr->data[evtindex];

      evtptr->evt_length = i4swap(evtptr->evt_length);
      evtptr->evt_type = i4swap(evtptr->evt_type);

      // see if we have got the end of buffer event
      if (evtptr->evt_length == 2 && evtptr->evt_type ==0) break;

      // swap the rest of the event header
      for (i=2; i<EVTHEADERLENGTH; i++) 
				bptr->data[evtindex+i] = i4swap(bptr->data[evtindex+i]);

      // mark first subevent
      frameindex = 0;

      while (frameindex < evtptr->evt_length - EVTHEADERLENGTH)
	{
	  // here we have a frame
	  frameptr =  &evtptr->data[frameindex];
	  byteSwapFrame(frameptr);   /* byte swap if wrong endianism */
	  
	  frameindex += getFrameLength(frameptr);
	}
      evtindex += evtptr->evt_length;
      
    }
  return 0;
}

// ---------------------------------------------------------
int buffer::i4swap(const int in)
{
  union 
  {
    int i4;
    char c[4];
  } i,o;

  i.i4 = in;
  o.c[0] = i.c[3];
  o.c[1] = i.c[2];
  o.c[2] = i.c[1];
  o.c[3] = i.c[0];
  return o.i4;
}
// ---------------------------------------------------------
int buffer::i22swap(const int in)
{
  union 
  {
    int i4;
    char c[4];
  } i,o;

  i.i4 = in;
  o.c[0] = i.c[1];
  o.c[1] = i.c[0];
  o.c[2] = i.c[3];
  o.c[3] = i.c[2];
  return o.i4;
}

// ---------------------------------------------------------
short buffer::i2swap(const  short in)
{
  union 
  {
    short i2;
    char c[2];
  } i,o;

  i.i2 = in;
  o.c[0] = i.c[1];
  o.c[1] = i.c[0];

  return o.i2;
}


// ---------------------------------------------------------
Event * buffer::getEvent()

{

  if ( current_index < 0 ) return 0;
 
  Event *evt;
  //  COUT << "at index " << current_index << " Event length = " << bptr->data[current_index]  << std::endl;

  int len = bptr->data[current_index];
  // are we pointing beyond the logical end of buffer?
  if (current_index +len > (buffer_size/4) -6 )  //6 is 2 end-of-buffer + 4 buffer header
    {
      COUT << "current index: " << current_index << " len " << len << " buffer: " << (buffer_size/4) -6 << std::endl;
      COUT << "corrupt event in Buffer " << bptr->Bufseq << " index " << current_index << std::endl; 
      current_index = -1;
      return 0;
    }

  if ( bptr->data[current_index+2] == 15491) 
    {
      FILE *ff = fopen("15491.dat","w");
      int iu;
      for (iu = current_index; iu < (buffer_size/4) -4; iu++) fwrite (&bptr->data[iu],4,1,ff);
      fclose(ff);
    }



  evt =  new A_Event( &bptr->data[current_index]);

  //  int len = evt->getEvtLength();

  // maybe there is something wrong with it?
  if (len <= 0) 
    {
      current_index = -1;
      return evt;
    }

  current_index += len;

  // now is the new index pointing outside the allocated memory?
  if (current_index < 0 || current_index > max_length)
    {
      //COUT << "end of buffer r1 " << current_index << std::endl;
      current_index = -1;
      return evt;
    }


  // are we pointing to an end-of-buffer event? 
  if (bptr->data[current_index] == 2 && bptr->data[current_index+1] == 0)
    {
      //COUT << "end of buffer r3 " << std::endl;
      current_index = -1;
      return evt;
    }

  // none of the above, just return
  return evt;

}


