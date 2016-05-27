#pragma once
#include <stdbool.h>

// �g�[�N���̎��
enum TokenType
{
	TOKEN_BAD, // �ُ�ȃg�[�N��
	TOKEN_EOF, // �I��
	TOKEN_NUM, // ���l
	TOKEN_LPAREN, // (
	TOKEN_RPAREN, // )
	TOKEN_PLUS, // +
	TOKEN_MINUS, // -
	TOKEN_TIMES, // *
	TOKEN_DIV // /
};

// 1�g�[�N���̏��
typedef struct Token
{
	enum TokenType type; // ���
	double data; // TOKEN_NUM �̐��l
} Token;

// �g�[�N���̘A�����X�g
typedef struct TokenList
{
	Token value;
	struct TokenList *next;
} TokenList;

// �W�����͂���ǂݎ���ăg�[�N�����X�g���쐬���܂��B
// ���s�܂��� EOF �ɏo��܂œǂݎ�葱���܂��B
TokenList* lexFromStdin(void);

// TokenList �Ŏg�p���Ă��郁�������J�����܂��B
void freeTokenList(TokenList *firstNode);

// TOKEN_BAD ���܂܂�Ă���Ȃ� true ��Ԃ��܂��B
bool hasBadToken(TokenList *pNode);