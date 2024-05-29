#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include "DataBase.h"
#include "Question.h"
using namespace std;
class Test
{
	int id;
	int idUser;
	string name;
	int count_question;
	float points;
	int idCat;
	int pass;
	int count_pass;
	int finished;
	float average;
public:
	Test()
	{
		pass = 0;
		id = 0;
		name = "";
		count_question = 0;
		points = 0;
		idCat = 0;
		finished = 0;
		idUser = 0;
		average = 0.0;
		count_pass = 0;
	}
	Test( int iT, int iC, string n, int fin, int p = 0, int iU = 0) : id(iT), idCat(iC), name(n), finished(fin), pass(p), idUser(iU) {}
	void deleteSelf();
	void create();
	void menu();
	void show();
	void chooseQuestion();
	bool finish();
	void deleteTestBase();
	void setCount(int c) { count_question = c; }
	void setPoints(float p) { points = p; }
	bool saveUnfin();
	void menuPass();
	bool saveFin();
	bool saveUserFin();
	
	
	bool finishPass();

};

//bool Test::saveUserUnfin()
//{
//	DataBase* db = nullptr;
//	bool res = false;
//	db = db->getInstance();
//	points = db->
//
//
//		
//	return false;
//}
bool Test::saveUserFin()
{
	DataBase* db = nullptr;
	bool res = false;
	db = db->getInstance();
	int allMark =0;
	int countCor = 0;
	map<int, int>n = db->querySelectMarkOfUnQs(id, idUser);
	for (auto m : n)
	{
		points = m.first;
		countCor = m.second;
	}
	points = (float)countCor / (float)count_question * 12;
	bool f = db->querySelectFinQs(id, idUser);
	if (!(f))
	{
		if (pass == -1)
		{
			average = float(points);
			count_pass = 1;
		}
		else
		{
			map<float, int> n = db->querySelectAverCount(id, idUser);
			for (auto m : n)
			{
				average = (float)m.first;
				count_pass = m.second;
				allMark = average * count_pass + points;
				count_pass++;
				average = (float)(allMark) / (float)count_pass;

			}
		}
		if (db->queryUpdateUserTest(id, idUser, points, 1, average, count_pass))
		{
			cout << "Test " << name << " was saved " << endl;
			cout << "Mark:" << points << endl;
			cout << "Correct answer: " << countCor << "/" << count_question << endl;
			cout << "Percent correct: " << countCor * 100 / count_question << "%" << endl;
			res = true;
		}
		else
		{
			cout << "Problem with database" << endl;
		}
	}
	else
	{
		cout << "You haven't answered all the questions yet" << endl;
	}
	system("pause");
	return res;
	

}
bool Test::finishPass()
{
	return finish();
}
void Test::menuPass()
{
	DataBase* db = nullptr;
	db = db->getInstance();
	map<int, string>m = db->querySelectQuestions(id);
	map<int, string> v = db->querySelectUsersQuestions(id, idUser);
	map<int, string> an;
	int mark = 0;
	int cor = 0;
	vector<string> nameV;
	vector<int> idV;
	int i = 1;
	int move = 0;
	string lastAns = "";
	string temp = "";
	int lastAnsId = 0;
	if (m.size() > 0)
	{
		if (pass != -1 && finished != 2)
		{
			for (auto n: m)
			{
				db->queryUpdateUserQuestion(n.first, 0, 0, 0);
			}
		}
		for (auto n : m)
		{
			idV.push_back(n.first);
			nameV.push_back(n.second);
		}
		while (true)
		{
			
			
			system("cls");

			string temp = returnString39(nameV[move]);
			if (!(db->querySelectUserQ(idV[move], idUser, id)))
				db->queryInsertUserQuestion(temp, idV[move], id, idUser);
			cout << "Test: " << name << endl;
			cout << i << ". " << nameV[move] << endl;
			an = db->queryAllAnswers(idV[move]);
			if(v.size() >0 && move <= (v.size()-1))
				 temp = v.at(idV[move]);
			if (temp == nameV[move])
			{
				lastAnsId = db->querySelectIdChAns(idV[move], id, idUser);
				if(lastAnsId >0)
					cout << "Your last answer = " << an.at(lastAnsId) << endl;
			}
			int idA = returnPosition(an, 1);
			if (idA < 0)
			{
				switch (idA)
				{
				case -2:
					if (move != (m.size() - 1))
					{
						move++;
						i++;
					}
					else
						cout << "It is last question" << endl;
					break;
				case -3:
					if (move != 0)
					{
						move--;
						i--;
					}
					else
						cout << "It is first question" << endl;
					break;
				case -4:
					if (finishPass())
						return;
					else
						break;
				default:
					return;
				}
			}
			else
			{
				cor = db->querySelectCorAns(idA);
				if (cor == 1)
				{
					int tempmark = db->querySelectMarkOfUserQ(idV[move], idUser, id);
					mark = db->querySelectMarkOfQ(idV[move]);
					if (tempmark == 0.0)
					{
						if (mark > 0)
						{
							points += mark;
						}
					}

				}
				else
				{
					int tempmark = db->querySelectMarkOfUserQ(idV[move], idUser, id);
					if (tempmark != 0.0)
					{
						points -= tempmark;
					}
				}
				if (db->queryUpdateUserQuestion(idV[move], mark, cor, idA))
				{
					mark = 0;
					if (move != (m.size() - 1))
					{
						move++;
						i++;
					}
					else
					{
						cout << "It is last question. You may finish test." << endl;
						system("pause");
					}
				}
			}
		}
		
		
		
	}

	
	
	
}
bool Test::saveUnfin()
{
	DataBase* db = nullptr;
	db = db->getInstance();
	points = db->querySelectMarkQuestion(id);
	if (db->queryUpdateTest(id, count_question, 0, points))
		return true;
	else
	{
		cout << "Problem with database..";
		system("pause");
		return false;
	}
}
bool Test::saveFin()
{
	DataBase* db = nullptr;
	db = db->getInstance();
	if (count_question >= 1)
	{
		points = db->querySelectMarkQuestion(id);
		if (points > 0.0)
		{
			if (db->querySelectQC0(id))
			{
				cout << "You have question which has less than two answers and this test will automatically be considered incomplete and will not be shown to the user!" << endl;
				cout << "This test can't save as finished test" << endl;
				system("pause");
				return false;
			}
			else
			{
				if (db->queryUpdateTest(id, count_question, 1, points))
				{
					cout << "This test saved as finished" << endl;
					system("pause");
					return true;
				}

			}
		}
		else
		{
			cout << "Problem with database.." << endl;
			system("pause");
			return false;
		}
	}
	else
	{
		cout << "There are no questions in this test yet and this test can't save as finished." << endl;
		system("pause");
		return false;
	}
}
void Test::deleteTestBase()
{
	DataBase* db = nullptr;
	db = db->getInstance();
	cout << "Test: " << name << endl;
	cout << "If you delete this test, it can't be restored!!!" << endl;
	int choose = Menu::select_vertical({ "Delete this test", "Exit" }, HorizontalAlignment::Left, 2);
	switch (choose)
	{
	case 0:
		if (db->queryDeleteTest(id))
			cout << "The test was succesfully removed" << endl;
		else
			cout << "Problem with database.." << endl;
		system("cls");
		return;
	case 1:
		
		return;
	default:
		return;
	}
}
void Test::show()
{
	DataBase* db = nullptr;
	db = db->getInstance();
	while (true)
	{
		system("cls");
		cout << "Read only mode" << endl;
		cout << "Test: " << name << "; Questions: " << endl;
		map<int, string>m = db->querySelectQuestions(id);
		int idQ = returnPosition(m, 0);
		if (idQ == -1)
			return;
		while (true)
		{
			system("cls");
			cout << "Question: " << db->querySelectNameOfQ(idQ)<< endl;
			cout << "Answers: " << endl;
			map<int, string>c = db->queryAllAnswers(idQ);
			for (auto a : c)
			{
				cout << a.second << endl;
			}
			int choose = Menu::select_vertical({ "Exit" }, HorizontalAlignment::Center, 10);
			switch (choose)
			{
			case 0:
				break;
			default:
				break;
			}
			break;
		}
		
	}
	

}
void Test::chooseQuestion()
{
	system("cls");
	DataBase* db = nullptr;
	db = db->getInstance();
	cout << "Test: " << name << endl;
	map<int, string>m = db->querySelectQuestions(id);
	int idQ = returnPosition(m, 0);
	if (idQ == -1)
		return;
	Question q(idQ, id);
	q.SetName(m.at(idQ));
	q.SetMark(db->querySelectMarkOfQ(idQ));
	q.SetCount(db->querySelectCountOfQ(idQ));
	q.menu();
	
}
bool Test::finish()
{

	system("cls");
	if (finished == 0 || finished == 2)
	{
		int choose = Menu::select_vertical({ "Save as unfinished test",
											"Save as finished test",
											"Exit"}, HorizontalAlignment::Left, 5);
		switch (choose)
		{
		case 0:
			if (idUser == 0)
				return saveUnfin();
			else
			{

				return true;
			}
		case 1:
			if(idUser == 0)
				return saveFin();
			{
				return saveUserFin();
			}
		case 2:
			return false;
		}
	}
	
}
void Test::deleteSelf()
{
	system("cls");
	cout << "Delete test" << endl;
	
	DataBase* db = nullptr;
	db = db->getInstance();
	map<int, string> m = db->queryAllCategories();
	cout << "Choose category:" << endl;
	int catId = returnPosition(m, 0);
	if (catId == -1)
		return;
	system("cls");
	cout << m.at(catId) << endl;
	cout << "Choose test: " << endl;
	map<int, string> b = db->queryAllTests(catId);
	int testId = returnPosition(b, 0);
	if (testId == -1)
		return;
	id = testId;
	name = b.at(testId);
	idCat = catId;
	while (true) {
		int choose = Menu::select_vertical({ "Show",
											"Delete",
												"Exit" }, HorizontalAlignment::Left, 5);
		switch (choose)
		{
		case 0:
			show();
			break;
		case 1:
			deleteTestBase();
			return;
		case 2:
			return;
		default:
			break;
		}
	}
}
void Test::menu()
{
	DataBase* db = nullptr;
	db = db->getInstance();
	while (true)
	{
		if (finished == 1)
		{
			cout << "This test is finished and read only" << endl;
		}
		int choose = Menu::select_vertical({ "New question",
														"Modify question",
														"Delete question",
														"Show",
														"Finish" }, HorizontalAlignment::Left, 5);
		Question q(id);
		switch (choose)
		{
		case 0:
			if (finished == 0)
			{
				system("cls");
				cout << "Test: " << name << endl;
				if (q.create())
				{
					count_question++;
					points = db->querySelectMarkQuestion(id);
					db->queryUpdateTest(id, count_question, 0, points);

				}
					
			}
				
			else
			{
				cout << "Finished test is read only. Use 'Show' to see the test" << endl;
			}
			break;
		case 1:
			if (finished == 0)
			{
				if (count_question >= 1)
				{
					chooseQuestion();
				}
				else
				{
					cout << "There are no questions in this test yet. Create the questions first" << endl;
					system("pause");
				}
			}
				
			else
			{
				cout << "Finished test is read only. Use 'Show' to see the test" << endl;
			}
			break;
		case 2:
			if (finished == 0)
			{
				if (count_question >= 1)
				{
					if (q.deleteQ())
					{
						count_question--;
						points = db->querySelectMarkQuestion(id);
						db->queryUpdateTest(id, count_question, 0, points);
					}
						
				}
				else
				{
					cout << "There are no questions in this test yet" << endl;
					system("pause");
				}
			}	
			else
			{
				cout << "Finished test is read only. Use 'Show' to see the test" << endl;
			}
			break;
		case 3:
			show();
			break;
		case 4:
			if (finish())
				return;
			else
				break;
		default:
			return;


		}
	}
}
bool menuForRepeatInput()
{
	int choose = Menu::select_vertical({ "Return to the main window", "Input again" }, HorizontalAlignment::Left, 5);
	return bool(choose);
}
void Test::create()
{
	system("cls");
	cout << "Create new test" << endl;
	int catId = 0;
	string tempNameTest = "";
	DataBase* db = nullptr;
	string temp = "";
	db = db->getInstance();
	map<int, string> m = db->queryAllCategories();
	if (m.size() > 0)
	{
		cout << "Categories:" << endl;
		catId = returnPosition(m, 0);
		if (catId == -1)
			return;
		system("cls");
		while (true)
		{
			cout << "Category: " << m.at(catId) << endl;
			cout << "Input name of new test: ";
			getline(cin, tempNameTest);
			if (tempNameTest.size() > 0)
			{
				temp = returnString39(tempNameTest);
				if (db->queryCatTest(temp, 2, catId))
				{
					cout << "Test with that name already exists" << endl;
					if (menuForRepeatInput())
						continue;
					else
						return;
				}
				else
					break;
			}
			else
			{
				cout << "Empty string" << endl;
				if (menuForRepeatInput())
					continue;
				else
					return;
			}
		}
	}
	else
	{
		cout << "You don't have any categories created yet" << endl;
		cout << "First choose 'Create category'" << endl;
		system("pause");
		return;
	}
	if (db->queryInsertTest(temp, catId))
	{
		this->id = db->querySelectMaxIdTest();
		if (this->id >= 1)
		{
			this->name = tempNameTest;
			this->idCat = catId;
			this->finished = 0;
			menu();
		}
			
	}	
}