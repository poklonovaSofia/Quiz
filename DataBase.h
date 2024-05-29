#pragma once
#include<iostream>
#include<string.h>
#include<string>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <algorithm>
#include<iomanip>
#include <map>
#include<vector>
using namespace std;
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000

string returnString39(string name)
{
    string temp = "";
    for (int i = 0; i < name.size(); i++)
    {
        if (name[i] == 39)
        {
            temp += "''";
        }
        else
        {
            temp += name[i];
        }

    }
    return temp;
}
class DataBase
{
private:

    //initializations
   
	string password;
	string username;
	string name;
    string driver;
    string port;
    bool connected;
	string server;
    int sql_s;
    bool isAdmin = false;
    SQLHANDLE sqlConnHandle;
    SQLHANDLE sqlEnvHandle;
    SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
    
	DataBase() {

        connected = false;
        password = "";
        username = "";
        port = "";
        server = "";
        name = "Exam";
    }
	static DataBase* instance;
public:
    void setDriver(string d) { driver = d; }
    void setServer(string s) { server = s; }
    void setPort(string p) { port = p; }
    void setUID(string u) { username = u; }
    void setPWD(string p) { password = p; }
	static DataBase* getInstance();
    ~DataBase();
	bool connect();
    bool queryAdmin();
    bool queryLoginPass(string loginstr, string password, int type, int stat = -1);

    bool queryInsertUser(string loginstr, string password, string pib, string address, string phone, int stat);
    bool queryInsertCategory(string name);
    bool queryInsertQuestion(string q, int idT);
    bool queryInsertTest(string name, int idCat);
    bool queryInsertAnswer(string answer, int idT, int idQ);
    bool queryInsertUsersTest(int idU, int idT, string name, string catName);
    bool queryInsertUserQuestion(string name, int idQ, int idT, int idUser);
    bool queryUpdateInfoUser(int idUser, string snf, string address, string number);
    int querySelectCorAns(int idA);
    map<int, vector<string>> querySelectInfoUser();

    void setAdmin(bool v) { isAdmin = v; }
    //int querySelectIdTest(string nTest, int idCat);
    //int querySelectIdQuestion(string name, int idT);
    
    bool querySelectQ(string question, int idT);
    bool querySelectCountPass(int idT, int idUser);
    int querySelectFinished(int id);
    bool getIsAdmin() { return isAdmin; }
    
    SQLWCHAR* StringToSQLWChar(string query);
    SQLWCHAR* StringToSQLWCharConnect(string query);
    bool queryCatTest(string info, int stat, int idCat = 0);
    bool queryDeleteAnswers( int IdQ, int idT);
    bool queryDeleteAnswer(int id, int IdQ, int idT);
    bool queryDeleteQuestion(int IdQ, int idT);
    int querySelectMarkQuestion(int id);
    string querySelectNameOfQ(int id);
    string querySelectAnsName(int id);
    float querySelectMarkOfQ(int id);
    float querySelectMarkOfTest(int id);
    int querySelectIdBU(string login, int stat);
    int querySelectIdChAns(int idQ, int idT, int idUser);
    bool queryDeleteUser(int idUser);

    map< int, string> queryAllCategories();
    map< int, string> queryAllCategoriesForUser(int id);
    map<int, string> queryAllCategoriesForUserUn(int id);
    map< int, string> queryAllTests(int idCat);
    map< int, string> querySelectQuestions(int idT);
    map< int, string> queryAllAnswers(int idQ);
    map< int, int> queryAllAnswersCor(int idQ);
    map< int, string> querySelectUsersQuestions(int idT, int idUser);
    map<map<int, string>, int>  queryAllFinishedTests(int catId, int id);
    map<int, string> queryAllFinishedTestsUn(int catId, int id);
    map<float, int> querySelectAverCount(int idT, int idUser);
    map<int, map<string, string>> querySelectUsersFinished(int id);

    bool queryUpdateQuestionName(string name, int id);
    bool queryUpdateAnswerName(string name, int id);
    bool queryUpdateQuestion(float points, int count, int id);
    bool queryUpdateAnswer(int id, int idQ, int idT);
    bool queryUpdateTest(int id, int countQ, int finish, int mark);
    bool queryUpdateCorrectAns(int id, int cor);
    bool queryUpdateCountQuestion(int id, int count);
    bool queryUpdateUserQuestion(int idQ, int mark, int cor, int idA);
    bool queryUpdateUserTest(int idT, int idUser, int mark, int fin, float average, int count);
    bool queryUpdatePartUserTest(int idUser, int idT, int fin, int mark);

    int querySelectMaxIdQ();
    int querySelectMaxIdTest();
    int querySelectCountOfQ(int id);
    int querySelectCountOfTest(int id);
    int querySelectMaxIdAns();
    bool querySelectQC0(int id);
    int querySelectMaxIdUser();
    int querySelectMinIdQIT(int id);
    bool querySelectUserQ(int idQ, int idUser, int idT);
    int querySelectMarkOfUserQ(int idQ, int idUser, int idT);
    int querySelectMarkOfUnTest(int idU, int testId);
    map<int, int> querySelectMarkOfUnQs(int idT, int idUser);
    bool querySelectFinQs(int id, int idUser);
    string querySelectNameUser(string l, string p);
    bool querySelectNumber(string numberT);
    bool queryInfoCategory();
    bool queryInfoTest();

