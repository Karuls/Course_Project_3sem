#pragma once

#define ID_MAXSIZE		    26 
#define ID_CURRENT_MAXSIZE  26 //F

#define	TI_MAXSIZE		4096 //макс кол-во строк в табл итендификаторов
#define TI_INT_DEFAULT	0x00000000	//значение uint по дефолту
#define	TI_STR_DEFAULT	0x00		//значение string по дефолту
#define	TI_NULLIDX		0xffffffff	//нет элемента таблицы итендификаторов
#define	TI_STR_MAXSIZE	255
#define ATOII_PARAMS_CNT 1

#define FALSYNUMBER 17//используй как нуль

#define ATOII_TYPE IT::IDDATATYPE::INT

namespace IT	//таблица итендификаторов
{
	enum IDDATATYPE {
		INT = 1,
		STR = 2,
		CHR = 3,
		SHO = 4
	};//таблица данных итендификаторов fls - empty
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4 };//типы итендификаторов - переменная функция параметр литерал

	struct Entry //строка таблицы итендификаторов
	{
		int idxfirstLE; //i первой строки в LEX
		char id[ID_MAXSIZE]; //идентификатор
		bool isExternal; //флаг подключения
		IDDATATYPE iddatatype; //тип данных
		IDTYPE idtype; //тип итендификатора
		union {
			unsigned int vint; //значение integer
			char vchar;//значение sting
			short vshort;//значение short
			struct
			{
				char len;//кол-во символов в string
				char* str;// [TI_STR_MAXSIZE] ;//символы стринг
			} vstr;//значение sting
		} value; //значение итендификатора
		Entry(int idxfirstLE, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, bool e = false) {
			this->isExternal = e;
			this->idxfirstLE = idxfirstLE;
			this->iddatatype = iddatatype;
			this->idtype = idtype;
			if (iddatatype == CHR)
			{
				this->value.vchar = '\0';
			}
			if (iddatatype == INT)
			{
				this->value.vint = 0;
			}
			if (iddatatype == SHO)
			{
				this->value.vshort = 0;
			}
			if (iddatatype == STR)
			{
				this->value.vstr.len = 0;
				this->value.vstr.str = nullptr;
			}



			int len = 0;
			for (int i = 0; id[i] != '\0' && i < ID_MAXSIZE; i++)
			{
				len++;
				this->id[i] = id[i];
			}
			if (len >= ID_MAXSIZE)
				this->id[ID_MAXSIZE - 1] = '\0';
			else
			{
				this->id[len] = '\0';
			}


		};
		Entry(int idxfirstLE, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, unsigned int data, bool e = false) {
			this->isExternal = e;
			this->idxfirstLE = idxfirstLE;
			this->iddatatype = iddatatype;
			this->idtype = idtype;
			this->value.vint = data;

			int len = 0;
			for (int i = 0; id[i] != '\0' && i < ID_MAXSIZE; i++)
			{
				len++;
				this->id[i] = id[i];
			}
			if (len >= ID_MAXSIZE)
				this->id[ID_MAXSIZE - 1] = '\0';
			else
			{
				this->id[len] = '\0';
			}

		};

		Entry(int idxfirstLE, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, short data, bool e = false) {
			this->isExternal = e;
			this->idxfirstLE = idxfirstLE;
			this->iddatatype = iddatatype;
			this->idtype = idtype;
			this->value.vshort = data;

			int len = 0;
			for (int i = 0; id[i] != '\0' && i < ID_MAXSIZE; i++)
			{
				len++;
				this->id[i] = id[i];
			}
			if (len >= ID_MAXSIZE)
				this->id[ID_MAXSIZE - 1] = '\0';
			else
			{
				this->id[len] = '\0';
			}


		};

		Entry(int idxfirstLE, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, char data, bool e = false) {
			this->isExternal = e;
			this->idxfirstLE = idxfirstLE;
			this->iddatatype = iddatatype;
			this->idtype = idtype;

			int len = 0;
			for (int i = 0; id[i] != '\0' && i < ID_MAXSIZE; i++)
			{
				len++;
				this->id[i] = id[i];
			}
			if (len >= ID_MAXSIZE)
				this->id[ID_MAXSIZE - 1] = '\0';
			else
			{
				this->id[len] = '\0';
			}


			this->value.vchar = data;
			//if (len > TI_STR_MAXSIZE)
			//{
			//	//error
			//	//std::cout << "TI STR MAXSIZE ERROR" << std::endl;
			//}
			//else
			//{
			//	//this->value.vstr.str = new char[len];
			//	for (int i = 1; i < len; i++)
			//	{
			//		this->value.vstr.str[i - 1] = data[i];
			//	}
			//	this->value.vstr.str[len - 1] = '\0';
			//}
		};
		Entry(int idxfirstLE, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, char* data, bool e = false) {
			this->isExternal = e;
			this->idxfirstLE = idxfirstLE;
			this->iddatatype = iddatatype;
			this->idtype = idtype;

			int len = 0;
			for (int i = 0; id[i] != '\0' && i < ID_MAXSIZE; i++)
			{
				len++;
				this->id[i] = id[i];
			}
			if (len >= ID_MAXSIZE)
				this->id[ID_MAXSIZE - 1] = '\0';
			else
			{
				this->id[len] = '\0';
			}


			len = 0;
			for (int i = 1; data[i] != '\"'; i++)
			{
				len++;
			}
			this->value.vstr.len = len + 3;
			this->value.vstr.str = new char[len + 3];

			for (int i = 0; i < len + 3; i++)
			{
				this->value.vstr.str[i] = data[i];
			}
			this->value.vstr.str[len + 3 - 1] = '\0';
			this->value.vstr.len = len + 2;

		};
		Entry() = default;
	};
	struct IdTable		//экземпляр таблицы ид
	{
		int maxsize;	//ёмкость таблицы
		int size;		//текущий размер
		Entry* table;	//массив строк таблицы итендификаторов
	};
	IdTable Create(	//создать таблицу ID
		int size	//ёмкость
	);
	void Add(
		IdTable& idtable,	//экземпляр таблицы итендификаторов
		Entry entry			//строка таблицы итендификаторов
	);
	Entry GetEntry(	//получить строку таблицы итендификаторов
		IdTable& idtable,	//экземпляр таблицы итендификаторов
		int n				//строка таблицы итендификаторов
	);
	int IsId(
		IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		char id[ID_MAXSIZE] //итендификатор
	);
	int IsLX(
		IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		IDDATATYPE type,
		unsigned int data
	);
	int IsLX(
		IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		IDDATATYPE type,
		short data
	);
	int IsLX(
		IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		IDDATATYPE type,
		int data
	);
	int IsLX(
		IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		IDDATATYPE type,
		char data
	);
	int IsLX(
		IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		IDDATATYPE type,
		char* data
	);

	bool isUniq(IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		char id[ID_MAXSIZE]);
	void Delete(IdTable& idtable);	//удалить таблицу итендификаторов
}