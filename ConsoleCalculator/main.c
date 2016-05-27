#include <stdio.h>
#include "parser.h"

// �g�[�N�����X�g��W���o�͂ɏ����o���܂��i�f�o�b�O�p�j�B
void printTokens(TokenList *pNode)
{
	while (true)
	{
		switch (pNode->value.type)
		{
		case TOKEN_BAD:
			printf("BAD\n");
			return;
		case TOKEN_EOF:
			printf("EOF\n");
			return;
		case TOKEN_NUM:
			printf("%lf\n", pNode->value.data);
			break;
		case TOKEN_LPAREN:
			printf("(\n");
			break;
		case TOKEN_RPAREN:
			printf(")\n");
			break;
		case TOKEN_PLUS:
			printf("+\n");
			break;
		case TOKEN_MINUS:
			printf("-\n");
			break;
		case TOKEN_TIMES:
			printf("*\n");
			break;
		case TOKEN_DIV:
			printf("/\n");
			break;
		default:
			printf("�s���ȃg�[�N��\n");
			return;
		}

		pNode = pNode->next;
	}
}

int main(void)
{
	printf("�v�Z�� => ");

	TokenList *pNode = lexFromStdin();
	printTokens(pNode);

	return 0;
}
