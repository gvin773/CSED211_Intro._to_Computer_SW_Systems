/* CSED 211 Fall '2021.  Lab L2 */

#if 0
FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict compared to the previous homework.
You are allowed to use looping and conditional control.
You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.

#endif

/*
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
    unsigned exp = uf & 0x7F800000;
    unsigned frac = uf & 0x007FFFFF;
    if((exp == 0x7F800000) && frac) return uf;
    return (uf ^ 0x80000000);
}
/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Rating: 4
 */
unsigned float_i2f(int x) {
    unsigned s = x & 0x80000000;
    unsigned E = 1;
    unsigned Bias = 127;
    unsigned exp = 0;
    unsigned frac = 0;
    unsigned G = 0, R = 0, S = 0;
    unsigned cnt = 25;

    if(x == 0) return 0;
    else if(x == 0x80000000) return 0xCF000000;


    if(s) x = (~x) + 1;
    while((x>>E) != 0) E++;
    E--;
    exp = E + Bias;

    if(E <= 23) frac = (x<<(23-E)) & 0x007FFFFF;
    else
    {
        frac = (x>>(E-23)) & 0x007FFFFF;
        G = frac & 1;
        R = (x>>(E-24)) & 1;
        while(E >= cnt)
        {
            S |= ((x>>(E-cnt)) & 1);
            cnt++;
        }

        if((R == 1 && S == 1) || (R == 1 && G == 1)) frac++;
        if(frac>>23)
        {
            exp++;
            frac = 0;
        }
    }

    return s | (exp<<23) | frac;
}
/*
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
    unsigned s = uf & 0x80000000;
    unsigned exp = uf & 0x7F800000;
    unsigned frac = uf & 0x007FFFFF;

    if(exp == 0x7F800000) return uf; //special value
    if(exp == 0) return s | (uf<<1); //denormalized value
    return s | (exp + 0x00800000) | frac; //normalized value
}

/*
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
    unsigned s = uf & 0x80000000;
    unsigned exp = uf & 0x7F800000;
    unsigned frac = uf & 0x007FFFFF;

    if((exp == 0x7F800000) && frac) return uf;
    return uf & 0x7FFFFFFF;
}
/*
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
    unsigned s = uf & 0x80000000;
    unsigned exp = uf & 0x7F800000;
    unsigned frac = uf & 0x007FFFFF;

    if(exp == 0x7F800000) return uf; //special value
    if((exp>>23) > 1)
    {
        exp -= 0x00800000;
        return s | exp | frac;
    }
    frac |= (exp & 0x00800000);
    exp = 0;
    if((frac & 3) == 3) frac++;
    frac >>= 1;
    return s | exp | frac;
}
