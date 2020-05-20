#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_
typedef struct
{
	unsigned char PID;
	unsigned char Data[8];
}Package;
extern Package Report_Package;
void Send_Package(Package *Pac);
#endif
