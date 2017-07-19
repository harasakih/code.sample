/*
 ***********************************************************************
 *	$Id: bigint.c,v 1.7 2006/06/25 16:03:27 hiroshi Exp $
 ***********************************************************************
 *	$Log: bigint.c,v $
 *	Revision 1.7  2006/06/25 16:03:27  hiroshi
 *	*** empty log message ***
 *
 ***********************************************************************
 *
 * Name		: bigint.c ++
 * Abstruct	: 整数のサイズを調査
 */

#include    <stdio.h>

#ifdef      GCC
#include    <stdint.h>
#define     Myint64     int64_t
#else       /* for ms visualC */
#define     Myint64     __int64
#endif


int     main(int argc, char *argv[])
{
        short s1, s2, s3;           /* BorlandC++ : 2 byte */
        int i1, i2, i3;             /* BorlandC++ : 4 byte */
        long l1, l2, l3;            /* BorlandC++ : 4 byte */
        Myint64 w1, w2, w3;         /* BorlandC++ : 8 byte */

        printf("short : %d\n", sizeof(short));
        printf("int   : %d\n", sizeof(int));
        printf("long  : %d\n", sizeof(long));
        printf("int64 : %d\n", sizeof(Myint64));

/* short */
        s1 =    0x7fff;
        s2 =    s1 + 1;
        s3 =    s2 * 2;
        printf("\nshort\n");
        printf("     s1     = %d(%04x)\n", s1, s1);
        printf("s2 = s1 + 1 = %d(%04x)\n", s2, s2);
        printf("s3 = s2 * 2 = %d(%04x)\n", s3, s3);
        printf("s3 = s2 * 2 = %d(%04x)\n", s2 * 2L, s2 * 2L);


/* int */
        i1 =    0x7fffffff;
        i2 =    i1 + 1;
        i3 =    i2 * 2;
        printf("\nint\n");
        printf("     i1     = %d(%08x)\n", i1, i1);
        printf("i2 = i1 + 1 = %d(%08x)\n", i2, i2);
        printf("i3 = i2 * 2 = %d(%08x)\n", i3, i3);

/* long */
        l1 =    0x7fffffff;
        l2 =    l1 + 1;
        l3 =    l2 * 2;
        printf("\nlong\n");
        printf("     l1     = %d(%08x)\n", l1, l1);
        printf("l2 = l1 + 1 = %d(%08x)\n", l2, l2);
        printf("l3 = l2 * 2 = %d(%08x)\n", l3, l3);

/* long long */
        w1 =    0x7fffffff;
        w2 =    w1 + 1;
        w3 =    w2 * 2;
        printf("\nint64\n");
#ifdef  GCC
        printf("GCC\n");
        printf("     w1     = %lld(%llx)\n", w1, w1);
        printf("w2 = w1 + 1 = %lld(%llx)\n", w2, w2);
        printf("w3 = w2 * 2 = %lld(%llx)\n", w3, w3);
#else
        printf("not GCC\n");
        printf("     w1     = %I64d(%I64x)\n", w1, w1);
        printf("w2 = w1 + 1 = %I64d(%I64x)\n", w2, w2);
        printf("w3 = w2 * 2 = %I64d(%I64x)\n", w3, w3);
#endif
        return 0;

}
