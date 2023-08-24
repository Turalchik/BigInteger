#ifndef __BIGINTEGER__2023__
#define __BIGINTEGER__2023__

#include <string>

class BigInteger {
	int32_t sign_;
	int32_t size_;
	int32_t maxsize_;
	uint32_t* digits_ = nullptr;

	//Classic swap for move semantics
	void swap(BigInteger&);
	//The number system of our BigInteger-ов
	static const uint64_t NUMBERSYSTEM = 1LL << 32;
	static const uint32_t MAXVALUEDIGIT = 0xFFFFFFFF;
	//Static function for a constructor from a string
	static bool divideLineIntoTwo(std::string&);
	//Function for removing insignificant zeros
	void removeZeros();
	//Function for comparison by absolute value
	int32_t cmpABS(const BigInteger&) const;
	//Auxiliary functions for to_string
	static void multiplyStringByNumberSystem(std::string&);
	static void addNumberToString(std::string&, uint32_t);
	//Method for division and remainder
	BigInteger algorithmKnut(const BigInteger&);
public:
	//Constructors
	BigInteger();
	BigInteger(int);
	BigInteger(unsigned int);
	BigInteger(long);
	BigInteger(unsigned long);
	BigInteger(long long);
	BigInteger(unsigned long long);
	BigInteger(std::string);
	BigInteger(const BigInteger&);
	BigInteger(BigInteger&&) noexcept;

	//Assignment operators
	BigInteger& operator= (const BigInteger&);
	BigInteger& operator= (BigInteger&&) noexcept;

	//Unary operators
	BigInteger operator- () const;
	BigInteger operator+ () const;

	//Increments and decrements
	BigInteger& operator++ ();
	BigInteger& operator-- ();
	BigInteger operator++ (int);
	BigInteger operator-- (int);

	//Comparison methods
	bool isLess(const BigInteger&) const;
	bool isEqual(const BigInteger&) const;

	//Arithmetic methods
	BigInteger& operator+= (const BigInteger&);
	BigInteger& operator-= (const BigInteger&);
	BigInteger& operator*= (const BigInteger&);
	BigInteger& operator/= (const BigInteger&);
	BigInteger& operator%= (const BigInteger&);

	//The number module
	BigInteger ABS() const;

	//Operator to translate to a string
	operator std::string() const;

	//Destructor
	~BigInteger() {
		delete[] digits_;
		sign_ = 0;
		size_ = 0;
		maxsize_ = 0;
	}
};

//Comparison operators
bool operator< (const BigInteger&, const BigInteger&);
bool operator> (const BigInteger&, const BigInteger&);
bool operator<= (const BigInteger&, const BigInteger&);
bool operator>= (const BigInteger&, const BigInteger&);
bool operator== (const BigInteger&, const BigInteger&);
bool operator!= (const BigInteger&, const BigInteger&);

//Arithmetic operators
BigInteger operator+ (const BigInteger&, const BigInteger&);
BigInteger operator- (const BigInteger&, const BigInteger&);
BigInteger operator* (const BigInteger&, const BigInteger&);
BigInteger operator/ (const BigInteger&, const BigInteger&);
BigInteger operator% (const BigInteger&, const BigInteger&);

//Function for translating BigInteger to a line
std::string to_string(const BigInteger&);

#endif