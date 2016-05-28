// 構文解析

#include <stdlib.h>
#include "parser.h"

// ParseState のポインタを持ってまわって状態を保持する
struct ParseState
{
	TokenList currentToken;
};

typedef struct ParseState* State;

// Level1 := Level2 | Level1 + Level2 | Level1 - Level2
Expr parseLevel1(State state);

// Level2 := Level3 | Level2 * Level3 | Level2 / Level3
Expr parseLevel2(State state);

// Level3 := Level4 | Level3 ^ Level4
Expr parseLevel3(State state);

// Level4 := Level5 | + Level5 | - Level5
Expr parseLevel4(State state);

// Level5 := NUM | ( Level1 ) | Level5 NUM | Level5 ( Level1 )
Expr parseLevel5(State state);

// 現在のトークンが token 引数と一致するなら currentToken を進めて true を返します。
bool eat(State state, enum TokenType token)
{
	// 現在のトークンが BAD か EOF なら読み進めない
	enum TokenType currentType = state->currentToken->value.type;
	switch (currentType)
	{
	case TOKEN_BAD:
	case TOKEN_EOF:
		return token == currentType;
	}

	// 次のトークンと比較
	if (token == state->currentToken->next->value.type)
	{
		state->currentToken = state->currentToken->next;
		return true;
	}
	return false;
}

// 動的にメモリを確保して expr を代入したポインタを返します。
Expr dynamicExpr(struct Expr expr)
{
	Expr p = malloc(sizeof(struct Expr));
	*p = expr;
	return p;
}

// 二項演算式オブジェクトを作成します。
Expr newBinaryExpr(enum ExprType type, Expr left, Expr right)
{
	struct Expr x = { .type = type, .binary = { left, right } };
	return dynamicExpr(x);
}

// 単項演算式オブジェクトを作成します。
Expr newUnaryExpr(enum ExprType type, Expr expr)
{
	struct Expr x = { .type = type, .unary = expr };
	return dynamicExpr(x);
}

// 定数式オブジェクトを作成します。
Expr newNumExpr(double value)
{
	struct Expr x = { .type = EXPR_NUM, .num = value };
	return dynamicExpr(x);
}

// 不正な式を表すオブジェクトを作成します。
Expr newBadExpr(void)
{
	Expr p = malloc(sizeof(struct Expr));
	p->type = EXPR_BAD;
	return p;
}

// 各レベルごとの構文解析。説明は上での定義を参照。

Expr parseLevel1(State state)
{
	// 左辺の読み取り
	Expr expr = parseLevel2(state);

	// 左再帰に対応するためループ
	while (true)
	{
		if (eat(state, TOKEN_PLUS))
		{
			// 加算
			// 右辺を読み取って、二項演算式を作成する。
			expr = newBinaryExpr(EXPR_ADD, expr, parseLevel2(state));
		}
		else if (eat(state, TOKEN_MINUS))
		{
			// 減算
			expr = newBinaryExpr(EXPR_SUB, expr, parseLevel2(state));
		}
		else
		{
			// + か - が次に来ないのでループ終了
			break;
		}
	}

	return expr;
}

Expr parseLevel2(State state)
{
	// 左辺の読み取り
	Expr expr = parseLevel3(state);

	while (true)
	{
		if (eat(state, TOKEN_TIMES))
		{
			// 乗算
			expr = newBinaryExpr(EXPR_MUL, expr, parseLevel3(state));
		}
		else if (eat(state, TOKEN_DIV))
		{
			// 除算
			expr = newBinaryExpr(EXPR_DIV, expr, parseLevel3(state));
		}
		else
		{
			// * か / が次に来ないのでループ終了
			break;
		}
	}

	return expr;
}

Expr parseLevel3(State state)
{
	// 左辺の読み取り
	Expr expr = parseLevel4(state);

	while (true)
	{
		if (eat(state, TOKEN_CARET))
		{
			// べき乗
			expr = newBinaryExpr(EXPR_POW, expr, parseLevel4(state));
		}
		else
		{
			// ^ が次に来ないのでループ終了
			break;
		}
	}

	return expr;
}

Expr parseLevel4(State state)
{
	if (eat(state, TOKEN_MINUS))
	{
		// 否定
		return newUnaryExpr(EXPR_NEG, parseLevel5(state));
	}

	eat(state, TOKEN_PLUS); // 単項演算子 + は何もしないので戻り値を気にしない
	return parseLevel5(state);
}

Expr parseLevel5(State state)
{
	Expr expr = NULL;

	while (true)
	{
		Expr right;
		if (eat(state, TOKEN_NUM))
		{
			// 定数式
			right = newNumExpr(state->currentToken->value.data);
		}
		else if (eat(state, TOKEN_LPAREN))
		{
			// 括弧
			right = parseLevel1(state); // 中身の式
			// ) があることを確認。 EOF は閉じ括弧省略とみなす
			if (!eat(state, TOKEN_RPAREN) && !eat(state, TOKEN_EOF))
			{
				// 括弧が閉じられていないので不正な式
				freeExpr(right);
				right = newBadExpr();
			}
		}
		else
		{
			// 定数でも括弧でもないのでループ終了
			break;
		}

		if (expr == NULL)
			expr = right;
		else
			// 連続した括弧は乗算扱い
			expr = newBinaryExpr(EXPR_MUL, expr, right);
	}

	// expr が NULL → 1トークンも読み取れなかったので BadExpr とする
	if (expr == NULL)
		return newBadExpr();

	return expr;
}

// 外部からの入り口
Expr parse(TokenList tokens)
{
	// ParseState を作成して parseLevel1 を開始
	struct TokenList first = { .value = { .type = TOKEN_NONE }, .next = tokens };
	struct ParseState state = { .currentToken = &first };
	return parseLevel1(&state);
}

void freeExpr(Expr expr)
{
	switch (expr->type)
	{
	case EXPR_ADD:
	case EXPR_SUB:
	case EXPR_MUL:
	case EXPR_DIV:
	case EXPR_POW:
		freeExpr(expr->binary.left);
		freeExpr(expr->binary.right);
		break;
	case EXPR_NEG:
		freeExpr(expr->unary);
		break;
	}

	free(expr);
}
