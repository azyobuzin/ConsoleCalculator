#pragma once
#include <stdbool.h>

// �g�[�N���̎��
enum TokenType
{
	TOKEN_NONE, // �p�[�T�œ����I�Ɏg�p����
	TOKEN_BAD, // �ُ�ȃg�[�N��
	TOKEN_EOF, // �I��
	TOKEN_NUM, // ���l
	TOKEN_LPAREN, // (
	TOKEN_RPAREN, // )
	TOKEN_PLUS, // +
	TOKEN_MINUS, // -
	TOKEN_TIMES, // *
	TOKEN_DIV, // /
	TOKEN_CARET // ^
};

// 1�g�[�N���̏��
typedef struct Token
{
	enum TokenType type; // ���
	double data; // TOKEN_NUM �̐��l
} Token;

// �g�[�N���̘A�����X�g
typedef struct TokenList* TokenList;
struct TokenList
{
	Token value;
	TokenList next;
};

// �W�����͂���ǂݎ���ăg�[�N�����X�g���쐬���܂��B
// ���s�܂��� EOF �ɏo��܂œǂݎ�葱���܂��B
TokenList lexFromStdin(void);

// TokenList �Ŏg�p���Ă��郁�������J�����܂��B
// �p�ӂ������ǖ��g�p
void freeTokenList(TokenList firstNode);

// �G���[������Ȃ� true ��Ԃ��܂��B
bool hasError(TokenList node);
