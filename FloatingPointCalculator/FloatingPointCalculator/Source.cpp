//Tracy Albers
//CS222

//10/14 fix outputs to show all desired values in special cases 0, +/- 0, +/- infinity

#include <stdio.h>
#include <math.h>
#pragma warning(disable : 4996) //using scanf per project requirements, disable warning

/*************************/
#define POSINF 0x7f800000
#define SIGNBIT 0x80000000
#define MANTISSABITS 0x7FFFFF

void DECtoIEEE()
{
	/* declare local variables */
	int exponent = 0;
	float decnum = 0.0;
	int ieee = 0;
	int neg = 0;
	int mantissa = 0;

	printf("Enter the decimal representation: ");	/* prompt for floating point decimal number */
	scanf("%f", &decnum);

	if (decnum == 0.0)	/* Check for 0--if so, print result */
	{
		printf("\nSign: 0\nExponent: 00000000\nMantissa: 00000000000000000000000\nIEEE: 0x00000000\n");
		return;
	}

	if (decnum > 0)	/* Print sign: if number>0, sign is 0, else 1 */
	{
		printf("\nSign: 0\n");
	}
	else
	{
		printf("\nSign: 1\n");
		decnum = -1 * decnum;	/* take absolute value of number before generating significand */
		neg = 1;
	}
	/* Normalize number:
	while number >2, divide by 2, increment exponent
	while number <1, multiply by 2, decrement exponent
	*/
	float tempnum = decnum;
	while (tempnum >= 2)
	{
		tempnum = tempnum / 2;
		exponent++;
	}
	while (tempnum < 1)
	{
		tempnum = tempnum * 2;
		exponent--;
	}
	exponent += 127;	/* Bias exponent by 127 and print each bit in binary with 8-iteration for-loop*/
	printf("Biased exponent: ");
	for (int i = 0; i < 8; i++)
	{
		if (0x80 & (exponent << i))
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
	}
	printf("\n");
	tempnum -= 1;	/* Hide 1 and print significand in binary with 23-iteration for-loop*/
	printf("Mantissa: ");
	for (int i = 0; i < 23; i++)
	{
		if (tempnum >= 0.5)
		{
			printf("1");
			tempnum -= 0.5;
			mantissa |= (1 << (22 - i));
		}
		else
			printf("0");
		tempnum *= 2;
	}
	printf("\nIEEE-754: 0x");
	if (neg == 1)
	{
		ieee |= SIGNBIT;
		ieee |= mantissa;
		ieee |= (exponent << (23));
	}
	else
	{
		ieee = mantissa;
		ieee += (exponent * (1 << 23));
	}
	printf("%x\n", ieee);

	/* Print IEEE-754 representation */
	return;
}
/***********************************************************************/
void IEEEtoDEC()
{
	/* declare local variables */
	int ieee = 0;
	float mantissa = 0;
	int exponent = 0;
	int sign = 1;
	float decimal = 0;

	/* prompt for IEEE-754 representation */
	printf("Enter the IEEE-754 number to be converted: ");
	scanf("%x", &ieee);
	printf("\n");
	/* check for special cases: 0, -0, +infinity, -infinity, NaN, if so, print and return */
	if ((~SIGNBIT & ieee) > POSINF)
	{
		printf("NaN\n");
		return;
	}
	else if ((~SIGNBIT & ieee) == POSINF)
	{
		if ((ieee) == POSINF) {
			printf("\nSign: +\nBiased Exponent: 255\nMantissa: 0\nDecimal Representation: + infinity\n");
			return;
		}
		else {
			printf("\nSign: -\nBiased Exponent: 255\nMantissa: 0\nDecimal Representation: - infinity\n");
			return;
		}
	}
	if ((~SIGNBIT & ieee) == 0)
	{
		if (ieee == 0) {
			printf("Sign: +\nBiased Exponent: 0\nMantissa: 0\nDecimal Representation: +0\n");
			return;
		}
		else {
			printf("Sign: -\nBiased Exponent: 0\nMantissa: 0\nDecimal Representation: -0\n");
			return;
		}
	}
	/* Mask sign from number: if sign=0, print "+", else print "-" */
	if ((SIGNBIT & ieee) != 0)
	{
		printf("Sign: -\n");
		sign = -1;
	}
	else
		printf("Sign +\n");

	mantissa = (ieee & MANTISSABITS) / (float)pow(2, 23);	/* Mask biased exponent and significand from number */
	exponent = ((ieee & POSINF) >> 23);

	/* If biased exponent=0, number is denormalized with unbiased exponent of -126,
	print denormalized number as fraction * 2^(-126), return */
	if (exponent == 0)
	{
		printf("Unbiased Exponent: -126\n");
		printf("Denormalized Special Case: %f * 2^-126\n", mantissa);
		return;
	}
	exponent -= 127;	/* Unbias exponent by subtracting 127 and print */
	printf("Unbiased Exponent: %d\n", exponent);
	printf("Decimal: %f\n", (mantissa + 1));/* Add hidden 1 and print normalized decimal number */
	/* Print decimal number */
	decimal = (sign*(mantissa + 1)*(float)pow(2, exponent));
	printf("Decimal Representation: %f\n", decimal);
	return;
}
int main()
{
	/* declare local variables */
	int choice = 1;
	while (choice != 3)
	{
		printf("Floating-point Conversion:\n---------------------\n1) Decimal to IEEE-754 conversion\n2) IEEE-754 to Decimal Conversion\n3) Exit\n");
		scanf("%d", &choice);
		switch (choice)
		{
		case (1):
		{
			DECtoIEEE();
			break;
		}
		case (2):
		{
			IEEEtoDEC();
			break;
		}
		case (3):
		{
			return 0;
			break;
		}
		default:
		{
			printf("Invalid entry.  Enter a choice between 1 and 3.\n");
		}
		/* until user chooses to quit, prompt for choice and select appropriate
		function */
		}
	}
	return 0;
}
//test input: 1 2.5 2 40200000 1 0 2 80400000 2 ffffffff 3
/*output
Sign +
Unbiased Exponent : 1
Decimal : 1.250000
Decimal Representation : 2.500000

Sign: 0
Exponent = 0
Significand = 0
IEEE : 0x00000000

Sign +
Unbiased Exponent : -126
Denormalized Special Case : 0.500000 * 2 ^ -126

 NaN
*/

