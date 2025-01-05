#include <stack>
#include "GRB.h"
#include "LT.h"
#include "Error.h"
#include "LEX.h"
#include "Log.h"

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

template <typename T>
struct use_container : T
{
	using T::T;
	using T::c;
};

typedef use_container<std::stack<short>> MFSTSTACK; //MFSTSTSTACK;

//typedef std::stack<short> MFSTSTACK;	//СТЕК АВТОМАТА
namespace MFST
{
	struct MfstState		//состояние автомата
	{
		short lenta_position;	//позиция на ленте
		short nrule;			//номер текущего правила
		short nrulechain;		//Номер текущей цепочки
		MFSTSTACK st;			 //стек автомата
		MfstState();
		MfstState(
			short pposition,	//позиция на ленте
			MFSTSTACK pst,		//стек автомата
			short pnrulechain);	//номер текущей цепочки, текущего правила
		MfstState(
			short pposition,	//позиция на ленте
			MFSTSTACK pst,		//стек автомата
			short pnrule,		//номер текущего правила
			short pnrulechain	//номер текущей цепокаи, текущего правила
		);
	};
	struct Mfst					//магазинный автомат
	{
		enum RC_STEP {
			NS_OK,
			NS_NORULE,
			NS_NORULECHAIN,
			NS_ERROR,
			TS_OK,
			TS_NOK,
			LENTA_END,
			SURPRISE
		};
		struct MfstDiagnosis
		{
			short lenta_position;
			RC_STEP rc_step; //код завершания шага
			short nrule; // N правила
			short nrule_chain; // N цепочки символа
			MfstDiagnosis();
			MfstDiagnosis(  // диагностика
				short plenta_position,
				RC_STEP prc_step, //код завершания шага
				short pnrule,// номер правила
				short pnrule_chain // номер цепочки символа
			);
		} diagnosis[MFST_DIAGN_NUMBER];

		GRBALPHABET* lenta; // лента LEX
		short lenta_position;
		short nrule;
		short nrulechain;
		short lenta_size;
		GRB::Greibach greibach; // граматика 
		LEX::LEX lex; // результат работы лекс.анализ.
		MFSTSTACK st;        // стек автрмата
		bool shallWrite;
		use_container<std::stack<MfstState>> storestate; // хранение состояний 
		Mfst();
		Mfst(
			LEX::LEX plex, // рез lex
			GRB::Greibach pgreibach, // грамматика грейбах
			bool shouldWrite
		);
		char* getCSt(char* buf); // получить стек
		char* getCLenta(char* buf, short pos, short n = 25); // лента: n символов с pos
		char* getDiagnosis(short n, char* buf); // получить n-ую строку диагностики
		bool savestate(const Log::LOG& log);
		bool reststate(const Log::LOG& log); // восстановить состояние автомата
		bool push_chain( // цепочка --> стек
			GRB::Rule::Chain chain // сама цепочка правила
		);
		RC_STEP step(const Log::LOG& log); // выполнить шаг автомата
		bool start(const Log::LOG& log); // зап автомат
		bool savediagnosis(
			RC_STEP pprc_step
		);
		void printrules(const Log::LOG& lo);

		struct Deducation
		{
			short size;
			short* nrules;
			short* nrulechains;
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		} deducation;

		bool savededucation();
	};
}