
#include <stdio.h>

#define MAX_RHS 10

#define MAX_SYMBOLS 10

#define MAX_RULES 10






typedef struct {

    char* symbol;
    int is_terminal;
    int is_start;
} CFGSymbol;







typedef struct {

    CFGSymbol lhs;
    CFGSymbol* rhs;
    int rhs_length;
} CFGProductionRule;



void init_CFGSymbol(CFGSymbol* symbol, char* text, int is_terminal, int is_start) {

    symbol->symbol = text;
    symbol->is_terminal = is_terminal;
    symbol->is_start = is_start;
}



void init_NonTerminal(CFGSymbol* symbol, char* text) {

    symbol->symbol = text;
    symbol->is_terminal = 0;
    symbol->is_start = 0;
}



void init_Terminal(CFGSymbol* symbol, char* text) {

    symbol->symbol = text;
    symbol->is_terminal = 1;
    symbol->is_start = 0;
}



void init_StartSymbol(CFGSymbol* symbol, char* text) {


    symbol->symbol = text;
    symbol->is_terminal = 0;
    symbol->is_start = 1;
}







CFGProductionRule createProductionRule(CFGSymbol lhs, CFGSymbol rhs[], int rhs_length) {
    CFGProductionRule rule;
    int i;
    
	
    
    rule.lhs = lhs;
	if(lhs.is_terminal){
        printf("Error: lhs should be non-terminal");
        rule.rhs_length = -1;
        return rule;
    }
    
	
	
	
    for(int i = 0 ; i < rhs_length  ; i ++){
        printf("rhs symbol: %s\n", rhs[i].symbol);
    }
    rule.rhs = rhs;
    rule.rhs_length = rhs_length;
    return rule;
}






void printProductionRule(CFGProductionRule rule) {
    int i;
    
    
    printf("%s", rule.lhs.symbol);
	printf(" --> ");
	
	
    
    for(int i = 0 ; i < rule.rhs_length ; i ++){
        printf("%s ", rule.rhs[i].symbol);
    }
    printf("\n");
	return;
}







typedef struct {

    CFGSymbol* symbols;
    CFGSymbol startSymbol;
    CFGProductionRule* rules;
    int symbol_count;
    int rule_count;
} CFG;






void init_CFG(CFG* cfg, CFGSymbol symbols[], int symbol_count, CFGSymbol startSymbol, CFGProductionRule rules[], int rule_count) {
    
    cfg->symbols = symbols;
    cfg->symbol_count = symbol_count;
    cfg->startSymbol = startSymbol;
    cfg->rules = rules;
    cfg->rule_count = rule_count;
}




void printCFG(const CFG cfg) {
    int i;
    
    for (i = 0; i < cfg.rule_count; i++) {
        
        
        printf("(%d):   ", i);
        printProductionRule(cfg.rules[i]);
    }
}



int main(void) {
    CFGSymbol S, B, T, F, AND, OR, LPAREN, RPAREN, TRUE, FALSE;
    CFGSymbol rhs1[1], rhs2[3], rhs3[1], rhs4[3], rhs5[1], rhs6[3], rhs7[1], rhs8[1];
    CFGProductionRule rule1, rule2, rule3, rule4, rule5, rule6, rule7, rule8;
    CFG cfg;
    CFGSymbol symbols[10];
    CFGProductionRule rules[8];

    
    init_StartSymbol(&S, "S");
    init_NonTerminal(&B, "B");
    init_NonTerminal(&T, "T");
    init_NonTerminal(&F, "F");
    init_Terminal(&AND, "AND");
    init_Terminal(&OR, "OR");
    init_Terminal(&LPAREN, "(");
    init_Terminal(&RPAREN, ")");
    init_Terminal(&TRUE, "true");
    init_Terminal(&FALSE, "false");

    
    rhs1[0] = B;
    rule1 = createProductionRule(S, rhs1, 1);

    rhs2[0] = B; rhs2[1] = OR; rhs2[2] = T;
    rule2 = createProductionRule(B, rhs2, 3);

    rhs3[0] = T;
    rule3 = createProductionRule(B, rhs3, 1);
	
	rhs4[0] = T; rhs4[1] = AND; rhs4[2] = F;
    rule4 = createProductionRule(T, rhs4, 3);
	
	rhs5[0] = F;
    rule5 = createProductionRule(T, rhs5, 1);
	
	rhs6[0] = LPAREN; rhs6[1] = B; rhs6[2] = RPAREN;
    rule6 = createProductionRule(F, rhs6, 3);

    rhs7[0] = TRUE;
    rule7 = createProductionRule(F, rhs7, 1);

    rhs8[0] = FALSE;
    rule8 = createProductionRule(F, rhs8, 1);

    
    symbols[0] = S;
    symbols[1] = B;
    symbols[2] = T;
    symbols[3] = F;
    symbols[4] = AND;
    symbols[5] = OR;
    symbols[6] = LPAREN;
    symbols[7] = RPAREN;
    symbols[8] = TRUE;
    symbols[9] = FALSE;

    rules[0] = rule1;
    rules[1] = rule2;
    rules[2] = rule3;
    rules[3] = rule4;
    rules[4] = rule5;
	rules[5] = rule6;
	rules[6] = rule7;
	rules[7] = rule8;

    int symbol_count = sizeof(symbols) / sizeof(symbols[0]);
    int rule_count = sizeof(rules) / sizeof(rules[0]);

    
    init_CFG(&cfg, symbols, symbol_count, S, rules, rule_count);
    printCFG(cfg);

    return 0;
}
