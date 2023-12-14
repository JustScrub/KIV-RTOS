#pragma once

int itoa(int input, char* output, unsigned int base, unsigned int n_of_digits=0);
int atoi(const char* input);
int ftoa(float n, char* res, int afterpoint=3);
float atof(const char* input);
/**
 * @brief returns whether the input is a valid input to atoi or atof
 * 
 * @param input the input string
 * @return true string is convertible
 * @return false string does not start with a number
 */
bool is_numeric(const char* input);
char* strncpy(char* dest, const char *src, int num);
int strncmp(const char *s1, const char *s2, int num);
int strlen(const char* s);
void bzero(void* memory, int length);
void memcpy(const void* src, void* dst, int num);
