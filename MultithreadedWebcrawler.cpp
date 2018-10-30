#include "MultithreadedWebcrawler.hpp"

MultithreadedWebcrawler::MultithreadedWebcrawler(const std::string & _start_url, const std::string & _search_regex, const int _depth, const int _threads_number) 
	: url_regex("(http|ftp|https)://([\\w_-]+(?:(?:\\.[\\w_-]+)+))([\\w.,@?^=%&:/~+#-]*[\\w@?^=%&/~+#-])?"){
	
	start_url = _start_url;
	search_regex = _search_regex;
	depth = _depth;
	threads_number = _threads_number;
	looking_for = 0;

	curl_global_init(CURL_GLOBAL_ALL);
}

void MultithreadedWebcrawler::crawl() {
	visited_urls.clear();

	awaiting_urls.push({start_url, 0});
	visited_urls.insert(start_url);

	for (int i = 0; i < threads_number; ++i) {
		std::thread t(&MultithreadedWebcrawler::threadCrawl, this);
		threads.push_back(std::move(t));
	}
	for (int i = 0; i < threads_number; ++i)
		threads[i].join();
}

void MultithreadedWebcrawler::threadCrawl() {
	while (1) {
		awaiting_urls_mutex.lock();
		if (looking_for && awaiting_urls.empty()) {
			awaiting_urls_mutex.unlock();
			continue;
		}
		if (awaiting_urls.empty()) {
			awaiting_urls_mutex.unlock();
			break;
		}
		++looking_for;
		std::pair<std::string, int> act_url = awaiting_urls.front();
		awaiting_urls.pop();

		awaiting_urls_mutex.unlock();

		if (act_url.first.substr(0, 8) == "https://")
			act_url.first.replace(0, 8, "");
		if (act_url.first.substr(0, 7) == "http://")
			act_url.first.replace(0, 7, "");
		if (act_url.first.substr(0, 4) != "www.")
			act_url.first = "www." + act_url.first;

		std::string result;
		CURL * curl = curl_easy_init();

		if (!curl) {
			out_mutex.lock();
			std::cerr << "Error: cannot init curl!" << std::endl;
			out_mutex.unlock();
			return;
		}

		setOptCURL(curl, act_url.first, &result);
		CURLcode rest = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		auto matches_begin = std::sregex_iterator(result.begin(), result.end(), search_regex);
		auto matches_end = std::sregex_iterator();

		for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
			visited_match_mutex.lock();
			if (visited_match.find(i->str()) == visited_match.end()) {
				out_mutex.lock();
				std::cout << std::this_thread::get_id() << ": " << i->str() << std::endl;
				out_mutex.unlock();
				visited_match.insert(i->str());
			}
			visited_match_mutex.unlock();
		}

		auto urls_begin = std::sregex_iterator(result.begin(), result.end(), url_regex);
		auto urls_end = std::sregex_iterator();

		for (std::sregex_iterator i = urls_begin; i != urls_end; ++i) {
			visited_urls_mutex.lock();
			if (act_url.second < depth && visited_urls.find(i->str()) == visited_urls.end()) {
				awaiting_urls_mutex.lock();
				awaiting_urls.push({i->str(), act_url.second + 1});
				awaiting_urls_mutex.unlock();
				visited_urls.insert(i->str());
			}
			visited_urls_mutex.unlock();
		}
		--looking_for;
	}
}

void MultithreadedWebcrawler::setOptCURL(CURL * curl, const std::string & url, std::string * result) {
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
	curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
	curl_easy_setopt(curl, CURLOPT_UNRESTRICTED_AUTH, 1L);
	curl_easy_setopt(curl, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
}

size_t MultithreadedWebcrawler::WriteCallback(void * contents, size_t size, size_t nmemb, void * userp) {
	((std::string *) userp)->append((char *) contents, size * nmemb);
	return size * nmemb;
}

void MultithreadedWebcrawler::setStartUrl(const std::string & _start_url) {
	start_url = _start_url;
}

std::string MultithreadedWebcrawler::getStartUrl() const {
	return start_url;
}

void MultithreadedWebcrawler::setRegex(const std::string & _search_regex) {
	search_regex = std::regex(_search_regex);
}

std::regex MultithreadedWebcrawler::getRegex() const {
	return search_regex;
}

void MultithreadedWebcrawler::setThreadsNumber(const int _threads_number) {
	threads_number = _threads_number;
}

int MultithreadedWebcrawler::getThreadsNumber() const {
	return threads_number;
}

void MultithreadedWebcrawler::setDepth(const int _depth) {
	depth = _depth;
}

int MultithreadedWebcrawler::getDepth() const {
	return depth;
}