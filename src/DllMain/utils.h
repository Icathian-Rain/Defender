#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <Windows.h>


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


std::string bytes2string(const BYTE * bytes, int len)
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
            str += byte + 'A' - 10;
        }
        byte = bytes[i] & 0xf;
        if (byte >= 0 && byte <= 9)
        {
            str += byte + '0';
        }
        else if (byte >= 10 && byte <= 15)
        {
            str += byte + 'A' - 10;
        }
    }
    return str;
}

std::string dword2hex(const DWORD src)
{
    char buf[9];
    sprintf_s(buf, "%08X", src);
    return std::string(buf);
}