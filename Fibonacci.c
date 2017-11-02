// James Upchurch
// ja389873
// COP 3502C
// Set of functions to calculate Fibonacci values greater than the limitations of data types

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include "Fibonacci.h"

//-------------------------------------------------

// Allocates memory for HugeInteger
HugeInteger *createHuge (int length)
{
	HugeInteger *huge;
	
	huge = malloc(sizeof(HugeInteger));
	if (huge == NULL)
		return NULL;

	huge->length = length;
	
	huge->digits = malloc(sizeof(int) * length);
	if (huge->digits == NULL)
	{
		free(huge);		
		return NULL;
	}

	return huge;
}

//------------------------------------------------

// Raises integer a to power of b

int raiseTo (int a, int b)
{
	int i, sum = 1;
	if (a == 0)
		return 0;

	if (b == 0)
		return 1;
	
	for(i = 0; i < b; i++)
		sum *= a;

	return sum;
}

//------------------------------------------------

// Moves all values in HugeInteger a to HugeInteger b
HugeInteger *reassign (HugeInteger *a)
{
	int i;
	HugeInteger *b;
	
	if (a == NULL)
		return NULL;

	if (a->digits == NULL)
	{
		free(a);
		return NULL;
	}

	b = createHuge(a->length);

	if (b == NULL)
		return NULL;

	if (b->digits == NULL)
	{
		free(b);
		return NULL;
	}

	for (i = 0; i < a->length; i++)
		b->digits[i] = a->digits[i];

	return b;
}

//------------------------------------------------

// Adds two HugeInteger numbers together
HugeInteger *hugeAdd (HugeInteger *p, HugeInteger *q)
{
	HugeInteger *sum, *tempHuge;
	int *tempSum, i, lLen, gLen, temp, temp1 = 0, temp2;

	if (p == NULL || q == NULL)
		return NULL;

	if (p->digits == NULL)
	{
		free(p);
		return NULL;
	}

	if (q->digits == NULL)
	{
		free(q);
		return NULL;
	}
	
	if (p->length <= q->length)
	{
		lLen = p->length;
		gLen = q->length;
	}
	else
	{
		lLen = q->length;
		gLen = p->length;
	}

	tempSum = malloc(sizeof(int) * (gLen + 1));

	if (tempSum == NULL)
		return NULL;

	// Adds digits of two HugeIntegers until reaching the end
	// of the shorter number
	for (i = 0; i < lLen; i++)
	{
		temp = p->digits[i] + q->digits[i];
		temp2 = temp + temp1;
		tempSum[i] = (temp2) % 10;
		temp1 = 0;
		
		if (temp2 >= 10)
			temp1++;
	}
	
	
	// Adds the remaining digits of the longer HugeInteger
	while (i < gLen)
	{
		if (p->length < q->length)
		{
			tempHuge = p;
			p = q;
			q = tempHuge;
		}
		
		tempSum[i] = (temp1 + p->digits[i]) % 10;
		
		if (temp1 + p->digits[i] >= 10)
			temp1 = 1;
		else
			temp1 = 0;
		i++;
	}
	
	// Increments last digit of the sum if the sum requires another digit
	if (temp1 == 1 && i == gLen)
	{
		tempSum[gLen] = temp1;
		
		sum = createHuge(gLen + 1);
		
		if (sum == NULL)
			return NULL;
			
		for (i = 0; i < sum->length; i++)
			sum->digits[i] = tempSum[i];

	} else if (temp1 == 0 && i == gLen)
	{
		sum = createHuge(gLen);

		if (sum == NULL)
			return NULL;
			
		for (i = 0; i < gLen; i++)
			sum->digits[i] = tempSum[i];
	}

	free(tempSum);

	return sum;
}

//-------------------------------------------------

// Frees up memory used by HugeInteger
HugeInteger *hugeDestroyer (HugeInteger *p)
{
	if (p == NULL)
		return NULL;

	if (p->digits == NULL)
	{
		free(p);
		return NULL;
	}

	free(p->digits);
	free(p);
	
	return NULL;
}

//-------------------------------------------------

