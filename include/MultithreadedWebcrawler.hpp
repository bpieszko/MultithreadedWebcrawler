#ifndef MULTITHREADEDWEBCRAWLER_HPP
#define MULTITHREADEDWEBCRAWLER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <regex>
#include <thread>
#include <mutex>
#include <set>
#include <curl/curl.h>

class MultithreadedWebcrawler {
private:
	const std::regex url_regex;

	std::string start_url;
	std::regex search_regex;
	int depth;
	int threads_number;
	int used_threads;
	int looking_for;

	std::vector<std::thread> threads;
	std::queue<std::pair<std::string, int>> awaiting_urls;
	std::set<std::string> visited_urls;
	std::set<std::string> visited_match;

	std::mutex awaiting_urls_mutex;
	std::mutex out_mutex;
	std::mutex visited_urls_mutex;
	std::mutex visited_match_mutex;

	virtual void setOptCURL(CURL * curl, const std::string & url, std::string * result);
	static size_t WriteCallback(void * contents, size_t size, size_t nmemb, void * userp);

	virtual void threadCrawl();

public:
	MultithreadedWebcrawler(const std::string & _start_url, const std::string & _search_regex, const int _depth, const int _threads_number);
	virtual void crawl();

	virtual void setStartUrl(const std::string & _start_url);
	virtual std::string getStartUrl() const;

	virtual void setRegex(const std::string & _search_regex);
	virtual std::regex getRegex() const;

	virtual void setThreadsNumber(const int _threads_number);
	virtual int getThreadsNumber() const;

	virtual void setDepth(const int _depth);
	virtual int getDepth() const;
};

#endif // MULTITHREADEDWEBCRAWLER_HPP