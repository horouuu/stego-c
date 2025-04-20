// Only using stdio, no string library needed for this.
#include <stdio.h>
// Define the maximum number of symbols on the RHS of a rule a 10 (should be enough).
#define MAX_RHS 10
// Define the maximum number of symbols in a CFG as 10 (should be enough).
#define MAX_SYMBOLS 10
// Define the maximum number of rules in a CFG as 10 (should be enough).
#define MAX_RULES 10


// Struct for CFG symbols.
// - symbol: A single character containing the representation of the CFG symbol.
// - is_terminal: An int, which indicates whether the symbol is terminal (0 = false, 1 = true)
// - is_start: An int, which indicates whether the symbol is a start symbol (0 = false, 1 = true)
typedef struct {
// ???
    char* symbol;
    int is_terminal;
    int is_start;
} CFGSymbol;


// Struct for a production rule of our CFG.
// - lhs: The left-hand side of the production rule, which consists of a single non-terminal CFGSymbol.
// - rhs: The right-hand side of the production rule, which consists of a sequence of CFGSymbols,
// defined as an array CFGSymbol elements, with size MAX_RHS.
// - rhs_length: An int denoting the number of symbols in the right-hand side array.
typedef struct {
// ???
    CFGSymbol lhs;
    CFGSymbol* rhs;
    int rhs_length;
} CFGProductionRule;


// Generic function to initialize a CFGSymbol.
void init_CFGSymbol(CFGSymbol* symbol, char* text, int is_terminal, int is_start) {
// ???
    symbol->symbol = text;
    symbol->is_terminal = is_terminal;
    symbol->is_start = is_start;
}


// Specific initializers for different types of symbols (non-terminal symbol).
void init_NonTerminal(CFGSymbol* symbol, char* text) {
// ???
    symbol->symbol = text;
    symbol->is_terminal = 0;
    symbol->is_start = 0;
}


// Specific initializers for different types of symbols (terminal symbol).
void init_Terminal(CFGSymbol* symbol, char* text) {
// ???
    symbol->symbol = text;
    symbol->is_terminal = 1;
    symbol->is_start = 0;
}


// Specific initializers for different types of symbols (start symbol).
void init_StartSymbol(CFGSymbol* symbol, char* text) {
// ???

    symbol->symbol = text;
    symbol->is_terminal = 0;
    symbol->is_start = 1;
}


// Function to create a production rule.
// - It should check if lhs is a non-terminal symbol.
// It will display an error message and set the rhs_length attribute to -1 otherwise.
// - It will then assign the sequence of symbols in CFGSymbol rhs[]
// to the production rule rhs attribute, and in the process, define the number of elements rhs_length.
CFGProductionRule createProductionRule(CFGSymbol lhs, CFGSymbol rhs[], int rhs_length) {
    CFGProductionRule rule;
    int i;
    
	// Check that lhs is not a terminal symbol (otherwise, problem) 
    // ???
    rule.lhs = lhs;
	if(lhs.is_terminal){
        printf("Error: lhs should be non-terminal");
        rule.rhs_length = -1;
        return rule;
    }
    // Iterate through the right-hand side symbols and print them.
	// Initialize rhs_length to 0.
	// Later, count the actual length of rhs until the '\0' symbol is found.
	// ???
    for(int i = 0 ; i < rhs_length  ; i ++){
        printf("rhs symbol: %s\n", rhs[i].symbol);
    }
    rule.rhs = rhs;
    rule.rhs_length = rhs_length;
    return rule;
}


// Function to print a production rule.
// - Can safely assumes that we will have a valid production rule already.
// - Print the left-hand side symbol, then print " --> ",
// and finally iterate through the right-hand side symbols and print them.
void printProductionRule(CFGProductionRule rule) {
    int i;
    
    // printf("%c", ???);
    printf("%s", rule.lhs.symbol);
	printf(" --> ");
	// for (???) {
	// 	printf(???);
    // }
    for(int i = 0 ; i < rule.rhs_length ; i ++){
        printf("%s ", rule.rhs[i].symbol);
    }
    printf("\n");
	return;
}

// Struct for the full CFG.
// - symbols: An array of possible symbols, defined as an array of CFGSymbol, with size MAX_SYMBOLS.
// - startSymbol: The start symbol of the CFG, as a single CFGSymbol.
// - rules: An array of possible production rules, defined as an array of CFGProductionRule, with size MAX_RULES.
// - symbol_count: An int, denoting the number of CFGSymbol elements in the symbols array.
// - rule_count: An int, denoting the number of CFGProductionRule elements in the rules array.
typedef struct {
// ???
    CFGSymbol* symbols;
    CFGSymbol startSymbol;
    CFGProductionRule* rules;
    int symbol_count;
    int rule_count;
} CFG;



// Function to create a CFG.
// - Receives a CFG struct, an array of symbols, an array of production rules and counters for the lengths of these arrays.
// - Should simply assign each of these arrays and int values to the appropriate attributes of the CFG struct.
void init_CFG(CFG* cfg, CFGSymbol symbols[], int symbol_count, CFGSymbol startSymbol, CFGProductionRule rules[], int rule_count) {
    // ???
    cfg->symbols = symbols;
    cfg->symbol_count = symbol_count;
    cfg->startSymbol = startSymbol;
    cfg->rules = rules;
    cfg->rule_count = rule_count;
}


// Function for printing the CFG as expected.
// - Should display all the production rules in the format "(k) lhs --> rhs".
void printCFG(const CFG cfg) {
    int i;
    
    for (i = 0; i < cfg.rule_count; i++) {
        // printf("(%d):   ", ???);
        // printProductionRule(???);
        printf("(%d):   ", i);
        printProductionRule(cfg.rules[i]);
    }
}


// Main Function
int main(void) {
    CFGSymbol S, B, T, F, AND, OR, LPAREN, RPAREN, TRUE, FALSE;
    CFGSymbol rhs1[1], rhs2[3], rhs3[1], rhs4[3], rhs5[1], rhs6[3], rhs7[1], rhs8[1];
    CFGProductionRule rule1, rule2, rule3, rule4, rule5, rule6, rule7, rule8;
    CFG cfg;
    CFGSymbol symbols[10];
    CFGProductionRule rules[8];

    // Initialize CFG symbols
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

    // Define production rules
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

    // Load symbols and rules into CFG
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

    // Initialize and print the CFG
    init_CFG(&cfg, symbols, symbol_count, S, rules, rule_count);
    printCFG(cfg);

    return 0;
}
