#include "parser.h"

struct ParseState
{
	TokenList *currentToken;
};

typedef struct ParseState* State;

// Level1 := Level2 | Level2 (+|-) Level2
Expr* parseLevel1(State state);

// Level2 := Level3 | Level3 (*|/) Level3
Expr* parseLevel2(State state)
