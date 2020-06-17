#include <iostream>
#include <cstring>
#include <cmath>
#include <thread>
#include <mutex>
#include <array>
#include <charconv>
#include <vector>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

//Compile with  : g++    -o mult mult.cc -std=c++17 -pthread
//For debugging : g++ -g -o mult mult.cc -std=c++17 -pthread
//For extra speed use: c++ -O3 -o mult mult.cc -std=c++17 -pthread
// For debug: 290300 numbers with a count value >=9 for 11 999999999

static int totalcount = 0;
static std::mutex mtx;
static std::mutex mu;
static int maxInterval;

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

void findNums(unsigned long long &i_min, unsigned long long &i_max, unsigned short &thres_ans, unsigned short &ID){ //The core function of the program
	std::unique_lock<std::mutex> ul(mu);
	std::cout << "Thread with ID " << ID << " started! i_min=" << i_min << ". i_max = " << i_max << "\n";
	ul.unlock();
	unsigned int ans = 1;
	unsigned short count = 0;
	unsigned int totalCountInside = 0;
	std::array <char, 25> str;
	for (unsigned long long i = i_min; i <= i_max; i++){
		std::to_chars(str.data(), str.data() + str.size(), i);
		long i_size = trunc(log10(i))+1;
		std::string_view str_i(str.data(),i_size);
		if (str_i.find("0") != std::string::npos || 
			str_i.find("5") != std::string::npos &&
			(str_i.find("2") != std::string::npos || str_i.find("4") != std::string::npos || str_i.find("6") != std::string::npos || str_i.find("8") != std::string::npos) ||
			checkNum(str_i)){
			continue;
		}
		count = 0;
		do{
			ans = 1;
			for (unsigned long j = 0; j < i_size; j++){
				ans *= (str_i[j]-'0');
			}
			std::to_chars(str.data(), str.data() + str.size(), ans);
			i_size = trunc(log10(ans))+1;
			std::string_view str_i(str.data(),i_size);
			count++;
		} while (trunc(log10(ans)) + 1 > 1);
		if (count >= thres_ans){
			//ul.lock();
			//std::cout << i << std::endl;
			//ul.unlock();
			totalCountInside += factorial_sp(i);
		}
	}
	ul.lock();
	//std::cout << "Total count = " << totalCountInside << ", ID: " << ID << "\n";
	totalcount = totalcount + totalCountInside;
	ul.unlock();

}

int main(int argc, char **argv){
	std::cout << "\033[1;32mNOTE\033[0m Maximum number of threads should not exceed " << std::thread::hardware_concurrency() << " on this machine." << std::endl;
	unsigned short hc = std::thread::hardware_concurrency()/2; //Amount of threads used. Don't make this number too high, because that will harm efficiency. 
	std::thread counters[hc];
	unsigned long long dif;
	unsigned short a, ta;
	unsigned short *idarr = new unsigned short[hc]; //Using arrays in this way really keeps the threads organized properly
	unsigned long long *maxArr = new unsigned long long[hc];
	unsigned long long*minArr = new unsigned long long[hc];
	if (argc > 3){
		dif = std::atol((const char *)argv[2]);
		std::cout << "Interval max was set to:  " << dif << std::endl;
	}
	else{
		dif = 100000;
		std::cout << "Size of interval set automatically to " << dif << "." << std::endl;
	}
	maxInterval = dif;
	if (argc > 2){
		minArr[0] = std::atol((const char *)argv[1]);
		std::cout << "Start of interval set to: " << minArr[0] << std::endl;
	}
	else{
		minArr[0] = 1111;
		std::cout << "Starting point was automatically set to " << minArr[0] << "." << std::endl;
	}
	unsigned long long intervalSize = dif - minArr[0];
	maxArr[0] = minArr[0] + floor(intervalSize / hc);
	for (unsigned short idc = 0; idc < hc; idc++){
		idarr[idc] = idc + 1;
		if (idc > 0){
			minArr[idc] = maxArr[idc - 1] + 1;
			maxArr[idc] = minArr[idc] + floor(intervalSize / hc);
			
		}
	}
	if (argc > 1){
		ta = std::atoi((const char *)argv[3]); //Printing threshold value for count variable in findNums().
	}
	else{
		std::cout << "Printing threshold automatically set to 9!" << std::endl;
		ta = 9;
	}
	std::cout << hc << " Threads will be started!" << '\n';
	for (a = 0; a < hc; a++){
		//std::cout << a << " ---> " << minArr[a] << " ---> " << maxArr[a] << "\n";
		counters[a] = std::thread{findNums, std::ref(minArr[a]), std::ref(maxArr[a]), std::ref(ta), std::ref(idarr[a])};
	}
	for (a = 0; a < hc; a++){
		counters[a].join();
	}
	std::cout << "\n" << "Found a total of " << totalcount << " numbers with a count value >=" << ta << std::endl;
	delete[] idarr;
	delete[] maxArr;
	delete[] minArr;
}
