#include <stdstring.h>

namespace
{
    const char CharConvArr[] = "0123456789ABCDEF";
}

int itoa(int input, char* output, unsigned int base, unsigned int n_of_digits)
{
	int i = 0;
	if (input == 0)
	{
		while (i < n_of_digits + !n_of_digits) 
			output[i++] = CharConvArr[0]; 
		return i;
	}

	if (input < 0)
	{
		output[i++] = '-';
		input = -input;
	}

	while (input > 0)
	{
		output[i] = CharConvArr[input % base];
		input /= base;
		i++;
	}

	while (i < n_of_digits + (output[0] == '-' ? 1 : 0)) 
        output[i++] = CharConvArr[0]; 

	output[i] = '\0';
	i--;
	int j = output[0] == '-' ? 1 : 0;

	for (; j <= i/2; j++)
	{
		char c = output[i - j];
		output[i - j] = output[j];
		output[j] = c;
	}
	return i+1;
}

int atoi(const char* input)
{
	int output = 0;
	int sign = 1;

	if(*input == '-')
	{
		sign = -1;
		input++;
	}

	while (*input != '\0')
	{
		output *= 10;
		if (*input > '9' || *input < '0')
			break;

		output += *input - '0';

		input++;
	}

	return sign * output;
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
	int i = 0;
	if(n < 0)
	{
		res[0] = '-'; // add minus sign
		n = -n;
		i++;
	}

    // Extract integer part 
    int ipart = (int)n; 
 
    // Extract floating part 
    float fpart = n - (float)ipart; 
 
    // convert integer part to string 
    i += itoa(ipart, res+i, 10); 
 
    // check for display option after point 
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 
 
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter 
        // is needed to handle cases like 233.007 
        fpart = fpart * (float)__slow_pow(10, afterpoint); 
 
        i += itoa((int)fpart, res + i + 1, 10, afterpoint) + 1; 
    } 

	return i;
} 

float atof(const char* input)
{
	float output = 0.0f;
	int divider = 0;
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
			divider = 1;
			input++;
			continue;
		}

		output *= 10.0f;
		if (*input > '9' || *input < '0')
			break;

		output += *input - '0';

		if (divider)
			divider *= 10;

		input++;
	}

	
	return sign * output / (divider? (float)divider : 1.0f);
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