//2nd test input: 1 -99.38671875 1 12.75 1 0 2 80000000 2 00000000 2 ff800000 2 7f800000 2 ffffffff 2 C2C6C600 2 80400000 2 414c0000 3

/*
output
Sign: 1
Exponent 10000101
Mantissa: 1000110 11000110 00000000
IEEE: 0xC2C6C600

Sign: 0
Exponent 10000010
Mantissa: 1001100 00000000 00000000
IEEE: 0x414C0000

Sign: 0
Exponent 00000000
Mantissa 0000000 00000000 00000000
IEEE: 0x00000000

Sign: -
Unbiased Exponent: 0
Mantissa: 0
Decimal Representation: -0

Sign: +
Unbiased Exponent: 0
Mantissa: 0
Decimal Representation: +0

Sign: -
Unbiased Exponent: 255
Mantissa: 0
Decimal Represntation: - infininity

Sign: +
Unbiased Exponent: 255
Mantissa: 0
Decimal Represntation: + infininity

NaN

Sign: -
Unbiased Exponent: 6
Mantissa: 1.552917
Decimal Representation: -99.38671875

Sign +
Unbiased Exponent : -126
Denormalized Special Case : 0.500000 * 2 ^ -126

Sign +
Unbiased Exponent:3
Mantissa: 1.593750
Decimal Representation: 12.75

*/

