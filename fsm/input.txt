#include <stdio.h>
#include <string.h>

#define MAX_RHS 10  // Maximum number of symbols on the RHS of a production rule
#define MAX_SYMBOLS 10  // Maximum number of symbols in the CFG
#define MAX_RULES 10  // Maximum number of rules in the CFG
#define MAX_TOKENS 20  // Maximum number of tokens in a tokenized string


typedef struct {
// Struct for CFG symbols.
// - symbol: Stores symbol as a string (e.g., "true", "false", "AND")
// - is_terminal: An int, which indicates whether the symbol is terminal (0 = false, 1 = true)
// - is_start: An int, which indicates whether the symbol is a start symbol (0 = false, 1 = true)
    char* symbol;
    int is_terminal;
    int is_start;
} CFGSymbol;

// Struct for production rules
typedef struct {
    // - lhs: Left-hand side of the production rule (always a non-terminal)
    // - rhs: Right-hand side of the production rule, with size MAX_RHS.
    // - rhs_length: Number of symbols on the RHS
    CFGSymbol lhs;
    CFGSymbol* rhs;
    int rhs_length;
} CFGProductionRule;

// Struct for CFG
// - symbols: Array of all CFG symbols,  with size MAX_SYMBOLS.
// - startSymbol: The start symbol of the CFG
// - rules: rray of production rules, with size MAX_RULES.
// - symbol_count: Number of symbols in the CFG
// - rule_count: Number of rules in the CFG
typedef struct {

    CFGSymbol* symbols;
    CFGSymbol startSymbol;
    CFGProductionRule* rules;
    int symbol_count;
    int rule_count;
} CFG;

// Function prototypes
void startDerivation(CFGSymbol* derivation, int* derivation_length, CFG* cfg);
void applyProductionRule(CFGSymbol* derivation, int* derivation_length, CFG* cfg, int ruleIndex, int position);
int checkDerivation(CFGSymbol* derivation, int derivation_length, CFGSymbol* tokens, int token_count);
void printArraySymbols(CFGSymbol* symbols, int count);

// Function to start derivation with the start symbol
void startDerivation(CFGSymbol* derivation, int* derivation_length, CFG* cfg) {
    ???
}

// Function to apply a production rule to a derivation step
void applyProductionRule(CFGSymbol* derivation, int* derivation_length, CFG* cfg, int ruleIndex, int position) {
    if (???) {
         // check the rule index
        printf("Invalid rule index.\n");
        return;
    }
    CFGProductionRule rule = cfg->rules[ruleIndex - 1];

    // Ensure the position is valid and matches the LHS of the production rule
    if (???) {
        printf("Rule cannot be applied at the given position.\n");
        return;
    }
    
    // Calculate new derivation length after applying the rule
    if (???) {
        printf("Applying the rule exceeds the maximum derivation length.\n");
        return;
    }

    // Shift symbols to accommodate the new RHS symbols
    for (???) {
        ???
    }
    
    // Insert RHS symbols into the derivation array
    for (???) {
        ???
    }
    
    *derivation_length = new_length;
}

// Function to check if derivation matches the expected token sequence
int checkDerivation(CFGSymbol* derivation, int derivation_length, CFGSymbol* tokens, int token_count) {
    if (???) { // check length matching
        printf("Derivation unsuccessful: Length mismatch.\n");
        return 0;
    }
    
    for (???) {
        if (???) { // check position matching
            printf("Derivation unsuccessful: Mismatch at position %d.\n", i);
            return 0;
        }
    }
    
    printf("Derivation successful!\n");
    return 1;
}

// Helper function for printing symbols
void printArraySymbols(CFGSymbol* symbols, int count) {
    for (int i = 0; i < count; i++) {
        printf("Token(%s) ", symbols[i].symbol);
    }
    printf("\n");
}

// Main function for testing derivation
int main() {
    printf("==== Test Manual Derivation Engine ====\n");

    // --- Step 1: Define Symbols ---
    CFG cfg;
    CFGSymbol symbols[MAX_SYMBOLS];
    int symbol_count = 0;

    // Manually define symbols
    CFGSymbol S = {"S", 0, 1};         // start symbol
    CFGSymbol B = {"B", 0, 0};
    CFGSymbol T = {"T", 0, 0};
    CFGSymbol F = {"F", 0, 0};
    CFGSymbol OR = {"OR", 1, 0};
    CFGSymbol AND = {"AND", 1, 0};
    CFGSymbol LP = {"(", 1, 0};
    CFGSymbol RP = {")", 1, 0};
    CFGSymbol TRUE = {"true", 1, 0};
    CFGSymbol FALSE = {"false", 1, 0};

    symbols[0] = S; symbols[1] = B; symbols[2] = T; symbols[3] = F;
    symbols[4] = OR; symbols[5] = AND; symbols[6] = LP; symbols[7] = RP;
    symbols[8] = TRUE; symbols[9] = FALSE;
    symbol_count = 10;

    cfg.symbol_count = symbol_count;
    memcpy(cfg.symbols, symbols, sizeof(symbols));
    cfg.startSymbol = S;

    // --- Step 2: Define Rules ---
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

    // --- Step 3: Test startDerivation ---
    CFGSymbol derivation[20];
    int derivation_length = 0;
    printf("\n[Test] startDerivation:\n");
    startDerivation(derivation, &derivation_length, &cfg);
    printArraySymbols(derivation, derivation_length);

    // --- Step 4: Apply Rule 1 (S → B) ---
    printf("\n[Test] applyProductionRule: Rule 1 (S → B)\n");
    applyProductionRule(derivation, &derivation_length, &cfg, 1, 0);
    printArraySymbols(derivation, derivation_length);

    // --- Step 5: Apply Rule 2 (B → T) ---
    printf("\n[Test] applyProductionRule: Rule 2 (B → T)\n");
    applyProductionRule(derivation, &derivation_length, &cfg, 2, 0);
    printArraySymbols(derivation, derivation_length);

    // --- Step 6: Apply Rule 3 (T → F) ---
    printf("\n[Test] applyProductionRule: Rule 3 (T → F)\n");
    applyProductionRule(derivation, &derivation_length, &cfg, 3, 0);
    printArraySymbols(derivation, derivation_length);

    // --- Step 7: Check Derivation Matches Token [F] ---
    printf("\n[Test] checkDerivation\n");
    
    // --- Test Case: Derivation length shorter than token count ---
    printf("[Test] Length mismatch (1 token vs 2 tokens)\n");
    CFGSymbol shortDerivation[1] = {F};
    int shortLen = 1;
    CFGSymbol longerTokens[2] = {F, F};
    int longerLen = 2;
    checkDerivation(shortDerivation, shortLen, longerTokens, longerLen);

    // --- Test Case: Symbol mismatch at position ---
    printf("[Test] Symbol mismatch at position 0\n");
    CFGSymbol wrongDerivation[1] = {T};  // Should be F
    int wrongLen = 1;
    CFGSymbol expectedToken[1] = {F};
    int expectedLen = 1;
    checkDerivation(wrongDerivation, wrongLen, expectedToken, expectedLen);

    // --- Test Case: Successful match ---
    printf("[Test] Exact match with tokens\n");
    CFGSymbol correctDerivation[3] = {TRUE, AND, FALSE};
    CFGSymbol matchingTokens[3] = {TRUE, AND, FALSE};
    int matchLen = 3;
    checkDerivation(correctDerivation, matchLen, matchingTokens, matchLen);

    return 0;
}
