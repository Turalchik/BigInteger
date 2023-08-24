#include "biginteger.h"
#include <iostream>

//Constructors from basic types
BigInteger::BigInteger() : size_(1), maxsize_(1), sign_(0) {
	digits_ = new uint32_t[maxsize_];
	digits_[0] = 0;
}
BigInteger::BigInteger(int number) : size_(1), maxsize_(1) {
	//Initialize the sign of our BigInteger
	if (number < 0) { sign_ = -1; }
	else if (number == 0) { sign_ = 0; }
	else { sign_ = 1; }

	//Create a BigInteger digit and write this number there
	digits_ = new uint32_t[maxsize_];
	digits_[0] = number * sign_;
}
BigInteger::BigInteger(unsigned int number) : size_(1), maxsize_(1) {
	//Initialize the sign of our BigInteger
	if (number == 0) { sign_ = 0; }
	else { sign_ = 1; }

	//Create a BigInteger digit and write this number there
	digits_ = new uint32_t[maxsize_];
	digits_[0] = number;
}
BigInteger::BigInteger(long number) : size_(1), maxsize_(1) {
	//Initialize the sign of our BigInteger
	if (number < 0) { sign_ = -1; }
	else if (number == 0) { sign_ = 0; }
	else { sign_ = 1; }

	//Create a BigInteger digit and write this number there
	digits_ = new unsigned int[maxsize_];
	digits_[0] = number * sign_;
}
BigInteger::BigInteger(unsigned long number) : size_(1), maxsize_(1) {
	//Initialize the sign of our BigInteger
	if (number == 0) { sign_ = 0; }
	else { sign_ = 1; }

	//Create a BigInteger digit and write this number there
	digits_ = new unsigned int[maxsize_];
	digits_[0] = number;
}
BigInteger::BigInteger(long long number) : size_(1), maxsize_(2) {
	//Initialize the sign of our BigInteger
	if (number < 0) { sign_ = -1; }
	else if (number == 0) { sign_ = 0; }
	else { sign_ = 1; }

	//Multiply the number by its own sign to get a non-negative value
	number *= sign_;
	//Creating two BigInteger digits
	digits_ = new uint32_t[maxsize_];
	//We write the first 32 bits of the number to the zero digit
	digits_[0] = number & MAXVALUEDIGIT;
	//Shift the number by 32 bits and write the remainder to the first digit
	if ((number >>= 32) > 0) { ++size_; }
	digits_[1] = static_cast<uint32_t>(number);
}
BigInteger::BigInteger(unsigned long long number) : size_(1), maxsize_(2) {
	//Initialize the sign of our BigInteger
	if (number == 0) { sign_ = 0; }
	else { sign_ = 1; }

	//Creating two BigInteger digits
	digits_ = new uint32_t[maxsize_];
	//We write the first 32 bits of the number to the zero digit
	digits_[0] = number & MAXVALUEDIGIT;
	//Shift the number by 32 bits and write the remainder to the first digit
	if ((number >>= 32) > 0) { ++size_; }
	digits_[1] = static_cast<uint32_t>(number);
}

//Constructor from string
BigInteger::BigInteger(std::string str) : size_(0), maxsize_(1) {

	size_t sizeString = str.length();
	if (sizeString == 0) { throw std::invalid_argument(str); }

	//Checking for the negativity of a number
	if (str[0] == '-') {
		sign_ = -1;
		str.erase(0, 1);
		--sizeString;
	}
	else { sign_ = 1; }

	if (sizeString == 0) { throw std::invalid_argument(str); }

	//Allocation of one memory cell
	digits_ = new uint32_t[maxsize_];
	digits_[0] = 0;

	//We write in blocks of 32 bits using
	//a function that divides a line into two
	uint32_t numberBits = 0;

	while (true) {
		//We increase the number of digits when
		//there is nowhere else to write the numbers
		if (size_ == maxsize_) {
			maxsize_ *= 2;
			uint32_t* tmp = new uint32_t[maxsize_];
			for (int index = 0; index < maxsize_; ++index) {
				tmp[index] = (index < size_) ? digits_[index] : 0;
			}
			delete[] digits_;
			digits_ = tmp;
		}

		//We look at the last digit of our number
		//(or rather at its last bit)
		digits_[size_] += (1 << numberBits++) *
			(static_cast<uint32_t>(str[sizeString - 1]) % 2);

		//If the string consists entirely of zeros, then dump
		try {
			if (divideLineIntoTwo(str)) { break; }
		}
		catch (...) {
			delete[] digits_;
			throw;
		}
		//We move on to the next block
		//when the number of bits has exceeded 32
		if (numberBits % 32 == 0) {
			numberBits = 0;
			++size_;
		}
	}

	//If there were only zeros in the string initially,
	//then we will not add anything to the zero digit,
	//and we will exit on the first pass of the cycle
	if (size_++ == 0 && digits_[0] == 0) { sign_ = 0; }
}

