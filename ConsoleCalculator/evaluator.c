#include <math.h>
#include "evaluator.h"

// “ñ€‰‰ŽZŽq‚ÌŽ®‚ð•]‰¿‚µ‚ÄA¶•ÓE‰E•Ó‚Æ‚à‚É•]‰¿¬Œ÷‚·‚ê‚Î true ‚ð•Ô‚·B
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

// “ñ€‰‰ŽZŽq‘½‚¢‚Ì‚Åƒ}ƒNƒ‰»
#define BIN(op) r.success = evalBinOp(expr, &left, &right); r.result = left op right

EvalResult eval(Expr expr)
{
	EvalResult r;

	double left, right; // ‰‰ŽZŽq‚ÌŽÀsŒ‹‰Ê‚ð“ü‚ê‚Ä‚¨‚­

	switch (expr->type)
	{
	case EXPR_NUM:
		// ’è”‚È‚Ì‚Å‚»‚Ì‚Ü‚Ü“Ë‚Áž‚Þ
		r.success = true;
		r.result = expr->u.num;
		break;
	case EXPR_ADD:
		// ‰ÁŽZ
		BIN(+);
		break;
	case EXPR_SUB:
		// Œ¸ŽZ
		BIN(-);
		break;
	case EXPR_MUL:
		// æŽZ
		BIN(*);
		break;
	case EXPR_DIV:
		// œŽZ
		BIN(/);
		break;
	case EXPR_POW:
		// ‚×‚«æ
		if (r.success = evalBinOp(expr, &left, &right))
			r.result = pow(left, right);
		break;
	case EXPR_NEG:
		// ”Û’è
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
