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
		// スペースとタブは読み飛ばす
		goto SCAN;

	case EOF:
	case '\r':
	case '\n':
		// EOFまたは改行は TOKEN_EOF を返して終了
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
		// 数値トークンの読み取り
		// scanf_s を使って読み取り直す
		ungetc(input, stdin);
		double data;
		switch (scanf_s("%lf", &data))
		{
		case EOF:
			// ストリームが終了しているので EOF 扱い（すでに上で検証してるからここには来ないはず）
			token.type = TOKEN_EOF;
			goto RETURN;
		case 0:
			// 小数として読み取れなかったのでエラーとする
			token.type = TOKEN_BAD;
			goto RETURN;
		}

		token.type = TOKEN_NUM;
		token.data = data;
		break;

	case '(':
		// ( トークン
		token.type = TOKEN_LPAREN;
		break;

	case ')':
		// ) トークン
		token.type = TOKEN_RPAREN;
		break;

	case '+':
		// + トークン
		token.type = TOKEN_PLUS;
		break;

	case '-':
		// - トークン
		token.type = TOKEN_MINUS;
		break;

	case '*':
		// * トークン
		token.type = TOKEN_TIMES;
		break;

	case '/':
		// / トークン
		token.type = TOKEN_DIV;
		break;

	default:
		// 対応していない文字が入力されたのでエラー
		token.type = TOKEN_BAD;
		goto RETURN;
	}

	// 再帰で次のトークンを読み取る
	pNode->next = lexFromStdin();

RETURN:
	pNode->value = token;
	return pNode;
}

void freeTokenList(TokenList *firstNode)
{
	// TOKEN_BAD または TOKEN_EOF が最後になるので、そこまで順番に開放していく
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
	// とにかくループで掘っていく
	// （これやるなら連結リストじゃないほうがよかったかも）
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
