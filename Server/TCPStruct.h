#pragma once
#include "ContactInfo.h"
#include <iostream>
#include <vector>

struct TCPStruct {
	int NumOfObjects;
	std::vector<ContactInfo> Contacts;
	//char* SerialContacts;

public:
	TCPStruct() {
		NumOfObjects = 0;
	}
	TCPStruct(char* Buffer) {
		memcpy(&NumOfObjects, Buffer, sizeof NumOfObjects);
		Buffer += sizeof NumOfObjects;

		for (int i = 0; i < NumOfObjects; i++) {
			Contacts.push_back(ContactInfo(Buffer));
			Buffer += sizeof ContactInfo;
		}
	}

	void addContact(ContactInfo newContact) {
		Contacts.push_back(newContact);
		//memcpy(SerialContacts + NumOfObjects * sizeof(ContactInfo) + NumOfObjects, newContact, sizeof ContactInfo);
		NumOfObjects++;
	}
	void serialize(char* Buffer) {
		memcpy(Buffer, &NumOfObjects, sizeof NumOfObjects);
		Buffer += sizeof NumOfObjects;
		for (int i = 0; i < NumOfObjects; i++) {
			Contacts[i].getData(Buffer);
			Buffer += sizeof ContactInfo;
		}
	}
	int getNumOfObjects() {
		return NumOfObjects;
	}
	
	void printAll() {
		for (int i = 0; i < NumOfObjects; i++) {
			Contacts[i].print();
		}
	}
};