//** This project uses Lamport Logical clocks to create time stamps to determine order of events occuring and use different processes**//

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>



using namespace std;

//Lamport class
class lamportClass {
public:
	vector <vector<int>> lamport;
	int LC;
	char s, r, num;
	lamportClass(const vector<vector<string>> testVec);
	void calculate_clock(const vector<vector <string>> testVec);
	void print_values(const vector<vector <string>> testVec, const string fileName);
	bool lamport_value_check(const int one, const int two);
private:
	void find_r(int& row, int& col, const vector<vector<string>> testVec);
	void numberRow(int row, int col, const vector<vector<string>> testVec);
	void testLamport();
	void if_s_check_left(int row, int col);
};
//constructor class, populates lamport vector with -1's and 0 for null
lamportClass::lamportClass(const vector<vector<string>> testVec) {
	LC = 1;
	s = 's';
	r = 'r';
	num = '1';
	vector <int> temp;
	for (auto i = 0; i < testVec.size(); i++) {
		for (auto j = 0; j < testVec[i].size(); j++) {
			if (testVec[i][j] == "NULL")
				temp.push_back(0);
			else
				temp.push_back(-1);
		}
		lamport.push_back(temp);
		temp.clear();
	}
}

#include "Header.h"

//main function of the clock  contains many help fuctions
void lamportClass::calculate_clock(const vector<vector<string>> testVec)
{
restart:
	for (auto i = 0; i < testVec.size(); i++) {
		for (auto j = 0; j < testVec[i].size(); j++) {
			string tempString = testVec[i][j];
			if (!lamport_value_check(i, j)) {
				if (tempString[0] == 's') {
					if (tempString[1] == num) {
						if_s_check_left(i, j);
						lamport[i][j] = ++LC;
						numberRow(i, j, testVec);
						find_r(i, j, testVec);
						goto restart;
					}
				}
			}
		}
	}
}

//prints the string vector and the values out
void lamportClass::print_values(const vector<vector <string>> testVec, const string fileName)
{
	ofstream myfile;
	myfile.open(fileName);
	myfile << "Lamport clock output\n\n\n";

	for (auto i = 0; i < testVec.size(); i++) {
		myfile << "Processor" << i << ": ";
		for (auto j = 0; j < testVec[i].size(); j++) {
			if (testVec[i][j] != "NULL")
				myfile << "\t" << testVec[i][j];
			else
				myfile << "\tNULL";
		}
		myfile << "\n";
	}

	myfile << "\n\n\n";
	for (auto i = 0; i < lamport.size(); i++) {
		myfile << "Processor" << i << ": ";
		for (auto j = 0; j < lamport[i].size(); j++) {
			if (lamport[i][j] != 0)
				myfile << "\t" << lamport[i][j];
			else
				myfile << "\t0";
		}
		myfile << "\n";
	}
	myfile.close();
}

//checks value at position[one][two] if lampart is > than 0 this has already been populated
bool lamportClass::lamport_value_check(const int one, const int two)
{
	if (lamport[one][two] > 0)
		return true;
	else
		return false;
}

//After finding and s value in the main function, this function is called to find the corresponding r and changes the value and updates LC
void lamportClass::find_r(int& row, int& col, const vector<vector<string>> testVec)
{
	string a;
	for (auto i = 0; i < testVec.size(); i++) {
		for (auto j = 0; j < testVec[i].size(); j++) {
			a = testVec[i][j];
			if (a[0] == r) {
				if (a[1] == num) {
					lamport[i][j] = ++LC;
					numberRow(i, j, testVec);
					goto found;
				}
			}
		}
	}
found:
	num++;
}

