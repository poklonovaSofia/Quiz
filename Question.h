#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include "Answer.h"
#include <map>

using namespace std;
int returnPosition(map<int, string> m, int mode)
{
	vector<string> nameCat;
	vector<int> id;
	int idSome = -1;

	for (auto n : m)
	{
		id.push_back(n.first);
		nameCat.push_back(n.second);
	}
	if (id.size() > 0 )
	{
		if (mode == 0)
		{
			nameCat.push_back("Exit");
			
		}
		else
		{
			char abetka(96);
			string temp = "";
			vector< string> t;
			for (auto n : nameCat)
			{
				abetka++;
				temp += abetka;
				temp += ") ";
				temp += n;
				t.push_back(temp);
				temp = "";
	
			}
			nameCat.clear();
			for (auto n : t)
			{
				nameCat.push_back(n);	
			}
			nameCat.push_back("Next");
			nameCat.push_back("Back");
			nameCat.push_back("Finish");
		}
		int choose = Menu::select_vertical(nameCat, HorizontalAlignment::Left, 3);
		if ((choose <= (id.size() - 1)) && (choose >= 0) && (id.size() != 0))
			idSome = id[choose];
		else if(choose == id.size() && mode == 1 )
		{
			idSome = -2;
		}
		else if (choose == id.size() + 1 && mode == 1)
		{
			idSome = -3;
		}
		else if (choose == id.size() + 2 && mode == 1)
		{
			idSome = -4;
		}	
	}
	else
	{
		cout << "There is nothing.it is empty" << endl;
		system("pause");
	}
	return idSome;
}
class Question
{
	int id;
	int idT;
	float points;
	int count_answers;

	string name;

public:
	Question() {
		id = 0;
		idT = 0;
		points = 0.0;
		count_answers = 0;
	}
	Question(int iT) {
		idT = iT; 
		id = 0;
		
		points = 0.0;
		count_answers = 0;
	}
	Question(int iQ, int iT) : id(iQ), idT(iT) {
		
		points = 0.0;
		count_answers = 0;
	}


	void menu();

	bool create();
	bool deleteAns();
	bool finish();
	void edit();
	void saveQN(string tname);
	bool deleteQ();
	void SetName(string n) { name = n; }
	void SetMark(float m) { points = m; }
	void SetCount(int c) { count_answers = c; }
	void setCorrect();
	void chooseAnswer();
	void inputPoints();
};
void Question::inputPoints()
{
	cout << "How many points does this question have?";
	cin >> this->points;
	cin.ignore();
}
bool Question::finish()
{
	bool res = false;
	bool flag = false;
	DataBase* db = nullptr;
	db = db->getInstance();
	while (true)
	{
		system("cls");
		cout << "Question: " << name << endl;
		if (points == 0.0)
		{
			inputPoints();
		}
		
		if (this->points > 0.0)
		{
			cout << "This question has mark - " << points << endl;
			cout << "Do you agree?" << endl;
			int choose = Menu::select_vertical({ "Yes",
											"No" }, HorizontalAlignment::Left, 5);
			switch (choose)
			{
			case 0:
				flag = true;
				break;
			case 1:
				inputPoints();
				flag = false;
				break;
			default:
				return false;
							
			}	
			if (flag)
				break;
			else
				continue;
		}
		else
		{
			cout << "Value must be greater than zero" << endl;
			system("pause");
			
		}	
	}
	if (count_answers > 0)
	{
		system("cls");
		map<int, int> m = db->queryAllAnswersCor(id);
		for (auto n : m)
		{
			if (n.second == 1)
				res = true;
		}
		int idAC = 0;
		if (res == false)
		{
			map<int, string> c = db->queryAllAnswers(id);
			while (true)
			{
				cout << "Question: " << name;
				cout << "; Choose correct answer: " << endl;
				idAC = returnPosition(c, 0);
				if (idAC == -1)
				{
					system("cls");
					cout << "You will return in menu question. Do you want to exit?" << endl;
					int choose = Menu::select_vertical({ "Yes",
												"No" }, HorizontalAlignment::Left, 5);
					switch (choose)
					{
					case 0:
						return false;
					case 1:
						continue;
					default:
						return false;

					}

				}
				else
				{
					break;
				}
			}
		}
		if (idAC >= 1)
			db->queryUpdateCorrectAns(idAC, 1);
	}
	
	
	if (db->queryUpdateQuestion(points, count_answers, id))
		return true;
	
}
void Question::chooseAnswer()
{
	DataBase* db = nullptr;
	db = db->getInstance();
	map<int, string> m = db->queryAllAnswers(id);
	cout << "Question: " << name << endl;
	int idA = returnPosition(m, 0);
	if (idA == -1)
		return;
	Answer a(idA, id, idT, m.at(idA));
	a.menu();
}

