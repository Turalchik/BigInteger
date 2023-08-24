#include <string>
#include <gtest/gtest.h>

#include "biginteger.h"

TEST(create, default_constructor)
{
    BigInteger x;
    BigInteger y = 0;
    EXPECT_EQ(BigInteger(0), x);
    EXPECT_EQ(y, x);
}

TEST(create, copy_constructor)
{
    BigInteger x = 3;
    BigInteger y = x;

    EXPECT_EQ(y, x);
    EXPECT_EQ(3, y);
}

TEST(create, copy_constructor_real_copy)
{
    BigInteger x = 2;
    BigInteger y = x;
    x = 4;

    EXPECT_EQ(2, y);
}

TEST(create, copy_constructor_real_copy_2)
{
    BigInteger x = 3;
    BigInteger y = x;
    y = 5;

    EXPECT_EQ(3, x);
}

TEST(create, constructor_invalid_string)
{
    EXPECT_THROW(BigInteger("abc"), std::invalid_argument);
    EXPECT_THROW(BigInteger("123x"), std::invalid_argument);
    EXPECT_THROW(BigInteger(""), std::invalid_argument);
    EXPECT_THROW(BigInteger("-"), std::invalid_argument);
    EXPECT_THROW(BigInteger("-x"), std::invalid_argument);
    EXPECT_THROW(BigInteger("123-456"), std::invalid_argument);
    EXPECT_THROW(BigInteger("--5"), std::invalid_argument);
    EXPECT_THROW(BigInteger("++5"), std::invalid_argument);
}

TEST(comparisons, comparison_operators)
{
    BigInteger a = 100;
    BigInteger b = 100;
    BigInteger c = 200;
    BigInteger d = -100;

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a != c);
    EXPECT_TRUE(a < c);
    EXPECT_TRUE(c > a);
    EXPECT_TRUE(a <= a);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a <= c);
    EXPECT_TRUE(c >= a);
    EXPECT_TRUE(d != 0);
    EXPECT_TRUE(d < a);
}

TEST(comparisons, compare_with_sign)
{
    BigInteger a = 1;
    BigInteger b = -a;

    EXPECT_TRUE(a != b);
}

TEST(comparisons, compare_zero_and_minus_zero)
{
    BigInteger a;
    BigInteger b = -a;

    EXPECT_TRUE(a == b);
}


TEST(assignments, assignment_operator)
{
    BigInteger a = 4;
    BigInteger b = 7;
    b = a;

    EXPECT_TRUE(a == b);
}

TEST(assignments, self_assignment)
{
    BigInteger a = 5;
    a = a;

    EXPECT_TRUE(a == 5);
}

TEST(assignments, assignment_return_value)
{
    BigInteger a = 4;
    BigInteger b = 7;
    (a = b) = a;

    EXPECT_TRUE(a == 7);
    EXPECT_TRUE(b == 7);
}

TEST(arithmetic, one_plus_zero)
{
    EXPECT_EQ(BigInteger(1), BigInteger(1) + BigInteger(0));
    EXPECT_EQ(BigInteger(1), BigInteger(1) + 0);
    EXPECT_EQ(BigInteger(1), 0 + BigInteger(1));
}

TEST(arithmetic, one_plus_one)
{
    EXPECT_EQ(BigInteger(2), BigInteger(1) + BigInteger(1));
    EXPECT_EQ(BigInteger(2), BigInteger(1) + 1); // implicit conversion from int must work
    EXPECT_EQ(BigInteger(2), 1 + BigInteger(1));
}

TEST(arithmetic, operator_plus)
{
    BigInteger a = 5;
    BigInteger b = 20;

    EXPECT_TRUE(a + b == 25);

    a += b;
    EXPECT_TRUE(a == 25);
}

TEST(arithmetic, operator_plus_signed)
{
    BigInteger a = 5;
    BigInteger b = -20;
    EXPECT_TRUE(a + b == -15);

    a += b;
    EXPECT_TRUE(a == -15);
}

TEST(arithmetic, operator_pluseq_return_value)
{
    BigInteger a = 5;
    BigInteger b = 1;

    (a += b) += b;
    EXPECT_EQ(7, a);
}