//Copy and Move Constructors
BigInteger::BigInteger(const BigInteger& other) {
	//Copy the sign from another BigInteger,
	//the number of occupied cells and the number of selected cells
	this->sign_ = other.sign_;
	this->size_ = other.size_;
	this->maxsize_ = other.maxsize_;

	//Now copy the contents of another BigInteger
	this->digits_ = new uint32_t[this->maxsize_];
	for (int32_t index = 0; index < this->maxsize_; ++index) {
		this->digits_[index] = other.digits_[index];
	}
}
BigInteger::BigInteger(BigInteger&& other) noexcept {
	this->swap(other);
}

//Assigment operators
BigInteger& BigInteger::operator= (const BigInteger& other) {
	if (this != &other) {
		BigInteger tmp(other);
		this->swap(tmp);
	}
	return *this;
}
BigInteger& BigInteger::operator= (BigInteger&& other) noexcept {
	if (this != &other) {
		this->swap(other);
	}
	return *this;
}

//Unary operators
BigInteger BigInteger::operator- () const {
	BigInteger result(*this);
	result.sign_ *= -1;
	return result;
}
BigInteger BigInteger::operator+ () const {
	return BigInteger(*this);
}

//Increments and decrements
BigInteger& BigInteger::operator++ () {
	//Pre-increment is the same as adding one
	//we do it and return the result
	return *this += 1;
}
BigInteger& BigInteger::operator-- () {
	//Pre-decrement is the same as subtracting one
	//we do it and return the result
	return *this += -1;
}
BigInteger BigInteger::operator++ (int) {
	//Post-increment is the same, only we return
	//the initial value, and then add one
	BigInteger result(*this);
	*this += 1;
	return result;
}
BigInteger BigInteger::operator-- (int) {
	//Post-decrement is the same, only we return
	//the initial value, and then we subtract one
	BigInteger result(*this);
	*this += -1;
	return result;
}

//Comparison methods
bool BigInteger::isLess(const BigInteger& other) const {
	//If the signs are different, then this-BigInteger is smaller
	//if and only if its sign is smaller than other-BigInteger
	if (this->sign_ != other.sign_) { return this->sign_ < other.sign_; }

	//The same as
	//(this->sign_ < 0 && this->cmpABS(other) > 0) ||
	//(this->sign_ > 0 && this->cmpABS(other) < 0)
	return (this->sign_ * this->cmpABS(other)) < 0;
}
bool BigInteger::isEqual(const BigInteger& other) const {

	//We check for the coincidence of signs
	if (this->sign_ != other.sign_) { return false; }

	//We check for matching modules
	return this->cmpABS(other) == 0;
}

