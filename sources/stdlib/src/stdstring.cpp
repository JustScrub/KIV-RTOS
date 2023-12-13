#include <stdstring.h>

namespace
{
    const char CharConvArr[] = "0123456789ABCDEF";
}

int itoa(unsigned int input, char* output, unsigned int base, unsigned int n_of_digits)
{
	int i = 0;

	while (input > 0)
	{
		output[i] = CharConvArr[input % base];
		input /= base;
		i++;
	}

	while (i < n_of_digits) 
        output[i++] = CharConvArr[0]; 

	output[i] = '\0';
	i--;

	for (int j = 0; j <= i/2; j++)
	{
		char c = output[i - j];
		output[i - j] = output[j];
		output[j] = c;
	}
	return i;
}

int atoi(const char* input)
{
	int output = 0;

	while (*input != '\0')
	{
		output *= 10;
		if (*input > '9' || *input < '0')
			break;

		output += *input - '0';

		input++;
	}

	return output;
}

int __slow_pow(int base, int exp)
{
	int output = 1;

	for (int i = 0; i < exp; i++)
		output *= base;

	return output;
}

// Converts a floating-point/double number to a string. 
// https://www.geeksforgeeks.org/convert-floating-point-number-string/
int ftoa(float n, char* res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
 
    // Extract floating part 
    float fpart = n - (float)ipart; 
 
    // convert integer part to string 
    int i = itoa(ipart, res, 10); 
 
    // check for display option after point 
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 
 
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter 
        // is needed to handle cases like 233.007 
        fpart = fpart * (float)__slow_pow(10, afterpoint); 
 
        i += itoa((int)fpart, res + i + 1, 10, afterpoint); 
    } 

	return i;
} 

float atof(const char* input)
{
	float output = 0.0f;
	float divider = 0.0f;
	float sign = 1.0f;

	if (*input == '-')
	{
		sign = -1.0f;
		input++;
	}

	while (*input != '\0')
	{
		if (*input == '.')
		{
			divider = 1.0f;
			input++;
			continue;
		}

		output *= 10.0f;
		if (*input > '9' || *input < '0')
			break;

		output += *input - '0';

		if (divider > 0.0f)
			divider *= 10.0f;

		input++;
	}

	return sign * output / divider;
}

char* strncpy(char* dest, const char *src, int num)
{
	int i;

	for (i = 0; i < num && src[i] != '\0'; i++)
		dest[i] = src[i];
	for (; i < num; i++)
		dest[i] = '\0';

   return dest;
}

int strncmp(const char *s1, const char *s2, int num)
{
	unsigned char u1, u2;
  	while (num-- > 0)
    {
      	u1 = (unsigned char) *s1++;
     	u2 = (unsigned char) *s2++;
      	if (u1 != u2)
        	return u1 - u2;
      	if (u1 == '\0')
        	return 0;
    }

  	return 0;
}

int strlen(const char* s)
{
	int i = 0;

	while (s[i] != '\0')
		i++;

	return i;
}

void bzero(void* memory, int length)
{
	char* mem = reinterpret_cast<char*>(memory);

	for (int i = 0; i < length; i++)
		mem[i] = 0;
}

void memcpy(const void* src, void* dst, int num)
{
	const char* memsrc = reinterpret_cast<const char*>(src);
	char* memdst = reinterpret_cast<char*>(dst);

	for (int i = 0; i < num; i++)
		memdst[i] = memsrc[i];
}
