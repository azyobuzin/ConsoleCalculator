#include <stdio.h>
#include <string.h>
#include "evaluator.h"
#include "printer.h"

int main(int argc, char *argv[])
{
	// �R�}���h���C�������� --debug ������Ȃ�f�o�b�O���[�h
	bool debugMode = false;
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "--debug") == 0)
		{
			debugMode = true;
			break;
		}
	}

	printf("�v�Z�� => ");

	// �v�Z����ǂݎ���ăg�[�N�����X�g���쐬
	TokenList tokens = lexFromStdin();

	if (debugMode)
	{
		// �f�o�b�O���[�h�Ȃ�g�[�N�����X�g��\��
		printf("Tokens: ");
		printTokens(tokens);
		printf("\n");
	}

	if (hasError(tokens))
	{
		// �����͂Ɏ��s�����̂ŃG���[��\�����ďI��
		if (debugMode)
		{
			printf("�����͎��s\n");
		}
		else
		{
			printf("���@�Ɍ�肪����܂��B\n�����͌���: ");
			printTokens(tokens);
			printf("\n");
		}
		return 1;
	}

	// �g�[�N�����X�g���玮�̖؂��쐬
	Expr expr = parse(tokens);
	
	// ���s�I�I
	EvalResult result = eval(expr);

	if (result.success)
	{
		printExpr(expr);
		printf(" = %lf\n", result.result);
	}
	else
	{
		printf("���@�Ɍ�肪����܂��B\n�����͌���: ");
		printTokens(tokens);
		printf("\n�\����͌���: ");
		printExpr(expr);
		printf("\n");
		return 1;
	}

	return 0;
}
