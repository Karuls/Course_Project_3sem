#include "Log.h"
#include "GRB.h"
#include "MFST.h"

int FST_TRACE_n = -1;
char rbuf[205], sbuf[205], lbuf[1024];

namespace MFST
{

	Mfst::MfstDiagnosis::MfstDiagnosis() : lenta_position(-1), rc_step(Mfst::RC_STEP::SURPRISE), nrule(-1), nrule_chain(-1) {}

	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain) : lenta_position(plenta_position), rc_step(prc_step), nrule(pnrule), nrule_chain(pnrule_chain) {}

	MfstState::MfstState()
	{
		lenta_position = 0;
		nrule = -1;
		nrulechain = -1;
	};
	MfstState::MfstState(short pposition, MFSTSTACK pst, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrulechain = pnrulechain;
	};
	MfstState::MfstState(short pposition, MFSTSTACK pst, short pnrule, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrule = pnrule;
		nrulechain = pnrulechain;
	};
	Mfst::Mfst() { lenta = 0; lenta_size = lenta_position = 0; };
	Mfst::Mfst(LEX::LEX plex, GRB::Greibach pgreibch, bool _shallWrite)
	{
		shallWrite = _shallWrite;
		greibach = pgreibch;
		lex = plex;
		lenta = new short[lenta_size = lex.lextable.size];
		for (int k = 0; k < lenta_size; k++)
			lenta[k] = TS(lex.lextable.table[k].lexema);
		lenta_position = 0;
		st.push(greibach.stbottomT);
		st.push(greibach.startN);
		nrulechain = -1;
	};
	Mfst::RC_STEP Mfst::step(const Log::LOG& log) // диагностических сообщений
	{
		RC_STEP rc = SURPRISE; // код возврата текущего шага
		if (lenta_position < lenta_size)
		{
			if (ISNS(st.top())) //N
			{
				GRB::Rule rule;
				if ((nrule = greibach.getRule(st.top(), rule)) >= 0) // извлечение пр из гр(N -> rule)
				{
					GRB::Rule::Chain chain;
					(nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)); // для тек символа ищем цепочку правила
					if (nrulechain >= 0)
					{
						if (shallWrite)MFST_TRACE1(log); // запист в log
						savestate(log); // сохр тек сост
						st.pop();// - N
						push_chain(chain); // обр chain
						rc = NS_OK;
						if (shallWrite)MFST_TRACE2(log);
					}
					else
					{
						if (shallWrite)MFST_TRACE4(log, "TNS_NORULECHAIN/NS_NORULE")
							savediagnosis(NS_NORULECHAIN); //записали диагностику
						rc = reststate(log) ? NS_NORULECHAIN : NS_NORULE; // можно ли восстановить сост
					};
				}
				else rc = NS_ERROR;
			}//
			else if ((st.top() == lenta[lenta_position])) // T
			{
				lenta_position++;
				st.pop(); // -T
				nrulechain = -1; // сброс тек str
				rc = TS_OK;
				if (shallWrite)MFST_TRACE3(log);
			}
			else { // символы не совпали
				if (shallWrite)MFST_TRACE4(log, "TS_NOK/NS_NORULECHAIN")
					rc = reststate(log) ? TS_NOK : NS_NORULECHAIN;
				// пытаюсь восстановаить пред сост
			};
		}
		else {// lenta is out
			rc = LENTA_END; if (shallWrite)MFST_TRACE4(log, "LENTA_END")
		}
		return rc;
	}


	bool Mfst::push_chain(GRB::Rule::Chain chain) {
		for (int k = chain.size - 1; k >= 0; k--) st.push(chain.nt[k]);
		return true;
	}

	bool Mfst::savestate(const Log::LOG& log) { // тек сост МКА
		storestate.push(MfstState(lenta_position, // текущую позицию
			st,// текущий стек
			nrule, // N тек правила грамматики
			nrulechain));// тек цеп
		if (shallWrite)MFST_TRACE6(log, "SAVESTATE:", storestate.size());
		return true;
	};

	bool Mfst::reststate(const Log::LOG& log) {
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			if (shallWrite)MFST_TRACE5(log, "RESSTATE")
				if (shallWrite)MFST_TRACE2(log)
		}
		return rc;
	};

	bool Mfst::savediagnosis(RC_STEP prc_step)
	{
		bool rc = false; // рез для хранения выполг функции
		short k = 0;
		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)k++;
		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);
			for (short j = k + 1; j < MFST_DIAGN_NUMBER; j++)diagnosis[j].lenta_position = -1;
		};
		return rc;
	};
	bool Mfst::start(const Log::LOG& log)
	{
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE];
		rc_step = step(log); //один такт

		// успешное выполнение && несмотря на отсутствие подходящей цепочки или ошибку в терминале
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
			rc_step = step(log);
		switch (rc_step)
		{
		case LENTA_END:         if (shallWrite)MFST_TRACE4(log, "------>LENTA_END")
			* log.stream << "-------------------------------------------------------------------------------------" << std::endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d:всего строк %d, синтаксический анализ выполнен без ошибок ", 0, lenta_size);
			*log.stream << std::setw(4) << std::left << 0 << ":всего строк " << lenta_size << ", синтаксический анализ выполнен без ошибок " << std::endl;
			rc = true;
			break;
		case NS_NORULE:  // нет подходящего правила
			if (shallWrite)MFST_TRACE4(log, "------>NS_NORULE")
				* log.stream << "-------------------------------------------------------------------------------------" << std::endl;
			*log.stream << getDiagnosis(0, buf) << std::endl;
			*log.stream << getDiagnosis(1, buf) << std::endl;
			*log.stream << getDiagnosis(2, buf) << std::endl;
			break;
		case NS_NORULECHAIN: // Ошибка цепочки
			if (shallWrite)MFST_TRACE4(log, "------>NS_NORULECHAIN")
				break;
		case NS_ERROR: // Общая ошибка
			if (shallWrite)MFST_TRACE4(log, "------>NS_ERROR")
				break;
		case SURPRISE: // Неожиданное поведение
			if (shallWrite)MFST_TRACE4(log, "------>SURPRISE")
				break;
		};
		return rc;

	};

	char* Mfst::getCSt(char* buf)
	{
		for (int k = (signed)st.size() - 1; k >= 0; --k) {
			short p = st.c[k];
			buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p);
		};
		buf[st.size()] = 0x00;
		return buf;
	};
	char* Mfst::getCLenta(char* buf, short pos, short n)
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
		buf[i - pos] = 0x00;
		return buf;
	};
	char* Mfst::getDiagnosis(short n,// N строки
		char* buf)// куда

	{
		char* rc = (char*)"";// рез пустая
		int errid = 0,// код огиьки
			lpos = -1;// поз на ленте
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0) { // поз > 0 n(str) < max_const
			errid = greibach.getRule(diagnosis[n].nrule).iderror; //N str with Error
			Error::ERROR err = Error::geterror(errid); // кокретная ошибка
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,%s", err.id, lex.lextable.table[lpos].sn, err.message); // заполняем буффер
			rc = buf; // обнул
		};
		return rc;
	};
	void Mfst::printrules(const Log::LOG& log)
	{
		MfstState state;
		GRB::Rule rule;
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate.c[k]; // Извлекается состояние
			rule = greibach.getRule(state.nrule); // Извлекается правило
			if (shallWrite)MFST_TRACE7(log)
		};
	};
	bool Mfst::savededucation()
	{
		MfstState state; //сост ис стека
		GRB::Rule rule;
		deducation.size = (short)storestate.size(); //к-во сохранённых состояний в стеке
		deducation.nrules = new short[deducation.size]; // n правил
		deducation.nrulechains = new short[deducation.size];// n цепочек
		for (unsigned short k = 0; k < storestate.size(); k++) // все состояния из стека storestate
		{
			state = storestate.c[k];
			deducation.nrules[k] = state.nrule;
			deducation.nrulechains[k] = state.nrulechain;
		};
		return true;
	};
}