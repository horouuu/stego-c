#include <stdio.h>
typedef enum
{
    START,
    IDENTIFIER_OR_KEYWORD,
    KEYWORD_CHECK,
    DIV_OR_COMMENT,
    SINGLE_LINE_COMMENT,
    MULTI_LINE_COMMENT,
    MLC_END_CHECK,
    END_COMMENT
} FSMState;

void run_fsm(const char *input, FILE *out);