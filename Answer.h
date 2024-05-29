#pragma once
#include<iostream>
#include<string>
#include<fstream>
using namespace std;

class Answer
{
	int id ;
	int idT;
	int idQ;
	string name;
	int correct;
public:
	Answer() 
	{
		id = 0;
		idT = 0;
		name = "";
		idQ = 0;
		correct = 0;
	}
	Answer(int iQ, int iT)
	{ 
		idQ = iQ;
		idT = iT; 
		id = 0;	
		name = "";
		correct = 0;
	}
	Answer(int i, int iQ, int iT, string n) 
	{ 
		id = i; 
		idQ = iQ; 
		idT = iT;
		name = n; 
		correct = 0;
	}
	void menu();
	void saveAnsName(string tname);
	bool create();
	void save(int idQ, int idT);
	bool deleteSelf();
	void edit();

};
void Answer::saveAnsName(string tname)
{
	DataBase* db = nullptr;
	db = db->getInstance();
	string temp = returnString39(tname);
	if (db->queryUpdateAnswerName(temp, id))
	{
		cout << "Answer was edited" << endl;
		name = tname;
	}
	else
	{
		cout << "Problem with database.." << endl;
	}
	system("pause");
	
}
void Answer::edit()
{
	if (name.size() > 0)
	{
		cout << "Your answer: " << endl;
		cout << name << endl;
		string tname = "";
		cout << "Input new answer: ";
		getline(cin, tname);
		if (name.size() > 0) {
			int choose = Menu::select_vertical({ "Save new answer",
												"Cancel" }, HorizontalAlignment::Left, 5);
			switch (choose)
			{
			case 0:
				saveAnsName(tname);
				break;
			case 1:
				break;
			default:
				break;
			}
		}
	}
}

bool Answer::create()
{
	bool res = false;
	DataBase* db = nullptr;
	db = db->getInstance();
	string answer = "";
	string temp = "";
	cout << "Input answer: ";
	getline(cin, answer);
	if (answer.size() > 0)
	{
		temp = returnString39(answer);
		if (db->queryInsertAnswer(temp, idT, idQ))
		{
			int tid = db->querySelectMaxIdAns();
			if (tid >= 1)
			{
				id = tid;
				cout << "Answer was added" << endl;
				res = true;
				system("cls");
			}
			else {
				cout << "Problem with database"<< endl;
				system("pause");
			}	
		}
		else {
			cout << "Problem with database" << endl;
			system("pause");
		}
	}
	else
	{
		cout << "Empty string";
		system("pause");
	}
	return res;
}
void Answer::menu()
{
	while (true)
	{
		cout << "Answer: " << name << endl;
		int choose = Menu::select_vertical({ "Edit answer",
											"Exit"}, HorizontalAlignment::Left, 5);
		switch (choose)
		{
		case 0:
			edit();
			break;
		case 1:
			return;
		default:
			break;
		}
	}
}
bool Answer::deleteSelf()
{
	bool res = false;
	DataBase* db = nullptr;
	db = db->getInstance();
	if (db->queryDeleteAnswer(id, idQ, idT))
	{
		cout << "Answer was deleted" << endl;
		res = true;
		system("pause");
	}
	else
	{
		cout << "Problem with database" << endl;
		
		system("pause");
	}
	return res;
}

void Answer::save(int idQ, int idT)
{
	DataBase* db = nullptr;
	db = db->getInstance();
	if(db->queryUpdateAnswer(id, idQ, idT))
	{
		cout << "Correct answer was selected" << endl;
		system("pause");
	}
	else
	{
		cout << "Answer wasn't update";
		system("pause");
	}
}

