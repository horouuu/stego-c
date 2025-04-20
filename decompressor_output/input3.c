#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 20

#define MAX_LENGTH 10


typedef struct {
    
    char* symbol;
    int is_terminal;
    int is_start;
} CFGSymbol;

void init_CFGSymbol(CFGSymbol* symbol, char* text, int is_terminal, int is_start) {
    
    symbol->symbol = text;
    symbol->is_terminal = is_terminal;
    symbol->is_start = is_start;
}


void init_Terminal(CFGSymbol* symbol, char* text) {
    
    symbol->symbol = text;
    symbol->is_terminal = 1;
    symbol->is_start = 0;
}


void tokenizeBooleanExpression(char* str, CFGSymbol* symbols, int* symbol_count,
                               CFGSymbol* and_sym, CFGSymbol* or_sym, CFGSymbol* true_sym,
                               CFGSymbol* false_sym, CFGSymbol* lparen, CFGSymbol* rparen) {
    *symbol_count = 0; 
    char buffer[MAX_LENGTH]; 
    int i = 0;
    int cnt = 0;
    
    while(*str != '\0'){
        i = 0;
        while(*str == ' '){
            str ++;
        }
        if(*str == '('){
            buffer[i] = *str;
            i++;
            str++;
            symbols[cnt] = *lparen;
            cnt += 1;
            continue;
        } else if(*str == ')'){
            buffer[i] = *str;
            i++;
            str++;
            symbols[cnt] = *rparen;
            cnt += 1;
            continue;
        } else if(isalpha(*str)){
            while(*str != ' ' && *str != '(' && *str != ')' && *str != '\0'){
                buffer[i] = *str;
                i++;
                str++;
            }
            buffer[i] = '\0';
            if(!strcmp(buffer, and_sym->symbol)){
                symbols[cnt] = *and_sym;
            } else if(!strcmp(buffer, or_sym->symbol)){
                symbols[cnt] = *or_sym;
            } else if(!strcmp(buffer, true_sym->symbol)){
                symbols[cnt] = *true_sym;
            } else if(!strcmp(buffer, false_sym->symbol)){
                symbols[cnt] = *false_sym;
            } 
            cnt += 1;
        } else {
            printf("[ERROR] Unexpected Character %c\n", *str);
            return;
        }
    }
    *symbol_count = cnt;
}


int main() {
    
    printf("\n[Test Case 1] Terminal Initialization\n");
    CFGSymbol and_sym, or_sym, true_sym, false_sym, lparen, rparen;

    init_Terminal(&and_sym, "AND");
    init_Terminal(&or_sym, "OR");
    init_Terminal(&true_sym, "true");
    init_Terminal(&false_sym, "false");
    init_Terminal(&lparen, "(");
    init_Terminal(&rparen, ")");

    printf("Expected: AND OR true false ( )\n");
    printf("Actual  : %s %s %s %s %s %s\n",
           and_sym.symbol, or_sym.symbol,
           true_sym.symbol, false_sym.symbol,
           lparen.symbol, rparen.symbol);

    
    printf("\n[Test Case 2] init_CFGSymbol() for Non-Terminal\n");
    CFGSymbol sym;
    init_CFGSymbol(&sym, "B", 0, 1);
    printf("Expected: B terminal=0 start=1\n");
    printf("Actual  : %s terminal=%d start=%d\n",
           sym.symbol, sym.is_terminal, sym.is_start);

    
    printf("\n[Test Case 3] Tokenizing Expression: true AND false\n");
    char expr1[] = "true AND false";
    CFGSymbol tokens1[MAX_TOKENS];
    int count1 = 0;

    tokenizeBooleanExpression(expr1, tokens1, &count1,
                              &and_sym, &or_sym, &true_sym,
                              &false_sym, &lparen, &rparen);

    printf("Expected Tokens: true AND false\nActual Tokens  : ");
    for (int i = 0; i < count1; i++) {
        printf("%s ", tokens1[i].symbol);
    }
    printf("\n");

    
    printf("\n[Test Case 4] Tokenizing Expression: (true OR false)\n");
    char expr2[] = "(true OR false)";
    CFGSymbol tokens2[MAX_TOKENS];
    int count2 = 0;

    tokenizeBooleanExpression(expr2, tokens2, &count2,
                              &and_sym, &or_sym, &true_sym,
                              &false_sym, &lparen, &rparen);

    printf("Expected Tokens: ( true OR false )\nActual Tokens  : ");
    for (int i = 0; i < count2; i++) {
        printf("%s ", tokens2[i].symbol);
    }
    printf("\n");

    
    printf("\n[Test Case 5] Tokenizing Invalid Expression: true && false\n");
    char expr3[] = "true && false";
    CFGSymbol tokens3[MAX_TOKENS];
    int count3 = 0;

    tokenizeBooleanExpression(expr3, tokens3, &count3,
                              &and_sym, &or_sym, &true_sym,
                              &false_sym, &lparen, &rparen);

    printf("Expected: Error on unexpected character '&'\n");

    return 0;
}
