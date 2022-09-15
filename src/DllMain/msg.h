#pragma once

#include <string>
#include <time.h>
#include "base64.h"

std::string GbkToUtf8(const char *src_str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	std::string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

std::string Utf8ToGbk(const char *src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}

std::string wchar2string(const wchar_t* wchar)
{
    std::string str;
    int len = WideCharToMultiByte(CP_UTF8, 0, wchar, -1, NULL, 0, NULL, NULL);
    char* chRtn = new char[len + 1];
    WideCharToMultiByte(CP_UTF8, 0, wchar, -1, chRtn, len, NULL, NULL);
    str = chRtn;
    delete[] chRtn;
    return str;
}


std::string bytes2string(const unsigned char* bytes, int len)
{
    std::string str = "";
    for (int i = 0; i < len; i++)
    {
        int byte = bytes[i] >> 4 & 0xf;
        if (byte >= 0 && byte <= 9)
        {
            str += byte + '0';
        }
        else if (byte >= 10 && byte <= 15)
        {
            str += byte + 'A';
        }
        byte = bytes[i] & 0xf;
        if (byte >= 0 && byte <= 9)
        {
            str += byte + '0';
        }
        else if (byte >= 10 && byte <= 15)
        {
            str += byte + 'A';
        }
    }
    return str;
}
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
    std::string temp_key = GbkToUtf8(key.c_str());
    std::string temp_value = value;
    if(flag == 1)
    {
        m_msg += ",";
    }
    else
    {
        flag = 1;
    }
    // temp_key = my_replace(temp_key, "\\", "\\\\");
    temp_key = base64_encode(temp_key);
    // temp_value = my_replace(temp_value, "\\", "\\\\");
    temp_value = base64_encode(temp_value);

    m_msg += "\n\"" + temp_key + "\": \"" + temp_value + "\"";
}

std::string Msg::getMsg()
{
    m_msg += "\n}\n";
    m_msg += " }\n";
    return m_msg;
}