//Utility functions
bool BigInteger::divideLineIntoTwo(std::string& str) {
	//The flag is needed in order to determine the moment
	//when our line consists of zeros
	bool flag = true;

	//We will follow the line from the beginning and use
	//the algorithm of dividing the number by 2 from Wikipedia
	//https://ru.wikipedia.org/wiki/%D0%94%D0%B5%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5_%D0%BD%D0%B0_%D0%B4%D0%B2%D0%B0
	size_t length = str.length();
	int32_t firstDigit = 0;
	int32_t newDigit = 0;

	for (unsigned int index = 0; index < length; ++index) {
		int32_t secondDigit = static_cast<int32_t>(str[index] - '0');

		switch (secondDigit) {
		case 1:
		case 0:
			newDigit = 0;
			break;
		case 2:
		case 3:
			newDigit = 1;
			break;
		case 4:
		case 5:
			newDigit = 2;
			break;
		case 6:
		case 7:
			newDigit = 3;
			break;
		case 8:
		case 9:
			newDigit = 4;
			break;
		default: throw std::invalid_argument(str);
		}
		if (firstDigit % 2 != 0) { newDigit += 5; }

		//We write the number in the line
		if (newDigit != 0) { flag = false; }
		str[index] = static_cast<char>(newDigit + '0');

		//Moving one position further
		firstDigit = secondDigit;
	}

	return flag;
}
void BigInteger::swap(BigInteger& other) {
	std::swap(this->sign_, other.sign_);
	std::swap(this->size_, other.size_);
	std::swap(this->maxsize_, other.maxsize_);
	std::swap(this->digits_, other.digits_);
}
void BigInteger::removeZeros() {
	for (int32_t index = size_ - 1; index > 0 && digits_[index] == 0; --index) {
		--size_;
	}
}
BigInteger BigInteger::ABS() const {
	BigInteger result(*this);
	result *= result.sign_;
	return result;
}
int32_t BigInteger::cmpABS(const BigInteger& other) const {
	//|this| < |other| => -1
	//|this| == |other| => 0
	//|this| > |other| =>  1

	if (this->size_ != other.size_) { return this->size_ < other.size_ ? -1 : 1; }
	for (int32_t index = this->size_ - 1; index >= 0; --index) {
		if (this->digits_[index] < other.digits_[index]) { return -1; }
		if (other.digits_[index] < this->digits_[index]) { return 1; }
	}
	return 0;
}
void BigInteger::multiplyStringByNumberSystem(std::string& str) {

	//Let's create a variable to store the length of the string
	size_t length = str.length();

	//We will multiply according to the same principle as in operator*=
	uint64_t product = 0;
	// only imagining that the multiplication goes by a single-digit number
	uint64_t remainder = 0;

	for (int32_t index = length - 1; index >= 0; --index) {
		//By bitwise shift we multiply the current digit by the base of the number system
		uint64_t product = (static_cast<uint64_t>(str[index] - '0') << 32) + remainder;
		//We write the last digit in the line
		str[index] = static_cast<char>((product % 10) + '0');
		//And everything else will remain in the remainder
		remainder = product / 10;
	}
	//If there is something left "in mind", then add it to the beginning of the line
	if (remainder != 0) { str = std::to_string(remainder) + str; }
}
void BigInteger::addNumberToString(std::string& str, uint32_t number) {

	//Let's create a variable to store the length of the string
	size_t length = str.length();

	//let's apply an algorithm similar to the addition algorithm in operator+=
	//(that is, just the usual column addition)
	uint32_t remainder = 0;
	uint32_t sum = 0;

	for (int32_t index = length - 1; index >= 0 && remainder + number != 0; --index) {
		//Calculate the sum of the corresponding digits,
		//taking into account the remainder from the previous iteration
		sum = number % 10 + static_cast<uint32_t>(str[index] - '0') + remainder;
		//The last digit will be just the number we need
		str[index] = static_cast<char>((sum % 10) + '0');
		//Everything else as usual goes into the remainder
		remainder = sum / 10;
		//We move on to the next digit of the added number
		number /= 10;
	}

	//It may turn out that the added number was large enough
	//that after the cycle there was something left to add
	sum = number + remainder;
	if (sum != 0) { str = std::to_string(sum) + str; }
}
BigInteger BigInteger::algorithmKnut(const BigInteger& other) {
	//----------INT GENERAL, WE WILL IMPLEMENT THE KNUT DIVISION ALGORITHM----------
	//https://vk.com/wall-54530371_113285 (volume 2 page 300)

	//The variable in which the division result will be stored
	BigInteger quotient;
	delete[] quotient.digits_;
	//As the algorithm says, the quotient will be of order (m + 1)
	quotient.maxsize_ = quotient.size_ = this->size_ - other.size_ + 1;
	quotient.digits_ = new uint32_t[quotient.maxsize_];
	//If the numbers have opposite signs, then the sign of the product must be negative,
	//and if the numbers have the same signs, then the sign must be positive
	//=> the product of signs and will give us the necessary
	quotient.sign_ = this->sign_ * other.sign_;

	//D1) First we calculate the value for normalization
	BigInteger d = (NUMBERSYSTEM / (static_cast<uint64_t>(other.digits_[other.size_ - 1]) + 1));
	//and then we normalize the divisible and the divisor
	BigInteger tmpDivisible = *this * d;
	BigInteger tmpDivisor = other * d;
	tmpDivisor.sign_ = 1;
	//Immediately apply static_cast, so as not to do this later in the loop many times
	uint64_t firstDigitDivisor = static_cast<uint64_t> (tmpDivisor.digits_[tmpDivisor.size_ - 1]);
	uint64_t secondDigitDivisor = static_cast<uint64_t> (tmpDivisor.digits_[tmpDivisor.size_ - 2]);

	//checking for a not-so-smart cpp-linter
	if (firstDigitDivisor == 0) { throw "Something went wrong"; }

	//We determine the length of our "window"
	const int32_t lengthWindow = other.size_ + 1;

	//D2)Assign j <- m and run the loop
	for (int32_t index = quotient.size_ - 1; index >= 0; --index) {

		//Again, we apply static_cast and save it to variables for their further use
		uint64_t firstDigitDivisible = static_cast<uint64_t> (tmpDivisible.digits_[index + other.size_]);
		uint64_t secondDigitDivisible = static_cast<uint64_t> (tmpDivisible.digits_[index + other.size_ - 1]);

		//D3)We are trying to guess the digits of the quotient and remainder fairly accurately
		uint64_t firstTwoDigitsDivisible = (firstDigitDivisible << 32) + secondDigitDivisible;
		uint64_t q = firstTwoDigitsDivisible / firstDigitDivisor;
		uint64_t r = firstTwoDigitsDivisible % firstDigitDivisor;
		//We correct the inaccuracy of the selected quotient, if they have arisen
		uint64_t thirdDigitDivisible = (index + other.size_ - 2 < tmpDivisible.size_) ?
			static_cast<uint64_t> (tmpDivisible.digits_[index + other.size_ - 2]) : 0;
		while (q == NUMBERSYSTEM || (q * secondDigitDivisor > (r << 32) + thirdDigitDivisible)) {
			--q;
			r += firstDigitDivisor;
			if (r >= NUMBERSYSTEM) { break; }
		}

		//D4)We make a replacement from the algorithm
		BigInteger tmpProduct = q * tmpDivisor;
		//This variable is needed in order to remember
		//how much we owe from the older category
		uint32_t credit = 0;
		//Let's create a variable in which
		//we will write the difference between the two digits
		uint32_t difference = 0;

		for (int32_t jndex = 0; jndex < lengthWindow; ++jndex) {
			//We remember our digits in separate variables
			uint32_t digitDivisible = tmpDivisible.digits_[index + jndex];
			uint32_t digitProduct = (jndex < tmpProduct.size_) ? tmpProduct.digits_[jndex] : 0;
			//Calculate the difference between the two corresponding digits
			difference = digitDivisible - digitProduct;
			//We put this difference in the corresponding category,
			//taking into account the debt from the previous iteration
			tmpDivisible.digits_[index + jndex] = difference - credit;
			//The final difference turned out to be more than the one being reduced =>
			//we subtracted too much value => we need to borrow from the senior category
			credit = (difference > digitDivisible) ? 1 : 0;
		}
		//As in the algorithm, we check the digits for negativity
		if (credit > 0) {
			//We replace each bit of the window with an addition to the number system
			tmpDivisible.digits_[index] = NUMBERSYSTEM - tmpDivisible.digits_[index];
			for (int32_t jndex = 1; jndex < lengthWindow; ++jndex) {
				tmpDivisible.digits_[index + jndex] = MAXVALUEDIGIT - tmpDivisible.digits_[index + jndex];
			}

			//D6)
			--q;
			//The variable in which we will put the result of adding the corresponding digits
			uint64_t sum = 0;
			//This variable is needed to remember the remainder after addition
			//(such as what we "keep in mind")
			uint32_t remainder = 0;
			for (int32_t jndex = 0; jndex < lengthWindow; ++jndex) {
				//Let's save the values in variables so that we don't write static_casts later
				uint64_t digitDivisible = static_cast<uint64_t>(tmpDivisible.digits_[jndex + index]);
				uint64_t digitDivisor = (jndex < tmpDivisor.size_) ? static_cast<uint64_t>(tmpDivisor.digits_[jndex]) : 0;
				//Calculate the sum of the corresponding digits
				sum = digitDivisible + digitDivisor + remainder;
				//Everything that fits, we put in the cell
				tmpDivisible.digits_[jndex + index] = sum & MAXVALUEDIGIT;
				//And if something does not fit, then we remember it in the remainder
				remainder = sum >> 32;
			}
		}

		//D5)
		//We substitute the found digit in the result
		quotient.digits_[index] = static_cast<uint32_t>(q);
	}

	//D8)
	//Saving the quotient in this
	quotient.removeZeros();
	this->swap(quotient);

	return tmpDivisible;
}

