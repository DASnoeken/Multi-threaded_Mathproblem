#include <iostream>
#include <cstring>
#include <cmath>
#include <array>
#include <charconv>

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
		std::cout << "Step: " << count << " value: " << ans << std::endl;
	}while(trunc(log10(ans)) + 1 > 1);
	return count;
}

int main(int argc, char** argv) {
	unsigned long long number;
	if (argc != 2) {
		std::cout << "Need number to check!\n";
		return -1;
	}
	else {
		number = std::atoll((const char*) argv[1]);
	}
	std::cout << "Number picked: " << number << "\n";
	int result = checkNum(number);
	std::cout << "Result = " << result << "\n";
}