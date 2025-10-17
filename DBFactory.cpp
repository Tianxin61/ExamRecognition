#include "DBFactory.h"
DBFactory* DBFactory::instance=nullptr;
DBFactory *DBFactory::getInstance()
{
    if(instance==nullptr)
    {
        instance=new DBFactory;
    }
    return instance;
}
void DBFactory::releaseInstance()
{
    if(instance!=nullptr)
    {
        delete instance;
        instance=nullptr;
    }
}


DBFactory::DBFactory()
{
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("sea.db");
    db.open();

//    userDao=new UserDao;
}

DBFactory::DBFactory(const DBFactory &other)
{
    Q_UNUSED(other);
}

DBFactory::~DBFactory()
{
    //db.close();
    //关闭所有数据库
    for(auto i:dbs)//使用auto遍历map，auto是pair类型
    {
        i.second.close();
    }
    //删除所有dao对象
    for(auto i:ExamDaos)
    {

    }
}