//Operator for the further to_string function
BigInteger::operator std::string() const {

	//Given that our number a_{n}a_{n-1}...a_1a_0, where a_i are the digits of the number,
	//can be represented as:
	//a_0*(2^32)^0 + a_1*(2^32)^1 + ... + a_{n-1}*(2^32)^{n-1} + a_{n}*(2^32)^{n}
	//we get the expression after alternately bracketing the expression 2^32:
	//a_0 + 2^32 * (a_1 + 2^32 * (a_2 + ... + 2^32 * (a_{n-1} + 2^32 * a_{n})...))     (*)

	std::string result = std::to_string(this->digits_[this->size_ - 1]);

	//It follows from the expression (*) that to get the resulting string,
	//we need (this-size_ - 1) to repeat two actions once:
	// 
	//1) Multiply the string by the base of the number system
	//2) Add the current digit

	for (int32_t index = this->size_ - 2; index >= 0; --index) {
		//1) Multiply the string by the base of the number system
		multiplyStringByNumberSystem(result);
		//2) Add the current digit
		addNumberToString(result, this->digits_[index]);
	}

	//We take into account the negativity of the number
	if (this->sign_ < 0) { result = std::string("-") + result; }

	return result;
}

//Comparison operators
bool operator< (const BigInteger& left, const BigInteger& right) {
	return left.isLess(right);
}
bool operator> (const BigInteger& left, const BigInteger& right) {
	return right.isLess(left);
}
bool operator<= (const BigInteger& left, const BigInteger& right) {
	return !right.isLess(left);
}
bool operator>= (const BigInteger& left, const BigInteger& right) {
	return !left.isLess(right);
}
bool operator== (const BigInteger& left, const BigInteger& right) {
	return left.isEqual(right);
}
bool operator!= (const BigInteger& left, const BigInteger& right) {
	return !left.isEqual(right);
}