//This function populates the left and right of the row if they are internal events with the right values based on position and LC
void lamportClass::numberRow(int row, int col, const vector<vector<string>> testVec)
{
	int tempValue = 1;
	for (auto i = 0; i <= col; i++) {
		if (lamport[row][i] < tempValue) {
			lamport[row][i] = tempValue;
			tempValue++;
		}
		else {
			tempValue = lamport[row][i];
		}
	}
	if (LC < tempValue) {
		LC = tempValue;
	}
	for (auto i = col + 1; i < testVec[row].size(); i++) {
		char a = testVec[row][i].front();
		if (testVec[row][i].front() == 's' || testVec[row][i].front() == 'r' || testVec[row][i].front() == 'N') {
			break;
		}
		else {
			this->lamport[row][i] = ++tempValue;
		}
	}
}
//Helping function for debugging, no other use other than debugging
inline void lamportClass::testLamport()
{
	for (auto i = 0; i < lamport.size(); i++) {
		for (auto j = 0; j < lamport[i].size(); j++) {
			cout << lamport[i][j] << "\t";
		}
		cout << "\n";
	}
	cout << "\n\n\n\n\n";
}
//Checks to the left of the s value to make sure the values are updated properly
inline void lamportClass::if_s_check_left(int row, int col)
{
	if (col == 0) {
		return;
	}
	else if (lamport[row][col - 1] >= LC) {
		LC = lamport[row][col - 1];
	}
}

// End of lamport clock Class //

/*******************************************************************************************************************************/

// Start of Verify Class // 


//Verify class function with many helper functions
class VerifyClass {
public:
	vector<vector<string>> verify;
	vector <vector <int>> location;
	vector <int> value;
	char num, internal_event;
	bool Error;

	VerifyClass(vector<vector <int>> testVec);
	void runVerify(vector<vector<int>> testVec);
	void outputVerify(vector<vector<int>> testVec, string fileName);
private:
	void find_gap(vector<vector<int>> testVec);
	void find_pairing_s(vector<vector<int>> testVec);
	int find_min_value_location_of_r();
	void populate_with_alphabet();
	bool errorCheck(vector<vector<int>> testVec);
	void testLocation();
	void testValue();
	void testVerify();
};


//Constructor class clears vectors values, populates vector verify with 0's and NULL if the value is 0
VerifyClass::VerifyClass(vector<vector<int>> testVec) {
	vector <string> temp;
	verify.clear();
	value.clear();
	location.clear();
	num = '1';
	internal_event = 'a';
	Error = false;

	for (auto i = 0; i < testVec.size(); i++) {
		for (auto j = 0; j < testVec[i].size(); j++) {
			if (testVec[i][j] == 0)
				temp.push_back("NULL");
			else
				temp.push_back("0");
		}
		verify.push_back(temp);
		temp.clear();
	}
}

//Finds the gap between all integers thus finding all receives
void VerifyClass::find_gap(vector<vector<int>> testVec)
{
	vector <int>tempLoc;
	for (auto i = 0; i < testVec.size(); i++) {
		for (auto j = 0; j < testVec[i].size(); j++) {
			if (j == 0 && testVec[i][j] > 1) {
				value.push_back(testVec[i][j] - 1);
				tempLoc.push_back(i);
				tempLoc.push_back(j);
				location.push_back(tempLoc);
				tempLoc.clear();
			}
			else if (j > 0 && testVec[i][j] - 1 > testVec[i][j - 1]) {
				value.push_back(testVec[i][j] - 1);
				tempLoc.push_back(i);
				tempLoc.push_back(j);
				location.push_back(tempLoc);
				tempLoc.clear();
			}
		}
	}
}

//Finds the pair s to the found r_gap value, updates new values to vector<int> verify and deletes the found nodes from location and value vectors
inline void VerifyClass::find_pairing_s(vector <vector<int>>testVec) {
	int locationRow = find_min_value_location_of_r();
	int locationColumn = location[locationRow][1];
	int tempValue = value[locationRow];
	for (auto i = 0; i < testVec.size(); i++) {
		for (auto j = 0; j < testVec[i].size(); j++) {
			if (testVec[i][j] == tempValue) {
				string temp = "s";
				verify[i][j] = temp += (num);
				temp = "r";
				verify[locationRow][locationColumn] = temp += num;
				num++;
			}
		}
	}
	location.erase(location.begin() + locationRow);
	value.erase(value.begin() + locationRow);
}


