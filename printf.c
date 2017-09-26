#if 0
set -e
cd "$(dirname "$0")"
gcc -o printf -O2 -Wall -Wextra "$0" -DTEST_PRINTF
exec ./printf
exit 1
#endif
#include <stdarg.h>
#include <stdbool.h>

extern int putchar(int c);

int puts(const char *str)
{
	const char *it;
	for (it = str; *it; it++) {
		putchar(*it);
	}
	return it - str;
}

int put_c(char c)
{
	putchar(c);
	return 1;
}

int put_s(int width, const char *s)
{
	int len = puts(s);
	while (len++ < width) {
		putchar(' ');
	}
	return len;
}

int put_digit(unsigned digit)
{
	if (digit < 10) {
		putchar('0' + digit);
	} else {
		putchar('a' + digit - 10);
	}
	return 1;
}

int put_d(int width, int base, char pad, int value)
{
	bool neg = value < 0;
	if (neg) {
		value = -value;
	}
	char buf[11];
	int len = 0;
	do {
		buf[len++] = value % base;
		value /= base;
	} while (value);
	int total = 0;
	for (int i = len; i < width; i++, total++) {
		putchar(pad);
	}
	if (neg) {
		putchar('-');
		total++;
	}
	do {
		put_digit(buf[--len]);
		total++;
	} while (len);
	return total;
}

int put_u(int width, int base, char pad, unsigned value)
{
	char buf[11];
	int len = 0;
	do {
		buf[len++] = value % base;
		value /= base;
	} while (value);
	int total = 0;
	for (int i = len; i < width; i++, total++) {
		putchar(pad);
	}
	do {
		put_digit(buf[--len]);
		total++;
	} while (len);
	return total;
}

int isdigit(int c)
{
	return c >= '0' && c <= '9';
}

int printf(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	char c;
	int total = 0;
	while ((c = *format++)) {
		if (c != '%') {
			total += put_c(c);
			continue;
		}
		if ((c = *format) == '%') {
			format++;
			total += put_c(c);
			continue;
		}
		int width = 0;
		while (isdigit((c = *format++))) {
			width *= 10;
			width += c - '0';
		}
		switch (c) {
		case 'c': total += put_c(va_arg(ap, int)); break;
		case 's': total += put_s(width, va_arg(ap, const char *)); break;
		case 'd': total += put_d(width, 10, ' ', va_arg(ap, int)); break;
		case 'u': total += put_u(width, 10, ' ', va_arg(ap, unsigned)); break;
		case 'x': total += put_u(width, 16, '0', va_arg(ap, unsigned)); break;
		case 'p': total += put_u(width, 16, '0', va_arg(ap, unsigned)); break;
		default:
			  printf("\nFormat specifier '%c' not supported\n", c);
			  return -1;
		}
	}
	va_end(ap);
	return total;
}

#if defined TEST_PRINTF
int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	int len = printf("c=%c \nesc=%% \nd=%4d \nu=%4u \nx=%4x \n", 'a', -32, 32, 1023);
	printf("len=%d\n", len);
	return 0;
}
#endif