TEST(arithmetic, operator_sub)
{
    BigInteger a = 20;
    BigInteger b = 5;

    EXPECT_TRUE(a - b == 15);

    a -= b;
    EXPECT_TRUE(a == 15);
}

TEST(arithmetic, operator_sub_signed)
{
    BigInteger a = 5;
    BigInteger b = 20;

    EXPECT_TRUE(a - b == -15);

    a -= b;
    EXPECT_TRUE(a == -15);

    a -= -100;
    EXPECT_TRUE(a == 85);
}

TEST(arithmetic, operator_subeq_return_value)
{
    BigInteger a = 5;
    BigInteger b = 1;

    (a -= b) -= b;
    EXPECT_EQ(3, a);
}

TEST(arithmetic, operator_mul)
{
    BigInteger a = 5;
    BigInteger b = 20;
    EXPECT_TRUE(a * b == 100);

    a *= b;
    EXPECT_TRUE(a == 100);
}

TEST(arithmetic, operator_mul_signed)
{
    BigInteger a = -5;
    BigInteger b = 20;

    EXPECT_TRUE(a * b == -100);

    a *= b;
    EXPECT_TRUE(a == -100);
}

TEST(arithmetic, operator_muleq_return_value)
{
    BigInteger a = 5;
    BigInteger b = 2;

    (a *= b) *= b;
    EXPECT_EQ(20, a);
}

TEST(arithmetic, operator_div)
{
    BigInteger a = 20;
    BigInteger b = 5;
    BigInteger c = 20;
    EXPECT_EQ(0, b / c);
    EXPECT_TRUE(a / b == 4);
    EXPECT_TRUE(a % b == 0);

    a /= b;
    EXPECT_TRUE(a == 4);

    c %= b;
    EXPECT_TRUE(c == 0);
}

TEST(arithmetic, operator_div_signed)
{
    BigInteger a = -20;
    BigInteger b = 5;

    EXPECT_TRUE(a / b == -4);
    EXPECT_TRUE(a % b == 0);
}

TEST(arithmetic, operator_div_rounding)
{
    BigInteger a = 23;
    BigInteger b = 5;

    EXPECT_TRUE(a / b == 4);
    EXPECT_TRUE(a % b == 3);
}

TEST(arithmetic, operator_div_rounding_negative)
{
    BigInteger a = 23;
    BigInteger b = -5;
    BigInteger c = -23;
    BigInteger d = 5;

    EXPECT_TRUE(a / b == -4);
    EXPECT_TRUE(c / d == -4);
    EXPECT_TRUE(a % b == 3);
    EXPECT_TRUE(c % d == -3);
}

TEST(arithmetic, operator_div_return_value)
{
    BigInteger a = 100;
    BigInteger b = 2;

    (a /= b) /= b;
    EXPECT_EQ(25, a);
}

TEST(arithmetic, operator_unary_plus)
{
    BigInteger a = 123;
    BigInteger b = +a;

    EXPECT_TRUE(a == b);
}

TEST(arithmetic, negation)
{
    BigInteger a = 666;
    BigInteger b = -a;

    EXPECT_TRUE(b == -666);
}

TEST(arithmetic, operator_increment)
{
    BigInteger a = 42;
    BigInteger pre = ++a;
    BigInteger post = a++;

    EXPECT_EQ(43, pre);
    EXPECT_EQ(43, post);
}

TEST(arithmetic, operator_increment_zero)
{
    BigInteger a = 0;
    BigInteger pre = ++a;
    BigInteger post = a++;

    EXPECT_EQ(1, pre);
    EXPECT_EQ(1, post);
}

TEST(arithmetic, operator_increment_to_zero)
{
    BigInteger a = -1;
    BigInteger pre = ++a;
    BigInteger post = a++;

    EXPECT_EQ(0, pre);
    EXPECT_EQ(0, post);
}

TEST(arithmetic, operator_decrement)
{
    BigInteger a = 42;
    BigInteger pre = --a;
    BigInteger post = a--;

    EXPECT_EQ(41, pre);
    EXPECT_EQ(41, post);
}

TEST(arithmetic, operator_decrement_zero)
{
    BigInteger a = 0;
    BigInteger pre = --a;
    BigInteger post = a--;

    EXPECT_EQ(-1, pre);
    EXPECT_EQ(-1, post);
}

