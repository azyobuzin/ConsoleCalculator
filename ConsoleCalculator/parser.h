#pragma once
#include "lexer.h"

// ®‚Ìí—Ş
typedef enum ExprType
{
	EXPR_BAD, // ˆÙí‚È®
	EXPR_NUM, // ”’l
	EXPR_ADD, // ‰ÁZ
	EXPR_SUB, // Œ¸Z
	EXPR_MUL, // æZ
	EXPR_DIV // œZ	
} ExprType;

typedef struct Expr Expr;

struct Expr
{
	ExprType type;
	union ExprUnion
	{
		struct OpExpr
		{
			Expr *left;
			Expr *right;
		} op;
		double num;
	} u;
};

Expr* parse(TokenList *tokens);

void freeExpr(Expr *root);