// Converts string representation of a number into HugeInteger
HugeInteger *parseString (char *str)
{
	char tempChar;	
	int temp, len, i;
	HugeInteger *huge;
	
	if (str == NULL)
		return NULL;

	len = strlen(str);
	
	huge = createHuge(len);

	if (huge == NULL)
		return NULL;

	if (huge->digits == NULL)
	{
		free(huge);
		return NULL;
	}

	// Converts ASCII value of string elements into meaningful integer values
	for (i = 0; i < len; i++)
	{
		tempChar = str[len - (i + 1)];
		temp = atoi(&tempChar);

		huge->digits[i] = temp;
	}
	
	return huge;
}

//-------------------------------------------------

// Puts unsigned integer in HugeInteger format
HugeInteger *parseInt (unsigned int n)
{
	int i, len = 0;
	unsigned int temp;
	HugeInteger *huge;

	temp = n;

	if (temp == 0)
		len = 1;
	
	// Determines number of digits in the number
	while (temp != 0)
	{
		temp = temp / 10;
		len++;
	}

	huge = createHuge(len);

	if (huge == NULL)
		return NULL;

	if (huge->digits == NULL)
	{
		free(huge);
		return NULL;
	}

	// Finds individual digit and places the digit in HugeInteger array
	for (i = 0; i < len; i++)
	{
		temp = (n / raiseTo(10, len - (i + 1))) % 10;
		huge->digits[len - (i + 1)] = temp;
	}
	
	return huge;
}

//-------------------------------------------------

// Converts HugeInteger numbers to unsigned integers
unsigned int *toUnsignedInt (HugeInteger *p)
{
	unsigned int *n, temp, sum = 0;
	HugeInteger *uintMax;
	int i;

	// Creates HugeInteger version of the max unsigned integer value
	uintMax = parseInt(UINT_MAX);

	if (uintMax == NULL)
		return NULL;

	if (uintMax->digits == NULL)
		return NULL;

	n = malloc(sizeof(unsigned int));

	if (n == NULL)
		return NULL;
	
	if (p == NULL)
	{
		hugeDestroyer(uintMax);
		free(n);
		return NULL;
	}
	
	if (p->digits == NULL)
	{
		hugeDestroyer(uintMax);
		free(n);
		free(p);
		return NULL;
	}

	// Reeturns NULL if HugeInteger is larger than UINT_MAX
	if (p->length == uintMax->length)
	{
		for (i = 0; i < p->length; i++)
		{
			if (p->digits[i] != uintMax->digits[i])
				continue;
			else
				if(p->digits[i] > uintMax->digits[i])
				{
					hugeDestroyer(uintMax);
					free(n);
					return NULL;
				}
		}
	}
	else if (p->length > uintMax->length)
	{
		hugeDestroyer(uintMax);
		free(n);
		return NULL;
	}
	else
		;
	
	// Converts HugeInteger to unsigned integer
	for (i = p->length - 1; i >= 0; i--)
	{
		temp = (unsigned int) p->digits[i];
		sum = sum + (temp * raiseTo(10, i));
	}

	hugeDestroyer(uintMax);
	
	*n = sum;

	return n;
}

//-------------------------------------------------

// Calculates integer n Fibonacci numbers
HugeInteger *fib (int n)
{
	HugeInteger *a, *b, *sum;
	int i, k;

	a = parseInt(0);
	b = parseInt(1);

	if (n == 0)
	{
		hugeDestroyer(b);
		return a;
	}

	if (n == 1)
	{
		hugeDestroyer(a);
		return b;
	}

	if (n == 2)
	{
		hugeDestroyer(a);
		hugeDestroyer(b);
		sum = parseInt(1);
		return sum;
	}

	if (a == NULL || b == NULL)
		return NULL;

	// Calculates Fibonacci numbers
	for (i = 1; i < n; i++)
	{
		sum = hugeAdd(a, b);

		a = hugeDestroyer(a);
		a = reassign(b);

		b = hugeDestroyer(b);
		b = reassign(sum);

		sum = hugeDestroyer(sum);
	}

	a = hugeDestroyer(a);

	return b;
}

//-------------------------------------------------

double difficultyRating(void)
{
	double difficulty = 3.5;
	return difficulty;
}

//-------------------------------------------------

double hoursSpent(void)
{
	double hours = 25;
	return hours;
}