void Question::saveQN(string tname)
{
	DataBase* db = nullptr;
	db = db->getInstance();
	string temp = "";
	temp = returnString39(tname);
	if (db->queryUpdateQuestionName(temp, id))
	{
		cout << "Question was updated" << endl;
		name = tname;
	}

	system("pause");
}
void Question::setCorrect()
{
	int stat = -1;
	map<int, string> m;
	map<int, int> cor;
	DataBase* db = nullptr;
	db = db->getInstance();
	int idC = 0;
	if (count_answers > 0)
	{
		bool is = false;
		
		
		
		m = db->queryAllAnswers(id);
		cor = db->queryAllAnswersCor(id);
		for (auto c : cor)
		{
			if (c.second == 1)
			{
				cout << "Right answer: " << m.at(c.first) << endl;
				idC = c.first;
				is = true;
				break;
			}
		}
		if (is)
		{
			cout << "This question has correct answer. Do you want to change it?" << endl;
			int choose = Menu::select_vertical({ "Yes",
												"No" }, HorizontalAlignment::Left, 5);
			switch (choose)
			{
			case 0:

				break;
			case 1:
				return;
			default:
				return;
			}
		}


	}
	else
	{
		cout << "There are no answers in this test yet. Create the answers first" << endl;
		system("pause");
		return;
	}
	system("cls");
	cout << "Question: " << name << endl <<"Choose new correct answers:" << endl;
	int idCA = returnPosition(m, 0);
	if (idCA == -1)
		return;
	db->queryUpdateCorrectAns(idC, 0);
	if (db->queryUpdateCorrectAns(idCA, 1))
	{
		cout << "New correct answer: " << m.at(idCA)<< endl;
		
	}
	system("pause");


}
void Question::edit()
{


	if (name.size() > 0)
	{
		cout << "Your question: " << endl;
		cout << name << endl;
		string tname = "";
		cout << "Input new question: ";
		getline(cin, tname);
		if (name.size() > 0) {
			int choose = Menu::select_vertical({ "Save new question",
												"Cancel" }, HorizontalAlignment::Left, 5);
			switch (choose)
			{
			case 0:
				saveQN(tname);
				break;
			case 1:
				break;
			default:
				break;
			}
		}
	}
}
void Question::menu()
{
	DataBase* db = nullptr;
	db = db->getInstance();
	while (true)
	{
		system("cls");
		cout << "Question: " << name << endl;
		int choose = Menu::select_vertical({ "New answer",
											"Modify answer",
											"Delete answer"	,
											"Change question",
											"Set corect",
											"Exit"
			}, HorizontalAlignment::Left, 5);
		Answer a(id, idT);
		switch (choose)
		{
		case 0:
			system("cls");
			cout << "Question: " << name << endl;
			if (a.create())
			{
				count_answers++;
				db->queryUpdateQuestion(points, count_answers, id);

				
			}
			break;
		case 1:
			if (count_answers >= 1)
			{
				chooseAnswer(); break;
			}
			else
			{
				cout << "There are no answers in this test yet. Create the answers first" << endl;
				system("pause");
			}

		case 2:
			if (count_answers >= 1)
			{
				if (deleteAns())
				{
					count_answers--;
					db->queryUpdateQuestion(points, count_answers, id);
				}
					
			}
			else
			{
				cout << "There are no answers in this test yet. Create the answers first" << endl;
				system("pause");
			}
			break;
		case 3:
			edit();
			break;
		case 4:
			setCorrect();
			break;
		case 5:
			if (finish())
				return;
			else
				break;
		default:
			return;
		}

	}
}

bool Question::deleteQ()
{
	bool res = false;
	DataBase* db = nullptr;
	db = db->getInstance();
	map<int, string>m = db->querySelectQuestions(idT);
	cout << "Choose question for delete:" << endl;
	int IdQ = returnPosition(m, 0);
	if (IdQ == -1)
		return res;
	db->queryDeleteAnswers(IdQ, idT);	
	if (db->queryDeleteQuestion(IdQ, idT))
	{
		cout << "Question deleted" << endl;
		res = true;
	}
		
	else
	{
		cout << "Problem with database.." << endl;
		res = false;
	}
		
	system("pause");
	
	return res;


}
bool Question::deleteAns()
{
	bool res = false;
	DataBase* db = nullptr;
	db = db->getInstance();
	cout << "Choose answer for delete:" << endl;
	map<int, string> m = db->queryAllAnswers(id);
	int idA = returnPosition(m, 0);
	if (idA == -1)
		return res;
	Answer a(idA, id, idT, m.at(idA));
	if (a.deleteSelf())
		res = true;
	return res;



}
bool Question::create()
{
	bool res = false;
	bool flag = false;
	string question;
	string temp = "";
	DataBase* db = nullptr;
	db = db->getInstance();
	while (true) {

		cout << "Input question: ";
		getline(cin, question);

		if (question.size() > 0)
		{
			temp = returnString39(question);
			if (db->querySelectQ(temp, idT))
			{
				cout << "This question is already exists. Continue?" << endl;
				int choose = Menu::select_vertical({ "Yes",
												"No, edit" }, HorizontalAlignment::Left, 5);
				switch (true)
				{
				case 0:
					flag = true;
					break;
				case 1:
					flag = false;
					break;
				default:
					return false;
				}
				if (!(flag))
					continue;
			}
			name = question;
			if (db->queryInsertQuestion(temp, idT))
			{
				int tid = db->querySelectMaxIdQ();
				if (tid >= 1)
				{
					this->id = tid;
					menu();
					res = true;
					return res;
					
				}
				
			}
			else
			{
				cout << "Problem with database" << endl;
				system("pause");
				
				return res;
			}

		}
		else
		{
			cout << "Empty string" << endl;
			system("pause");
			return res;
		}
	}
	return res;
}
