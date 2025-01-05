#include "tchar.h"
#include <iostream>
#include <locale>
#include <cwchar>
#include "MFST.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "IT.h"
#include "FST.h"
#include "Semantic.h"
#include "NotaciaPolska.h"
#include "CodeGeneration.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");

	Log::LOG log = Log::INITLOG;
	Out::OUT out = Out::INITOUT;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		out = Out::getout(parm.out);
		Log::WriteParm(log, parm);
		Log::WriteLog(log);
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);

		LEX::LEX tables(LT::Create(in.lexems.size()), IT::Create(in.lexems.size()));

		Log::WriteLine(log, "--------!!!!Начало лексического анализа!!!!--------", "");

		FST::LexAnalyzer(in, out, log, tables.lextable, tables.idtable);

		if (parm.showTables)Log::LogTables(log, tables);
		if (parm.showITables)Log::LogIDTables(log, tables);
		Log::WriteLine(log, "--------!!!!Конец лексического анализа!!!!--------", "");


		Log::WriteLine(log, "--------!!!!Начало синтаксического анализа!!!!--------", "");
		if (parm.showMfst)MFST_TRACE_START(log);
		MFST::Mfst mfst(tables, GRB::getGreibach(), parm.showMfst);
		if (!mfst.start(log))
			throw ERROR_THROW(609);
		mfst.savededucation();
		mfst.printrules(log);
		Log::WriteLine(log, "--------!!!!Конец синтаксического анализа!!!!--------", "");

		Log::WriteLine(log, "--------!!!!Начало семантического анализа!!!!--------", "");
		Semantic::doAnalyse(tables);
		Log::WriteLine(log, "--------!!!!Конец семантического анализа!!!!--------", "");

		NotaciaPolska::Wykonac(tables);
		Log::LogTables(log, tables);
		Log::WriteLine(log, "--------!!!!Все анализаторы отработали без ошибок!!!!--------", "");

		CG::Generate(tables, out);
		Log::WriteLine(log, "--------!!!!Код сгенерирован без ошибок!!!!--------", "");

	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
	}
	Log::Close(log);
}