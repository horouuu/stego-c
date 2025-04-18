#include <string.h>

typedef struct
{
    const char *keyword;
    unsigned char byte;
} KeywordEntry;

static KeywordEntry keyword_table[] = {
    {"auto", 0x80},
    {"break", 0x81},
    {"case", 0x82},
    {"char", 0x83},
    {"const", 0x84},
    {"continue", 0x85},
    {"default", 0x86},
    {"do", 0x87},
    {"double", 0x88},
    {"else", 0x89},
    {"enum", 0x8A},
    {"extern", 0x8B},
    {"float", 0x8C},
    {"for", 0x8D},
    {"goto", 0x8E},
    {"if", 0x8F},
    {"int", 0x90},
    {"long", 0x91},
    {"register", 0x92},
    {"return", 0x93},
    {"short", 0x94},
    {"signed", 0x95},
    {"sizeof", 0x96},
    {"static", 0x97},
    {"struct", 0x98},
    {"switch", 0x99},
    {"typedef", 0x9A},
    {"union", 0x9B},
    {"unsigned", 0x9C},
    {"void", 0x9D},
    {"volatile", 0x9E},
    {"while", 0x9F},
};

#define NUM_KEYWORDS (sizeof(keyword_table) / sizeof(KeywordEntry))

int is_keyword(const char *token, unsigned char *output_byte)
{
    int i;
    for (i = 0; i < NUM_KEYWORDS; i++)
    {
        if (strcmp(token, keyword_table[i].keyword) == 0)
        {
            *output_byte = keyword_table[i].byte;
            return 1;
        }
    }
    return 0;
}
