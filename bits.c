/* WARNING: Do not include any other libraries here,
 * otherwise you will get an error while running test.py
 * You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 *
 * Using printf will interfere with our script capturing the execution results.
 * At this point, you can only test correctness with ./btest.
 * After confirming everything is correct in ./btest, remove the printf
 * and run the complete tests with test.py.
 */

 /*
 * bitAnd - x & y using only ~ and |
 * Example: bitAnd(4, 5) = 4
 * Legal ops: ~ |
 * Max ops: 7
 * Difficulty: 1
 */
int bitAnd(int x, int y) {
    /* De Morgan: x & y == ~(~x | ~y) */
    return ~(~x | ~y);
}

/*
 * bitXor - x ^ y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 7
 *   Difficulty: 1
 */
int bitXor(int x, int y) {
    /* x ^ y == (x | y) & ~(x & y), and x | y == ~(~x & ~y) by De Morgan */
    return ~(~x & ~y) & ~(x & y);
}

/*
 * samesign - Determines if two integers have the same sign.
 *   0 is not positive, nor negative
 *   Example: samesign(0, 1) = 0, samesign(0, 0) = 1
 *            samesign(-4, -5) = 1, samesign(-4, 5) = 0
 *   Legal ops: >> << ! ^ && if else &
 *   Max ops: 12
 *   Difficulty: 2
 *
 * Parameters:
 *   x - The first integer.
 *   y - The second integer.
 *
 * Returns:
 *   1 if x and y have the same sign , 0 otherwise.
 */
int samesign(int x, int y) {
    /* There are three sign classes: negative, zero, positive.
     * (x >> 31) collapses negative->all-ones and {zero,positive}->0,
     * so it separates negative from the rest.
     * (!x) separates zero from nonzero.
     * Both must agree, hence the two XORs each negated with !. */
    return !((x >> 31) ^ (y >> 31)) && !((!x) ^ (!y));
}

/*
 * logtwo - Calculate the base-2 logarithm of a positive integer using bit
 *   shifting. (Think about bitCount)
 *   Note: You may assume that v > 0
 *   Example: logtwo(32) = 5
 *   Legal ops: > < >> << |
 *   Max ops: 25
 *   Difficulty: 4
 */
int logtwo(int v) {
    /* Binary search on the exponent: at each step ask "is v at least 2^k?"
     * and if so, set bit k of the answer and consume those k bits by
     * shifting v right. */
    int r = 0;
    int t;
    int s;

    t = (v > 0xFFFF);  s = (t << 4);  r = r | s;  v = v >> s;
    t = (v > 0xFF);    s = (t << 3);  r = r | s;  v = v >> s;
    t = (v > 0xF);     s = (t << 2);  r = r | s;  v = v >> s;
    t = (v > 0x3);     s = (t << 1);  r = r | s;  v = v >> s;
    t = (v > 0x1);     r = r | t;

    return r;
}

/*
 *  byteSwap - swaps the nth byte and the mth byte
 *    Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *              byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *    Note: You may assume that 0 <= n <= 3, 0 <= m <= 3
 *    Legal ops: ! ~ & ^ | + << >>
 *    Max ops: 17
 *    Difficulty: 2
 */
int byteSwap(int x, int n, int m) {
    /* Pull both bytes out, clear their original slots with a combined mask,
     * then OR them back into each other's positions. n == m also works:
     * clearing once and OR-ing the same value twice is a no-op. */
    int nn = n << 3;
    int mm = m << 3;
    int a = (x >> nn) & 0xFF;
    int b = (x >> mm) & 0xFF;

    x = x & ~((0xFF << nn) | (0xFF << mm));
    x = x | (a << mm) | (b << nn);

    return x;
}

/*
 * reverse - Reverse the bit order of a 32-bit unsigned integer.
 *   Example: reverse(0xFFFF0000) = 0x0000FFFF reverse(0x80000000)=0x1 reverse(0xA0000000)=0x5
 *   Note: You may assume that an unsigned integer is 32 bits long.
 *   Legal ops: << | & - + >> for while ! ~ (You can define unsigned in this function)
 *   Max ops: 30
 *   Difficulty: 3
 */
unsigned reverse(unsigned v) {
    /* Shift the answer left while feeding it the low bit of v, 32 times. */
    unsigned r = 0;
    int i = 32;

    while (i) {
        r = (r << 1) | (v & 1);
        v = v >> 1;
        i = i - 1;
    }

    return r;
}

/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Note: You can assume that 0 <= n <= 31
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Difficulty: 3
 */
int logicalShift(int x, int n) {
    /* >> on a negative int copies the sign bit into the top n bits.
     * ((1 << 31) >> n) << 1 builds a mask whose top n bits are 1,
     * so its complement clears exactly those polluted bits. */
    return (x >> n) & ~(((1 << 31) >> n) << 1);
}

/*
 * leftBitCount - returns count of number of consective 1's in left-hand (most) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12,
 *             leftBitCount(0xFE00FF0F) = 7
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Difficulty: 4
 */
