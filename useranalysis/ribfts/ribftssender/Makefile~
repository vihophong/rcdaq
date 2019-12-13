ALL : ribftssender_mar17ca

CFLAGS =   -DUNIX -DLINUX -DLINUX64 -DPOSIX -I/home/NiGIRI/brIKEN2017/ribfts/DataXferLib/V4_TCP
LOADLIBES=   -L/usr/ucblib -lpthread -L/home/NiGIRI/brIKEN2017/ribfts/MIDAS/Linux/lib64 -lxfer

ribftssender_mar17ca : ribftssender_mar17ca.o bi-tcp.o ridf.o 

clean : 
	rm -f *.o
