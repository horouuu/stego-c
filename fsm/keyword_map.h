#ifndef KEYWORD_MAP_H
#define KEYWORD_MAP_H

int is_compressed_keyword(unsigned char byte, char *output_keyword);
int is_keyword(const char *token, unsigned char *out_byte);

#endif
