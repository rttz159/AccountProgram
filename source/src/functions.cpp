#include<Windows.h>
#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<sstream>
#include<fstream>
#include"functions.h"
#include"inputStatement.h"
#include"statementLog.h"

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

    vector<inputStatement> statement_list;
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
        inputStatement tempStatement;
        int count = 0;
        while(getline(inputFile,temp)){
            if(count > 8){
                dataGetter(tempVector, temp);
                int* tempDate = dateSpilter(tempVector[0]);
				if(tempVector[3] == "NA"){
					tempVector[3] = "0";
				}
				if(tempVector[4] == "NA"){
					tempVector[4] = "0";
				}
                tempStatement = {{tempDate[0],tempDate[1],tempDate[2]},tempVector[1],tempVector[2],stof(tempVector[3]),stof(tempVector[4])};
                statement_list.push_back(tempStatement);
                tempVector.clear();
            }
            count++;
        }

		inputFile.close();
		currentMonthDataDisplayer(statement_list); 
    }


}

void currentMonthDataDisplayer (vector<inputStatement> &statement_list){
	int date[3];
	currentDate(date);
	int choice = 0;

	vector<statementLog> logList;
	ifstream inputFile;
	inputFile.open(R"(src/StatementLog.csv)");
	string temp;
	while(getline(inputFile,temp)){
		vector<string> tempVector;
		statementLog tempLog;
		dataGetter(tempVector,temp);
		int* date = dateSpilter(tempVector[0]);
		tempLog = {{date[0],date[1],date[2]}, stof(tempVector[1])};
		logList.push_back(tempLog);
	}
	inputFile.close();

	int previousDate[3];

	if(date[2] != 1){
		previousDate[0] = date[0];
		previousDate[1] = date[1] - 1;
		previousDate[2] = date[2]; 
	}else{
		previousDate[0] = date[0];
		previousDate[1] = 12;
		previousDate[2] = date[2] - 1; 
	}

	vector<float> previousBalance;

	for(int i = 0; i < logList.size(); i++){
		if((previousDate[1] == logList[i].date[1]) && (previousDate[2] == logList[i].date[2])){
			previousBalance.push_back(logList[i].balance);
		}
	}

	
	float totalincome = 0.0, totalexpenses = 0.0, totalbalance = 0.0;
	do{
		system("CLS");
		cout << fixed << setprecision(2);
		cout << "------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
		cout << setw(60.5) << " " << "MONTHLY STATEMENT (" << month[date[1]-1] << ")"<< endl; 
		cout << "------------------------------------------------------------------------------------------------------------------------------------------------" << endl << endl;
		cout << setw(5) << left << "NO." << setw(20) << left << "DATE" << setw(60) << left << "DESCRIPTION" << setw(20) << "INCOME" << setw(20) << "EXPENSES" << setw(20) << "BALANCE" << endl;
		cout << endl << "------------------------------------------------------------------------------------------------------------------------------------------------" << endl << endl;
		cout << setw(5) << left << "0" << setw(140) << previousBalance[0] << endl;
		for(int i = 0; i < statement_list.size(); i++){
			string tempDate = to_string(statement_list[i].date[0]) + "-" + to_string(statement_list[i].date[1]) + "-" + to_string(statement_list[i].date[2]);
			float tempCurrentBalance = previousBalance[i] + statement_list[i].income - statement_list[i].expenses;
			previousBalance.push_back(tempCurrentBalance);
			cout << setw(5) << left << i + 1 << setw(20) << left << tempDate << setw(60) << left << statement_list[i].description << setw(20) << statement_list[i].income << setw(20) << statement_list[i].expenses << setw(20) << tempCurrentBalance << endl; 
		}
		for(int i = 0; i < statement_list.size(); i++){
			totalincome += statement_list[i].income;
			totalexpenses += statement_list[i].expenses;
		}
		totalbalance = previousBalance[previousBalance.size() - 1];
		cout << setw(105) << totalincome << setw(20) << totalexpenses << setw(20) << totalbalance << endl;
		cout << endl << "------------------------------------------------------------------------------------------------------------------------------------------------" << endl << endl;

		int choice2 = 0;

		cout << setw(5) << " " << "1. Return to Main Menu and Save" << endl;
		cout << setw(5) << " " << "2. Add Statement" << endl;
		cout << setw(5) << " " << "3. Insert Statement" << endl;
		cout << setw(5) << " " << "4. Make Changes" << endl;
		cout << setw(5) << " " << "5. Delete Statement" << endl << endl;
		cout << "-------------------------------------------------------------------------------------------------------------------------------------------" << endl;
		choice2 = choiceGetter(5);
		cout << "-------------------------------------------------------------------------------------------------------------------------------------------" << endl;

		switch (choice2) {
		int choice3, choice4;
		case 2:
				cout << "---------------------------------------------------------------" << endl << endl;
			break;

		case 4:
			cout << setw(5) << " " << "Select the No. of the data that needed for changes" << endl;
			choice4 = choiceGetter(statement_list.size()) - 1;
			do {
				system("CLS");
				cout << "---------------------------------------------------------------" << endl;
				cout << setw(25.5) << " " << "Statement Info" << endl;
				cout << "---------------------------------------------------------------" << endl << endl;
				cout << setw(5) << " " << "Please Select the Information which need to be changed" << endl;
				cout << setw(5) << " " << "----------------------------" << endl << endl;
				cout << setw(5) << " " << "1. DATE : " << statement_list[choice4].date[0] << "-" << statement_list[choice4].date[1] << "-" << statement_list[choice4].date[2] << endl;
				cout << setw(5) << " " << "2. DESCRIPTION : " << statement_list[choice4].description << endl;
				cout << setw(5) << " " << "3. INCOME : " << statement_list[choice4].income << endl;
				cout << setw(5) << " " << "4. EXPENSES : " << statement_list[choice4].expenses << endl;
				cout << setw(5) << " " << "5. CONFIRMED (Nothing to Change)" << endl << endl;
				cout << "---------------------------------------------------------------" << endl;
				choice3 = choiceGetter(5);
				cout << "---------------------------------------------------------------" << endl << endl;

				string strQuantity;
				string strPrice;
				switch (choice3) {
				case 1:
					cout << endl << "---------------------------------------------------------------" << endl;
					break;
				case 2:
					break;
				case 3:
					cout << endl << "---------------------------------------------------------------" << endl;
					break;
				case 4:
					cout << endl << "---------------------------------------------------------------" << endl;
					break;
				case 5:
					cout << endl << "---------------------------------------------------------------" << endl;
					break;
				default:
					break;
				}
			} while (choice2 != 5);
			break;
		case 5:
			cout << setw(5) << " " << "Select the No. of the statement needed to Delete" << endl;
			choice3 = choiceGetter(statement_list.size());
			statement_list.erase(statement_list.begin() + (choice - 1));
			break;
		default:
			break;
		}

	}while(choice != 1);
	
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