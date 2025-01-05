//polish
//получить приотритет для польской нотации
#include <stack>
#include <vector>
#include <iostream>
#include "NotaciaPolska.h"
#include "Error.h"

namespace NotaciaPolska {
	template <typename T>
	struct container : T // c лекции
	{
		using T::T; // позволяет использовать конструкторы базового класса
		using T::c; // делает доступным внутренний контейнер
	};

	std::string toString(int n) {
		char buf[40];
		sprintf_s(buf, "%d", n);
		return buf;
	}

	bool find_elem(std::stack<char> stack, size_t size, char elem) { // Ищет элемент elem в копии стека
		for (size_t i = 0; i < size; i++)
			if (stack.top() == elem) // Если находит elem, возвращает true, иначе продолжает проверку
				return true;
			else
				stack.pop();
		return false; // если стек пуст или элемент не найден.
	}
	int get_priority(char a) // () - 0   , - 1     (-, +, *, %, /, и др.) имеют приоритет от 2 до 3.
	{
		switch (a)
		{
		case '(':
			return 0;
		case ')':
			return 0;
		case ',':
			return 1;
		case '-':
			return 2;
		case '+':
			return 2;
		case '*':
			return 3;
		case '%':
			return 3;
		case '/':
			return 3;
		case '\\':
			return 3;
		case ':':
			return 3;
		default: {
			return 0;
		}
		}
	}

	void fixIt(LT::LexTable& lextable,
		const std::string& str, // строка, представляющая преобразованное выражение.
		size_t length, // сходная длина выражения в таблице
		size_t pos,  //начальная позиция выражения в таблице
		const std::vector<int>& ids) { // вектор индексов идентификаторов
		for (size_t i = 0, q = 0; i < str.size(); i++) { // q - index vectora
			lextable.table[pos + i].lexema = str[i]; // символ польской нотации (str[i]) в соответствующую ячейку лексической таблицы (lexema).
			if (lextable.table[pos + i].lexema == LEX_ID || lextable.table[pos + i].lexema == LEX_LITERAL) { // (LEX_ID \ LEX_LITERAL)?
				lextable.table[pos + i].idxTI = ids[q]; // то записывается индекс идентификатора
				q++;
			}
			else
				lextable.table[pos + i].idxTI = LT_TI_NULLIDX; // Если текущий символ — не идентификатор или литерал, то индекс idxTI помечается как LT_TI_NULLIDX (нет привязки к таблице идентификаторов).


		}
		int temp = str.size() + pos; // Вычисляет новую позицию temp, с которой начнется "очистка" оставшихся ячеек в лексической таблице.
		for (size_t i = 0; i < length - str.size(); i++) { // Проход по оставшимся позициям в лексической таблице, которые нужно "очистить".
			lextable.table[temp + i].idxTI = LT_TI_NULLIDX; // Устанавливает индекс идентификатора (idxTI) как LT_TI_NULLIDX для неиспользуемых ячеек.
			lextable.table[temp + i].lexema = '!'; // Помечает такие ячейки специальным символом !.
			lextable.table[temp + i].sn = -1; // Устанавливает строку (sn) как -1, указывая, что эта ячейка не относится к текущему выражению.
		}
	}