//Arithmetic methods
BigInteger& BigInteger::operator+= (const BigInteger& other) {

	//To begin with , let 's consider a few special cases

	//Additions with zero
	if (this->sign_ == 0) {
		BigInteger tmp(other);
		this->swap(tmp);
		return *this;
	}
	if (other.sign_ == 0) { return *this; }

	//We find out the ratio of modules in our numbers
	int32_t lessABS = this->cmpABS(other);

	//Opposite numbers
	if (this->sign_ != other.sign_ && lessABS == 0) {
		BigInteger ZERO;
		this->swap(ZERO);
		return *this;
	}

	//If we have a common case, then we will create
	//a new BigInteger, where we will add the result
	BigInteger result;
	delete[] result.digits_;

	//We allocate in the result itself one more cell in reserve
	result.size_ = result.maxsize_ =
		(this->size_ < other.size_) ? other.size_ + 1 : this->size_ + 1;

	result.digits_ = new uint32_t[result.maxsize_];
	//We reset all the cells, then it will come in handy
	for (int32_t index = 0; index < result.maxsize_; ++index) {
		result.digits_[index] = 0;
	}

	if (this->sign_ == other.sign_) {
		//If the sign was the same, then the result will have the same sign
		result.sign_ = this->sign_;

		//The variable in which we will put the result of adding the corresponding digits
		uint64_t sum = 0;
		//This variable is needed to remember the remainder after addition
		//(such as what we "keep in mind")
		uint32_t remainder = 0;
		for (int32_t index = 0; index < result.size_; ++index) {
			//We are checking, since the numbers can be of arbitrary sizes
			uint64_t ourDigit = (index < this->size_) ? this->digits_[index] : 0;
			uint64_t anotherDigit = (index < other.size_) ? other.digits_[index] : 0;

			sum = ourDigit + anotherDigit + remainder;
			//Everything that fits, we put in the cell
			result.digits_[index] = sum & MAXVALUEDIGIT;
			//And if something does not fit, then we remember it in the remainder
			remainder = sum >> 32;
		}
	}
	else {
		if (lessABS > 0) {
			//If the signs were different, and the module this-BigInteger was larger,
			//then the result will have a sign like this-BigInteger
			//(for example, -5 + 3 = -2, where -5 -- this-BigInteger)
			result.sign_ = this->sign_;

			//This variable is needed in order to remember
			//how much we owe from the older category
			uint32_t credit = 0;
			//Let's create a variable in which
			//we will write the difference between the two digits
			uint32_t difference = 0;

			for (int32_t index = 0; index < this->size_; ++index) {
				uint32_t anotherDigit = (index < other.size_) ? other.digits_[index] : 0;

				difference = this->digits_[index] - anotherDigit;
				result.digits_[index] = difference - credit;
				//The final difference turned out to be more than the one being reduced =>
				//we subtracted too much value => we need to borrow from the senior category
				credit = (difference > this->digits_[index]) ? 1 : 0;
			}
			result.digits_[result.size_ - 1] -= credit;
		}
		else {
			//If the signs were different, and the module other-BigInteger was larger,
			//then the result will have a sign like other-BigInteger
			//(for example, 1 - 3 = -2, where -3 -- other-BigInteger)
			result.sign_ = other.sign_;

			//This variable is needed in order to remember
			//how much we owe from the older category
			uint32_t credit = 0;
			//Let's create a variable in which
			//we will write the difference between the two digits
			uint32_t difference = 0;

			for (int32_t index = 0; index < other.size_; ++index) {
				uint32_t ourDigit = (index < this->size_) ? this->digits_[index] : 0;

				difference = other.digits_[index] - ourDigit;
				result.digits_[index] = difference - credit;
				//The final difference turned out to be more than the one being reduced =>
				//we subtracted too much value => we need to borrow from the senior category
				credit = (difference > other.digits_[index]) ? 1 : 0;
			}
			result.digits_[result.size_ - 1] -= credit;
		}
	}
	//We remove the leading zeros and swap-аем
	result.removeZeros();
	this->swap(result);
	//Now the result lies in this-BigInteger
	return *this;
}
BigInteger& BigInteger::operator-= (const BigInteger& other) {
	//(a - b) -> -(-a + b)
	this->sign_ *= -1;
	*this += other;
	this->sign_ *= -1;
	return *this;
}
BigInteger& BigInteger::operator*= (const BigInteger& other) {

	//Let 's consider some special cases
	//Multiplying by zero or multiplying zero by something
	if (other == 0) {
		BigInteger ZERO(0);
		this->swap(ZERO);
		return *this;
	}
	if (*this == 0) { return *this; }

	//Multiplying by one or multiplying one by something
	if (other == 1) { return *this; }
	if (*this == 1) {
		*this = other;
		return *this;
	}

	//The result of multiplying our two numbers
	BigInteger result;
	delete[] result.digits_;

	//We will allocate memory with a large margin
	result.size_ = result.maxsize_ = this->size_ + other.size_ + 1;
	result.digits_ = new uint32_t[result.maxsize_];
	for (int32_t index = 0; index < result.size_; ++index) {
		result.digits_[index] = 0;
	}
	//If the numbers have opposite signs, then the sign of the product must be negative,
	//and if the numbers have the same signs, then the sign must be positive
	//=> the product of signs and will give us the necessary
	result.sign_ = this->sign_ * other.sign_;

	//Multiplication by a number with one digit
	if (other.size_ == 1) {
		//Creating a variable where we will record the result of multiplication
		uint64_t product = 0;
		//This variable is needed to remember the remainder after multiplication
		//(such as what we "keep in mind")
		uint32_t remainder = 0;
		for (int32_t index = 0; index < this->size_; ++index) {
			product = this->digits_[index];
			product *= other.digits_[0];
			product += remainder;

			//We save the first 32 bits from the end in the cell
			result.digits_[index] = product & MAXVALUEDIGIT;
			//And we put everything else in the remainder
			remainder = product >> 32;
		}
		result.digits_[this->size_] = remainder;

		//We remove the leading zeros and swap-аем
		result.removeZeros();
		this->swap(result);
		//Now the result lies in this-BigInteger
		return *this;
	}
	//The multiplication of a number with one digit is similar
	if (this->size_ == 1) {
		//Creating a variable where we will record the result of multiplication
		uint64_t product = 0;
		//This variable is needed to remember the remainder after multiplication
		//(such as what we "keep in mind")
		uint32_t remainder = 0;
		for (int32_t index = 0; index < other.size_; ++index) {
			product = other.digits_[index];
			product *= this->digits_[0];
			product += remainder;

			//We save the first 32 bits from the end in the cell
			result.digits_[index] = product & MAXVALUEDIGIT;
			//And we put everything else in the remainder
			remainder = product >> 32;
		}
		result.digits_[other.size_] = remainder;

		//We remove the leading zeros and swap-аем
		result.removeZeros();
		this->swap(result);
		//Now the result lies in this-BigInteger
		return *this;
	}

	//We proceed to multiplication in the general case
	for (int32_t thisDigit = 0; thisDigit < this->size_; ++thisDigit) {
		//Creating a variable where we will record the result of multiplication
		uint64_t product = 0;
		//This variable is needed to remember the remainder after multiplication
		//(such as what we "keep in mind")
		uint64_t remainder = 0;

		for (int32_t otherDigit = 0; otherDigit < other.size_; ++otherDigit) {
			product = static_cast<uint64_t>(this->digits_[thisDigit]);
			product *= static_cast<uint64_t>(other.digits_[otherDigit]);
			product += remainder;
			product += static_cast<uint64_t>(result.digits_[thisDigit + otherDigit]);

			//We save the first 32 bits from the end in the cell
			result.digits_[thisDigit + otherDigit] = product & MAXVALUEDIGIT;
			//And we put everything else in the remainder
			remainder = product >> 32;
		}

		result.digits_[thisDigit + other.size_] += remainder;
	}

	//We remove the leading zeros and swap-аем
	result.removeZeros();
	this->swap(result);
	//Now the result lies in this-BigInteger
	return *this;
}
BigInteger& BigInteger::operator/= (const BigInteger& other) {

	//Checking for division by zero
	if (other == 0) {
		throw "Oh, bro, don't divide by zero!";
	}
	//We also check some special cases
	if (*this == 0) { return *this; }
	//We create a variable to store the relationship of modules
	int32_t lessABS = this->cmpABS(other);
	//If the divisor is greater than the divisible,
	//then the quotient will be zero
	if (lessABS < 0) {
		BigInteger ZERO;
		this->swap(ZERO);
		return *this;
	}
	//If modulo the divisor and the divisible
	//coincide, then you get 1 or -1
	if (lessABS == 0) {
		BigInteger ONE(this->sign_ * other.sign_);
		this->swap(ONE);
		return *this;
	}

	//Dividing a long number by a short number
	//https://neerc.ifmo.ru/wiki/index.php?title=%D0%90%D1%80%D0%B8%D1%84%D0%BC%D0%B5%D1%82%D0%B8%D0%BA%D0%B0_%D1%87%D0%B8%D1%81%D0%B5%D0%BB_%D0%B2_b-%D0%B8%D1%87%D0%BD%D0%BE%D0%B9_%D1%81%D0%B8%D1%81%D1%82%D0%B5%D0%BC%D0%B5_%D1%81%D1%87%D0%B8%D1%81%D0%BB%D0%B5%D0%BD%D0%B8%D1%8F_%28%D0%94%D0%BB%D0%B8%D0%BD%D0%BD%D0%B0%D1%8F_%D0%B0%D1%80%D0%B8%D1%84%D0%BC%D0%B5%D1%82%D0%B8%D0%BA%D0%B0%29
	if (other.size_ == 1) {
		//This variable is needed to remember the remainder after multiplication
		//(such as what we "keep in mind")
		uint32_t remainder = 0;
		//This variable is used to store the value
		uint64_t current = 0;

		for (int32_t index = this->size_ - 1; index >= 0; --index) {
			current = this->digits_[index];
			current += static_cast<uint64_t> (remainder) << 32;

			this->digits_[index] = static_cast<uint32_t>(current / other.digits_[0]);
			remainder = current % other.digits_[0];
		}
		//If the numbers have opposite signs, then the sign of the product must be negative,
		//and if the numbers have the same signs, then the sign must be positive
		//=> the product of signs and will give us the necessary
		this->sign_ *= other.sign_;
		//We remove the leading zeros
		this->removeZeros();
		//Now the result lies in this-BigInteger
		return *this;
	}

	this->algorithmKnut(other);
	return *this;
}
BigInteger& BigInteger::operator%= (const BigInteger& other) {

	//Checking for division by zero
	if (other == 0) {
		throw "Oh, bro, don't divide by zero!";
	}
	//We also check some special cases
	if (*this == 0) { return *this; }

	//We create a variable to store the relationship of modules
	int32_t lessABS = this->cmpABS(other);
	//If the divisor is greater than the divisible,
	//then the remainder is equal to the divisible itself
	if (lessABS < 0) { return *this; }
	//If the divisible and the divisor are equal in modulus,
	//then the remainder of the division is zero
	if (lessABS == 0) {
		BigInteger ZERO;
		this->swap(ZERO);
		return *this;
	}

	//Dividing a long number by a short number
	//https://neerc.ifmo.ru/wiki/index.php?title=%D0%90%D1%80%D0%B8%D1%84%D0%BC%D0%B5%D1%82%D0%B8%D0%BA%D0%B0_%D1%87%D0%B8%D1%81%D0%B5%D0%BB_%D0%B2_b-%D0%B8%D1%87%D0%BD%D0%BE%D0%B9_%D1%81%D0%B8%D1%81%D1%82%D0%B5%D0%BC%D0%B5_%D1%81%D1%87%D0%B8%D1%81%D0%BB%D0%B5%D0%BD%D0%B8%D1%8F_%28%D0%94%D0%BB%D0%B8%D0%BD%D0%BD%D0%B0%D1%8F_%D0%B0%D1%80%D0%B8%D1%84%D0%BC%D0%B5%D1%82%D0%B8%D0%BA%D0%B0%29
	if (other.size_ == 1) {
		//This variable is needed to remember the remainder after multiplication
		//(such as what we "keep in mind")
		uint32_t remainder = 0;
		//This variable is used to store the value
		uint64_t current = 0;

		for (int32_t index = this->size_ - 1; index >= 0; --index) {
			current = this->digits_[index];
			current += static_cast<uint64_t> (remainder) << 32;

			remainder = current % other.digits_[0];
		}
		//If the numbers have opposite signs, then the sign of the product must be negative,
		//and if the numbers have the same signs, then the sign must be positive
		//=> the product of signs and will give us the necessary
		int32_t resultSign = (remainder != 0) ? this->sign_ : 0;
		//The "remainder" variable will store the response
		*this = BigInteger(remainder);
		this->sign_ = resultSign;
		//Now the result lies in this-BigInteger
		return *this;
	}
	//Calling the division method, which returns the remainder
	*this = this->algorithmKnut(other);

	//Creating a variable in which we put the remainder of the division
	BigInteger remainder;
	delete[] remainder.digits_;

	remainder.maxsize_ = remainder.size_ = other.size_;
	remainder.sign_ = this->sign_;

	//rewriting the last other.size_ digits in the remainder
	remainder.digits_ = new uint32_t[remainder.maxsize_];
	for (int32_t index = 0; index < remainder.maxsize_; ++index) {
		remainder.digits_[index] = this->digits_[index];
	}
	remainder.removeZeros();
	//The remainder will be just the last other.size_ digits of the
	//divisible, divided by the normalization value
	*this = remainder / BigInteger(NUMBERSYSTEM / (static_cast<uint64_t>(other.digits_[other.size_ - 1]) + 1));
	return *this;
}

