#include "pch.h"
#include "framework.h"
#include <iostream>
#include <ctime>
#include <math.h>
#include<string>
#include <Windows.h>
#include <time.h>

extern "C"
{
	unsigned int __stdcall ConvertCharToInt(char ch)
	{
		return (unsigned int)ch;
	}
	unsigned char __stdcall ConvertIntToChar(unsigned int number)
	{
		return (unsigned char)number;
	}

	unsigned int __stdcall WriteMonthNOW(unsigned int number)
	{
		if (number == 1)
		{
			time_t rawtime;
			struct tm timeinfo;
			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);
			int month = timeinfo.tm_mon + 1;
			return (unsigned int)month;
		}
		else
		{
			return 1;
		}
	}

	unsigned int __stdcall WriteDateNOW(unsigned int number)
	{
		if (number == 1)
		{
			time_t rawtime;
			struct tm timeinfo;
			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);
			int month = timeinfo.tm_mon + 1;
			int day = timeinfo.tm_mday;
			int year = timeinfo.tm_year + 1900;
			return (unsigned int)day * 1000000 + (unsigned int)month * 10000 + (unsigned int)year;
		}
		else
		{
			return 1;
		}
	}

	unsigned int __stdcall WriteMinutesNOW(unsigned int number)
	{
		if (number == 1)
		{
			time_t rawtime;
			struct tm timeinfo;
			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);
			int minutes = timeinfo.tm_min;
			return (unsigned int)minutes;
		}
		else
		{
			return 1;
		}
	}

	unsigned int __stdcall WriteHoursNOW(unsigned int number)
	{
		if (number == 1)
		{
			time_t rawtime;
			struct tm timeinfo;
			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);
			int hour = timeinfo.tm_hour;
			return (unsigned int)hour;
		}
		else
		{
			return 1;
		}
	}

	void __stdcall writelineUint(unsigned int a)
	{
		std::cout << a << std::endl;
	}
	void __stdcall writelineShort(short a)
	{
		std::cout << a << std::endl;
	}
	void __stdcall writeUint(unsigned int a)
	{
		std::cout << a;
	}
	void __stdcall writeShort(short a)
	{
		std::cout << a;
	}
	void __stdcall writelineChar(char a)
	{
		std::cout << a << std::endl;
	}
	void __stdcall writeChar(char a)
	{
		std::cout << a;
	}
	void __stdcall writeStr(void* in)
	{
		char* a = (char*)in;
		setlocale(0, "");
		if (a != NULL)
			std::cout << a;
	}
	void __stdcall writelineStr(void* in)
	{
		char* a = (char*)in;
		setlocale(0, "");
		if (a != NULL)
			std::cout << a << std::endl;
	}
	int __stdcall _checkingValue(int value, int min, int max)
	{
		if (value > max || value < min)
		{
			std::cout << "Переполнение типа данных";
			return 1;
		}
		return 0;
	}

	const char* __stdcall compareStr(const char* str1, const char* str2)
	{

		if (str1 == nullptr || str2 == nullptr)
		{
			return "nullptr";
		}

		// Проходим по символам обеих строк
		while (*str1 != '\0' && *str2 != '\0')
		{
			if (*str1 < *str2)
			{
				return "str2 больше str1";
			}
			else if (*str1 > *str2)
			{
				return "str1 больше str2";
			}

			// Переходим к следующему символу
			str1++;
			str2++;
		}

		// Если строки равны
		if (*str1 == '\0' && *str2 == '\0')
		{
			return "равны";
		}

		// Если одна из строк короче
		return (*str1 == '\0') ? "больше" : "меньше";
	}

}

