#ifndef EXAMDAO_H
#define EXAMDAO_H
#include<QString>
#include<QDateTime>
#include<vector>
using namespace std;
struct ExamEntity
{
    int subjectId;//考场号
    QString subjectName;//科目名称
    int stuid;//学生考号
    QDateTime enterTime;//进场时间
    QDateTime leaveTime;//离场时间
};

class ExamDao
{
public:
    ExamDao();
    bool addHistory(ExamEntity& ee);//添加时间信息
    bool selectAll(vector<ExamEntity>& v);//查询所有考试场次的时间
    bool updateHistory(ExamEntity& ee);//更新离开时间
};

#endif // EXAMDAO_H
