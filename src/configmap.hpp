#ifndef __CONFIGMAP_HPP__
#define __CONFIGMAP_HPP__

#include <map>
#include <regex>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>
#include <cstdio>

class configmap
{
public:
    struct value_t
    {
    public:
        explicit value_t(std::string data)
            : data_(std::move(data))
        {}
        template <typename T>
        T as() const
        {
            std::stringstream ss(data_);       
            T rv{};
            ss >> rv;
            return rv;
        }
        const std::string& raw() const
        {
            return data_;
        }
    private:
        std::string data_;
    };

    configmap() = default;
    void load(std::string file)
    {
        std::ifstream infile(file);
        if (!infile.is_open()) 
        {
            return;
        }

        static const std::regex arger("^(.+?)[ ]*=[ ]{0,1}(.*?)$");

        std::string line;

        while (std::getline(infile, line))
        {
            std::smatch match;
            
            if (std::regex_match(line, match, arger))
            {
                config.try_emplace(match[1].str(), match[2].str());
            }
        }
    }

    template<typename Index>
    value_t at_or(Index&& index, std::string def="") const
    {
        auto it = config.find(std::forward<Index>(index));
        if (it == config.end())
        {
            // printf("configmanager: not found! def=%s", def.c_str());
            return value_t{std::move(def)};
        }
        // printf("configmanager: found! key=\"%s\" val=\"%s\"\n", it->first.c_str(), it->second.raw().c_str());
        return it->second;
    }

    const std::map<std::string, value_t, std::less<>>& get()
    {
        return config;
    }

private:
    std::map<std::string, value_t, std::less<>> config;
};

#endif // __CONFIGMAP_HPP__
