// Copyright Jonathan Carlson 2022

#include "miscellaneous_test.h"

/*------------------------------------------------------------------------------------------------*/

bool Miscellaneous_Test::generateRandomNumber_test() {

	srand(time(NULL));

	short output1 = Miscellaneous::generateRandomNumber(1);
	short output2 = Miscellaneous::generateRandomNumber(3);
	short output3 = Miscellaneous::generateRandomNumber(10);	
	short output4 = Miscellaneous::generateRandomNumber(100);

	if(false) { // Set as true to view outputs
		cout << "Random number 1-1: " << output1 << endl;
		cout << "Random number 1-3: " << output2 << endl;
		cout << "Random number 1-10: " << output3 << endl;
		cout << "Random nubmer 1-100: " << output4 << endl;
	}
	
	bool outOfRange1 = ((output3 <= 0) || (output1 > 1));
	bool outOfRange2 = ((output1 <= 0) || (output2 > 3));
	bool outOfRange3 = ((output2 <= 0) || (output3 > 10));
	bool outOfRange4 = ((output3 <= 0) || (output4 > 100));

	bool errorExists = outOfRange1 || outOfRange2 || outOfRange3 || outOfRange4;

	if(errorExists) {
		cout << "ERROR: Miscellaneous::generateRandomNumber() is generating out of range numbers!\n"
		     << "output1: " << output1;
		if(outOfRange1) cout << " <--- Error!"; 
		cout << endl;

		cout << "output2: " << output2;
		if(outOfRange2) cout << " <--- Error!";
		cout << endl;

		cout << "output3: " << output3;
		if(outOfRange3) cout << " <--- Error!";
		cout << endl;

		cout << "output4: " << output4;
		if(outOfRange4) cout << " <--- Error!";
		cout << endl;

		return false;
	}

	set<short> randomNumbers;
	for(int i = 0; i < 10; i++) {
		randomNumbers.insert(Miscellaneous::generateRandomNumber(100));
	}

	if(randomNumbers.size() == 1) {
		cout << "ERROR: Random number generator is generating only duplicates!" << endl;
		for(auto i : randomNumbers)
			cout << "Number: " << i << endl;
		return false;
	}

	return true;
}

/*------------------------------------------------------------------------------------------------*/

bool Miscellaneous_Test::convertStringToFloat_test() {

	vector<string> testCases = {
		"1.111", "2.222", "3.333", "4.444", "0.000", "123.456", "9999999.99999999"
	};

	vector<float> expectedResults = {
		1.111, 2.222, 3.333, 4.444, 0.000, 123.456, 9999999.99999999
	};

	for(short i = 0; i < testCases.size(); i++) {
		string error = "Miscellaneous::convertStringToFloat() encountered an error!";

		float output = Miscellaneous::convertStringToFloat(testCases[i]);
		const type_info& type = typeid(output);
		const char* myType = type.name();

		if(*myType != 'f') {
			cout << error << endl;
			return false;
		}

		if(output != expectedResults[i]) {
			cout << error << endl;
			return false;
		}
	}

	return true;
}

/*------------------------------------------------------------------------------------------------*/
