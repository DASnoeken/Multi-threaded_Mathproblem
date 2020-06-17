#include <iostream>
#include <cstring>
#include <cmath>
#include <thread>
#include <mutex>
#include <array>
#include <charconv>
#include <vector>
#include <chrono>
#include <climits>
#include <string>
typedef std::chrono::high_resolution_clock Clock;

#ifndef MULT_HH
#define MULT_HH
class Mult {
public:
	Mult() {
		
	}

	void single(std::string inputNumber) {
		unsigned long long number;
		std::cout << "\033[1;32mPlease note that your input should not exceed: " << ULLONG_MAX << "\033[0m" << std::endl;
		const char* input = inputNumber.c_str();
		std::cout << input << std::endl;
		char* iEnd;
		number = std::strtoull(input, &iEnd, 10);
		if (number == ULLONG_MAX) {
			std::cout << "\033[1;31mNumber was larger than or equal to ULLONG_MAX!\033[0m" << '\n';
			std::cout << "Number picked: " << input << "\n";
			int result = s_checkNum(std::string(input));
			std::cout << "Result = " << result << std::endl;
			return;
		}
		
		std::cout << "Number picked: " << number << "\n";
		int result = s_checkNum(number);
		std::cout << "Result = " << result << "\n";
	}

	void interval(unsigned long long minimum, unsigned long long maxInterval,unsigned short threshold,int numThreads) {
		if (numThreads <= 0) {
			std::cout << "\033[1;31mIllegal number of threads!\033[0m" << std::endl;
			return;
		}
		if (threshold <= 1) {
			std::cout << "\033[1;31mIllegal threshold value!\033[0m" << std::endl;
			return;
		}
		if (minimum<0 || minimum>=maxInterval) {
			std::cout << "\033[1;31mIllegal interval bounds!\033[0m" << std::endl;
			return;
		}
		std::cout << "\033[1;32mNOTE\033[0m Maximum number of threads should not exceed " << std::thread::hardware_concurrency() << " on this machine." << std::endl;
		unsigned short hc = numThreads; //Amount of threads used. Don't make this number too high, because that will harm efficiency. 
		if (hc > std::thread::hardware_concurrency()) {
			hc = std::thread::hardware_concurrency();
			std::cout << "Number of threads has been set to " << std::thread::hardware_concurrency() << std::endl;
		}
		//std::thread* counters = new std::thread[hc];
		std::vector<std::thread> counters;
		counters.reserve(hc);
		unsigned short a;
		unsigned short ta = threshold;
		unsigned short* idarr = new unsigned short[hc]; //Using arrays in this way really keeps the threads organized properly
		unsigned long long* maxArr = new unsigned long long[hc];
		unsigned long long* minArr = new unsigned long long[hc];
		unsigned long long intervalSize = maxInterval - minimum;
		minArr[0] = minimum;
		maxArr[0] = minArr[0] + floor(intervalSize / hc);
		for (unsigned short idc = 0; idc < hc; idc++) {
			idarr[idc] = idc + 1;
			if (idc > 0) {
				minArr[idc] = maxArr[idc - 1] + 1;
				maxArr[idc] = minArr[idc] + floor(intervalSize / hc);
			}
		}
		std::cout << hc << " Threads will be started!" << '\n';
		for (a = 0; a < hc; a++) {
			//std::cout << a << " ---> " << minArr[a] << " ---> " << maxArr[a] << "\n";
			counters.emplace_back(std::thread(&Mult::findNums, this, std::ref(minArr[a]), std::ref(maxArr[a]), std::ref(ta), std::ref(idarr[a])));
		}
		for (a = 0; a < hc; a++) {
			counters[a].join();
		}
		std::cout << "\n" << "Found a total of " << totalcount << " numbers with a count value >=" << ta << std::endl;
		delete[] idarr;
		delete[] maxArr;
		delete[] minArr;
	}

private:
	int totalcount = 0;
	std::mutex mtx;
	std::mutex mu;
	int maxInterval = 0;

