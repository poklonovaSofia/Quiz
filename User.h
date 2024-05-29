#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include "DataBase.h"
#include"Question.h"
#include"BaseUser.h"
#include"Test.h"
#include"MD5.h"
using namespace std;
class User:public BaseUser
{
    
public:
    User(){}
    User(int i):id(i){}
    int id;

	virtual void menu()override;
    virtual bool registration() override;
   
    void showRaitings();
    void menuPass();
    void chooseTest();
    void passUnfinishedTest();
    
    int getId() { return id; }

};

void User::chooseTest()
{
    system("cls");
    DataBase* db = nullptr;
    db = db->getInstance();
    map<int, string> m = db->queryAllCategoriesForUser(id);
    cout << "Categories:" << endl;
    int catId = returnPosition(m, 0);
    if (catId == -1)
        return;
    system("cls");
    cout << m.at(catId) << endl;
    cout << "Tests: " << endl;
    map<int, string> b;
    map<map<int, string>, int> d= db->queryAllFinishedTests(catId, id);
    for (auto n : d)
    {
        for (auto c : n.first)
        {
            b.insert({ c.first, c.second });
        }
    }
    int testId = returnPosition(b, 0);
    if (testId == -1)
        return;
    map<int, string> v;
    v.insert({ testId, b.at(testId) });
    int pass =  d.at(v);
    if (pass != -1)
    {
        cout << "Test: " << b.at(testId) << " was passed. If you start to pass again, the previous result will be deleted!" << endl;
        cout << "Start?" << endl;
        int choose = Menu::select_vertical({ "Yes", "No" }, HorizontalAlignment::Center);
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
    Test testt(testId, catId, b.at(testId), 0, pass, id);
    if (pass == -1)
    {
        if (db->queryInsertUsersTest(id, testId, b.at(testId), m.at(catId)))
        {
            testt.setPoints(0);
            testt.setCount(db->querySelectCountOfTest(testId));
            testt.menuPass();
            
        }
    }
    else
    {
        if (db->queryUpdatePartUserTest(id, testId, 0, 0))
        {
            testt.setPoints(0);
            testt.setCount(db->querySelectCountOfTest(testId));
            testt.menuPass();
            
        }
        else
        {
            cout << "Problem with database";
        }
        
    }

}
void User::passUnfinishedTest()
{
    system("cls");
    DataBase* db = nullptr;
    db = db->getInstance();
    map<int, string> m = db->queryAllCategoriesForUserUn(id);
    cout << "Categories:" << endl;
    int catId = returnPosition(m, 0);
    if (catId == -1)
        return;
    system("cls");
    cout << m.at(catId) << endl;
    cout << "Unfinished tests: " << endl;
   
    map<int, string> d = db->queryAllFinishedTestsUn(catId, id);
    int testId = returnPosition(d, 0);
    if (testId == -1)
        return;
    int pass = 0;
    if (db->querySelectCountPass(testId, id))
        pass = -1;

    Test te(testId, catId, d.at(testId), 2, pass, id);
    te.setPoints(0);
    te.setCount(db->querySelectCountOfTest(testId));
    
    te.menuPass();

}
void User::menuPass()
{
    while (true)
    {
        system("cls");
        int choose = Menu::select_vertical({ "New test", "Unfinished test", "Exit" }, HorizontalAlignment::Center);
        switch (choose)
        {
        case 0:
            chooseTest();
            break;
        case 1:
            passUnfinishedTest();
            break;
        case 2:
            return;

        default:
            break;
        }
    }
}
void User::showRaitings()
{
    system("cls");
    cout << "User: " << getPib() << endl;
    DataBase* db = nullptr;
    db = db->getInstance();
    int points = 0;
    int countCor = 0;
    int countQ = 0;
    float aver = 0;
    int countPass = 0;
    int z = 0;
    map<int, map<string, string>> idTNameNameCat = db->querySelectUsersFinished(id);
    for (auto n : idTNameNameCat)
    {
        map<int, int>pointCountCor = db->querySelectMarkOfUnQs(n.first, id);
        for (auto d : pointCountCor)
        {
            points = d.first;
            countCor = d.second;
        }
        
        countQ = db->querySelectCountUsersQ(n.first, id);
        points = (float)countCor / (float)countQ * 12;
        map<float, int> averCountPass = db->querySelectAverCount(n.first, id);
        for (auto i : averCountPass)
        {
            aver = i.first;
            countPass = i.second;
        }
        for (auto i : n.second)
        {
            if(z == 0)
                cout << "***" << endl;
            cout << "Category: " << i.second << ";" << endl << "Test: " << i.first << "; " << endl;
            cout << "Mark: " << points << ";" << endl << "Count correct : " << countCor << "/" << countQ << ";" << endl << "Percent correct: " << countCor * 100 / countQ << "%;" << endl;
            cout << "Average mark of all passed: " << aver << "; " << endl << "Count passed: " << countPass << ";" << endl;
            cout << "***" << endl;
            if (z == 0)
                z++;
        }
    }
    system("pause");
}
void User::menu()
{
    while (true)
    {
       
        system("cls");
        cout << "User: " << getPib() << endl;
        int choose = Menu::select_vertical({ "Pass the test", "See the raitings", "Exit" }, HorizontalAlignment::Center);
        switch (choose)
        {
        case 0:
            menuPass();
            break;
        case 1:
            showRaitings();
            break;
        case 2:
            return;

        default:
            break;
        }
    }
}

bool User::registration()
{
    system("cls");
    map<int, bool> m;
    bool reg = false;
    int choose = Menu::select_vertical({ "Sign in", "Sign up", "Exit"}, HorizontalAlignment::Center);
    switch (choose)
    {
    case 0:
        m = signIn(0);
        break;
    case 1:
        m = signUp(0);
        break;
    case 2:
        return reg;
    default:
        break;
       
    }
    for (auto n : m)
    {
        reg = n.second;
        id = n.first;
    }
    
    return reg;
}
