#pragma once
#include "ContactInfo.h"
#include <iostream>
#include <vector>

struct TCPStruct {
	int NumOfObjects;						//The number of contacts
	std::vector<ContactInfo> Contacts;		//Hashset of Contacts

public:
	TCPStruct() {
		NumOfObjects = 0;
	}

	TCPStruct(char* Buffer) {				//Deserialize
		memcpy(&NumOfObjects, Buffer, sizeof NumOfObjects);
		Buffer += sizeof NumOfObjects;

		for (int i = 0; i < NumOfObjects; i++) {
			Contacts.push_back(ContactInfo(Buffer));
			Buffer += sizeof ContactInfo;
		}
	}

	//Add a new contact to the hashset
	void addContact(ContactInfo newContact) {
		Contacts.push_back(newContact);
		NumOfObjects++;
	}

	//Serialize the header (Num of objects) then sequentially serialize the data inside
	void serialize(char* Buffer) {
		memcpy(Buffer, &NumOfObjects, sizeof NumOfObjects);
		Buffer += sizeof NumOfObjects;
		for (int i = 0; i < NumOfObjects; i++) {
			Contacts[i].getData(Buffer);
			Buffer += sizeof ContactInfo;
		}
	}

	/// @brief This function is used to get the Number of Objects in my Vector (HashSet).
	/// @params None
	/// @return NumOfObjects (int)
	int getNumOfObjects() {
		return NumOfObjects;
	}

	//Loop through hashset and print them out
	void printAll() {
		for (int i = 0; i < NumOfObjects; i++) {
			Contacts[i].print();
		}
	}
};