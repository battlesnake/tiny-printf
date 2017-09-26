int printf(const char *format, ...)
	__attribute__((format(printf, 1, 2)));

int puts(const char *str);

int put_digit(unsigned digit);

int put_s(int width, const char *s);
int put_d(int width, int base, char pad, int value);
int put_u(int width, int base, char pad, unsigned value);

int isdigit(int c);
