#ifndef DBFACTORY_H
#define DBFACTORY_H
#include<QSqlDatabase>
#include"UserDao.h"
#include<map>
#include"ExamDao.h"
using namespace std;
class DBFactory
{
public:
    static DBFactory* getInstance();
    static void releaseInstance();
    inline ExamDao* getExamDao(){return examDao;}
private:
    DBFactory();
    DBFactory(const DBFactory& other);
    ~DBFactory();

    static DBFactory* instance;
    QSqlDatabase db;
    map<QString,QSqlDatabase> dbs;
    //UserDao* userDao;
    map<QString,ExamDao*> ExamDaos;
    ExamDao* examDao;
};

#endif // DBFACTORY_H
