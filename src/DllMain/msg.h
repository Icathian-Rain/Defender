#pragma once

#include <string>
#include <time.h>
#include "base64.h"
class Msg
{
public:
    Msg(const std::string& funcName);
    ~Msg();

    void setItem(const std::string& key, const std::string& value);
    std::string getMsg();
private:
    std::string m_msg;
    int flag;
};

std::string getCurrentTime()
{
    time_t nowtime;
    struct tm t;   
    nowtime = time(NULL);  
    localtime_s(&t, &nowtime); 
    char tmp[64];   
    strftime(tmp,sizeof(tmp),"%Y-%m-%d %H:%M:%S",&t);
    return tmp;
}

Msg::Msg(const std::string& funcName)
{
    m_msg = "{ \n \"funcName\": \"" + funcName + "\",\n";
    m_msg += "\"time\": \"" + getCurrentTime() + "\",\n";
    m_msg += "\"info\": {";
    flag = 0;
}

Msg::~Msg()
{
}

std::string my_replace(const std::string& str, const std::string& old_value, const std::string& new_value)
{
    std::string str1 = str;
    std::string::size_type pos = 0;
    while((pos = str1.find(old_value.c_str(), pos)) != std::string::npos)
    {
        str1.replace(pos, 1, new_value.c_str());
        pos += new_value.length();
    }
    return str1;
}

void Msg::setItem(const std::string& key, const std::string& value)
{
    if(flag == 1)
    {
        m_msg += ",";
    }
    else
    {
        flag = 1;
    }
    m_msg += "\n\"" + base64_encode(my_replace(key)) + "\": \"" + base64_encode(my_replace(value)) + "\"";
}

std::string Msg::getMsg()
{
    m_msg += "\n}\n";
    m_msg += " }\n";
    return m_msg;
}