#include "stdafx.h"
#include <future>
#include <ctime>
#include<iostream>
#include <locale>
#include <iomanip>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <fstream>
#include <streambuf>
class timer_callback
{
public:
	template <class F, class ... Args>
	timer_callback(int after, bool async, F&& f, Args&&... args)
	{
		using return_type = typename std::result_of<F(Args...)>::type;
		
		std::function<return_type()> task(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

		if (async)
		{
			std::thread([after, task]() {
				std::this_thread::sleep_for(std::chrono::seconds(after));
				task();

			}).detach();
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::seconds(after));
			task();
		}
	}
};

void check(void)
{
	std::cout << "this is asynchronous :";
	std::time_t now = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&now)) << std::endl;
}

double getTimDifference(std::string schedule_time)
{
	std::tm tmTime = boost::posix_time::to_tm(boost::posix_time::time_from_string(schedule_time));
	std::time_t later = mktime(&tmTime);	
	//std::cout << later <<std::endl;

	std::time_t now = std::time(nullptr);
	//std::cout <<  std::asctime(std::localtime(&now)) << std::endl;
	auto number = std::difftime(later, now);
	//std::cout << number << std::endl;
	return number;
}

void readFile(std::string inputfile, std::vector<std::string>& list)
{
	std::ifstream f(inputfile);	
	std::string line;

	while (std::getline(f, line))
	{
		list.emplace_back(line);
	}		
}

int main()

{
	std::vector<std::string> input_time_list;
	readFile("C:\\Work\\builds\\date_time_bkcup.txt", input_time_list);

	for (auto l : input_time_list)
	{
		if (l.size() < 10)
			continue;

		auto diff = getTimDifference(l);
		if (diff > 0)
			timer_callback t(getTimDifference(l), true, &check);
	}
	//Other work
	while (1) {};
}