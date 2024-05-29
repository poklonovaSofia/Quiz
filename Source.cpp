#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include"DataBase.h"
#include"Creators.h"
#include<fstream>

#include "MD5.h"

using namespace std;

int main()
{
  
    CreatorBaseUser* cbu = nullptr;
    DataBase* db = nullptr;
    db = db->getInstance();
    ifstream infoDB;
    infoDB.open("infoDB.txt");
    string temp = "";
    if (infoDB.is_open())
    {
        getline(infoDB, temp);
        db->setDriver(temp);
        
        getline(infoDB, temp);
        db->setServer(temp);
       
        getline(infoDB, temp);
        db->setPort(temp);

        getline(infoDB, temp);
        db->setUID(temp);

        getline(infoDB, temp);
        db->setPWD(temp);    
    }
    infoDB.close();
    bool conn = db->connect();
    while (true) {
        system("cls");
        if (!(db->queryAdmin()) )
        {
            cbu = new CreatorAdmin();  
        }
        else
        {
            int choose = Menu::select_vertical({ "User", "Admin", "Exit"}, HorizontalAlignment::Center);
            switch (choose)
            {
            case 0:
                cbu = new CreatorUser();
                break;
            case 1:
                cbu = new CreatorAdmin();
                db->setAdmin(true);
                break;
            case 2:
                exit(0);
            default:
                break;
            }
        }
        BaseUser* bu = cbu->create_buser();
        
        if (bu->registration())
            bu->menu();
    }
}