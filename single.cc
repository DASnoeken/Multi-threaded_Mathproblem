#include <iostream>
#include <cstring>
#include <cmath>
#include <array>
#include <charconv>
#include <climits>

int checkNum(unsigned long long& num) {
	unsigned long long ans = 1;
	std::array <char, 25> str;
	std::to_chars(str.data(), str.data() + str.size(), num);
	int i_size = trunc(log10(num)) + 1;
	std::string_view str_i(str.data(), i_size);
	int count = 0;
	do {
		ans = 1;
		for (unsigned int j = 0; j < i_size; j++) {
			ans *= (str_i[j] - '0');
		}
		std::to_chars(str.data(), str.data() + str.size(), ans);
		i_size = trunc(log10(ans)) + 1;
		std::string_view str_i(str.data(), i_size);
		count++;
		std::cout << "Step: " << count << " value: " << ans << '\n';
	}while(trunc(log10(ans)) + 1 > 1);
	return count;
}

int checkNum(const std::string& num) {	//String implementation for extremely large numbers. Use with caution
	unsigned long long ans = 1;
	int count = 0;
	std::string num2 = num;
	std::cout << "\033[1;31mWARNING! I'm doing something very dangerous right now!\nI have no idea if this is an effective way of treating large numbers like this!\033[0m" << '\n';
	do {
		ans = 1;
		for (int j = 0; j < num2.length(); j++) {
			ans *= (num2[j] - '0');
		}
		if (ans == ULLONG_MAX) {
			std::cout << "\033[1;31mWARNING! First step generated ULLONG_MAX, answer might be incorrect!\033[0m" << '\n';
		}
		num2 = std::to_string(ans);
		count++;
		std::cout << "Step: " << count << " value: " << ans << '\n';
	} while (num2.length() > 1);
	return count;
}

int main(int argc, char** argv) {
	unsigned long long number;
	std::cout << "\033[1;32mPlease note that your input should not exceed: " << ULLONG_MAX << "\033[0m" << std::endl;
	if (argc != 2) {
		std::cout << "Need number to check!\n";
		return -1;
	}
	else {
		const char* input = (const char*)argv[1];
		char* iEnd;
		number = std::strtoull(input,&iEnd,10);
		if (number == ULLONG_MAX) {
			std::cout << "\033[1;31mNumber was larger than or equal to ULLONG_MAX!\033[0m" << '\n';
			std::cout << "Number picked: " << input << "\n";
			int result = checkNum(std::string(input));
			std::cout << "Result = " << result << std::endl;
			return 0;
		}
	}
	std::cout << "Number picked: " << number << "\n";
	int result = checkNum(number);
	std::cout << "Result = " << result << "\n";
}