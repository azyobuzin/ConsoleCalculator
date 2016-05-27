#pragma once
#include "parser.h"

// トークンリストを標準出力に書き出します。
void printTokens(TokenList node);

// 構文解析された式を標準出力に書き出します。
void printExpr(Expr expr);
