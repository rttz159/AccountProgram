#pragma once

#include<vector>
#include<string>

using namespace std;

//Menu
int main_menu();
void month_statement();

//Getter
int choiceGetter(int num);
void dataGetter(vector<string>& tempVector, string str);
void companyInfoGetter(string companyInfo[]);

//ExtraFunction
void currentDate(int* temp);
int* dateSpilter(string date);

//Validator
bool checkDigit(string pstr);
bool confirmationValidator(string& pstr);