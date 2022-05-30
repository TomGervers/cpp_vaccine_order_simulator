#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
using namespace std;

// Defining the customerRecord class used for storing customer information
class customerRecord {
	public:
		// First letter on an input file for customers is C
		char identifier = 'C';
		// Customers unique identifier
		int customerNo;
		// Customers name
		string customerName;
		// Current amount the customer has ordered today
		int quantityOrdered = 0;
};

// Defining the salesRecord class used for storing sales information
class salesRecord {
	public:
		// First letter on an input file for sales is S
		char identifier = 'S';
		// Date of the order
		int date;
		// Wether the order is express or not X or N
		char express;
		// Unique identifier of the customer who made the order
		int customerNo;
		// Quantity of the order
		int quantity;
};

// Defining the eodRecord class used for storing information about the end of day
class eodRecord {
	public:
		// First letter on an input file for end of day is E
		char identifier = 'E';
		// Date of day which is ending
		int date;
};

//------- Main Function -------
int main(int argc, char* argv[])
{
	// If 2 arguments are not given on command line
	if (argc != 2) {
		// Give error message explaining usage
		cerr
			<< "Usage: " << argv[0] << " inputFileName.txt" << endl;
	}
	
	// Tracks the number of invoices starting at 1000
	int invoiceNo = 1000;
	// String that each line of input file is stored in temporarily
	string recordInfo;
	// String that customerRecord's are initialised with
	string objectName;	

	// Vector for storing multiple customerRecord's
	vector<customerRecord> customerVector;
	// Vector for storing multiple salesRecord's
	vector<salesRecord> salesVector;
	// Vector for storing multiple eodVector's
	vector<eodRecord> eodVector;

	// Input stream object for storing the input file
	ifstream inputFile;
	// Open the file using the second command line argument
	inputFile.open(argv[1]);

	// If the file is not open
	if (!inputFile.is_open()){
		// Give error message explaining
		cerr
			<< "File failed to open" << endl;
	}

	// While loop for each line of the unput file
	while(getline(inputFile, recordInfo)){
		// If first character of the line is C
		if (recordInfo[0] == 'C'){
			// Convert 2nd to 5th letters to int representing the customerNo
			int customerRecordNo = stoi(recordInfo.substr(1,4));
			// Everything 6th letter onwards is the customerName
			string customerRecordName = recordInfo.substr(5);

			// Storing a name for the object as C followed by customerNo
			objectName = "C" + to_string(customerRecordNo);
			
			// Initilaising a new customerRecord object
			customerRecord objectName;

			// Assigning values to the object
			objectName.customerNo = customerRecordNo;
			objectName.customerName = customerRecordName;
			
			// Push the object to the back of the customerVector vector
			customerVector.push_back(objectName);

			// Send message to standard output stream
			cout
				<< "OP: customer " << setfill('0') << setw(4) << objectName.customerNo
				<< " added" << endl;
		}
		// If first character of the line is S
		else if (recordInfo[0] == 'S'){
			// Convert 2nd to 9th letter to int representing date
			int salesRecordDate = stoi(recordInfo.substr(1,8));
			// 10th letter indicates if express
			string salesRecordExpressString = recordInfo.substr(9,1);
			// Convert to char
			char salesRecordExpress = salesRecordExpressString[0];
			// 11th to 15th letters as int representing customerNo
			int salesRecordCustomerNo = stoi(recordInfo.substr(10,4));
			// 15th to 18th letters as int representing the quantity
			int salesRecordQuantity = stoi(recordInfo.substr(14,3));
			// Any letters after will be ignored

			// Initalizing a new salesRecord object
			salesRecord recordInfo;

			// Assigning value to the object
			recordInfo.date = salesRecordDate;
			recordInfo.express = salesRecordExpress;
			recordInfo.customerNo = salesRecordCustomerNo;
			recordInfo.quantity = salesRecordQuantity;

			// Push object to back of salesVector vector
			salesVector.push_back(recordInfo);

			// Customer who ordered the sale to access in the customerVector
			int linkedCustomer = salesRecordCustomerNo - 1;

			// If normal order
			if ( salesRecordExpress == 'N' ){
				// Send order processing message to standard output stream
				cout 
					<< "OP: customer " << setfill('0') << setw(4) << (customerVector.at(linkedCustomer)).customerNo
					<< ": normal order: quantity " << recordInfo.quantity << endl;
				
				// Increment amount customer has ordered by quantity of this order
				(customerVector.at(linkedCustomer)).quantityOrdered =
					(customerVector.at(linkedCustomer)).quantityOrdered + recordInfo.quantity;
			}
			// If express order
			else if ( salesRecordExpress == 'X' ){
				// Send order processing message to standard output stream
				cout
					<< "OP: customer " << setfill('0') << setw(4) << (customerVector.at(linkedCustomer)).customerNo
					<< ": EXPRESS order: quantity " << recordInfo.quantity << endl;

				// Increment amount customer has ordered by quantity of this order
				(customerVector.at(linkedCustomer)).quantityOrdered =
					(customerVector.at(linkedCustomer)).quantityOrdered + recordInfo.quantity;

				// Send shipping message to standard output stream
				cout
					<< "OP: customer " << (customerVector.at(linkedCustomer)).customerNo
					<< ": shipped quantity " << (customerVector.at(linkedCustomer)).quantityOrdered << endl;

				// Send invoice message to standard output stream
				cout
					<< "SC: customer " << (customerVector.at(linkedCustomer)).customerNo << ": invoice " << invoiceNo
					<< ": date " << recordInfo.date << ": quantity "
					<< (customerVector.at(linkedCustomer)).quantityOrdered << endl;

				// Increment the invoice number
				invoiceNo++;

				// Reset quantity ordered to 0 as express order ships immediately
				(customerVector.at(linkedCustomer)).quantityOrdered = 0;
			}
			// If not normal or express order
			else {
				// Send error message to standard output stream
				cerr
					<< "File format invalid" << endl;
			}
		}
		// If first letter of line is E
		else if (recordInfo[0] == 'E'){
			// Convert 2nd to 9th letters to int representing date
			int eodRecordDate = stoi(recordInfo.substr(1,8));

			// Create new eodRecord object
			eodRecord recordInfo;

			// Assigning date value to object
			recordInfo.date = eodRecordDate;

			// Push object to back of eodVector vector
			eodVector.push_back(recordInfo);

			// Send end of day processing message to standard output stream
			cout
				<< "OP: end of day " << recordInfo.date << endl;

			// Loop through all objects in the customerVector
			for (int j = 0; j < customerVector.size(); j++)
			{
				// If the customer did not order 0 today
				if ((customerVector.at(j)).quantityOrdered != 0)
				{
					// Send shipping message to standard output stream
					cout
						<< "OP: customer " << setfill('0') << setw(4) << (customerVector.at(j)).customerNo
						<< ": shipped quantity " << (customerVector.at(j)).quantityOrdered << endl;
					// Send invoice message to standard output stream
					cout
						<< "SC: customer " << setfill('0') << setw(4) << (customerVector.at(j)).customerNo
						<< ": invoice " << invoiceNo << ": date " << recordInfo.date
						<< ": quantity: " << (customerVector.at(j)).quantityOrdered << endl;
					
					// Increment the invoice number
					invoiceNo++;

					// Reset quantity customer has ordered to 0 as all ordered have been shipped
					customerVector.at(j).quantityOrdered = 0;
				}
			}
		}
		// If first letter is anything else
		else{
			// Send error message to standard output stream
			cerr
				<< "File format incorrect" << endl;
		}
	}
	
	return 0;
}