    bool queryDeleteTest(int id);
    int querySelectCountUsersQ(int idTest, int id);
    bool queryDeleteCategory(int idCat);
};
bool DataBase::queryInfoTest()
{
    bool is = false;
    int sum = 0;
    int countUser = 0;
    int countMark;
    float average = 0.0;
    SQLCHAR  name[240];
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return is;
    }
    SQLWCHAR* ret1;
    int ii = 1;
    ret1 = StringToSQLWChar("SELECT COUNT(DISTINCT ID_USER), SUM(MARK), COUNT(MARK), NAME FROM USERS_TESTS WHERE FINISHED=1 GROUP BY NAME ORDER BY NAME");
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);

    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        SQLINTEGER ptrSqlVersion;

        cout << "  " << setw(50) << "Test" << setw(25) << "Count User" << setw(25) << "Average Mark " << endl;
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {

           
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &countUser, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_INTEGER, &sum, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 3, SQL_INTEGER, &countMark, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 4, SQL_C_CHAR, (SQLPOINTER)name, SQL_RESULT_LEN, &ptrSqlVersion);
            average = (float)sum / (float)countMark;
            cout << ii << ". " << setw(50) << name << setw(25) << countUser << setw(25) << average << endl;
            ii++;
        }
    }
    system("pause");
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::queryInfoCategory()
{
    bool is = false;
    int sum = 0;
    int count = 0;
    SQLCHAR  name[240];
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return is;
    }
    SQLWCHAR* ret1;
    int ii = 1;
    ret1 = StringToSQLWChar("SELECT SUM(COUNT_PASS), COUNT(DISTINCT ID_TEST), NAME_CAT FROM USERS_TESTS WHERE FINISHED=1 GROUP BY NAME_CAT  ORDER BY NAME_CAT");
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        
        cout << "  " << setw(50) << "Category" << setw(25) << "Count Passed" << setw(25) << "Count Test Passed " << endl;
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &sum, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_INTEGER, &count, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 3,  SQL_C_CHAR, (SQLPOINTER)name, SQL_RESULT_LEN, &ptrSqlVersion);
            cout << ii << ". " << setw(50) << name << setw(25) << sum << setw(25) << count << endl;
            ii++;
            

        }
    }
    system("pause");
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::querySelectNumber(string numberT)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return is;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("SELECT SNF FROM USERS WHERE NUMBER="+numberT).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        i = SQLFetch(sqlStmtHandle);
        if (i != SQL_NO_DATA) {

            is = true;
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
map<int, vector<string>>DataBase::querySelectInfoUser()
{
    int id = 0;
    SQLCHAR  snf[240];
    SQLCHAR  address[240];
    SQLCHAR  number[240];
    map<int, vector<string>>m;
    vector<string> info;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar("SELECT U.ID, U.SNF, U.ADDRESS, U.NUMBER FROM USERS AS U WHERE U.STATUS = 0");
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &id, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)snf, SQL_RESULT_LEN, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 3, SQL_C_CHAR, (SQLPOINTER)address, SQL_RESULT_LEN, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 4, SQL_C_CHAR, (SQLPOINTER)number, SQL_RESULT_LEN, &ptrSqlVersion);
            string  nStrsnf((const char*)snf);
            string  nStraddress((const char*)address);
            string  nStrnumber((const char*)number);
            info.push_back(nStrsnf);
            info.push_back(nStraddress);
            info.push_back(nStrnumber);
            m.insert({ id, info });
            info.clear();
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return m;
}
bool DataBase::queryUpdatePartUserTest(int idUser, int idT, int fin, int mark)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return is;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("UPDATE USERS_TESTS SET MARK =" + to_string(mark) + ", FINISHED =" + to_string(fin) + " WHERE ID_TEST=" + to_string(idT) + " AND ID_USER=" + to_string(idUser)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::querySelectFinQs(int id, int idUser)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return is;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("SELECT ID_CHOOSE_ANSWER FROM USERS_QUESTIONS WHERE ID_USER =" + to_string(idUser) + " AND ID_TEST =" + to_string(id) +" AND ID_CHOOSE_ANSWER= 0 ").c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        i = SQLFetch(sqlStmtHandle);
        if (i != SQL_NO_DATA) {

            is = true;
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
int DataBase::querySelectCountUsersQ(int idTest, int id)
{
    int count = 0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return count;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("SELECT COUNT(NAME) FROM USERS_QUESTIONS WHERE ID_TEST =" + to_string(idTest) + " AND ID_USER=" + to_string(id)).c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return count;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &count, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return count;
}
int DataBase::querySelectMarkOfUserQ(int idQ, int idUser, int idT)
{
    int mark = 0.0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return mark;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("SELECT MARK FROM USERS_QUESTIONS WHERE ID_QUESTION =" + to_string(idQ)+ " AND ID_TEST =" + to_string(idT) + " AND ID_USER="+to_string(idUser)).c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return mark;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &mark, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return mark;
}
map<int, int> DataBase::querySelectMarkOfUnQs(int idT, int idUser)
{
    int sum = 0;
    int cR = 0;
    map<int, int> m;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        return m;
    }
    SQLWCHAR* ret1;

    ret1 = StringToSQLWChar(("SELECT SUM(MARK), COUNT(CORRECT) FROM USERS_QUESTIONS WHERE ID_TEST=" + to_string(idT) + " AND ID_USER=" + to_string(idUser) + "AND CORRECT= 1").c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &sum, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_INTEGER, &cR, 4, &ptrSqlVersion);
            m.insert({ sum, cR });
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return m;
}
int DataBase::querySelectMarkOfUnTest(int idU, int testId)
{
    int mark = 0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return mark;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("SELECT MARK FROM USERS_TEST WHERE ID_USER =" + to_string(idU) + " AND ID_TEST =" + to_string(testId) + " AND FINISHED= 0 " ).c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return mark;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &mark, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return mark;
}
int DataBase::querySelectIdChAns(int idQ, int idT, int idUser)
{
    int id = 0.0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return id;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("SELECT ID_CHOOSE_ANSWER FROM USERS_QUESTIONS WHERE ID_QUESTION =" + to_string(idQ) + " AND ID_TEST =" + to_string(idT) + " AND ID_USER=" + to_string(idUser)).c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return id;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &id, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return id;
}
bool DataBase::queryUpdateCountQuestion(int id, int count)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("UPDATE TESTS SET COUNT_QUESTION =" + to_string(count) +  " WHERE ID = " + to_string(id)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::querySelectUserQ(int idQ, int idUser, int idT)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return is;
    }
    SQLWCHAR* ret1 = {};
    ret1 = StringToSQLWChar(("SELECT NAME FROM USERS_QUESTIONS WHERE ID_QUESTION = " + to_string(idQ) + " AND ID_USER =" + to_string(idUser) + " AND ID_TEST =" + to_string(idT)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        i = SQLFetch(sqlStmtHandle);
        if (i != SQL_NO_DATA) {

            is = true;
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::querySelectQC0(int id)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return is;
    }
    SQLWCHAR* ret1 = {};
    ret1 = StringToSQLWChar(("SELECT COUNT_ANSWERS FROM QUESTION WHERE ID_TEST =" + to_string(id) + " AND COUNT_ANSWERS<2" ).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        i = SQLFetch(sqlStmtHandle);
        if (i != SQL_NO_DATA) {

            is = true;
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
int DataBase::querySelectMaxIdAns()
{
    int Id = 0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return Id;
    }
   
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT MAX(ID) FROM ANSWERS", SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {


        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &Id, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return Id;

}
int DataBase::querySelectMaxIdQ()
{
    int Id = 0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return Id;
    }
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT MAX(ID) FROM QUESTION", SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &Id, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return Id;
}
int DataBase::querySelectMinIdQIT(int id)
{
    int Id = 0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return Id;
    }
    
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("SELECT MIN(ID) FROM QUESTION WHERE ID_TEST="+to_string(id)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &Id, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return Id;
}
int DataBase::querySelectMaxIdTest()
{
    int Id = 0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return Id;
    }
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT MAX(ID) FROM TESTS", SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &Id, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return Id;
}
int DataBase::querySelectMaxIdUser()
{
    int Id = 0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return Id;
    }
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT MAX(ID) FROM USERS", SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &Id, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return Id;
}
int DataBase::querySelectIdBU(string login, int stat)
{
    int Id = 0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return Id;
    }
 
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("SELECT ID FROM USERS WHERE LOGIN='" + login + "' AND STATUS ="+to_string(stat)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &Id, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return Id;
}
bool DataBase::queryUpdateTest(int id, int countQ, int finish, int mark)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("UPDATE TESTS SET MARK =" + to_string(mark) + ", COUNT_QUESTION=" + to_string(countQ) +", FINISHED="+ to_string(finish)+ " WHERE ID = " + to_string(id)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);


    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::queryUpdateQuestion(float points, int count, int id)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("UPDATE QUESTION SET MARK =" + to_string(points) + ", COUNT_ANSWERS="+to_string(count) +" WHERE ID =" + to_string(id)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
 bool DataBase::queryUpdateQuestionName(string name, int id)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("UPDATE QUESTION SET NAME ='"+ name+"' WHERE ID ="+ to_string(id)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
 bool DataBase::queryUpdateAnswerName(string name, int id)
 {
     bool is = false;
     SQLHANDLE sqlStmtHandle;
     sqlStmtHandle = NULL;
     if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
     {
         SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
         return false;
     }
     SQLWCHAR* ret1;
     ret1 = StringToSQLWChar(("UPDATE ANSWERS SET NAME ='" + name + "' WHERE ID =" + to_string(id)).c_str());
     int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
     if (SQL_SUCCESS != i) {
         SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
         return false;
     }
     else {
         is = true;
     }
     SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
     return is;
 }
bool DataBase::queryUpdateCorrectAns(int id, int cor)
 {
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("UPDATE ANSWERS SET CORRECT =" + to_string(cor) + " WHERE ID =" + to_string(id)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
 }
bool DataBase::queryUpdateUserTest(int idT, int idUser, int mark, int fin, float average, int count)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("UPDATE USERS_TESTS SET MARK =" +to_string(mark)+ ", AVERAGE_MARK=" +to_string(average)+", COUNT_PASS=" + to_string(count) + ", FINISHED =" + to_string(fin) + " WHERE ID_TEST="+to_string(idT) + " AND ID_USER="+to_string(idUser)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
 bool DataBase::queryUpdateAnswer(int id, int idQ, int idT)
 {
     bool is = false;
     SQLHANDLE sqlStmtHandle;
     sqlStmtHandle = NULL;
     if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
     {
         SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
         return false;
     }
     SQLWCHAR* ret1;
     ret1 = StringToSQLWChar(("UPDATE ANSWERS SET CORRECT = 1 WHERE ID = " + to_string(id) + " AND ID_TEST = " + to_string(idT) + " AND ID_QUESTION=" + to_string(idQ)).c_str());
     int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
     if (SQL_SUCCESS != i) {
         SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
         return false;
     }
     else {
         is = true;
     }
     SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
     return is;
 }
 bool DataBase::queryDeleteCategory(int idCat)
 {
     bool is = false;
     SQLHANDLE sqlStmtHandle;
     sqlStmtHandle = NULL;
     if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
     {
         SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
         return false;
     }
     SQLWCHAR* ret1;
     ret1 = StringToSQLWChar(("DELETE FROM CATEGORIES WHERE ID=" + to_string(idCat)).c_str());
     int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
     if (SQL_SUCCESS != i) {
         SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
         return false;
     }
     else {
         is = true;
     }
     SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
     return is;
 }
 bool DataBase::queryDeleteUser(int idUser)
 {
     bool is = false;
     SQLHANDLE sqlStmtHandle;
     sqlStmtHandle = NULL;
     if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
     {

         SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
         return false;
     }
     SQLWCHAR* ret1;
     ret1 = StringToSQLWChar(("DELETE FROM USERS_QUESTIONS WHERE ID_USER=" + to_string(idUser) + "; DELETE FROM USERS_TESTS WHERE ID_USER=" + to_string(idUser) + "; DELETE FROM USERS WHERE ID=" + to_string(idUser)).c_str());
     int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
     if (SQL_SUCCESS != i && i != SQL_SUCCESS_WITH_INFO && i != SQL_NO_DATA) {
         SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
         return false;
     }
     else {
         is = true;
     }
     SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
     return is;
 }
 bool DataBase::queryDeleteTest(int id)
 {
     bool is = false;
     SQLHANDLE sqlStmtHandle;
     sqlStmtHandle = NULL;
     if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
     {

         SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
         return false;
     }
     SQLWCHAR* ret1;
     ret1 = StringToSQLWChar(("DELETE FROM ANSWERS WHERE ID_TEST=" + to_string(id) + "; DELETE FROM QUESTION WHERE ID_TEST=" + to_string(id) + "; DELETE FROM TESTS WHERE ID=" + to_string(id) + "").c_str());
     int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
     if (SQL_SUCCESS != i) {
         SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
         return false;
     }
     else {
         is = true;
     }
     SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
     return is;
 }
bool DataBase::queryDeleteAnswers(int IdQ, int idT)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("DELETE FROM ANSWERS WHERE ID_QUESTION="+to_string(IdQ)+" AND ID_TEST=" +to_string(idT)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::queryDeleteAnswer(int id, int IdQ, int idT)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("DELETE FROM ANSWERS WHERE ID_QUESTION=" + to_string(IdQ) + " AND ID_TEST=" + to_string(idT) + " AND ID=" + to_string(id)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
float DataBase::querySelectMarkOfQ(int id)
{
    float mark = 0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return mark;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("SELECT MARK FROM QUESTION WHERE ID =" + to_string(id)).c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return mark;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_C_FLOAT, &mark, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return mark;
}

float DataBase::querySelectMarkOfTest(int id)
{
    float mark = 0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {

        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return mark;
    }
    SQLWCHAR* ret1;

    ret1 = StringToSQLWChar(("SELECT MARK FROM TESTS WHERE ID =" + to_string(id)).c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return mark;
    }
    else {


        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_C_FLOAT, &mark, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return mark;
}
int DataBase::querySelectCountOfQ(int id)
{
    int count = 0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return count;
    }
    SQLWCHAR* ret1;

    ret1 = StringToSQLWChar(("SELECT COUNT_ANSWERS FROM QUESTION WHERE ID =" + to_string(id)).c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return count;
    }
    else {


        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &count, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return count;
}

int DataBase::querySelectCountOfTest(int id)
{
    int count = 0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return count;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("SELECT COUNT_QUESTION FROM TESTS WHERE ID =" + to_string(id)).c_str());
    //ret1 = StringToSQLWChar(("SELECT COUNT(*) AS COUNT_QUESTION  FROM QUESTION WHERE ID_TEST =" + to_string(id)).c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return count;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &count, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return count;
}
string DataBase::querySelectNameUser(string l, string p)
{
    string temp = "";
    SQLCHAR  cval[240];
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return temp;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("SELECT SNF FROM USERS WHERE LOGIN ='" + l + "' AND PASSWORD='" + p + "'").c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return temp;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_C_CHAR, (SQLPOINTER)cval, SQL_RESULT_LEN, &ptrSqlVersion);
            string name((const char*)cval);
            temp = name;
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return temp;
}
string DataBase::querySelectNameOfQ( int id)
{
    string temp = "";
    SQLCHAR  cval[240];
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return temp;
    }
    SQLWCHAR* ret1;

    ret1 = StringToSQLWChar(("SELECT NAME FROM QUESTION WHERE ID =" + to_string(id)).c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return temp;
    }
    else {


        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_C_CHAR, (SQLPOINTER)cval, SQL_RESULT_LEN, &ptrSqlVersion);
            string name((const char*)cval);
            temp = name;
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return temp;
}
bool DataBase::querySelectCountPass(int idT, int idUser)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return is;
    }
    SQLWCHAR* ret1 = {};
    ret1 = StringToSQLWChar(("SELECT COUNT_PASS FROM USERS_TESTS WHERE ID_TEST="+to_string(idT)+" AND ID_USER="+to_string(idUser) + " AND COUNT_PASS=0").c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        i = SQLFetch(sqlStmtHandle);
        if (i != SQL_NO_DATA) {

            is = true;
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
 bool DataBase::querySelectQ(string question, int idT)
{
     bool is = false;
     SQLHANDLE sqlStmtHandle;
     sqlStmtHandle = NULL;
     if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
     {
         SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
         return is;
     }
     SQLWCHAR* ret1 = {};
     ret1 = StringToSQLWChar(("SELECT NAME FROM QUESTION WHERE NAME ='" + question + "' AND ID_TEST="+to_string(idT)).c_str());
     int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
     if (SQL_SUCCESS != i) {
         SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
         return false;
     }
     else {
         i = SQLFetch(sqlStmtHandle);
         if (i != SQL_NO_DATA) {

             is = true;
         }
     }
     SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
     return is;
}
bool DataBase::queryDeleteQuestion(int IdQ, int idT)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("DELETE FROM QUESTION WHERE ID=" + to_string(IdQ) + " AND ID_TEST=" + to_string(idT)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
string to_string();
bool DataBase::queryUpdateInfoUser(int idUser, string snf, string address, string number)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("UPDATE USERS SET SNF = '" + snf + "', ADDRESS= '" + address + "', NUMBER= '" + number + "' WHERE ID = " + to_string(idUser)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::queryUpdateUserQuestion(int idQ, int mark, int cor, int idA)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("UPDATE USERS_QUESTIONS SET CORRECT = "+ to_string(cor) +", MARK= "+ to_string(mark) + ", ID_CHOOSE_ANSWER= " + to_string(idA) + " WHERE ID_QUESTION = " + to_string(idQ)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}

bool DataBase::queryInsertQuestion(string q, int idT)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("INSERT INTO QUESTION(NAME, ID_TEST) VALUES('" + q + "', " + to_string(idT) + ")").c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::queryInsertUserQuestion(string name, int idQ, int idT, int idUser)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("INSERT INTO USERS_QUESTIONS(NAME, ID_TEST, ID_QUESTION, ID_USER) VALUES('" + name + "', " + to_string(idT) + ", " + to_string(idQ) + ", " + to_string(idUser) + ")").c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::queryInsertAnswer(string answer, int idT, int idQ)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("INSERT INTO ANSWERS(NAME, ID_TEST, ID_QUESTION) VALUES('" + answer + "', " + to_string(idT) + ", " + to_string(idQ) + ")").c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::queryInsertUser(string loginstr, string password,  string pib, string address, string phone, int stat)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("INSERT INTO USERS(STATUS,PASSWORD,LOGIN, SNF, ADDRESS, NUMBER) VALUES("+(to_string(stat))+", '"+password + "', '" + loginstr + "', '" + pib + "', '" + address + "', '" + phone + "')").c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {

        SQLCHAR login[100];
        SQLINTEGER ptrSqlVersion;
        i = SQLFetch(sqlStmtHandle);
        if (i != SQL_NO_DATA) {

            is = true;
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::queryInsertUsersTest(int idU, int idT, string name, string catName)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("INSERT INTO USERS_TESTS(ID_USER, ID_TEST, NAME, NAME_CAT) VALUES(" + (to_string(idU)) + ", " + (to_string(idT)) + ", '"+name+ "', '"+catName+"')").c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {

        SQLCHAR login[100];
        SQLINTEGER ptrSqlVersion;
        i = SQLFetch(sqlStmtHandle);
        if (i != SQL_NO_DATA) {

            is = true;
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
//int DataBase::querySelectIdTest(string nTest, int idCat)
//{
//    int Id = 0;
//    SQLHANDLE sqlStmtHandle;
//    sqlStmtHandle = NULL;
//    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
//        return Id;
//    SQLWCHAR* ret1;
//    
//    ret1 = StringToSQLWChar(("SELECT ID FROM TESTS WHERE NAME ='" + nTest + "' AND ID_CATEGORY='" + to_string(idCat) + "'").c_str());
//    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
//        return Id;
//    }
//    else {
//
//        
//        SQLINTEGER ptrSqlVersion;
//        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
//            
//            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &Id, 4, &ptrSqlVersion);
//        }
//    }
//    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
//    return Id;
//    
//
//}
int DataBase::querySelectCorAns(int idA)
{
    int cor = -1;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return cor;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("SELECT CORRECT FROM ANSWERS WHERE ID =" + to_string(idA)).c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return cor;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &cor, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return cor;
}
int DataBase::querySelectFinished(int id)
{
    int fin = -1;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return fin;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("SELECT FINISHED FROM TESTS WHERE ID =" +  to_string(id)).c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return fin;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &fin, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return fin;
}
//int DataBase::querySelectIdQuestion(string name, int idT)
//{
//    int Id = 0;
//    SQLHANDLE sqlStmtHandle;
//    sqlStmtHandle = NULL;
//    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
//        return Id;
//    SQLWCHAR* ret1;
//
//    ret1 = StringToSQLWChar(("SELECT ID FROM QUESTION WHERE NAME ='" + name + "' AND ID_TEST='" + to_string(idT) + "'").c_str());
//    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
//        return Id;
//    }
//    else {
//
//
//        SQLINTEGER ptrSqlVersion;
//        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
//
//            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &Id, 4, &ptrSqlVersion);
//        }
//    }
//    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
//    return Id;
//}

int DataBase::querySelectMarkQuestion(int id)
{
    int sum = 0;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return sum;
    }
    SQLWCHAR* ret1;

    ret1 = StringToSQLWChar(("SELECT SUM(MARK) FROM QUESTION WHERE ID_TEST=" + to_string(id)).c_str());
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return sum;
    }
    else {


        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &sum, 4, &ptrSqlVersion);
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return sum;
}
bool DataBase::queryInsertTest(string name, int idCat)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("INSERT INTO TESTS(NAME, ID_CATEGORY) VALUES('" + name + "', " + to_string(idCat)+ ")").c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::queryInsertCategory(string name)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(("INSERT INTO CATEGORIES(NAME) VALUES('" + name +  "')").c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) 
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else 
    {
        is = true;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::queryCatTest(string info, int stat, int idCat)
{
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return is;
    }
    SQLWCHAR* ret1 = {};
    if (stat == 1)
        ret1 = StringToSQLWChar(("SELECT NAME FROM CATEGORIES WHERE NAME ='" + info + "'").c_str());
    else if(stat == 2)
        ret1 = StringToSQLWChar(("SELECT NAME FROM TESTS WHERE NAME ='" + info + "' AND ID_CATEGORY="+to_string(idCat)).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {

        SQLCHAR login[100];
        SQLINTEGER ptrSqlVersion;
        i = SQLFetch(sqlStmtHandle);
        if (i != SQL_NO_DATA) {

            is = true;
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
bool DataBase::queryLoginPass(string loginstr, string password, int type, int stat)
{
    //+ "AND STATUS='" + (to_string(type)) + "'"
    bool is = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return is;
    }
    SQLWCHAR* ret1 = {};
    if (type == 1)
        ret1 = StringToSQLWChar(("SELECT LOGIN FROM USERS WHERE LOGIN ='" + loginstr + "'" ).c_str());
    else if (type == 2)
        ret1 = StringToSQLWChar(("SELECT PASSWORD FROM USERS WHERE PASSWORD ='" + password + "'" ).c_str());
    else if (type == 3)
        ret1 = StringToSQLWChar(("SELECT LOGIN, PASSWORD FROM USERS WHERE PASSWORD ='" + password + "' AND LOGIN='"+loginstr+"'" + "AND STATUS=" + (to_string(stat))).c_str());
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS !=i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {

        SQLCHAR login[100];
        SQLINTEGER ptrSqlVersion;
        i = SQLFetch(sqlStmtHandle);
        if (i != SQL_NO_DATA) {
 
            is = true;
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is;
}
DataBase::~DataBase()
{
    
    if (connected)
    {
        //SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLDisconnect(sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
    }
}
bool DataBase::queryAdmin()
{
    bool is_Admin = false;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }

    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT STATUS FROM USERS WHERE STATUS = 1", SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return false;
    }
    else {
  
        SQLCHAR status[5];
        SQLINTEGER ptrSqlVersion;
        if (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
            is_Admin = true;
           
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return is_Admin;
}
 map<int, map<string, string>> DataBase::querySelectUsersFinished(int id)
{
    int idT = 0;
    SQLCHAR  cval[240];
    SQLCHAR  cval2[240];
    map<int, map<string, string>>d ;
    map<string, string>m;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return d;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar("SELECT ID_TEST, NAME, NAME_CAT FROM USERS_TESTS WHERE ID_USER =" + to_string(id));
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return d;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &idT, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)cval, SQL_RESULT_LEN, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 3, SQL_C_CHAR, (SQLPOINTER)cval2, SQL_RESULT_LEN, &ptrSqlVersion);

            string  nStr((const char*)cval);
            string  nStr2((const char*)cval2);

            m.insert({ nStr, nStr2 });
            d.insert({ idT, m });
            m.clear();
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return d;
}
map<int, string> DataBase::querySelectUsersQuestions(int idT, int idUser)
{
    int b = 0;
    SQLCHAR  cval[240];
    map<int, string>m;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar("SELECT ID_QUESTION, NAME FROM USERS_QUESTIONS WHERE ID_TEST =" + to_string(idT)+ " AND ID_USER=" + to_string(idUser));
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &b, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)cval, SQL_RESULT_LEN, &ptrSqlVersion);
            string  nStr((const char*)cval);
            m.insert({ b, nStr });
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return m;
}
map<int, string> DataBase::querySelectQuestions(int idT)
{
    int b = 0;
    SQLCHAR  cval[240];
    map<int, string>m;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar("SELECT ID, NAME FROM QUESTION WHERE ID_TEST ="+to_string(idT));
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    else {

        SQLINTEGER ptrSqlVersion;

        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &b, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)cval, SQL_RESULT_LEN, &ptrSqlVersion);
            string  nStr((const char*)cval);
            m.insert({ b, nStr });


        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return m;
}
map<float, int> DataBase::querySelectAverCount(int idT, int idUser)
{
    float b = 0.0;
    int v = 0;
    SQLCHAR  cval[10];

    map<float, int>m;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar("SELECT AVERAGE_MARK, COUNT_PASS FROM USERS_TESTS WHERE ID_TEST="+to_string(idT)+" AND ID_USER=" + to_string(idUser));
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            SQLGetData(sqlStmtHandle, 1, SQL_C_CHAR, (SQLPOINTER)cval, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_INTEGER, &v, 4, &ptrSqlVersion);
            string  nStr((const char*)cval);
            b =stof(nStr);
            m.insert({ b, v });
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return m;
}
map<int, string> DataBase::queryAllCategoriesForUser(int id)
{
    int b = 0;
    SQLCHAR  cval[240];
    map<int, string>m;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(" SELECT C.ID, C.NAME FROM [Exam].[dbo].CATEGORIES AS C INNER JOIN [Exam].[dbo].TESTS AS T ON [T].[ID_CATEGORY] = C.ID  LEFT JOIN USERS_TESTS AS UT ON T.ID = UT.ID_TEST AND [UT].[ID_USER] = "+to_string(id)+" WHERE T.FINISHED=1 AND ( UT.FINISHED = 1 OR[UT].[ID_USER] IS NULL)  GROUP BY C.ID, C.NAME");
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &b, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)cval, SQL_RESULT_LEN, &ptrSqlVersion);

            string  nStr((const char*)cval);
            m.insert({ b, nStr });
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return m;
}
map<int, string> DataBase::queryAllCategoriesForUserUn(int id)
{
    int b = 0;
    SQLCHAR  cval[240];
    map<int, string>m;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar(" SELECT C.ID, C.NAME FROM [Exam].[dbo].CATEGORIES AS C INNER JOIN [Exam].[dbo].TESTS AS T ON [T].[ID_CATEGORY] = C.ID  LEFT JOIN USERS_TESTS AS UT ON T.ID = UT.ID_TEST AND [UT].[ID_USER] = " + to_string(id) + " WHERE T.FINISHED=1 AND  UT.FINISHED = 0   GROUP BY C.ID, C.NAME");
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &b, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)cval, SQL_RESULT_LEN, &ptrSqlVersion);

            string  nStr((const char*)cval);
            m.insert({ b, nStr });
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return m;
}
map<int, string> DataBase::queryAllCategories()
{
    int b = 0;  
    SQLCHAR  cval[240];
    map<int, string>m;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar("SELECT ID, NAME FROM CATEGORIES");
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    else {       
        SQLINTEGER ptrSqlVersion;
        while(SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &b, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)cval, SQL_RESULT_LEN, &ptrSqlVersion);
            
            string  nStr((const char*)cval);
            m.insert({ b, nStr});        
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return m;
}
map<map<int, string>, int>  DataBase::queryAllFinishedTests(int idCat, int id)
{
    int b = 0;
    int n= 0;
    SQLCHAR  cval[240];
    map<map<int, string>, int> m;
    map<int, string> c;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar("SELECT T.ID, T.NAME, ISNULL(UT.ID_TEST, -1) AS ID_TEST FROM CATEGORIES AS C INNER JOIN TESTS AS T ON T.ID_CATEGORY = C.ID LEFT JOIN USERS_TESTS AS UT ON T.ID = UT.ID_TEST AND [UT].[ID_USER] = "+to_string(id)+" WHERE T.FINISHED = 1 AND T.ID_CATEGORY = "+to_string(idCat)+" AND( UT.FINISHED = 1 OR [UT].[ID_USER] IS NULL)");
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &b, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)cval, SQL_RESULT_LEN, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 3, SQL_INTEGER, &n, 4, &ptrSqlVersion);
            string  nStr((const char*)cval);
            c.insert({ b, nStr });
            m.insert({ c, n });
        
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return m;
}
map<int, string>  DataBase::queryAllFinishedTestsUn(int idCat, int id)
{
    int b = 0;
    
    SQLCHAR  cval[240];
    map<int, string>m;
    
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar("SELECT T.ID, T.NAME AS ID_TEST FROM CATEGORIES AS C INNER JOIN TESTS AS T ON T.ID_CATEGORY = C.ID LEFT JOIN USERS_TESTS AS UT ON T.ID = UT.ID_TEST AND [UT].[ID_USER] = "+to_string(id)+"WHERE T.FINISHED = 1 AND T.ID_CATEGORY = " + to_string(idCat) + " AND UT.FINISHED = 0");
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &b, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)cval, SQL_RESULT_LEN, &ptrSqlVersion);
           
            string  nStr((const char*)cval);
           
            m.insert({ b, nStr });

        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return m;
}
map<int, string> DataBase::queryAllTests(int idCat)
{
    int b = 0;
    SQLCHAR  cval[240];
    map<int, string>m;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar("SELECT ID, NAME FROM TESTS WHERE ID_CATEGORY="+to_string(idCat));
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &b, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)cval, SQL_RESULT_LEN, &ptrSqlVersion);
            string  nStr((const char*)cval);
            m.insert({ b, nStr });
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return m;
}
map<int, string> DataBase::queryAllAnswers(int idQ)
{
    int b = 0;
    SQLCHAR  cval[240];
    map<int, string>m;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar("SELECT ID, NAME FROM ANSWERS WHERE ID_QUESTION=" + to_string(idQ));
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &b, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)cval, SQL_RESULT_LEN, &ptrSqlVersion);
            string  nStr((const char*)cval);
            m.insert({ b, nStr });
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return m;
}
map<int, int> DataBase::queryAllAnswersCor(int idQ)
{
    int b = 0;
    int v = 0;
    
    map<int, int>m;
    SQLHANDLE sqlStmtHandle;
    sqlStmtHandle = NULL;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    SQLWCHAR* ret1;
    ret1 = StringToSQLWChar("SELECT ID, CORRECT FROM ANSWERS WHERE ID_QUESTION=" + to_string(idQ));
    int i = SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)ret1, SQL_NTS);
    if (SQL_SUCCESS != i) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        return m;
    }
    else {
        SQLINTEGER ptrSqlVersion;
        while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
            SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &b, 4, &ptrSqlVersion);
            SQLGetData(sqlStmtHandle, 2, SQL_INTEGER, &v, 4, &ptrSqlVersion);
            m.insert({ b, v });
        }
    }
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return m;
}
SQLWCHAR* DataBase::StringToSQLWChar(string query)
{
    const int max_size = 2000;
    SQLWCHAR cval[max_size];
    for (int i = 0; i < query.size(); i++)
    {

        cval[i] = query[i];
        cval[i + 1] = 0;
    }
    return cval;
}
SQLWCHAR* DataBase::StringToSQLWCharConnect(string query)
{
    const int max_size = 1024;
    SQLWCHAR cval[max_size];
    for (int i = 0; i < query.size(); i++)
    {

        cval[i] = query[i];
        cval[i + 1] = 0;
    }
    return cval;
}
bool DataBase::connect()
{

    //define handles and variables
    
    sqlConnHandle = NULL;
    
    //allocations
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
        return false;
    if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        return false;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
        return false;
    SQLWCHAR* ret1;
    ret1 = StringToSQLWCharConnect(("DRIVER={"+driver+"};SERVER="+server+","+port+";DATABASE=Exam;UID="+username+";PWD="+password+";Trusted=true;").c_str());
    if (SQLDriverConnect(sqlConnHandle,
        NULL,
        //(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=SOFANOTE,2172;DATABASE=Exam;UID=sa;PWD=password;Trusted=true;",
        ret1,
        SQL_NTS,
        retconstring,
        1024,
        NULL,
        SQL_DRIVER_NOPROMPT) == SQL_SUCCESS)
    {
        connected = true;
        return true;

    }
    return false;
 
}
DataBase* DataBase::instance = nullptr;
DataBase* DataBase::getInstance()
{
	if (instance == nullptr)
		instance = new DataBase();
	return instance;
}