// �\�����

#include <stdlib.h>
#include "parser.h"

// ParseState �̃|�C���^�������Ă܂���ď�Ԃ�ێ�����
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

// ���݂̃g�[�N���� token �����ƈ�v����Ȃ� currentToken ��i�߂� true ��Ԃ��܂��B
bool eat(State state, enum TokenType token)
{
	// ���݂̃g�[�N���� BAD �� EOF �Ȃ�ǂݐi�߂Ȃ�
	enum TokenType currentType = state->currentToken->value.type;
	switch (currentType)
	{
	case TOKEN_BAD:
	case TOKEN_EOF:
		return token == currentType;
	}

	// ���̃g�[�N���Ɣ�r
	if (token == state->currentToken->next->value.type)
	{
		state->currentToken = state->currentToken->next;
		return true;
	}
	return false;
}

// ���I�Ƀ��������m�ۂ��� expr ���������|�C���^��Ԃ��܂��B
Expr dynamicExpr(struct Expr expr)
{
	Expr p = malloc(sizeof(struct Expr));
	*p = expr;
	return p;
}

// �񍀉��Z���I�u�W�F�N�g���쐬���܂��B
Expr newBinaryExpr(enum ExprType type, Expr left, Expr right)
{
	struct Expr x = { .type = type, .binary = { left, right } };
	return dynamicExpr(x);
}

// �P�����Z���I�u�W�F�N�g���쐬���܂��B
Expr newUnaryExpr(enum ExprType type, Expr expr)
{
	struct Expr x = { .type = type, .unary = expr };
	return dynamicExpr(x);
}

// �萔���I�u�W�F�N�g���쐬���܂��B
Expr newNumExpr(double value)
{
	struct Expr x = { .type = EXPR_NUM, .num = value };
	return dynamicExpr(x);
}

// �s���Ȏ���\���I�u�W�F�N�g���쐬���܂��B
Expr newBadExpr(void)
{
	Expr p = malloc(sizeof(struct Expr));
	p->type = EXPR_BAD;
	return p;
}

// �e���x�����Ƃ̍\����́B�����͏�ł̒�`���Q�ƁB

Expr parseLevel1(State state)
{
	// ���ӂ̓ǂݎ��
	Expr expr = parseLevel2(state);

	// ���ċA�ɑΉ����邽�߃��[�v
	while (true)
	{
		if (eat(state, TOKEN_PLUS))
		{
			// ���Z
			// �E�ӂ�ǂݎ���āA�񍀉��Z�����쐬����B
			expr = newBinaryExpr(EXPR_ADD, expr, parseLevel2(state));
		}
		else if (eat(state, TOKEN_MINUS))
		{
			// ���Z
			expr = newBinaryExpr(EXPR_SUB, expr, parseLevel2(state));
		}
		else
		{
			// + �� - �����ɗ��Ȃ��̂Ń��[�v�I��
			break;
		}
	}

	return expr;
}

Expr parseLevel2(State state)
{
	// ���ӂ̓ǂݎ��
	Expr expr = parseLevel3(state);

	while (true)
	{
		if (eat(state, TOKEN_TIMES))
		{
			// ��Z
			expr = newBinaryExpr(EXPR_MUL, expr, parseLevel3(state));
		}
		else if (eat(state, TOKEN_DIV))
		{
			// ���Z
			expr = newBinaryExpr(EXPR_DIV, expr, parseLevel3(state));
		}
		else
		{
			// * �� / �����ɗ��Ȃ��̂Ń��[�v�I��
			break;
		}
	}

	return expr;
}

Expr parseLevel3(State state)
{
	// ���ӂ̓ǂݎ��
	Expr expr = parseLevel4(state);

	while (true)
	{
		if (eat(state, TOKEN_CARET))
		{
			// �ׂ���
			expr = newBinaryExpr(EXPR_POW, expr, parseLevel4(state));
		}
		else
		{
			// ^ �����ɗ��Ȃ��̂Ń��[�v�I��
			break;
		}
	}

	return expr;
}

Expr parseLevel4(State state)
{
	if (eat(state, TOKEN_MINUS))
	{
		// �ے�
		return newUnaryExpr(EXPR_NEG, parseLevel5(state));
	}

	eat(state, TOKEN_PLUS); // �P�����Z�q + �͉������Ȃ��̂Ŗ߂�l���C�ɂ��Ȃ�
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
			// �萔��
			right = newNumExpr(state->currentToken->value.data);
		}
		else if (eat(state, TOKEN_LPAREN))
		{
			// ����
			right = parseLevel1(state); // ���g�̎�
			// ) �����邱�Ƃ��m�F�B EOF �͕����ʏȗ��Ƃ݂Ȃ�
			if (!eat(state, TOKEN_RPAREN) && !eat(state, TOKEN_EOF))
			{
				// ���ʂ������Ă��Ȃ��̂ŕs���Ȏ�
				freeExpr(right);
				right = newBadExpr();
			}
		}
		else
		{
			// �萔�ł����ʂł��Ȃ��̂Ń��[�v�I��
			break;
		}

		if (expr == NULL)
			expr = right;
		else
			// �A���������ʂ͏�Z����
			expr = newBinaryExpr(EXPR_MUL, expr, right);
	}

	// expr �� NULL �� 1�g�[�N�����ǂݎ��Ȃ������̂� BadExpr �Ƃ���
	if (expr == NULL)
		return newBadExpr();

	return expr;
}

// �O������̓����
Expr parse(TokenList tokens)
{
	// ParseState ���쐬���� parseLevel1 ���J�n
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
