#pragma once
#include "parser.h"

// 計算結果
typedef struct EvalResult
{
	// 成功したかどうか（BadExprが含まれるかどうかによる）
	bool success;
	// 計算結果の数値
	double result;
} EvalResult;

// 構文解析結果を使って実際に計算します。
EvalResult eval(Expr expr);
