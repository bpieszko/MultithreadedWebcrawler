#include "Crawler.hpp"

Crawler::Crawler(const Config & a_config)
    : m_config(a_config)
{
    curl_global_init(CURL_GLOBAL_ALL);
}

size_t Crawler::WriteCallback(void * contents, size_t size, size_t nmemb, void * userp)
{
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

void Crawler::setOpts(CURL * a_curl, const std::string & a_url, std::string * a_result)
{
    curl_easy_setopt(a_curl, CURLOPT_URL, a_url.c_str());
    curl_easy_setopt(a_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(a_curl, CURLOPT_WRITEDATA, a_result);
    curl_easy_setopt(a_curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    curl_easy_setopt(a_curl, CURLOPT_UNRESTRICTED_AUTH, 1L);
    curl_easy_setopt(a_curl, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
    curl_easy_setopt(a_curl, CURLOPT_FOLLOWLOCATION, 1);
}

void Crawler::crawl()
{
    m_visited_urls.clear();

    m_awaiting_urls.push({m_config.getArg("start_url"), 0});
    m_visited_urls.insert(m_config.getArg("start_url"));

    unsigned max_threads = std::stoi(m_config.getArg("max_threads"));
    max_threads = std::min(max_threads, std::thread::hardware_concurrency());
    max_threads = std::max(max_threads, 2u);

    for (unsigned i = 0; i < max_threads; ++i)
    {
        std::thread t(&Crawler::threadCrawl, this);
        m_threads.push_back(std::move(t));
    }

    for (std::thread & t : m_threads)
        if (t.joinable())
            t.join();

    curl_global_cleanup();
}

std::pair<std::string, unsigned> Crawler::getAwaitingUrl()
{
    std::lock_guard<std::mutex> l(m_awaiting_urls_mutex);
    if (m_awaiting_urls.empty())
        return std::make_pair("", 0);
    
    std::pair<std::string, unsigned> url = m_awaiting_urls.front();
    m_awaiting_urls.pop();

    return url;
}

void Crawler::setUrlFormat(std::string & a_url)
{
    if (a_url.substr(0, 8) == "https://")
        a_url.replace(0, 8, "");
    if (a_url.substr(0, 7) == "http://")
        a_url.replace(0, 7, "");
    if (a_url.substr(0, 4) != "www.")
        a_url = "www." + a_url;
}

void Crawler::sendError(const std::string & a_msg)
{
    std::lock_guard<std::mutex> l(m_stdout_mutex);
    std::cout << "Error: " << a_msg << std::endl;
}

void Crawler::catchMatches(const std::string & str)
{
    std::regex reg(m_config.getArg("regex"));

    auto mbeg = std::sregex_iterator(str.begin(), str.end(), reg);
    auto mend = std::sregex_iterator();

    for (auto i = mbeg; i != mend; ++i)
    {
        std::lock_guard<std::mutex> l(m_matches_mutex);

        if (m_matches.find(i->str()) != m_matches.end())
            continue;

        std::lock_guard<std::mutex> lo(m_stdout_mutex);
        std::cout << i->str() << std::endl;

        m_matches.insert(i->str());
    }
}

void Crawler::catchUrls(const std::string & str, const unsigned a_depth)
{
    std::regex reg(m_config.getArg("url_regex"));

    auto mbeg = std::sregex_iterator(str.begin(), str.end(), reg);
    auto mend = std::sregex_iterator();

    for (auto i = mbeg; i != mend; ++i)
    {
        std::lock_guard<std::mutex> l(m_visited_urls_mutex);

        if (m_visited_urls.find(i->str()) != m_visited_urls.end())
            continue;

        m_visited_urls.insert(i->str());

        std::lock_guard<std::mutex> al(m_awaiting_urls_mutex);
        m_awaiting_urls.push({i->str(), a_depth + 1});
    }
}

void Crawler::threadCrawl()
{
    while (1)
    {
        std::pair<std::string, unsigned> url = getAwaitingUrl();
        
        if (url == std::make_pair(std::string(""), 0u))
        {
            if (m_threads_working > 0)
                continue;
            else
                break;
        }

        if (url.second >= std::stoi(m_config.getArg("max_depth")))
            continue;

        ++m_threads_working;

        setUrlFormat(url.first);
        
        CURL * curl = curl_easy_init();

        if (!curl)
        {
            sendError("Cannot init curl!");
            return;
        }

        std::string result;
        setOpts(curl, url.first, &result);

        CURLcode rest = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        catchMatches(result);
        catchUrls(result, url.second);

        --m_threads_working;
    }
}