//Arithmetic operators
BigInteger operator+ (const BigInteger& left, const BigInteger& right) {
	//Creating a copy from the left argument
	BigInteger result(left);
	//We add the right argument to the copy
	result += right;
	//We get the result
	return result;
}
BigInteger operator- (const BigInteger& left, const BigInteger& right) {
	//Creating a copy from the left argument
	BigInteger result(left);
	//Subtract the right argument from the copy
	result -= right;
	//We get the result
	return result;
}
BigInteger operator* (const BigInteger& left, const BigInteger& right) {
	//Creating a copy from the left argument
	BigInteger result(left);
	//Multiply the right argument by a copy
	result *= right;
	//We get the result
	return result;
}
BigInteger operator/ (const BigInteger& left, const BigInteger& right) {
	//Creating a copy from the left argument
	BigInteger result(left);
	//Divide the copy by the right argument
	result /= right;
	//We get the result
	return result;
}
BigInteger operator% (const BigInteger& left, const BigInteger& right) {
	//Creating a copy from the left argument
	BigInteger result(left);
	//Calculate the remainder of dividing the copy by the right argument
	result %= right;
	//We get the result
	return result;
}

//Function for translating BigInteger to a line
std::string to_string(const BigInteger& BigInt) {
	return (std::string)BigInt;
}