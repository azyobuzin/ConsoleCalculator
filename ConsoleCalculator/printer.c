// 字句解析・構文解析結果を標準出力に書き出す関数

#include <stdio.h>
#include "printer.h"

void printTokens(TokenList node)
{
	while (true)
	{
		switch (node->value.type)
		{
		case TOKEN_BAD:
			printf("BAD ");
			return;
		case TOKEN_EOF:
			printf("EOF ");
			return;
		case TOKEN_NUM:
			printf("%lf ", node->value.data);
			break;
		case TOKEN_LPAREN:
			printf("( ");
			break;
		case TOKEN_RPAREN:
			printf(") ");
			break;
		case TOKEN_PLUS:
			printf("+ ");
			break;
		case TOKEN_MINUS:
			printf("- ");
			break;
		case TOKEN_TIMES:
			printf("* ");
			break;
		case TOKEN_DIV:
			printf("/ ");
			break;
		case TOKEN_CARET:
			printf("^ ");
			break;
		default:
			printf("? ");
			return;
		}

		node = node->next;
	}
}

// 二項演算子の式を (left op right) の形で出力します。
void printBinaryExpr(Expr expr, char op)
{
	printf("(");
	printExpr(expr->u.binary.left);
	printf(" %c ", op);
	printExpr(expr->u.binary.right);
	printf(")");
}

void printExpr(Expr expr)
{
	switch (expr->type)
	{
	case EXPR_BAD:
		printf("BAD");
		break;
	case EXPR_NUM:
		printf("%lf", expr->u.num);
		break;
	case EXPR_ADD:
		printBinaryExpr(expr, '+');
		break;
	case EXPR_SUB:
		printBinaryExpr(expr, '-');
		break;
	case EXPR_MUL:
		printBinaryExpr(expr, '*');
		break;
	case EXPR_DIV:
		printBinaryExpr(expr, '/');
		break;
	case EXPR_POW:
		printBinaryExpr(expr, '^');
		break;
	case EXPR_NEG:
		printf("- ");
		printExpr(expr->u.unary);
		break;
	default:
		printf("?");
		break;
	}
}
