/* CSED 211 Fall '2021.  Lab L1 */



#if 0
LAB L1 INSTRUCTIONS:

#endif

/*
 * bitAnd - x&y using only ~ and |
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 */
int bitAnd(int x, int y)
{
    return ~(~x | ~y);
}

/*
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1,
 *   Legal ops: ! ~ & ^ | + << >>
 */
int addOK(int x, int y)
{
    int ret = (x & 1) & (y & 1);
    ret = (ret & x & y) | (!ret & ((x>>1)&1) & ((y>>1)&1)) | (ret & !((x>>1)&1) & ((y>>1)&1)) | (ret & ((x>>1)&1) & !((y>>1)&1));
    ret = (ret & x & y) | (!ret & ((x>>2)&1) & ((y>>2)&1)) | (ret & !((x>>2)&1) & ((y>>2)&1)) | (ret & ((x>>2)&1) & !((y>>2)&1));
    ret = (ret & x & y) | (!ret & ((x>>3)&1) & ((y>>3)&1)) | (ret & !((x>>3)&1) & ((y>>3)&1)) | (ret & ((x>>3)&1) & !((y>>3)&1));
    ret = (ret & x & y) | (!ret & ((x>>4)&1) & ((y>>4)&1)) | (ret & !((x>>4)&1) & ((y>>4)&1)) | (ret & ((x>>4)&1) & !((y>>4)&1));
    ret = (ret & x & y) | (!ret & ((x>>5)&1) & ((y>>5)&1)) | (ret & !((x>>5)&1) & ((y>>5)&1)) | (ret & ((x>>5)&1) & !((y>>5)&1));
    ret = (ret & x & y) | (!ret & ((x>>6)&1) & ((y>>6)&1)) | (ret & !((x>>6)&1) & ((y>>6)&1)) | (ret & ((x>>6)&1) & !((y>>6)&1));
    ret = (ret & x & y) | (!ret & ((x>>7)&1) & ((y>>7)&1)) | (ret & !((x>>7)&1) & ((y>>7)&1)) | (ret & ((x>>7)&1) & !((y>>7)&1));
    ret = (ret & x & y) | (!ret & ((x>>8)&1) & ((y>>8)&1)) | (ret & !((x>>8)&1) & ((y>>8)&1)) | (ret & ((x>>8)&1) & !((y>>8)&1));
    ret = (ret & x & y) | (!ret & ((x>>9)&1) & ((y>>9)&1)) | (ret & !((x>>9)&1) & ((y>>9)&1)) | (ret & ((x>>9)&1) & !((y>>9)&1));
    ret = (ret & x & y) | (!ret & ((x>>10)&1) & ((y>>10)&1)) | (ret & !((x>>10)&1) & ((y>>10)&1)) | (ret & ((x>>10)&1) & !((y>>10)&1));
    ret = (ret & x & y) | (!ret & ((x>>11)&1) & ((y>>11)&1)) | (ret & !((x>>11)&1) & ((y>>11)&1)) | (ret & ((x>>11)&1) & !((y>>11)&1));
    ret = (ret & x & y) | (!ret & ((x>>12)&1) & ((y>>12)&1)) | (ret & !((x>>12)&1) & ((y>>12)&1)) | (ret & ((x>>12)&1) & !((y>>12)&1));
    ret = (ret & x & y) | (!ret & ((x>>13)&1) & ((y>>13)&1)) | (ret & !((x>>13)&1) & ((y>>13)&1)) | (ret & ((x>>13)&1) & !((y>>13)&1));
    ret = (ret & x & y) | (!ret & ((x>>14)&1) & ((y>>14)&1)) | (ret & !((x>>14)&1) & ((y>>14)&1)) | (ret & ((x>>14)&1) & !((y>>14)&1));
    ret = (ret & x & y) | (!ret & ((x>>15)&1) & ((y>>15)&1)) | (ret & !((x>>15)&1) & ((y>>15)&1)) | (ret & ((x>>15)&1) & !((y>>15)&1));
    ret = (ret & x & y) | (!ret & ((x>>16)&1) & ((y>>16)&1)) | (ret & !((x>>16)&1) & ((y>>16)&1)) | (ret & ((x>>16)&1) & !((y>>16)&1));
    ret = (ret & x & y) | (!ret & ((x>>17)&1) & ((y>>17)&1)) | (ret & !((x>>17)&1) & ((y>>17)&1)) | (ret & ((x>>17)&1) & !((y>>17)&1));
    ret = (ret & x & y) | (!ret & ((x>>18)&1) & ((y>>18)&1)) | (ret & !((x>>18)&1) & ((y>>18)&1)) | (ret & ((x>>18)&1) & !((y>>18)&1));
    ret = (ret & x & y) | (!ret & ((x>>19)&1) & ((y>>19)&1)) | (ret & !((x>>19)&1) & ((y>>19)&1)) | (ret & ((x>>19)&1) & !((y>>19)&1));
    ret = (ret & x & y) | (!ret & ((x>>20)&1) & ((y>>20)&1)) | (ret & !((x>>20)&1) & ((y>>20)&1)) | (ret & ((x>>20)&1) & !((y>>20)&1));
    ret = (ret & x & y) | (!ret & ((x>>21)&1) & ((y>>21)&1)) | (ret & !((x>>21)&1) & ((y>>21)&1)) | (ret & ((x>>21)&1) & !((y>>21)&1));
    ret = (ret & x & y) | (!ret & ((x>>22)&1) & ((y>>22)&1)) | (ret & !((x>>22)&1) & ((y>>22)&1)) | (ret & ((x>>22)&1) & !((y>>22)&1));
    ret = (ret & x & y) | (!ret & ((x>>23)&1) & ((y>>23)&1)) | (ret & !((x>>23)&1) & ((y>>23)&1)) | (ret & ((x>>23)&1) & !((y>>23)&1));
    ret = (ret & x & y) | (!ret & ((x>>24)&1) & ((y>>24)&1)) | (ret & !((x>>24)&1) & ((y>>24)&1)) | (ret & ((x>>24)&1) & !((y>>24)&1));
    ret = (ret & x & y) | (!ret & ((x>>25)&1) & ((y>>25)&1)) | (ret & !((x>>25)&1) & ((y>>25)&1)) | (ret & ((x>>25)&1) & !((y>>25)&1));
    ret = (ret & x & y) | (!ret & ((x>>26)&1) & ((y>>26)&1)) | (ret & !((x>>26)&1) & ((y>>26)&1)) | (ret & ((x>>26)&1) & !((y>>26)&1));
    ret = (ret & x & y) | (!ret & ((x>>27)&1) & ((y>>27)&1)) | (ret & !((x>>27)&1) & ((y>>27)&1)) | (ret & ((x>>27)&1) & !((y>>27)&1));
    ret = (ret & x & y) | (!ret & ((x>>28)&1) & ((y>>28)&1)) | (ret & !((x>>28)&1) & ((y>>28)&1)) | (ret & ((x>>28)&1) & !((y>>28)&1));
    ret = (ret & x & y) | (!ret & ((x>>29)&1) & ((y>>29)&1)) | (ret & !((x>>29)&1) & ((y>>29)&1)) | (ret & ((x>>29)&1) & !((y>>29)&1));
    ret = (ret & x & y) | (!ret & ((x>>30)&1) & ((y>>30)&1)) | (ret & !((x>>30)&1) & ((y>>30)&1)) | (ret & ((x>>30)&1) & !((y>>30)&1));
    ret = (ret & x & y) | (!ret & ((x>>31)&1) & ((y>>31)&1)) | (ret & !((x>>31)&1) & ((y>>31)&1)) | (ret & ((x>>31)&1) & !((y>>31)&1));

    return !ret;
}

/*
 * isNegative - return 1 if x < 0, return 0 otherwise
 *   Example: isNegative(-1) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNegative(int x)
{
    return (x>>31) & 1;
}

/*
 * logicalShift - logical right shift of x by y bits, 1 <= y <= 31
 *   Example: logicalShift(-1, 1) = TMax.
 *   Legal ops: ! ~ & ^ | + << >>
 */
int logicalShift(int x, int y)
{
    int temp = ((1<<31)>>y)<<1;
    return (x>>y) & ~temp;
}


/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 */
int bitCount(int x)
{
    int checker = 0x11111111, sum = 0, ret = 0;
    sum = checker & x;
    sum += (checker & (x>>1));
    sum += (checker & (x>>2));
    sum += (checker & (x>>3));

    ret = sum & 0x0000000F;
    ret += (sum>>4) & 0x0000000F;
    ret += (sum>>8) & 0x0000000F;
    ret += (sum>>12) & 0x0000000F;
    ret += (sum>>16) & 0x0000000F;
    ret += (sum>>20) & 0x0000000F;
    ret += (sum>>24) & 0x0000000F;
    ret += (sum>>28) & 0x0000000F;

    return ret;
}
