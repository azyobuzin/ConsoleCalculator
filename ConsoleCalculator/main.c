#include <stdio.h>
#include <string.h>
#include "printer.h"

int main(int argc, char *argv[])
{
	// コマンドライン引数に --debug があるならデバッグモード
	bool debugMode = false;
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "--debug") == 0)
		{
			debugMode = true;
			break;
		}
	}

	printf("計算式 => ");

	// 計算式を読み取ってトークンリストを作成
	TokenList tokens = lexFromStdin();

	if (debugMode)
	{
		// デバッグモードならトークンリストを表示
		printf("Tokens: ");
		printTokens(tokens);
		printf("\n");
	}

	if (hasError(tokens))
	{
		// 字句解析に失敗したのでエラーを表示して終了
		if (debugMode)
		{
			printf("字句解析失敗\n");
		}
		else
		{
			printf("文法に誤りがあります。\n字句解析結果: ");
			printTokens(tokens);
			printf("\n");
		}
		return 1;
	}

	// トークンリストから式の木を作成
	Expr expr = parse(tokens);

	if (debugMode)
	{
		// デバッグモードなら式を表示
		printf("Expr: ");
		printExpr(expr);
		printf("\n");
	}

	return 0;
}
