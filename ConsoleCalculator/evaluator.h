#pragma once
#include "parser.h"

// �v�Z����
typedef struct EvalResult
{
	// �����������ǂ����iBadExpr���܂܂�邩�ǂ����ɂ��j
	bool success;
	// �v�Z���ʂ̐��l
	double result;
} EvalResult;

// �\����͌��ʂ��g���Ď��ۂɌv�Z���܂��B
EvalResult eval(Expr expr);
