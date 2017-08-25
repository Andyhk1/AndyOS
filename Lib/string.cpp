#include "string.h"
#include "definitions.h"

unsigned int strcmp(const char* str1, const char* str2)
{
	int res = 0;
	while (!(res = *(unsigned char*)str1 - *(unsigned char*)str2) && *str2)
		++str1, ++str2;

	if (res < 0)
		res = -1;
	if (res > 0)
		res = 1;

	return res;
}

char* strcpy(char* dest, const char* src)
{
	char *s1_p = dest;
	while (*dest++ = *src++);
	return s1_p;
}

unsigned int strlen(const char* str)
{

	unsigned int len = 0;
	while (str[len++]);
	return len;
}

char* strcat(char* dest, const char* src)
{
	char *rdest = dest;

	while (*++dest);

	while (*dest++ = *src++);
	return rdest;
}

void* memcpy(void* dest, void* src, unsigned int n)
{
	char* a = (char*)src;
	char* b = (char*)dest;

	while (n--)
		*b++ = *a++;

	return dest;
}

void memcpy_fast_128(void* src, void* dest, unsigned int n)
{
	if (n % 128 != 0)
		return;

	__asm
	{
		mov esi, src;
		mov edi, dest;

		mov ebx, n;
		shr ebx, 7;

	loop_copy:
		movdqa xmm0, 0[ESI]
			movdqa xmm1, 16[ESI]
			movdqa xmm2, 32[ESI]
			movdqa xmm3, 48[ESI]
			movdqa xmm4, 64[ESI]
			movdqa xmm5, 80[ESI]
			movdqa xmm6, 96[ESI]
			movdqa xmm7, 112[ESI]

			movntdq 0[EDI], xmm0
			movntdq 16[EDI], xmm1
			movntdq 32[EDI], xmm2
			movntdq 48[EDI], xmm3
			movntdq 64[EDI], xmm4
			movntdq 80[EDI], xmm5
			movntdq 96[EDI], xmm6
			movntdq 112[EDI], xmm7

			add esi, 128;
		add edi, 128;
		dec ebx;

		jnz loop_copy;
	}
}

#pragma function(memset)
void* __cdecl memset(void* dest, int val, int n)
{
	unsigned char *temp = (unsigned char *)dest;
	for (; n != 0; n--, temp[n] = val);
	return dest;
}

void* memset16(void *dest, unsigned short val, unsigned int n)
{
	unsigned short *temp = (unsigned short *)dest;
	for (; n != 0; n--, temp[n] = val);
	return dest;
}

void* memset32(void *dest, unsigned int val, unsigned int n)
{
	unsigned int *temp = (unsigned int *)dest;
	for (; n != 0; n--, temp[n] = val);
	return dest;
}

void* memset_fast_128(void* dest, unsigned int val, unsigned int n)
{
	if (n % 128 != 0)
		return 0;

	_asm
	{
		mov edi, dest;

		mov ebx, n;
		shr ebx, 7;

		movsd xmm0, [val]
			movsd xmm1, [val]
			movsd xmm2, [val]
			movsd xmm3, [val]
			movsd xmm4, [val]
			movsd xmm5, [val]
			movsd xmm6, [val]
			movsd xmm7, [val]

			loop_copy:
		movntdq 0[EDI], xmm0
			movntdq 16[EDI], xmm1
			movntdq 32[EDI], xmm2
			movntdq 48[EDI], xmm3
			movntdq 64[EDI], xmm4
			movntdq 80[EDI], xmm5
			movntdq 96[EDI], xmm6
			movntdq 112[EDI], xmm7

			add edi, 128;
		dec ebx;

		jnz loop_copy;
	}

	return (unsigned char*)dest + n;
}

int memcmp(const void* a, const void* b, unsigned int n)
{
	const unsigned char* cs = (const unsigned char*)a;
	const unsigned char* ct = (const unsigned char*)b;

	for (int i = 0; i < n; i++, cs++, ct++)
	{
		if (*cs < *ct)
			return -1;
		else if (*cs > *ct)
			return 1;
	}
	return 0;
}

//void* memset(void* dest, int a, int b)
//{
//	//return nullptr;
//	return memset(dest, a, b);
//}
