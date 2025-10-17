#include "SubjectInfo.h"
SubjectInfo* SubjectInfo::instance=nullptr;
SubjectInfo *SubjectInfo::getInstance()
{
    if(instance==nullptr)
    {
        instance=new SubjectInfo;
    }
    return instance;
}

void SubjectInfo::releaseInstance()
{
    if(instance!=nullptr)
    {
        delete instance;
        instance=nullptr;
    }
}

SubjectInfo::SubjectInfo()
{

}

SubjectInfo::SubjectInfo(const SubjectInfo *other)
{
    Q_UNUSED(other);
}

SubjectInfo::~SubjectInfo()
{

}
