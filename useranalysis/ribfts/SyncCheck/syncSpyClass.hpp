/***********************************************************************************
 >  *        File:   syncSpy.cpp                                                 *
 >  * Description:   read the "DataSink" (MIDAS data shared program) 
 >  *                                  shared memory directly                *
 >  *      Author:   Alvaro Tolosa Delgado  IFIC/CSIC spain                           *
 >  *Site Contact:   alvaro.tolosa@ific.uv.es                                           *
 >  * Last Change:   02/11/2016                                                    *
 >  *******************************************************************************/

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string>
#include<vector>
class SpyDataSink
{

public:
  SpyDataSink(){};
  SpyDataSink(const std::string & Addr , int size ){
    MyAddr=Addr;
	_Size=size;
  };
  void Test(){
    std::cout<<"Ready to connect on:"<<MyAddr<<std::endl;
  }
  int Spy(char * buffer){
    	std::vector<uint32_t> MyBuffer;
	uint32_t DummyBuffer;
     bool StoreData=false;
    std::ifstream   input(MyAddr.c_str(), std::ios::binary );
   uint16_t lineCounter(0);
   while(input.read((char *)&DummyBuffer,sizeof(DummyBuffer)) && lineCounter<_Size){
     if(lineCounter<6){
         memcpy(buffer+lineCounter*4,&DummyBuffer,sizeof(DummyBuffer));
//	*(buffer+lineCounter*4)=DummyBuffer;
     lineCounter++;
       MyBuffer.push_back(DummyBuffer);
  //     std::cout << std::hex <<"++++0x" << DummyBuffer<<"\t" <<std::endl;
     }
	else if(DummyBuffer==0x12){
		StoreData=true;
	}
    	if(StoreData ){
         memcpy(buffer+lineCounter*4,&DummyBuffer,sizeof(DummyBuffer));
     lineCounter++;
      MyBuffer.push_back(DummyBuffer);
    //  if(lineCounter<30) std::cout << std::hex <<"----0x" << DummyBuffer<<"\t";
  //     if(lineCounter%10==0)
//	 std::cout<< lineCounter <<std::endl;
     }
     
     
   }
 //  std::cout<<"Total Words read "<< lineCounter <<std::endl;
   input.close();
 //  buffer=(char *)&MyBuffer[0];
   return lineCounter;
  

  };
private:
  std::string MyAddr;
	int _Size;
};
