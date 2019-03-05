#include "Config.hpp"

Config::Config(const std::string & a_file)
{
    if (!isFileExists(a_file))
        throw ConfigException("File \"" + a_file + "\" does not exist.");

    std::ifstream file(a_file);
    if (!file.is_open())
        throw ConfigException("Cannot open config file.");
    
    try
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::string key = getKey(line);
            std::string value = getValue(line);

            if (m_args.find(key) != m_args.end())
                throw ConfigException("Multiple \"" + key + "\" in config file.");
        
            m_args.insert({key, value});
        }
    }
    catch (std::exception & e)
    {
        file.close();
        throw e;
    }
    file.close();
}

const std::string & Config::getArg(const std::string & a_key) const
{
    if (m_args.find(a_key) == m_args.end())
        throw ConfigException("\"" + a_key + "\" is not configured.");

    return m_args.at(a_key);
}

inline bool Config::isFileExists(const std::string & a_file) const
{
    std::ifstream f(a_file);
    return f.good();
}

const std::string Config::getKey(const std::string & a_line) const
{
    if (a_line.find('=') == std::string::npos)
        throw ConfigException("Config file is incorrect: \"" + a_line + "\"");
    
    return a_line.substr(0, a_line.find('='));
}

const std::string Config::getValue(const std::string & a_line) const
{
    if (a_line.find('=') == std::string::npos)
        throw ConfigException("Config file is incorrect: \"" + a_line + "\"");

    return a_line.substr(a_line.find('=') + 1, a_line.size() - a_line.find('='));
}