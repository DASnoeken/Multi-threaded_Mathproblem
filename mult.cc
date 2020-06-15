#include <iostream>
#include <cstring>
#include <cmath>
#include <thread>
#include <mutex>
#include <array>
#include <charconv>

//Compile with  : g++    -o mult mult.cc -std=c++17 -pthread
//For debugging : g++ -g -o mult mult.cc -std=c++17 -pthread
//For extra speed use: c++ -O3 -o mult mult.cc -std=c++17 -pthread

int totalcount = 0;
std::mutex mtx;
std::mutex mu;

void findNums(unsigned long &i_min, unsigned long &i_max, unsigned short &thres_ans, unsigned short &ID){
	std::unique_lock<std::mutex> ul(mu);
	std::cout << "Thread with ID " << ID << " started! i_min=" << i_min << ". i_max = " << i_max << "\n";
	ul.unlock();
	unsigned int ans = 1;
	unsigned short count = 0;
	unsigned int totalCountInside = 0;
	std::array <char, 15> str;
	for (unsigned long i = i_min; i <= i_max; i++){
		std::to_chars(str.data(), str.data() + str.size(), i);
		int i_size = trunc(log10(i))+1;
		std::string_view str_i(str.data(),i_size);
		if (str_i.find("5") != std::string::npos && (str_i.find("2") != std::string::npos || str_i.find("4") != std::string::npos || str_i.find("6") != std::string::npos || str_i.find("8") != std::string::npos) || str_i.find("0") != std::string::npos){
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
			totalCountInside++;
		}
	}
	ul.lock();
	std::cout << "Total count = " << totalCountInside << ", ID: " << ID << "\n";
	totalcount = totalcount + totalCountInside;
	ul.unlock();

}

int main(int argc, char **argv){
	std::cout << "\033[1;32mNOTE\033[0m Maximum number of threads should not exceed " << std::thread::hardware_concurrency() << " on this machine." << std::endl;
	unsigned short hc = std::thread::hardware_concurrency()/2; //Amount of threads used. Don't make this number too high, because that will harm efficiency. 
	std::thread counters[hc];
	unsigned long dif;
	unsigned short a, ta;
	unsigned short *idarr = new unsigned short[hc]; //Using arrays in this way really keeps the threads organized properly
	unsigned long *maxArr = new unsigned long[hc];
	unsigned long *minArr = new unsigned long[hc];
	if (argc > 3){
		dif = std::atol((const char *)argv[3]);
		std::cout << "Interval max was set to: " << dif << std::endl;
	}
	else{
		dif = 100000;
		std::cout << "Size of interval set automatically to " << dif << "." << std::endl;
	}
	if (argc > 2){
		minArr[0] = std::atol((const char *)argv[2]);
		std::cout << "Start of interval set to: " << minArr[0] << std::endl;
	}
	else{
		minArr[0] = 1111;
		std::cout << "Starting point was automatically set to " << minArr[0] << "." << std::endl;
	}
	maxArr[0] = floor(dif / hc);
	for (unsigned long idc = 0; idc < hc; idc++){
		idarr[idc] = (short) idc + 1;
		if (idc > 0){
			minArr[idc] = maxArr[idc - 1] + 1;
			maxArr[idc] = floor((idc + 1) * dif / hc);
		}
	}
	if (argc > 1){
		ta = std::atoi((const char *)argv[1]); //Printing threshold value for count variable in findNums().
	}
	else{
		std::cout << "Printing threshold automatically set to 9!" << std::endl;
		ta = 9;
	}
	for (a = 0; a < hc; a++){
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
