#pragma once

#include "Config.hpp"
#include <string>
#include <curl/curl.h>
#include <memory>
#include <set>
#include <queue>
#include <thread>
#include <mutex>
#include <regex>

class Crawler
{
public:
    Crawler() = delete;
    Crawler(const Config & a_config);

    void crawl();

private:
    static size_t WriteCallback(void * contents, size_t size, size_t nmemb, void * userp);
    void threadCrawl();
    void setOpts(CURL * a_curl, const std::string & a_url, std::string * a_result);
    std::pair<std::string, unsigned> getAwaitingUrl();
    void setUrlFormat(std::string & a_url);
    void sendError(const std::string & a_msg);
    void catchMatches(const std::string & str);
    void catchUrls(const std::string & str, const unsigned a_depth);

private:
    const Config m_config;

    unsigned m_threads_working;
    std::vector<std::thread> m_threads;

    std::set<std::string> m_matches;
    std::set<std::string> m_visited_urls;
    std::queue<std::pair<std::string, unsigned>> m_awaiting_urls;

    std::mutex m_awaiting_urls_mutex;
    std::mutex m_stdout_mutex;
    std::mutex m_matches_mutex;
    std::mutex m_visited_urls_mutex;
};