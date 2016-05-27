#pragma once
#include "lexer.h"

// ���̎��
typedef enum ExprType
{
	EXPR_BAD, // �ُ�Ȏ�
	EXPR_NUM, // ���l
	EXPR_ADD, // ���Z
	EXPR_SUB, // ���Z
	EXPR_MUL, // ��Z
	EXPR_DIV // ���Z	
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
