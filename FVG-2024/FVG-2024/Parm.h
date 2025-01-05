#pragma once
#include "tchar.h"
#define PARM_IN L"-in:"   //���� ��� ����� ��������� �����
#define PARM_OUT L"-out:" //���� ��� ����� ���������� �����
#define PARM_LOG L"-log:" //���� ��� ����� �������

#define PARM_MAX_SIZE 300 //������������ ����� ������ ����������
#define PARM_OUT_DEFAULT_EXT L".asm" //���������� ����� ���������� ���� �� ��������
#define PARM_LOG_DEFAULT_EXT L".txt" //���������� ����� ��������� �� ���������

namespace Parm // ��������� ������� ����������
{
	struct PARM //������� ���������
	{
		wchar_t in[PARM_MAX_SIZE];// -in:  ��� ����� ��������� ����
		wchar_t out[PARM_MAX_SIZE];// -out: ��� ����� ���������� ����
		wchar_t log[PARM_MAX_SIZE];// -log: ��� ����� ���������
		bool showMfst;
		bool showTables;
		bool showITables;
	};

	PARM getparm(int argc, _TCHAR* argv[]);
}