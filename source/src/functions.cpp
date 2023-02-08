#include<Windows.h>
#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<sstream>
#include<fstream>
#include"functions.h"
#include"statement.h"

using namespace std;

string month[12] = { "JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEPT","OCT","NOV","DEC" };

//Menu
int main_menu(){

    int choice;
    int date[3];
    currentDate(date);

    system("CLS");
    cout << "---------------------------------------------------------------" << endl;
    cout << setw(27) << " " << "MAIN MENU" << endl;
    cout << "---------------------------------------------------------------" << endl << endl;
    cout << setw(5) << " " << "Please Select the Function" << endl;
    cout << setw(5) << " " << "----------------------------" << endl << endl;
    cout << setw(5) << " " << "1. QUIT PROGRAM" << endl;
    cout << setw(5) << " " << "2. THIS MONTH'S STATEMENT (" << month[date[1] - 1] << ")" << endl;
    cout << setw(5) << " " << "3. EXISTED STATEMENT" << endl << endl;
    cout << "---------------------------------------------------------------" << endl;
    choice = choiceGetter(3);
    cout << "---------------------------------------------------------------" << endl << endl;

	return choice;
}

void month_statement(string companyInfo[]){
    
    int date[3];
    currentDate(date);
    stringstream sso;
    sso << "Statements\\" << month[date[1]-1] << date[2] << ".csv";
    string filename;
    getline(sso,filename);

    vector<statement> statement_list;
    ifstream inputFile;
    inputFile.open(filename);
    if(inputFile.fail()){

        ofstream outputFile;
        outputFile.open(filename);
		vector<string> address;
		dataGetter(address,companyInfo[1]);
		outputFile << endl;
		outputFile << companyInfo[0] << endl;
		outputFile << "\"" << address[0] << ", " << address[1] << ", " << address[2] << ", " << address[3] << "\"" << endl;
		outputFile << "Tel : " << companyInfo[2] << endl;
		outputFile << "Email : " << companyInfo[3] << endl;
		outputFile << endl << endl;
        outputFile << "DATE,DESCRIPTION,INVOICE NO,INCOME,EXPENSES,BALANCE" << endl;
        outputFile.close();
		inputFile.close();
        cout << "This month's statement wasn't found, generating a new statement" << endl;
        cout << "User will be returned to the main menu" << endl;
        Sleep(800);
        return;

    }else{
        string temp;
        vector<string> tempVector;
        statement tempStatement;
        int count = 0;
        while(getline(inputFile,temp)){
            if(count > 8){
                dataGetter(tempVector, temp);
                int* tempDate = dateSpilter(tempVector[0]);
                tempStatement = {{tempDate[0],tempDate[1],tempDate[2]},tempVector[1],tempVector[2],stof(tempVector[3]),stof(tempVector[4]),stof(tempVector[5])};
                statement_list.push_back(tempStatement);
                tempVector.clear();
            }
            count++;
        }

		inputFile.close();

    }


}

//Getter
int choiceGetter(int num) {

	string choice;
	int intchoice = -1;
	do {

		cout << setw(5) << " " << "Enter your choice -> ";
		getline(cin, choice);
		if (checkDigit(choice) && !choice.empty()) {
			if (choice.length() > 1) {
				cout << setw(5) << " " << "Please enter the option available only." << endl;
				getline(cin, choice);
			}
			else {
				if (checkDigit(choice)) {
					if (stoi(choice) > 0 && stoi(choice) <= num) {
						intchoice = stol(choice);
						choice = "";
					}
					else {
						cout << setw(5) << " " << "Please enter the option available only." << endl;
						getline(cin, choice);
					}
				}
				else {
					cout << setw(5) << " " << "Please enter the option available only." << endl;
					getline(cin, choice);
				}
			}
		}
		else {
			cout << setw(5) << " " << "Please enter the option available only." << endl;
			getline(cin, choice);
		}
	} while (intchoice < 1 || intchoice > num || (!(checkDigit(choice)) && choice.empty()));

	return intchoice;

}

void dataGetter(vector<string>& tempVector, string str) {

	stringstream sso(str);

	string temp;
	char skip;

	while (sso) {
		if (sso.peek() != ',') {
			if (getline(sso, temp, ',')) {
				tempVector.push_back(temp);
			}
		}
		else {
			sso >> skip;
		}
	}
}

void companyInfoGetter(string companyInfo[]) {
	ifstream inputFile;
	inputFile.open(R"(src/CompanyInfo.csv)");
	if (inputFile.fail()) {
		cout << "Source file not found." << endl;
	}
	else {
		string temp;
		int idx = 0;
		while (getline(inputFile, temp)) {
			companyInfo[idx] = temp;
			idx++;
		}
	}

	inputFile.close();
}

//ExtraFunction
void currentDate(int* temp) {

	time_t current_time;

	current_time = time(NULL);

	tm* local_time = localtime(&current_time);
	temp[0] = (local_time->tm_mday);
	temp[1] = (local_time->tm_mon + 1);
	temp[2] = (local_time->tm_year + 1900);

}

int* dateSpilter(string date) {

	int* dateptr;
	dateptr = new int[3];

	stringstream sso(date);
	string temp;
	int count = 0;
	char skip;
	while (sso) {
		if (sso.peek() != '-') {
			getline(sso, temp, '-');
			dateptr[count] = stoi(temp);
		}
		else {
			sso >> skip;
		}
		count++;
	}

	return dateptr;

}

//Validator
bool checkDigit(string pstr) {

	bool digit = true;
	int count = 0;

	for (int i = 0; i < pstr.length(); i++) {
		if (!(isdigit(pstr[i]))) {
			count++;
		}
	}

	if (count != 0) {
		digit = false;
	}

	return digit;

}

bool confirmationValidator(string& pstr) {
	bool valid = false;

	if (pstr.length() == 1) {
		pstr = toupper(pstr[0]);
		if (pstr == "Y" || pstr == "N") {
			valid = true;
		}
	}

	return valid;
}