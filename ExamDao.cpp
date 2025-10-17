#include "ExamDao.h"
#include<QSqlQuery>
#include<QVariant>
#include<QSqlRecord>
ExamDao::ExamDao()
{
    //创建user表
    QSqlQuery query;
    query.exec("create table if not exists examhistoryf(\
                   subjectid integer ,\
                   subjectname char(20),\
                   stuid integer,\
                   entertime datetime,\
                   leavetime datetime,\
               )");

}

bool ExamDao::addHistory(ExamEntity &ee)
{
    QSqlQuery query;
    query.prepare("insert into examhistory values(?,?,?,datetime('now'),datetime('now'))");
    query.bindValue(0,ee.subjectId);
    query.bindValue(1,ee.subjectName);
    query.bindValue(2,ee.stuid);
    return query.exec();
}

bool ExamDao::selectAll(vector<ExamEntity> &v)
{
    QSqlQuery query;
    bool ok=query.exec("select * from examhistory");

    while(query.next())
    {
        ExamEntity ee;
        ee.subjectId=query.record().value("subjectid").toInt();
        ee.subjectName=query.record().value("subjectname").toString();
        ee.stuid=query.record().value("stuid").toInt();
        ee.enterTime=query.record().value("entertime").toDateTime();
        ee.leaveTime=query.record().value("leavetime").toDateTime();
        v.push_back(ee);
    }
    return ok;
}

bool ExamDao::updateHistory(ExamEntity &ee)
{
    QSqlQuery query;
    query.prepare("update examhistory set leavetime=date('now) where stuid=? and subject =?");
    query.bindValue(0,ee.subjectId);
    query.bindValue(1,ee.subjectName);
    query.bindValue(2,ee.stuid);
    return query.exec();
}