	int s_checkNum(const std::string& num) {	//This part of the code doesn't have to be efficient
		unsigned long long ans = 1;
		unsigned long long ans2 = ans;
		int count = 0;
		std::string num2 = num;		
		std::cout << "\033[1;31mWARNING! I'm doing something very dangerous right now!\nI have no idea if this is an effective way of treating large numbers like this!\033[0m" << '\n';
		std::cout << num2 << std::endl;
		/*std::vector<int> v_num;
		v_num.reserve(num.length());
		for (int j = 0; j < num.length(); j++) {
			v_num.emplace_back(num2[j] - '0');
		}*/
		do {
			ans = 1;
			ans2 = ans;
			for (int j = 0; j < num2.length(); j++) {
				ans *= (num2[j] - '0');
				if (ans < ans2 && (num2[j] - '0')>=1) {
					std::cout << "WARNING! Suspicious behavior! "<< j << ": " <<ans<<" < "<<ans2<< std::endl;
					std::cout << "I\'m not able to do this number at this point." << std::endl;
					return -1;
				}
				ans2 = ans;
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

	int s_checkNum(unsigned long long& num) {
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
		} while (trunc(log10(ans)) + 1 > 1);
		return count;
	}

	int factorial(const int& N) {							//Calculates N!
		if (N == 0 || N == 1) {
			return 1;
		}
		int ans = 1;
		for (int i = N; i > 1; i--) {
			ans *= i;
		}
		return ans;
	}

	int factorial_sp(const unsigned long long& j) {			//Calculates N!/(P!Q!R!...)
		//auto t1 = Clock::now();
		int N = trunc(log10(j)) + 1;
		int* jArr = new int[N];
		for (int i = 0; i < N; i++) {
			jArr[i] = j / (int)pow(10, i) % 10;
		}
		/*
		for (int i = 0; i < N; i++) {
			std::cout << jArr[i] << " ";
		}
		std::cout << "\n";
		*/

		std::vector<int> countVec;
		countVec.reserve(N);
		int count = 1;
		for (int i = 1; i < N; i++) {
			if (jArr[i] == jArr[i - 1]) {
				count++;
			}
			else {
				countVec.emplace_back(count);
				count = 1;
			}
		}
		countVec.emplace_back(count);
		int ans = factorial(N);
		std::vector<int>::iterator iter;
		for (iter = countVec.begin(); iter != countVec.end(); ++iter) {
			if (*iter != 0 && *iter != 1) {
				ans /= factorial(*iter);
			}
		}
		delete[] jArr;

		//auto t2 = Clock::now();
		//std::cout << "Delta t2-t1: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() << " nanoseconds" << '\n';
		return ans;
	}

	bool checkNum(std::string_view number) {			//We only need to check combinations of digits once. 39 has the same result as 93, now only 39 is checked and 93 is skipped
		int tmp = number[0] - '0';
		int chk;
		for (unsigned short i = 1; i < number.length(); i++) {
			chk = number[i] - '0';
			if (chk < tmp) {
				return true;
			}
			else {
				tmp = chk;
			}
		}
		return false;
	}

	void findNums(unsigned long long& i_min, unsigned long long& i_max, unsigned short& thres_ans, unsigned short& ID) { //The core function of the program
		std::unique_lock<std::mutex> ul(mu);
		ul.unlock();
		unsigned int ans = 1;
		unsigned short count = 0;
		unsigned int totalCountInside = 0;
		std::array <char, 25> str;
		for (unsigned long long i = i_min; i <= i_max; i++) {
			std::to_chars(str.data(), str.data() + str.size(), i);
			long i_size = trunc(log10(i)) + 1;
			std::string_view str_i(str.data(), i_size);
			if (str_i.find("0") != std::string::npos ||
				str_i.find("5") != std::string::npos &&
				(str_i.find("2") != std::string::npos || str_i.find("4") != std::string::npos || str_i.find("6") != std::string::npos || str_i.find("8") != std::string::npos) ||
				checkNum(str_i)) {
				continue;
			}
			count = 0;
			do {
				ans = 1;
				for (long j = 0; j < i_size; j++) {
					ans *= (str_i[j] - '0');
				}
				std::to_chars(str.data(), str.data() + str.size(), ans);
				i_size = trunc(log10(ans)) + 1;
				std::string_view str_i(str.data(), i_size);
				count++;
			} while (trunc(log10(ans)) + 1 > 1);
			if (count >= thres_ans) {
				totalCountInside += factorial_sp(i);
			}
		}
		ul.lock();
		totalcount = totalcount + totalCountInside;
		ul.unlock();

	}
};
#endif // !MULT_HH