TEST(arithmetic, operator_decrement_to_zero)
{
    BigInteger a = 1;
    BigInteger pre = --a;
    BigInteger post = a--;

    EXPECT_EQ(0, pre);
    EXPECT_EQ(0, post);
}

TEST(arithmetic, add_long)
{
    BigInteger a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("10000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000");

    EXPECT_EQ(c, a + b);
}

TEST(arithmetic, add_long_signed)
{
    BigInteger a("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(0, a + b);
}

TEST(arithmetic, add_long_signed2)
{
    BigInteger a("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("-999999999999999999999999999999999999999999999999999900000000000000000000000000000000000000");

    EXPECT_EQ(c, a + b);
}

TEST(arithmetic, add_long_pow2)
{
    BigInteger a("18446744073709551616");
    BigInteger b("-18446744073709551616");
    BigInteger c("36893488147419103232");

    EXPECT_EQ(c, a + a);
    EXPECT_EQ(a, b + c);
    EXPECT_EQ(a, c + b);
}

TEST(arithmetic, sub_long)
{
    BigInteger a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("9999999999999999999999999999999999999999999999999999900000000000000000000000000000000000000");

    EXPECT_EQ(c, a - b);
}

TEST(arithmetic, sub_long_pow2)
{
    BigInteger a("36893488147419103232");
    BigInteger b("36893488147419103231");

    EXPECT_EQ(1, a - b);
}

TEST(arithmetic, mul_long)
{
    BigInteger a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000");

    EXPECT_EQ(c, a * b);
}

TEST(arithmetic, mul_long_signed)
{
    BigInteger a("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000");

    EXPECT_EQ(c, a * b);
}

TEST(arithmetic, mul_long_signed2)
{
    BigInteger a("-100000000000000000000000000");
    BigInteger c("10000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(c, a * a);
}

TEST(arithmetic, mul_long_pow2)
{
    BigInteger a("18446744073709551616");
    BigInteger b("340282366920938463463374607431768211456");
    BigInteger c("115792089237316195423570985008687907853269984665640564039457584007913129639936");

    EXPECT_EQ(b, a * a);
    EXPECT_EQ(c, b * b);
}


TEST(arithmetic, div_long)
{
    BigInteger a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("100000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(c, a / b);
}

TEST(arithmetic, div_long_signed)
{
    BigInteger a("-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("-100000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(c, a / b);
}

TEST(arithmetic, div_long_signed2)
{
    BigInteger a("-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("-100000000000000000000000000000000000000");
    BigInteger c("100000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(c, a / b);
}

TEST(arithmetic, negation_long)
{
    BigInteger a("10000000000000000000000000000000000000000000000000000");
    BigInteger c("-10000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(-a, c);
    EXPECT_EQ(a, -c);
}

TEST(converting, string_conv)
{
    EXPECT_EQ("100", to_string(BigInteger("100")));
    EXPECT_EQ("100", to_string(BigInteger("0100")));
    EXPECT_EQ("0", to_string(BigInteger("0")));
    EXPECT_EQ("0", to_string(BigInteger("-0")));
    EXPECT_EQ("-1000000000000000", to_string(BigInteger("-1000000000000000")));

    EXPECT_EQ("2147483647", to_string(BigInteger("2147483647")));
    EXPECT_EQ("2147483648", to_string(BigInteger("2147483648")));
    EXPECT_EQ("-2147483649", to_string(BigInteger("-2147483649")));
}

namespace
{
    template <typename T>
    void test_converting_ctor(T value)
    {
        using std::to_string;

        BigInteger bi = value;
        EXPECT_EQ(to_string(value), to_string(bi));
    }
}

TEST(converting, converting_ctor2)
{
    BigInteger a(1);
    BigInteger b(1U);
    BigInteger c(1L);
    BigInteger d(1UL);
    BigInteger e(1LL);
    BigInteger f(1ULL);

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a == c);
    EXPECT_TRUE(a == d);
    EXPECT_TRUE(a == e);
    EXPECT_TRUE(a == f);
}

TEST(converting, converting_ctor3)
{
    BigInteger a(-1);
    BigInteger b(-1L);
    BigInteger c(-1LL);

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a == c);
}
