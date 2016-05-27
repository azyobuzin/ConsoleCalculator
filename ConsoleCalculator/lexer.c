#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

TokenList* lexFromStdin(void)
{
	int input;
	Token token;
	TokenList *pNode = malloc(sizeof(TokenList));

SCAN:
	input = getchar();
	switch (input)
	{
	case ' ':
	case '\t':
		// �X�y�[�X�ƃ^�u�͓ǂݔ�΂�
		goto SCAN;

	case EOF:
	case '\r':
	case '\n':
		// EOF�܂��͉��s�� TOKEN_EOF ��Ԃ��ďI��
		token.type = TOKEN_EOF;
		goto RETURN;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '.':
		// ���l�g�[�N���̓ǂݎ��
		// scanf_s ���g���ēǂݎ�蒼��
		ungetc(input, stdin);
		double data;
		switch (scanf_s("%lf", &data))
		{
		case EOF:
			// �X�g���[�����I�����Ă���̂� EOF �����i���łɏ�Ō��؂��Ă邩�炱���ɂ͗��Ȃ��͂��j
			token.type = TOKEN_EOF;
			goto RETURN;
		case 0:
			// �����Ƃ��ēǂݎ��Ȃ������̂ŃG���[�Ƃ���
			token.type = TOKEN_BAD;
			goto RETURN;
		}

		token.type = TOKEN_NUM;
		token.data = data;
		break;

	case '(':
		// ( �g�[�N��
		token.type = TOKEN_LPAREN;
		break;

	case ')':
		// ) �g�[�N��
		token.type = TOKEN_RPAREN;
		break;

	case '+':
		// + �g�[�N��
		token.type = TOKEN_PLUS;
		break;

	case '-':
		// - �g�[�N��
		token.type = TOKEN_MINUS;
		break;

	case '*':
		// * �g�[�N��
		token.type = TOKEN_TIMES;
		break;

	case '/':
		// / �g�[�N��
		token.type = TOKEN_DIV;
		break;

	default:
		// �Ή����Ă��Ȃ����������͂��ꂽ�̂ŃG���[
		token.type = TOKEN_BAD;
		goto RETURN;
	}

	// �ċA�Ŏ��̃g�[�N����ǂݎ��
	pNode->next = lexFromStdin();

RETURN:
	pNode->value = token;
	return pNode;
}

void freeTokenList(TokenList *firstNode)
{
	// TOKEN_BAD �܂��� TOKEN_EOF ���Ō�ɂȂ�̂ŁA�����܂ŏ��ԂɊJ�����Ă���
	TokenList *current = firstNode, *next;
	while (true)
	{
		bool isLast = current->value.type == TOKEN_BAD || current->value.type == TOKEN_EOF;

		next = current->next;
		free(current);

		if (isLast) return;
		current = next;
	}
}

bool hasBadToken(TokenList *pNode)
{
	// �Ƃɂ������[�v�Ō@���Ă���
	// �i������Ȃ�A�����X�g����Ȃ��ق����悩���������j
	while (true)
	{
		switch (pNode->value.type)
		{
		case TOKEN_BAD:
			return true;
		case TOKEN_EOF:
			return false;
		}

		pNode = pNode->next;
	}
}
