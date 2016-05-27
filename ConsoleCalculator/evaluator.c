#include <math.h>
#include "evaluator.h"

// �񍀉��Z�q�̎���]�����āA���ӁE�E�ӂƂ��ɕ]����������� true ��Ԃ��B
bool evalBinOp(Expr expr, double *destLeft, double *destRight)
{
	EvalResult leftResult = eval(expr->u.binary.left);
	if (!leftResult.success) return false;

	EvalResult rightResult = eval(expr->u.binary.right);
	if (!rightResult.success) return false;

	*destLeft = leftResult.result;
	*destRight = rightResult.result;
	return true;
}

// �񍀉��Z�q�����̂Ń}�N����
#define BIN(op) r.success = evalBinOp(expr, &left, &right); r.result = left op right

EvalResult eval(Expr expr)
{
	EvalResult r;

	double left, right; // ���Z�q�̎��s���ʂ����Ă���

	switch (expr->type)
	{
	case EXPR_NUM:
		// �萔�Ȃ̂ł��̂܂ܓ˂�����
		r.success = true;
		r.result = expr->u.num;
		break;
	case EXPR_ADD:
		// ���Z
		BIN(+);
		break;
	case EXPR_SUB:
		// ���Z
		BIN(-);
		break;
	case EXPR_MUL:
		// ��Z
		BIN(*);
		break;
	case EXPR_DIV:
		// ���Z
		BIN(/);
		break;
	case EXPR_POW:
		// �ׂ���
		if (r.success = evalBinOp(expr, &left, &right))
			r.result = pow(left, right);
		break;
	case EXPR_NEG:
		// �ے�
		r = eval(expr->u.unary);
		if (r.success)
			r.result = -r.result;
		break;
	default:
		// BAD
		r.success = false;
		break;
	}

	return r;
}

#undef BIN
