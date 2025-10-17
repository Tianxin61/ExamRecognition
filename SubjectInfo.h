#ifndef SUBJECTINFO_H
#define SUBJECTINFO_H
#include<QString>
//单例模式保存科目信息
class SubjectInfo
{
public:
    static SubjectInfo* getInstance();
    static void releaseInstance();

    inline void setSubjectName(QString name){subjectName=name;}
    inline QString getSubjectName(){return subjectName;}
    inline void setSubjectId(int id){subjectId=id;}
    inline int getSubjectId(){return subjectId;}
    inline void setNum(int num){this->num=num;}
    inline int getNum(){return num;}
private:
    SubjectInfo();
    SubjectInfo(const SubjectInfo* other);
    ~SubjectInfo();
    static SubjectInfo* instance;
private:
    QString subjectName;//科目名称
    int subjectId;//科目id
    int num;//人数
};

#endif // SUBJECTINFO_H
