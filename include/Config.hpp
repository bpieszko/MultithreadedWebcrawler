#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>

class Config
{
public:
    Config() = delete;
    Config(const std::string & a_file);

    const std::string & getArg(const std::string & a_key) const;

private:
    inline bool isFileExists(const std::string & a_file) const;

    const std::string getKey(const std::string & a_line) const;
    const std::string getValue(const std::string & a_line) const;

    std::map<std::string, std::string> m_args;
};

class ConfigException : public std::exception
{
public:
    explicit ConfigException(const std::string & a_msg)
        : m_msg(a_msg) { }
    virtual const char * what() const throw()
    {
        return m_msg.c_str();
    }
private:
    std::string m_msg;
};