#pragma once
#include <stdbool.h>

// トークンの種類
enum TokenType
{
	TOKEN_NONE, // パーサで内部的に使用する
	TOKEN_BAD, // 異常なトークン
	TOKEN_EOF, // 終了
	TOKEN_NUM, // 数値
	TOKEN_LPAREN, // (
	TOKEN_RPAREN, // )
	TOKEN_PLUS, // +
	TOKEN_MINUS, // -
	TOKEN_TIMES, // *
	TOKEN_DIV, // /
	TOKEN_CARET // ^
};

// 1トークンの情報
typedef struct Token
{
	enum TokenType type; // 種類
	double data; // TOKEN_NUM の数値
} Token;

// トークンの連結リスト
typedef struct TokenList* TokenList;
struct TokenList
{
	Token value;
	TokenList next;
};

// 標準入力から読み取ってトークンリストを作成します。
// 改行または EOF に出会うまで読み取り続けます。
TokenList lexFromStdin(void);

// TokenList で使用しているメモリを開放します。
// 用意したけど未使用
void freeTokenList(TokenList firstNode);

// エラーがあるなら true を返します。
bool hasError(TokenList node);
