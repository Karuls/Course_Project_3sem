#pragma once
#include "Semantic.h"
namespace Semantic {
	void doAnalyse(LEX::LEX t) {
		operands(t);
		parameters(t);
		returns(t);
		/*

		parameters(t);

		equals(t);*/

	};

	void operands(LEX::LEX t) { // проверка операций
		for (int i = 0; i < t.lextable.size; i++) {
			if (t.lextable.table[i].lexema == LEX_OPERATOR)
			{
				switch (t.lextable.table[i].data)
				{
				case '=':
				{
					int pos = -1;
					int datatype = -1;
					while (t.lextable.table[i + pos].lexema != LEX_SEMICOLON) { // пока не дойдем до ";"
						if (t.lextable.table[i + pos].lexema == LEX_LITERAL || t.lextable.table[i + pos].lexema == LEX_ID) // Lit | ID
						{
							if (datatype == -1)
								datatype = t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype;//инциализ как тек тип
							else
								if (datatype != t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype) {// cовпадает ли он с типом текущего элемента?
									if (datatype != 4 && t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype != IT::INT) {
										throw ERROR_THROW_IN(704, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
											/*t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype = IT::SHO;
											if (t.idtable.table[t.lextable.table[i + pos].idxTI].value.vint < 0) {
												short w = t.idtable.table[t.lextable.table[i + pos].idxTI].value.vint;
												t.idtable.table[t.lextable.table[i + pos].idxTI].value.vshort = -w;
											}*/

									}

								}

							if (t.idtable.table[t.lextable.table[i + pos].idxTI].idtype == IT::F) { // skip alem before ")" (если func) 
								while (t.lextable.table[i + pos].lexema != LEX_RIGHTHESIS)
									pos++;
							}
						}
						if (datatype == IT::STR && t.lextable.table[i + pos].lexema == LEX_OPERATOR && pos != 0 && t.lextable.table[i + pos].data != '+') { // только "+" для str
							throw ERROR_THROW_IN(708, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
						}
						if (datatype == IT::CHR && t.lextable.table[i + pos].lexema == LEX_OPERATOR && pos != 0 && t.lextable.table[i + pos].data != '+' && t.lextable.table[i + pos].data != '-') { // только "+\-" для char
							throw ERROR_THROW_IN(710, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
						}

						//TODO SHO
						pos++;// след элем
					}
					i += pos - 1; // позиция = N пройденных шагов
					break;
				}
				}
			}
			if (t.lextable.table[i].lexema == LEX_UNTIL) {
				int pos = 2;
				while (true) {

					if (t.lextable.table[i + pos].lexema == LEX_LITERAL || t.lextable.table[i + pos].lexema == LEX_ID) // Lit | ID
					{
						if (t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype == IT::STR) // (Lit | ID) = str? Error: ok 
							throw ERROR_THROW_IN(709, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
					}

					pos++;
					if (t.lextable.table[i + pos].lexema == LEX_RIGHTHESIS || pos > 6)
					{
						break;
					}

				}
			}
		}
	}

	void parameters(LEX::LEX t) { //  проверка параметров функций.
		for (int i = 0; i < t.lextable.size; i++) {
			if ((t.lextable.table[i].lexema == LEX_WRITE || t.lextable.table[i].lexema == LEX_WRITELINE) && t.idtable.table[t.lextable.table[i + 1].idxTI].idtype == IT::F) // ввод\вывод -> func
				throw ERROR_THROW_IN(701, t.lextable.table[i + 1].sn, t.lextable.table[i + 1].cn)
				if (t.lextable.table[i].lexema == LEX_ID && t.idtable.table[t.lextable.table[i].idxTI].idtype == IT::F && t.lextable.table[i - 3].lexema != LEX_EXPORT && t.lextable.table[i - 3].lexema != LEX_DECLARE)//id(func) (!=export&&!=declare)
				{
					short* types = new short[256]; // типы параметров функции
					int typesLen = 0; // сами эти типы
					int funcDeclarePos = t.idtable.table[t.lextable.table[i].idxTI].idxfirstLE; // poz первого упоминания func в lextable, чтобы найти её declare
					while (t.lextable.table[++funcDeclarePos].lexema != LEX_RIGHTHESIS) // получение типов парм func из declare
					{
						if (t.lextable.table[funcDeclarePos].lexema == LEX_ID || t.lextable.table[funcDeclarePos].lexema == LEX_LITERAL) // (ID|lex)? -> mass types
							types[typesLen++] = t.idtable.table[t.lextable.table[funcDeclarePos].idxTI].iddatatype;
					}
					int pos = 1; //положение относительно функции
					int paramCount = 0; // количество фактических параметров
					while (t.lextable.table[i + pos].lexema != LEX_RIGHTHESIS) { // пока недошли до ")"
						if (t.lextable.table[i + pos].lexema == LEX_ID || t.lextable.table[i + pos].lexema == LEX_LITERAL) // (ID|lex)?
							if (t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype != types[paramCount++])// (тип текущего параметра вызова)? != типом соответ. парам из declare func
								throw ERROR_THROW_IN(703, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn) // 
								pos++;
					}
					if (paramCount != typesLen)// количеством параметров в декларации - (typesLen)
						throw ERROR_THROW_IN(702, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
						i += pos; // пропуск обработанных лекскем
				}
		}
	}

	void returns(LEX::LEX t) { // проверка возвращаемых значений функций
		for (int i = 0; i < t.lextable.size; i++) {
			if (t.lextable.table[i].lexema == LEX_ID && t.idtable.table[t.lextable.table[i].idxTI].idtype == IT::F && t.lextable.table[i - 3].lexema == LEX_DECLARE) // (ID\lex) принадлежит Declare/lex
			{
				int pos = 1; // для тек поз
				short retType = t.idtable.table[t.lextable.table[i].idxTI].iddatatype; // тип возврата
				while (t.lextable.table[i + pos].lexema != LEX_RETURN) { // ищем return
					pos++;
				}
				pos++;
				if ((t.lextable.table[i + pos].lexema == LEX_ID || t.lextable.table[i + pos].lexema == LEX_LITERAL) && t.idtable.table[t.lextable.table[i + pos].idxTI].idtype != IT::F && (short)t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype != retType) // (ID/lex) & !=func & type_func!=return_type
					throw ERROR_THROW_IN(701, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
			}
		}
	}

}
