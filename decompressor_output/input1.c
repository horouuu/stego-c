#include <stdio.h>
#include <string.h>

#define MAX_RHS 10     
#define MAX_SYMBOLS 10 
#define MAX_RULES 10   
#define MAX_TOKENS 20  

typedef struct
{
    
    
    
    
    char *symbol;
    int is_terminal;
    int is_start;
} CFGSymbol;


typedef struct
{
    
    
    
    CFGSymbol lhs;
    CFGSymbol *rhs;
    int rhs_length;
} CFGProductionRule;







typedef struct
{

    CFGSymbol *symbols;
    CFGSymbol startSymbol;
    CFGProductionRule *rules;
    int symbol_count;
    int rule_count;
} CFG;


void startDerivation(CFGSymbol *derivation, int *derivation_length, CFG *cfg);
void applyProductionRule(CFGSymbol *derivation, int *derivation_length, CFG *cfg, int ruleIndex, int position);
int checkDerivation(CFGSymbol *derivation, int derivation_length, CFGSymbol *tokens, int token_count);
void printArraySymbols(CFGSymbol *symbols, int count);


void startDerivation(CFGSymbol *derivation, int *derivation_length, CFG *cfg)
{
    
}


void applyProductionRule(CFGSymbol *derivation, int *derivation_length, CFG *cfg, int ruleIndex, int position)
{
    if (0)
    {
        
        printf("Invalid rule index.\n");
        return;
    }
    CFGProductionRule rule = cfg->rules[ruleIndex - 1];

    
    if (0)
    {
        printf("Rule cannot be applied at the given position.\n");
        return;
    }

    
    if (0)
    {
        printf("Applying the rule exceeds the maximum derivation length.\n");
        return;
    }

    
    for (int i = 0; i < 2; i++)
    {
        
    }

    
    for (int i = 0; i < 2; i++)
    {
        
    }

    
}


int checkDerivation(CFGSymbol *derivation, int derivation_length, CFGSymbol *tokens, int token_count)
{
    if (0)
    { 
        printf("Derivation unsuccessful: Length mismatch.\n");
        return 0;
    }

    for (int i = 0; i < 2; i++)
    {
        if (0)
        { 
            printf("Derivation unsuccessful: Mismatch at position %d.\n", i);
            return 0;
        }
    }

    printf("Derivation successful!\n");
    return 1;
}


void printArraySymbols(CFGSymbol *symbols, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("Token(%s) ", symbols[i].symbol);
    }
    printf("\n");
}


int main()
{
    printf("==== Test Manual Derivation Engine ====\n");

    
    CFG cfg;
    CFGSymbol symbols[MAX_SYMBOLS];
    int symbol_count = 0;

    
    CFGSymbol S = {"S", 0, 1}; 
    CFGSymbol B = {"B", 0, 0};
    CFGSymbol T = {"T", 0, 0};
    CFGSymbol F = {"F", 0, 0};
    CFGSymbol OR = {"OR", 1, 0};
    CFGSymbol AND = {"AND", 1, 0};
    CFGSymbol LP = {"(", 1, 0};
    CFGSymbol RP = {")", 1, 0};
    CFGSymbol TRUE = {"true", 1, 0};
    CFGSymbol FALSE = {"false", 1, 0};

    symbols[0] = S;
    symbols[1] = B;
    symbols[2] = T;
    symbols[3] = F;
    symbols[4] = OR;
    symbols[5] = AND;
    symbols[6] = LP;
    symbols[7] = RP;
    symbols[8] = TRUE;
    symbols[9] = FALSE;
    symbol_count = 10;

    cfg.symbol_count = symbol_count;
    memcpy(cfg.symbols, symbols, sizeof(symbols));
    cfg.startSymbol = S;

    
    cfg.rule_count = 3;
    cfg.rules[0].lhs = S;
    cfg.rules[0].rhs[0] = B;
    cfg.rules[0].rhs_length = 1;

    cfg.rules[1].lhs = B;
    cfg.rules[1].rhs[0] = T;
    cfg.rules[1].rhs_length = 1;

    cfg.rules[2].lhs = T;
    cfg.rules[2].rhs[0] = F;
    cfg.rules[2].rhs_length = 1;

    
    CFGSymbol derivation[20];
    int derivation_length = 0;
    printf("\n[Test] startDerivation:\n");
    startDerivation(derivation, &derivation_length, &cfg);
    printArraySymbols(derivation, derivation_length);

    
    printf("\n[Test] applyProductionRule: Rule 1 (S  B)\n");
    applyProductionRule(derivation, &derivation_length, &cfg, 1, 0);
    printArraySymbols(derivation, derivation_length);

    
    printf("\n[Test] applyProductionRule: Rule 2 (B  T)\n");
    applyProductionRule(derivation, &derivation_length, &cfg, 2, 0);
    printArraySymbols(derivation, derivation_length);

    
    printf("\n[Test] applyProductionRule: Rule 3 (T  F)\n");
    applyProductionRule(derivation, &derivation_length, &cfg, 3, 0);
    printArraySymbols(derivation, derivation_length);

    
    printf("\n[Test] checkDerivation\n");

    
    printf("[Test] Length mismatch (1 token vs 2 tokens)\n");
    CFGSymbol shortDerivation[1] = {F};
    int shortLen = 1;
    CFGSymbol longerTokens[2] = {F, F};
    int longerLen = 2;
    checkDerivation(shortDerivation, shortLen, longerTokens, longerLen);

    
    printf("[Test] Symbol mismatch at position 0\n");
    CFGSymbol wrongDerivation[1] = {T}; 
    int wrongLen = 1;
    CFGSymbol expectedToken[1] = {F};
    int expectedLen = 1;
    checkDerivation(wrongDerivation, wrongLen, expectedToken, expectedLen);

    
    printf("[Test] Exact match with tokens\n");
    CFGSymbol correctDerivation[3] = {TRUE, AND, FALSE};
    CFGSymbol matchingTokens[3] = {TRUE, AND, FALSE};
    int matchLen = 3;
    checkDerivation(correctDerivation, matchLen, matchingTokens, matchLen);

    return 0;
}
