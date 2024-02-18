#pragma once
#include <memory>
#include <iostream>
#include <fstream>
#include <cstring>

const int NAMELENGTH = 20;
class ContactInfo {
	struct ContactData
	{
		int ID;
		char Name[NAMELENGTH];
		int Age;
	} ContactData;

public:
	ContactInfo(char* Buffer) {
		std::memcpy(&ContactData.ID, Buffer, sizeof(ContactData.ID));
		Buffer += sizeof(ContactData.ID);

		std::memcpy(ContactData.Name, Buffer, NAMELENGTH);
		Buffer += NAMELENGTH;

		std::memcpy(&ContactData.Age, Buffer, sizeof(ContactData.Age));
	}

	ContactInfo(int ID, char* Name, int Age) {
		ContactData.ID = ID;
		std::memcpy(ContactData.Name, Name, NAMELENGTH);
		ContactData.Age = Age;
	}


	void getData(char* Buffer) {
		std::memcpy(Buffer, &ContactData.ID, sizeof(ContactData.ID));
		Buffer += sizeof(ContactData.ID);

		std::memcpy(Buffer, ContactData.Name, NAMELENGTH);
		Buffer += NAMELENGTH - 1; // Move buffer pointer past the copied name
		Buffer[0] = '\0'; // Null-terminate the string
		Buffer++;

		std::memcpy(Buffer, &ContactData.Age, sizeof(ContactData.Age));
	}

	void print() {
		std::cout << (int)this->ContactData.ID << " - " << this->ContactData.Name << " - " << (int)this->ContactData.Age << std::endl;
	}
};