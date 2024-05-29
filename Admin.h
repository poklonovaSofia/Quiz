#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include"Menu.h"
#include "DataBase.h"
#include "BaseUser.h"
#include"MD5.h"
#include<algorithm>
#include<map>
#include<vector>
#include"Test.h"
#include"Question.h"
#include "Creators.h"
#include "User.h"

using namespace std;

class Admin:public BaseUser
{
    
public:

	virtual void menu()override;
    virtual bool registration() override;
	
	virtual map<int, bool> signUp(int stat) override;
    
    void createCategory();
    
    void deleteCategory();
    void CategoryMenu();
    void TestMenu();
    void UsersMenu();
    bool menuForRepeatInput();
    void chooseTest();
    void showTests(int idCat);
    void deleteCategoryBase(int idCat);
    void createUser();
    void modifyUser();
    void deleteUser();
    void statisticUser();
    void statisticTest();
    void statisticCategory();
    map<int, vector<string>> chooseUser();
    
};
void Admin::statisticUser()
{
    map<int, vector<string>> f = chooseUser();
    if (f.size() > 0)
    {
        string info = "";
        int idUser = 0;
        for (auto m : f)
        {
            idUser = m.first;
            for (auto i : m.second)
            {
                info += i;
                info += "; ";
            }
        }
        User u(idUser);
        u.setPib(info);
        u.showRaitings();


    }

}
void Admin::statisticTest()
{
    DataBase* db = nullptr;
    db = db->getInstance();
    db->queryInfoTest();
}
void Admin::statisticCategory()
{
    DataBase* db = nullptr;
    db = db->getInstance();
    db->queryInfoCategory();
}
map<int, vector<string>> Admin::chooseUser()
{
    DataBase* db = nullptr;
    db = db->getInstance();
    map<int, vector<string>> m =db->querySelectInfoUser();
    map<int, string> idInfo;
    map<int, vector<string>> f;
    string infoUser = "";
    for (auto n : m)
    {

        for (auto i : n.second)
        {
            infoUser += i;
            infoUser += "; ";
        }
        
        idInfo.insert({ n.first, infoUser });
        infoUser = "";
    }
    int idUser = 0;
    idUser = returnPosition(idInfo, 0);
    if (idUser == -1)
        return f;
    
    f.insert({ idUser, m.at(idUser) });
    return f;
    

}
void Admin::modifyUser()
{
    DataBase* db = nullptr;
    db = db->getInstance();
    map<int, vector < string >> n = chooseUser();
    if (n.size() > 0)
    {
        string snf = "";
        string address = "";
        string number = "";
        int idUser = 0;
        for (auto m : n)
        {
            idUser = m.first;
        }
        if (idUser != 0)
        {
            for (auto m : n)
            {

                cout << "If you don't want to change, click 'Enter'.In order to change, input a new value" << endl;
                for (int i = 0; i < m.second.size(); i++)
                {
                    if (i == 0)
                    {
                        cout << "Your last snf: ";
                        cout << m.second[i];
                        cout << endl << "New: ";
                        getline(cin, snf);
                        if (snf.size() == 0)
                        {
                            snf = m.second[i];
                        }
                    }
                    else if (i == 1)
                    {
                        cout << "Your last address: ";
                        cout << m.second[i];
                        cout << endl << "New: ";
                        getline(cin, address);
                        if (address.size() == 0)
                        {
                            address = m.second[i];
                        }
                    }
                    else if (i == 2)
                    {
                        bool flaf = true;
                        cout << "Your last number: ";
                        cout << m.second[i];
                        cout << endl << "New: +380";

                        getline(cin, number);
                        for (int i = 0; i < number.size(); i++)
                        {

                            if (!(isdigit(number[i])))
                            {
                                flaf = false;
                                break;
                            }

                        }
                        if (number.size() == 0 || flaf == false)
                        {

                            number = m.second[i];
                        }
                    }
                }
                break;
            }
            system("cls");
            address = returnString39(address);
            snf = returnString39(snf);
            if (db->queryUpdateInfoUser(idUser, snf, address, number))
            {
                cout << "New values were added" << endl;
            }
            else
            {
                cout << "Problem with database..";
            }
            system("pause");
            system("cls");

        }
    }
}
void Admin::deleteUser()
{
    DataBase* db = nullptr;
    db = db->getInstance();
    map<int, vector < string >> n = chooseUser();
    if (n.size() > 0)
    {
        string info = "";
        int idUser = 0;
        for (auto m : n)
        {
            idUser = m.first;
            for (auto i : m.second)
            {
                info += i;
                info += "; ";
            }
        }
        if (idUser != 0)
        {
            system("cls");
            cout << "User: " << info;
            cout << "Do you want to delete this user? All information were destroyed!" << endl;
            int choose = Menu::select_vertical({ "Delete this user", "Exit" }, HorizontalAlignment::Left, 2);
            switch (choose)
            {
            case 0:
                if (db->queryDeleteUser(idUser))
                    cout << "The user was succesfully removed" << endl;
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
    }
}
void Admin::createUser()
{
    User u;
    u.signUp(1);
}
void Admin::showTests(int idCat)
{
    DataBase* db = nullptr;
    db = db->getInstance();
    system("cls");
    map<int, string> m = db->queryAllTests(idCat);
    cout << "Tests:" << endl;
    for (auto n:m)
    {
        cout << n.second << endl;
    }
    int choose = Menu::select_vertical({ "Exit" }, HorizontalAlignment::Center, 10);
    switch (choose)
    {
    case 0:
        break;
    default:
        break;
    }
   
}

void Admin::deleteCategoryBase(int idCat)
{
    DataBase* db = nullptr;
    db = db->getInstance();
    map<int, string> m = db->queryAllTests(idCat);
    cout << "If you delete this category, it can't be restored!!!All tests in this category will be deleted!" << endl;
    int choose = Menu::select_vertical({ "Delete this category", "Exit" }, HorizontalAlignment::Left, 2);
    switch (choose)
    {
    case 0:
        for (auto n : m)
        {
            db->queryDeleteTest(n.first);
        }
        if (db->queryDeleteCategory(idCat))
            cout << "The category was succesfully removed" << endl;
        else
            cout << "Problem with database.." << endl;

        return;
    case 1:
        
        return;
    default:
        return;
    }


   
    
    
}
void Admin::chooseTest()
{
    system("cls");
    DataBase* db = nullptr;
    db = db->getInstance();
    map<int, string> m = db->queryAllCategories();
    cout << "Categories:" << endl;
    int catId = returnPosition(m, 0);
    if (catId == -1)
        return;
    system("cls");
    cout << m.at(catId) << endl;
    cout << "Tests: " << endl;
    map<int, string> b = db->queryAllTests(catId);
    int testId = returnPosition(b, 0);
    if (testId == -1)
        return;
    
    int fin = db->querySelectFinished(testId);
    if (fin >= 0)
    {
        Test t(testId, catId, b.at(testId), fin);
        t.setCount(db->querySelectCountOfTest(testId));
        t.setPoints(db->querySelectMarkOfTest(testId));
        t.menu();
    }
    else
    {
        cout << "Oops.." << endl;
    }  
}

void Admin::createCategory()
{
    system("cls");
    bool flag = false;
    DataBase* db = nullptr;
    db = db->getInstance();
    string nc = "";
    string temp = "";
    cout << "Input name of category: ";
    getline(cin, nc);
    if (nc.size() > 0) {
        temp = returnString39(nc);
        if (db->queryCatTest(temp, 1))
        {
            cout << "Category with that name already exists" << endl;
        }
        else
        {
            if (db->queryInsertCategory(temp))
                cout << "Category " + nc + " was added" << endl;
            else
                cout << "Problem with database.." << endl;
        }
    }
    system("pause");
    system("cls");
}
void Admin::deleteCategory()
{
    system("cls");
    cout << "Delete category" << endl;
    DataBase* db = nullptr;
    db = db->getInstance();
    map<int, string> m = db->queryAllCategories();
    cout << "Choose category:" << endl;
    int catId = returnPosition(m, 0);
    if (catId == -1)
        return;
    while (true) {
        system("cls");
        cout << "You in category: " << m.at(catId) << endl;
        int choose = Menu::select_vertical({ "Show tests",
                                            "Delete",
                                                "Exit" }, HorizontalAlignment::Left, 5);
        switch (choose)
        {
        case 0:
            showTests(catId);
            break;
        case 1:
            cout << "You in category: " << m.at(catId) << "; ";
            deleteCategoryBase(catId);
            return;
        case 2:
            return;
        default:
            break;
        }
    }
}
void Admin::UsersMenu()
{
    while (true)
    {
        system("cls");
        cout << "Users" << endl;
        int choose = Menu::select_vertical({ "New", "Modify", "Delete", "Statistics","Exit" }, HorizontalAlignment::Left, 5);
        switch (choose)
        {
        case 0:
            createUser();
            break;
        case 1:
            modifyUser();
            break;
        case 2:
            deleteUser();
            break;
        case 3:
            statisticUser();
            break;
        case 4:
            return;
        default:
            break;
        }

    }
}
void Admin::CategoryMenu()
{
    while (true)
    {
        system("cls");
        cout << "Categories" << endl;
        int choose = Menu::select_vertical({ "New", "Delete","Statistics", "Exit"}, HorizontalAlignment::Left, 5);
        switch (choose)
        {
        case 0:
            createCategory();
            break;
        case 1:
            deleteCategory();
            break;
        case 2:
            statisticCategory();
            break;
        case 3:
            return;
        default:
            break;
        }

    }
}
void Admin::TestMenu()
{
    while (true)
    {
        system("cls");
        cout << "Tests" << endl;
        int choose = Menu::select_vertical({ "New", "Modify", "Delete", "Statistics", "Exit"}, HorizontalAlignment::Left, 5);
        Test t;
        switch (choose)
        {
        case 0:
            
            t.create();
            break;
        case 1:
            chooseTest();
            break;
        case 2:
            t.deleteSelf();
            break;
        case 3:
            statisticTest();
            break;
        case 4:
            return;
        default:
            break;
        }

    }
}
bool Admin::menuForRepeatInput()
{
    int choose = Menu::select_vertical({ "Return to the main window", "Input again" }, HorizontalAlignment::Left, 2);
    return bool(choose);
}


 void Admin::menu()
{
     system("cls");
     while (true)
     {
         cout << "ADMIN: " << getPib() << endl;
         int choose = Menu::select_vertical({ "Category", "Test", "User",  "Exit" }, HorizontalAlignment::Center);
         
         switch (choose)
         {
         case 0:
             CategoryMenu();
             break;
         case 1:
             TestMenu();
             break;
         case 2:
             UsersMenu();
             break;
         case 3:
             return;
         default:
             break;
         }
     }
    
}
 map<int, bool> Admin::signUp(int stat)
{
    system("cls");
    bool reg = false;
    bool flaf = true;
    string l = "";
    map<int, bool> m;
    string p = "";
    DataBase* db = nullptr;
    db = db->getInstance();
    while (true) {
        cout << "Input login: ";
        getline(cin, l);
        
        if (l.size() > 0) {
            l = md5(l);
            break;
        }
          
        else
        {
            int choose = Menu::select_vertical({ "Input again", "Return to the main window" }, HorizontalAlignment::Center);
            if (choose == 0)
                continue;
            else
               
                return m;
        }
    }
    
    
    while (true) {
        cout << "Input password: ";
        getline(cin, p);
        
        if (p.size() > 0)
        {
            p = md5(p);
            break;
        }
            
        else
        {
            int choose = Menu::select_vertical({ "Input again", "Return to the main window" }, HorizontalAlignment::Center);
            if (choose == 0)
                continue;
            else
                return m;
        }
    }
    string pib = "";
    string pibT = "";
    while (true)
    {
        cout << "Input PIB: ";
        getline(cin, pib);
        if (pib.size() > 0)
        {
            pibT = returnString39(pib);
            break;
        }
        else
        {
            cout << "Empty string" << endl;
            system("pause");
            system("cls");
            continue;
        }
    }
    setPib(pib);
    string addressT = "";
    string numberT = "";
    if (flaf) {
        if (db->queryInsertUser(l, p, pibT, addressT, numberT,  1)) {
            cout << "You signed up! Welcome" << endl;
            int id = db->querySelectMaxIdUser();

            reg = true;
            m.insert({ id, reg });
        }
    }
    return m;
    system("pause");
}

bool Admin::registration()
{
    system("cls");
    DataBase* db = nullptr;
    db = db->getInstance();
    map <int, bool> m;
    bool reg = false;
    if (db->getIsAdmin()) {
        int choose = Menu::select_vertical({ "Sign in", "Exit" }, HorizontalAlignment::Center);
        switch (choose)
        {
        case 0:
            m = signIn(1);
            break;
        case 1:
            
            return reg;
        default:
            break;
        }
        for (auto n : m)
        {
            reg = n.second;
        }

    }
    else {
        cout << "Registration admin" << endl;
        int choose = Menu::select_vertical({ "Sign up", "Exit" }, HorizontalAlignment::Center);
        switch (choose)
        {
        case 0:
            m = signUp(1);
            break;
        case 1:
            exit(0);
        default:
            break;
        }
        for (auto n : m)
        {
            reg = n.second;
        }
    }
    
    return reg;
}