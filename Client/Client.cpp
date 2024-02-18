#include "ContactInfo.h"
#include "TCPStruct.h"


#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

const std::string FILENAME = "CastOfCasaBlanca.txt";

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
	//Create the struct to hold the objects
	TCPStruct newTCPStruct = TCPStruct();			

	//Open the file
	std::ifstream f(FILENAME);
	if (!f.is_open()) {
		std::cerr << "Error opening file\n";
		return 1; 
	}

	//Parse File
	if(f.is_open()) {
		std::string line;
		while (std::getline(f, line)) {
			std::stringstream ss(line);
			std::string data[3];

			std::getline(ss, data[0], ',');				// ID
			std::getline(ss, data[1], ',');				// Name
			std::getline(ss, data[2], ',');				// Age

			char Name[NAMELENGTH];
			std::memcpy(Name, data[1].c_str(), NAMELENGTH);			//Convert string to char*, also helps ensures names longer than 20 are truncated
			ContactInfo newContact(std::stoi(data[0]), Name, std::stoi(data[2]));
			//newContact.print();
			newTCPStruct.addContact(newContact);
		}
	}

	//newTCPStruct.printAll();
	//Calculate buffer size
	int sizeOfBuffer = newTCPStruct.getNumOfObjects() * sizeof ContactInfo;		//28*n
	sizeOfBuffer += sizeof newTCPStruct.getNumOfObjects();						//size of int
	char* TxBuffer = new char[sizeOfBuffer];									//28*n + 4
	
	//Serialize the data before sending
	newTCPStruct.serialize(TxBuffer);
	
	send(ClientSocket, (char*) TxBuffer, sizeOfBuffer, 0);
	
	//Release the memory
	delete[] TxBuffer;

	//closes connection and socket
	closesocket(ClientSocket);

	//frees Winsock DLL resources
	WSACleanup();

	return 1;
}