int leftBitCount(int x) {
    /* Binary search on the run length, same idea as logtwo but counting
     * leading ONES instead of locating the highest bit.
     * !(~(x >> k)) is 1 exactly when the top k bits are all ones.
     * Whenever a block matches we add its width and shift it away.
     * A final extra 1-bit step is what lets the all-ones input reach 32. */
    int r = 0;
    int t;
    int s;

    t = !(~(x >> 16));  s = (t << 4);  r = r + s;  x = x << s;
    t = !(~(x >> 24));  s = (t << 3);  r = r + s;  x = x << s;
    t = !(~(x >> 28));  s = (t << 2);  r = r + s;  x = x << s;
    t = !(~(x >> 30));  s = (t << 1);  r = r + s;  x = x << s;
    t = !(~(x >> 31));  r = r + t;     x = x << t;
    t = !(~(x >> 31));  r = r + t;

    return r;
}

/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but it is to be interpreted as
 *   the bit-level representation of a single-precision floating point values.
 *   Legal ops: if else while for & | ~ + - >> << < > ! ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned float_i2f(int x) {
    /* Normalize by shifting |x| left until bit 31 is set, which makes
     * "1.ffff" sit right under the top bit. The exponent then falls out of
     * how many shifts we needed. Bits 30..8 become the 23-bit fraction and
     * bits 7..0 are the round/sticky bits for round-half-to-even. */
    unsigned sign;
    unsigned abs;
    unsigned e = 31;
    unsigned frac;
    unsigned rest;

    if (x == 0) return 0;

    sign = x & 0x80000000;
    abs = x;
    if (sign) abs = -abs;

    while (!(abs & 0x80000000)) {
        abs = abs << 1;
        e = e - 1;
    }

    frac = (abs >> 8) & 0x7FFFFF;
    rest = abs & 0xFF;

    if (rest > 0x80) {
        frac = frac + 1;
    } else {
        if (rest == 0x80) {
            if (frac & 1) frac = frac + 1;   /* tie -> round to even */
        }
    }

    if (frac == 0x800000) {                  /* carry out of the fraction */
        frac = 0;
        e = e + 1;
    }

    return sign | ((e + 127) << 23) | frac;
}

/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: & >> << | if > < >= <= ! ~ else + ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatScale2(unsigned uf) {
    /* Doubling a normal number is just exponent + 1.
     * A denormal has no implicit 1, so its value is linear in the fraction:
     * shifting the whole pattern left by one doubles it (and naturally
     * promotes the largest denormal into the smallest normal).
     * exp == 0xFF means Inf or NaN -> the argument is returned unchanged. */
    unsigned sign = uf & 0x80000000;
    unsigned exp = (uf >> 23) & 0xFF;

    if (exp == 0xFF) return uf;

    if (exp == 0) return sign | (uf << 1);

    exp = exp + 1;
    if (exp == 0xFF) return sign | 0x7F800000;   /* overflow -> Inf */

    return sign | (exp << 23) | (uf & 0x7FFFFF);
}

/*
 * float64_f2i - Convert a 64-bit IEEE 754 floating-point number to a 32-bit signed integer.
 *   The conversion rounds towards zero.
 *   Note: Assumes IEEE 754 representation and standard two's complement integer format.
 *   Parameters:
 *     uf1 - The lower 32 bits of the 64-bit floating-point number.
 *     uf2 - The higher 32 bits of the 64-bit floating-point number.
 *   Returns:
 *     The converted integer value, or 0x80000000 on overflow, or 0 on underflow.
 *   Legal ops: >> << | & ~ ! + - > < >= <= if else
 *   Max ops: 60
 *   Difficulty: 3
 */
int float64_f2i(unsigned uf1, unsigned uf2) {
    /* A double is sign | 11-bit exp | 52-bit fraction.
     * After removing the bias, e = exp - 1023 is the position of the binary
     * point. e < 0 means |value| < 1 -> truncates to 0. e >= 31 blows past
     * the 32-bit signed range -> overflow. Otherwise the integer part is
     * 1 (implicit) followed by the top e fraction bits, and everything
     * below is simply dropped, which is exactly truncation toward zero. */
    unsigned sign = uf2 >> 31;
    unsigned exp = (uf2 >> 20) & 0x7FF;
    unsigned hi20;
    unsigned lo32;
    unsigned mag;
    int e;

    if (exp >= 0x7FF) return ~0x7FFFFFFF;    /* Inf / NaN -> overflow */
    if (exp <= 0) return 0;                  /* denormal or zero */
    e = exp - 1023;
    if (e < 0) return 0;                     /* |value| < 1 -> underflow */
    if (e >= 31) return ~0x7FFFFFFF;         /* too large */

    hi20 = uf2 & 0xFFFFF;
    lo32 = uf1;

    if (e <= 20) {
        /* the whole kept part lives in the high fraction word */
        mag = (1 << e) + (hi20 >> (20 - e));
    } else {
        /* straddles both words */
        mag = (1 << e) + ((hi20 << (e - 20)) | (lo32 >> (52 - e)));
    }

    if (sign) return -mag;
    return mag;
}

/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: < > <= >= << >> + - & | ~ ! if else &&
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatPower2(int x) {
    /* 2^x as a float has fraction 0 and exponent field x + 127.
     * Valid normal range: -126..127.
     * Below that it degrades into a denormal: the single set bit slides
     * down and is worth 2^(x + 149). Past 2^-149 there is nothing left,
     * and above 2^127 the exponent field would overflow into Inf. */
    if (x > 127) return 0x7F800000;          /* +Inf */
    if (x >= -126) return (x + 127) << 23;   /* normal */
    if (x >= -149) return 1 << (x + 149);    /* denormal */
    return 0;                                /* too small */
}
