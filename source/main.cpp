#include <iostream>
#include "Crawler.hpp"
#include "Config.hpp"

#define ERROR_RETURN 1
#define CORRECT_RETURN 0

const std::string getHelp()
{
	return 
		"Usage:\n"
		"\t./app.e [config]\n"
		"Description:\n"
		"\t[config] - path to config file\n"
		"Example:\n"
		"\t./app.e config.cfg";
}

int main (int argc, char * argv[])
{
	if (argc != 2)
	{
		std::cout << getHelp() << std::endl;
		return ERROR_RETURN;
	}

	try
	{
		Config cfg(argv[1]);

		Crawler crawler(cfg);
		crawler.crawl();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
		return ERROR_RETURN;
	}
	

	return CORRECT_RETURN;
}