//Finds the minium value of LC, IE finds lowest Receive order
inline int VerifyClass::find_min_value_location_of_r()
{
	int tempValue = 1000000;
	int tempValueLocation = 0;
	for (auto i = 0; i < location.size(); i++) {
		if (value[i] < tempValue) {
			tempValue = value[i];
			tempValueLocation = i;
		}
	}
	return tempValueLocation;
}

//Populates unpopulated locations with the alphabet representing the internal events
inline void VerifyClass::populate_with_alphabet()
{
	for (auto i = 0; i < verify.size(); i++) {
		for (auto j = 0; j < verify[i].size(); j++) {
			if (verify[i][j] == "0") {
				verify[i][j] = internal_event;
				internal_event++;
			}
		}
	}
}

//Main function to verify the program, if error found will not go further 
inline void VerifyClass::runVerify(vector<vector<int>> testVec)
{
	if (errorCheck(testVec) == false) {
		find_gap(testVec);
		int size = location.size();
		for (auto i = 0; i < size; i++) {
			find_pairing_s(testVec);
		}
		populate_with_alphabet();
	}
	else {
		Error = true;
	}
}

inline void VerifyClass::outputVerify(vector<vector<int>> testVec, string fileName)
{
	{
		ofstream myfile;
		myfile.open(fileName);
		myfile << "Verify clock output\n\n\n";
		if (Error == false) {
			for (auto i = 0; i < testVec.size(); i++) {
				myfile << "Processor " << i << ": ";
				for (auto j = 0; j < testVec[i].size(); j++) {
					if (testVec[i][j] != 0)
						myfile << "\t" << testVec[i][j];
					else
						myfile << "\t0";
				}
				myfile << "\n";
			}
			myfile << "\n\n";
			for (auto i = 0; i < verify.size(); i++) {
				myfile << "Processor " << i << ": ";
				for (auto j = 0; j < verify[i].size(); j++) {
					if (verify[i][j] != "NULL")
						myfile << "\t" << verify[i][j];
					else
						myfile << "\tNULL";
				}
				myfile << "\n";
			}
		}
		else {
			myfile << "\nError has Occured\n\nError has Occured\n\nError has Occured1\n";
		}
		myfile.close();
	}
}
//error checking the clock takes a min and a max and verifies that all values are accounted for
//if values are missing, clock is wrong and an error has occured
inline bool VerifyClass::errorCheck(vector<vector<int>> testVec)
{
	int min = 1000;
	int max = -1;
	for (auto i = 0; i < testVec.size(); i++) {
		for (auto j = 0; j < testVec[i].size(); j++) {
			if (testVec[i][j] > max) {
				max = testVec[i][j];
			}
			if (testVec[i][j] < min && testVec[i][j] != 0) {
				min = testVec[i][j];
			}
		}
	}
	int minNotfound = min;
	do {
		for (auto i = 0; i < testVec.size(); i++) {
			for (auto j = 0; j < testVec[i].size(); j++) {
				if (min == testVec[i][j]) {
					min++;
					minNotfound = min;
				}
			}
		}
		if (minNotfound == min) {
			Error = true;
			return Error;
		}
	} while (min == max);
	return false;
}
//Debugging function, Serves no other purpose
inline void VerifyClass::testLocation()
{
	cout << "\nLocation test: \n";
	for (auto i = 0; i < location.size(); i++)
	{
		cout << "[" << location[i][0] << "]" << "[" << location[i][1] << "]" << "\t";
	}
	cout << endl << endl;
}
//Debugging function, servers no other purpose
inline void VerifyClass::testValue()
{
	cout << "\nValue test: \n";
	for (auto i = 0; i < value.size(); i++) {
		cout << value[i] << "\t";
	}
	cout << endl << endl;
}
//debugging function, serves no other purpose
inline void VerifyClass::testVerify()
{
	cout << "\nVerify test: \n";
	for (auto i = 0; i < verify.size(); i++) {
		for (auto j = 0; j < verify[i].size(); j++) {
			cout << verify[i][j] << "\t";
		}
		cout << "\n";
	}
	cout << "\n\n\n\n\n";
}