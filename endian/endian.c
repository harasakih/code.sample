#include	<stdio.h>

int		main(int argc, char *argv[])
{
	union	 {
		unsigned char	c[2];
		short			i;
	} _short ;
	union {
		unsigned char	c[4];
		int				i;
	} _int ;
	union {
		unsigned char	c[8];
		long long		i;
	} _long ;
	unsigned char	buf[128];

	_short.i = 0x0102;
	_int.i   = 0x01020304;
	_long.i  = 0x0102030405060708;

	printf("size of short %lu\n", sizeof(_short.i));
	printf("size of int   %lu\n", sizeof(_int.i));
	printf("size of long  %lu\n", sizeof(_long.i));

	printf("val short:%d(%04x)\n",   _short.i, _short.i);
	printf("val int  :%d(%08x)\n",   _int.i,   _int.i);
	printf("val long :%ld(%016lx)\n", _long.i, _long.i);

	printf("hex short:");
	for(int i=0; i < sizeof(_short.i); i++) {
		printf("%02x", _short.c[i]);
	}; printf("\n");

	printf("hex int  :");
	for(int i=0; i < sizeof(_int.i); i++) {
		printf("%02x", _int.c[i]);
	}; printf("\n");

	printf("hex long :");
	for(int i=0; i < sizeof(_long.i); i++) {
		printf("%02x", _long.c[i]);
	}; printf("\n");

}
