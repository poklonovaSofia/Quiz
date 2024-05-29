#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include <sql.h>
#include"MD5.h"
#include "Menu.h"
#include<Windows.h>
#include "DataBase.h"
using namespace std;
class BaseUser
{
	string login;
	string password;
    string pib;
public:
	virtual ~BaseUser() {}
	virtual void setLogin(string log) { login = log; }
	virtual void setPassword(string pass) { password = pass; }

    virtual bool registration() = 0;
	virtual void menu()=0;
	virtual map<int, bool> signIn(int stat);
	virtual  map<int, bool> signUp(int stat);
    virtual string getPib() { return pib; }
    virtual void setPib(string p) { pib = p; }
    
};

map<int, bool> BaseUser::signIn(int stat)
{
    system("cls");
    bool reg = false;
    string l = "";
    string p = "";
    map<int, bool> m;
    cout << "Input login: ";
    getline(cin, l);
    if (l.size() > 0)
        l = md5(l);
    else
    {
        cout << "Empty string";
        system("pause");
        return m;
    }
        
    
    cout << "Input password: ";
    getline(cin, p);
    if (p.size() > 0)
        p = md5(p);
    else
    {
        cout << "Empty string";
        system("pause");
        return m;
    }
  
    
    DataBase* db = nullptr;
    db = db->getInstance();
    
    if (db->queryLoginPass(l, p, 3, stat))
    {
        setLogin(l);
        setPassword(p);
        int id = db->querySelectIdBU(l, stat);
        
        cout << "You signed in" << endl;
        reg = true;
        pib = db->querySelectNameUser(l, p);
        m.insert({ id, reg });
        system("pause");
    }
    return m;
}
map<int, bool> BaseUser::signUp(int stat)
{
    system("cls");
    bool reg = false;
    string l = "";
    map<int, bool> m;
    string p = "";
    DataBase* db = nullptr;
    db = db->getInstance();
    while (true)
    {
        cout << "Input login: ";
        getline(cin, l);
        if (l.size() > 0) {
            l = md5(l);
            if (db->queryLoginPass(l, p, 1))
            {
                cout << "This login is already taken" << endl;
                system("pause");
                system("cls");
                int choose = Menu::select_vertical({ "Input again", "Return to the main window" }, HorizontalAlignment::Center);
                if (choose == 0)
                    continue;
                else
                    return m;
            }
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
    while (true)
    {
        cout << "Input password: ";
        /*HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(hStdin, &mode);
        SetConsoleMode(hStdin, mode & (-ENABLE_ECHO_INPUT));*/
        getline(cin, p);
        if (p.size() > 0)
        {
            p = md5(p);
            break;
        }
        else
        {
            cout << "Empty string" << endl;
            system("pause");
            system("cls");
            continue;
        }
        /*SetConsoleMode(hStdin, mode);*/
        //tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
    

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
    string addressT = "";
    while (true)
    {
        cout << "Input address: ";
        getline(cin, addressT);
        if (addressT.size() > 0)
        {
            addressT = returnString39(addressT);
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
    string numberT = "+380";
    string number = "";
    while (true)
    {
        bool flaf = true;
        cout << "Input number of phone: +380";
        getline(cin, number);
        if (number.size() ==  9)
        {
            numberT += number;
            if (!(db->querySelectNumber(numberT)))
            {
                for (int i = 0; i < number.size(); i++)
                {

                    if (!(isdigit(number[i])))
                    {
                        flaf = false;
                        break;
                    }

                }
                if (flaf == false)
                {
                    cout << "Incorrect phone number" << endl;
                    continue;
                }
                    
               
                break;
            }
            else
            {
                cout << "This number already exists" << endl;
                system("pause");
                system("cls");
                continue;

            }
        }
        else
        {
            cout << "No correct phone. You must input 9 number" << endl;
            system("pause");
            system("cls");
            continue;
        }
    }
   if (db->queryInsertUser(l, p, pibT, addressT, numberT, 0))
    {
       if (stat == 0)
       {
           cout << "You signed up! Welcome" << endl;
           int id = db->querySelectMaxIdUser();
           reg = true;
           m.insert({ id, reg });
       }
       else
       {
           cout << "User was added" << endl;
       }

       
    }
   else
   {
       cout << "Problem with database..";
   }
    return m;
    system("pause");
}