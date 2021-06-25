#include "Header.h"

int main() {
	vector <vector <string>> test_1 = { {"a", "s1", "r3", "b" },          // 1 2 8 9
										{ "c","r2","s3","NULL" },		  // 1 6 7 0
										{ "r1","d","s2","e" } };		  // 3 4 5 6
 	vector <vector <string>> test_2 = { { "s1", "b", "r3", "e" },		  //1 2 8 9
										{ "a","r2","s3","NULL" },		  //1 6 7 0
										{ "r1","c","d","s2" } };		  //2 3 4 5

	lamportClass lampTest1(test_1);
	lampTest1.calculate_clock(test_1);
	lampTest1.print_values(test_1, "output1.txt");

	lamportClass lampTest2(test_2);
	lampTest2.calculate_clock(test_2);
	lampTest1.print_values(test_2, "output2.txt");



	vector <vector<int>> test_3 = {		{1,2,8,9},  //a s1 r3 b
										{1,6,7,0},  //c r2 s3 NULL
										{3,4,5,6}   //r1 d s2 e
	};
	vector <vector<int >> test_4 = {	{1,2,8,9},  //s1 b r3 e
										{1,6,7,0},	//a r2 s3 NULL
										{2,3,4,5}   //r1 c d s2
	};
	vector <vector<int>> test_5{	{1,2,8,9} , //INCORRECT
									{1,6,7,0} , 
									{2,4,5,6} 
	};

	VerifyClass verify1(test_3);
	VerifyClass verify2(test_4);
	VerifyClass verify3(test_5);

	verify1.runVerify(test_3);
	verify1.outputVerify(test_3, "output3.txt");
	verify2.runVerify(test_4);
	verify2.outputVerify(test_4, "output4.txt");
	verify3.runVerify(test_5);
	verify3.outputVerify(test_5, "output5.txt");


	return 0;
}