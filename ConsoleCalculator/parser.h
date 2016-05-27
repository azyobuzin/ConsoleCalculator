#pragma once
#include "lexer.h"

// 式の種類
typedef enum ExprType
{
	EXPR_BAD, // 異常な式
	EXPR_NUM, // 定数
	EXPR_ADD, // 加算
	EXPR_SUB, // 減算
	EXPR_MUL, // 乗算
	EXPR_DIV, // 除算
	EXPR_POW, // べき乗
	EXPR_NEG // 否定（符号反転）
} ExprType;

// 式を表すオブジェクト
typedef struct Expr* Expr;
struct Expr
{
	ExprType type;
	union // 式の種類ごとに使うデータ
	{
		// 二項演算子の式
		struct { Expr left; Expr right; } binary;
		// 単項演算子の式
		Expr unary;
		// 定数式
		double num;
	} u;
};

// トークンリストから式の木を作成します。
Expr parse(TokenList tokens);

// メモリを解放します。
// 用意したけど未使用
void freeExpr(Expr root);
