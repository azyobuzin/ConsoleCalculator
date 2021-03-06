#include <math.h>
#include "evaluator.h"

// 二項演算子の式を評価して、左辺・右辺ともに評価成功すれば true を返す。
bool evalBinOp(Expr expr, double *destLeft, double *destRight)
{
	EvalResult leftResult = eval(expr->binary.left);
	if (!leftResult.success) return false;

	EvalResult rightResult = eval(expr->binary.right);
	if (!rightResult.success) return false;

	*destLeft = leftResult.result;
	*destRight = rightResult.result;
	return true;
}

// 二項演算子多いのでマクロ化
#define BIN(op) \
	r.success = evalBinOp(expr, &left, &right); \
	r.result = left op right

EvalResult eval(Expr expr)
{
	EvalResult r;

	double left, right; // 演算子の実行結果を入れておく

	switch (expr->type)
	{
	case EXPR_NUM:
		// 定数なのでそのまま突っ込む
		r.success = true;
		r.result = expr->num;
		break;
	case EXPR_ADD:
		// 加算
		BIN(+);
		break;
	case EXPR_SUB:
		// 減算
		BIN(-);
		break;
	case EXPR_MUL:
		// 乗算
		BIN(*);
		break;
	case EXPR_DIV:
		// 除算
		BIN(/);
		break;
	case EXPR_POW:
		// べき乗
		if (r.success = evalBinOp(expr, &left, &right))
			r.result = pow(left, right);
		break;
	case EXPR_NEG:
		// 否定
		r = eval(expr->unary);
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
