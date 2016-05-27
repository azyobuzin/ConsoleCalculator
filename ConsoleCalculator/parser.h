#pragma once
#include "lexer.h"

// ���̎��
typedef enum ExprType
{
	EXPR_BAD, // �ُ�Ȏ�
	EXPR_NUM, // �萔
	EXPR_ADD, // ���Z
	EXPR_SUB, // ���Z
	EXPR_MUL, // ��Z
	EXPR_DIV, // ���Z
	EXPR_POW, // �ׂ���
	EXPR_NEG // �ے�i�������]�j
} ExprType;

// ����\���I�u�W�F�N�g
typedef struct Expr* Expr;
struct Expr
{
	ExprType type;
	union // ���̎�ނ��ƂɎg���f�[�^
	{
		// �񍀉��Z�q�̎�
		struct { Expr left; Expr right; } binary;
		// �P�����Z�q�̎�
		Expr unary;
		// �萔��
		double num;
	} u;
};

Expr parse(TokenList tokens);

void freeExpr(Expr root);