	bool NotaciaPolska(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		container<std::stack<char>> stack; // стек для временного хранения операторов и скобок
		std::string PolishString; // результат 
		std::vector<int> ids; //  вектор индексов таблицы IT LT a+b*C
		
		int operators_count = 0, operands_count = 0, iterator = 0, right_counter = 0, left_counter = 0, params_counter = 0;

		//enum IDTYPE { V = 1, F = 2, P = 3, L = 4 };//типы итендификаторов - переменная функция параметр литерал
		for (int i = lextable_pos; i < lextable.size; i++, iterator++) { // Цикл обрабатывает лексемы начиная с lextable_pos до конца таблицы.
			char lexem = lextable.table[i].lexema;
			char data = lextable.table[i].data; // дополнительные данные лексемы (например, символ оператора)
			size_t stack_size = stack.size();
			if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F) { // функция
				stack.push('@');
				operands_count--;// Уменьшается счетчик операндов (так как функция потребует их в будущем).
			}
			switch (lexem) {
			case LEX_OPERATOR:
			{
				if (!stack.empty() && stack.top() != LEX_LEFTHESIS) {
					while (!stack.empty() && get_priority(data) <= get_priority(stack.top())) { // приоритет текущего оператора (data) меньше либо равен приоритету оператора на вершине стека.
						PolishString += stack.top(); // Верхний элемент стека добавляется к строке PolishString, формируя постфиксное представление.
						stack.pop();
					}
				}
				stack.push(data);
				operators_count++;
				break;
			}
			case LEX_COMMA:
			{
				while (!stack.empty()) { // Извлекаются операторы из стека до ближайшей открывающей скобки
					if (stack.top() == LEX_LEFTHESIS)
						break;
					PolishString += stack.top();
					stack.pop();
				}
				operands_count--;
				break;
			}
			case LEX_LEFTHESIS:
			{
				left_counter++;
				stack.push(lexem);
				break;
			}
			case LEX_RIGHTHESIS:
			{
				right_counter++;
				if (!find_elem(stack, stack_size, LEX_LEFTHESIS))// Проверяем, есть ли в стеке открывающая скобка
					return false;
				while (stack.top() != LEX_LEFTHESIS) { // Пока верхний элемент стека не является открывающей скобкой:
					PolishString += stack.top(); // Добавляем верхний элемент стека в строку PolishString
					stack.pop(); // Удаляем 
				}
				stack.pop(); // Удаляем 
				if (!stack.empty() && stack.top() == '@') { // стек не пуст
					PolishString += stack.top() + toString(params_counter - 1); // Добавляем в PolishString символ @ (обозначение функции) и количество параметров, передаваемых в функцию
					params_counter = 0;
					stack.pop();
				}
				break;
			}
			case LEX_SEMICOLON:// конец выражения (роверяет, корректно ли выражение (скобки, операторы и операнды))
			{
				if (operators_count != 0 && operands_count != 0)
					if ((!stack.empty() && (stack.top() == LEX_RIGHTHESIS || stack.top() == LEX_LEFTHESIS))//  не остались ли в стеке незакрытые скобки и Проверяется, совпадает ли количество открывающих и закрывающих скобок.
						|| right_counter != left_counter || operands_count - operators_count != 1) // Проверяется правильное соотношение между операндами и операторами. В корректном выражении разница между количеством операндов и операторов должна быть равна 1.
						return false;
				while (!stack.empty()) { // очищать стек от оставшихся операторов
					PolishString += stack.top();
					stack.pop();
				}
				fixIt(lextable, PolishString, iterator, lextable_pos, ids);// обновляет таблицу лексем
				return true;
				break;
			}
			case LEX_ID: {
				if (std::find(stack.c.begin(), stack.c.begin(), '@') != stack.c.end()) //@ - func
					params_counter++;
				PolishString += lexem; // Добавляется текущий идентификатор
				if (lextable.table[i].idxTI != LT_TI_NULLIDX) // сли индекс существует (не равен LT_TI_NULLIDX), значит, идентификатор зарегистрирован в таблице.
					ids.push_back(lextable.table[i].idxTI);
				operands_count++;
				break;
			}
			case LEX_LITERAL: {
				if (std::find(stack.c.begin(), stack.c.begin(), '@') != stack.c.end())// @ - func
					params_counter++;
				PolishString += lexem;
				if (lextable.table[i].idxTI != LT_TI_NULLIDX) // Проверяется, зарегистрирован ли литерал в таблице идентификаторов Если литерал имеет индекс в таблице (не равен LT_TI_NULLIDX), этот индекс добавляется в список ids.
					ids.push_back(lextable.table[i].idxTI);
				operands_count++;
				break;
			}
			}

		}
		return true;
	}
	void Wykonac(LEX::LEX t) { // брабатывает таблицу лексем 
		for (int i = 0; i < t.lextable.size; i++)
			if (t.lextable.table[i].lexema == LEX_EQUAL)
				if (!NotaciaPolska(i + 1, t.lextable, t.idtable)) // Если после знака = выражение не удалось преобразовать в польскую нотацию 
					throw ERROR_THROW(130);
		for (int i = 0; i < t.lextable.size; i++) // Второй проход по таблице лексем. Цель — корректировать типы лексем для операторов
			if (t.lextable.table[i].lexema == '+' || t.lextable.table[i].lexema == '-' || t.lextable.table[i].lexema == '*' ||
				t.lextable.table[i].lexema == '/' || t.lextable.table[i].lexema == '\\' || t.lextable.table[i].lexema == ':' ||
				t.lextable.table[i].lexema == '%')
			{
				t.lextable.table[i].data = t.lextable.table[i].lexema; // пирует символ оператора из поля lexema в поле data.
				t.lextable.table[i].lexema = LEX_OPERATOR; // Меняет тип текущей лексемы на LEX_OPERATOR
			}
	}
}