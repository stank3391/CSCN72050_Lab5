#include "ContactInfo.h"
#include "TCPStruct.h"


#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
	//starts Winsock DLLs
	WSADATA wsaData;
	if ((WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		return 0;
	}

	//initializes socket. SOCK_STREAM: TCP
	SOCKET ClientSocket;
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET) {
		WSACleanup();
		return 0;
	}

	//Connect socket to specified server
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;						//Address family type itnernet
	SvrAddr.sin_port = htons(27000);					//port (host to network conversion)
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");	//IP address
	if ((connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
		closesocket(ClientSocket);
		WSACleanup();
		return 0;
	}
	//Testing Data
	TCPStruct newTCPStruct = TCPStruct();			//new

	std::ifstream f("InputFile.txt");

	if (f.is_open())
	{
		while (!f.eof())
		{
			std::string ID;
			std::string Name;
			std::string Age;
			std::getline(f, ID, ',');
			std::getline(f, Name, ',');
			std::getline(f, Age, ',');
			ContactInfo newContact(std::ID, Name, 22);
		}
	}

	char Name[20] = "SeanTank";
	ContactInfo newContact(1234, Name, 22);
	newContact.print();
	newTCPStruct.addContact(newContact);			//new

	char Name1[20] = "Not Sean Tank";
	ContactInfo newContact1(5678, Name, 33);
	newContact1.print();
	newTCPStruct.addContact(newContact1);			//new

	int sizeOfBuffer = newTCPStruct.getNumOfObjects() * sizeof ContactInfo;		//28*n
	sizeOfBuffer += sizeof newTCPStruct.getNumOfObjects();						//size of int
	char* TxBuffer = new char[sizeOfBuffer];		//Should be 28*n + 4
	newTCPStruct.serialize(TxBuffer);
	
	//char TxBuffer[sizeof ContactInfo];
	//newContact.getData(TxBuffer);
	//cout << TxBuffer[9];
	send(ClientSocket, (char*) TxBuffer, sizeOfBuffer, 0);
	
	delete[] TxBuffer;

	//closes connection and socket
	closesocket(ClientSocket);

	//frees Winsock DLL resources
	WSACleanup();

	return 1;
}
