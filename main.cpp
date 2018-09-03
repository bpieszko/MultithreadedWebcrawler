#include <iostream>
#include "MultithreadedWebcrawler.hpp"

int main (int argc, char * argv[]) {
	std::cout << argc << std::endl;
	if (argc != 5) {
		std::cout << "Usage:" << std::endl;
		std::cout << "\t./app.e [url] [regex] [depth] [threads number]" << std::endl;
		std::cout << "Description:" << std::endl;
		std::cout << "\t[url] - url of page to start crawling" << std::endl;
		std::cout << "\t[regex] - regex of strings to search for" << std::endl;
		std::cout << "\t[depth] - depth of crawling (0 - default)" << std::endl;
		std::cout << "\t[threads number] - number of threads to use" << std::endl;
		std::cout << "Example:" << std::endl;
		std::cout << "\t./app.e 'https://panoramafirm.pl/szukaj?k=notariusz' '[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+' 2 4" << std::endl;
		return 1;
	}

	MultithreadedWebcrawler mwc(argv[1], argv[2], std::stoi(argv[3]), std::stoi(argv[4]));
	mwc.crawl();

	return 0;
}