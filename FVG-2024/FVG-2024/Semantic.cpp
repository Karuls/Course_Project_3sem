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

	void operands(LEX::LEX t) { // �������� ��������
		for (int i = 0; i < t.lextable.size; i++) {
			if (t.lextable.table[i].lexema == LEX_OPERATOR)
			{
				switch (t.lextable.table[i].data)
				{
				case '=':
				{
					int pos = -1;
					int datatype = -1;
					while (t.lextable.table[i + pos].lexema != LEX_SEMICOLON) { // ���� �� ������ �� ";"
						if (t.lextable.table[i + pos].lexema == LEX_LITERAL || t.lextable.table[i + pos].lexema == LEX_ID) // Lit | ID
						{
							if (datatype == -1)
								datatype = t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype;//�������� ��� ��� ���
							else
								if (datatype != t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype) {// c�������� �� �� � ����� �������� ��������?
									if (datatype != 4 && t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype != IT::INT) {
										throw ERROR_THROW_IN(704, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
											/*t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype = IT::SHO;
											if (t.idtable.table[t.lextable.table[i + pos].idxTI].value.vint < 0) {
												short w = t.idtable.table[t.lextable.table[i + pos].idxTI].value.vint;
												t.idtable.table[t.lextable.table[i + pos].idxTI].value.vshort = -w;
											}*/

									}

								}

							if (t.idtable.table[t.lextable.table[i + pos].idxTI].idtype == IT::F) { // skip alem before ")" (���� func) 
								while (t.lextable.table[i + pos].lexema != LEX_RIGHTHESIS)
									pos++;
							}
						}
						if (datatype == IT::STR && t.lextable.table[i + pos].lexema == LEX_OPERATOR && pos != 0 && t.lextable.table[i + pos].data != '+') { // ������ "+" ��� str
							throw ERROR_THROW_IN(708, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
						}
						if (datatype == IT::CHR && t.lextable.table[i + pos].lexema == LEX_OPERATOR && pos != 0 && t.lextable.table[i + pos].data != '+' && t.lextable.table[i + pos].data != '-') { // ������ "+\-" ��� char
							throw ERROR_THROW_IN(710, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
						}

						//TODO SHO
						pos++;// ���� ����
					}
					i += pos - 1; // ������� = N ���������� �����
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

	void parameters(LEX::LEX t) { //  �������� ���������� �������.
		for (int i = 0; i < t.lextable.size; i++) {
			if ((t.lextable.table[i].lexema == LEX_WRITE || t.lextable.table[i].lexema == LEX_WRITELINE) && t.idtable.table[t.lextable.table[i + 1].idxTI].idtype == IT::F) // ����\����� -> func
				throw ERROR_THROW_IN(701, t.lextable.table[i + 1].sn, t.lextable.table[i + 1].cn)
				if (t.lextable.table[i].lexema == LEX_ID && t.idtable.table[t.lextable.table[i].idxTI].idtype == IT::F && t.lextable.table[i - 3].lexema != LEX_EXPORT && t.lextable.table[i - 3].lexema != LEX_DECLARE)//id(func) (!=export&&!=declare)
				{
					short* types = new short[256]; // ���� ���������� �������
					int typesLen = 0; // ���� ��� ����
					int funcDeclarePos = t.idtable.table[t.lextable.table[i].idxTI].idxfirstLE; // poz ������� ���������� func � lextable, ����� ����� � declare
					while (t.lextable.table[++funcDeclarePos].lexema != LEX_RIGHTHESIS) // ��������� ����� ���� func �� declare
					{
						if (t.lextable.table[funcDeclarePos].lexema == LEX_ID || t.lextable.table[funcDeclarePos].lexema == LEX_LITERAL) // (ID|lex)? -> mass types
							types[typesLen++] = t.idtable.table[t.lextable.table[funcDeclarePos].idxTI].iddatatype;
					}
					int pos = 1; //��������� ������������ �������
					int paramCount = 0; // ���������� ����������� ����������
					while (t.lextable.table[i + pos].lexema != LEX_RIGHTHESIS) { // ���� ������� �� ")"
						if (t.lextable.table[i + pos].lexema == LEX_ID || t.lextable.table[i + pos].lexema == LEX_LITERAL) // (ID|lex)?
							if (t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype != types[paramCount++])// (��� �������� ��������� ������)? != ����� �������. ����� �� declare func
								throw ERROR_THROW_IN(703, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn) // 
								pos++;
					}
					if (paramCount != typesLen)// ����������� ���������� � ���������� - (typesLen)
						throw ERROR_THROW_IN(702, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
						i += pos; // ������� ������������ �������
				}
		}
	}

	void returns(LEX::LEX t) { // �������� ������������ �������� �������
		for (int i = 0; i < t.lextable.size; i++) {
			if (t.lextable.table[i].lexema == LEX_ID && t.idtable.table[t.lextable.table[i].idxTI].idtype == IT::F && t.lextable.table[i - 3].lexema == LEX_DECLARE) // (ID\lex) ����������� Declare/lex
			{
				int pos = 1; // ��� ��� ���
				short retType = t.idtable.table[t.lextable.table[i].idxTI].iddatatype; // ��� ��������
				while (t.lextable.table[i + pos].lexema != LEX_RETURN) { // ���� return
					pos++;
				}
				pos++;
				if ((t.lextable.table[i + pos].lexema == LEX_ID || t.lextable.table[i + pos].lexema == LEX_LITERAL) && t.idtable.table[t.lextable.table[i + pos].idxTI].idtype != IT::F && (short)t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype != retType) // (ID/lex) & !=func & type_func!=return_type
					throw ERROR_THROW_IN(701, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
			}
		}
	}

}
