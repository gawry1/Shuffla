#include "data_without_typing.hpp"
#include "../../logger/logger.hpp"

#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>

DataWithoutTyping::DataWithoutTyping()
{
    //ctor
}

DataWithoutTyping::~DataWithoutTyping()
{
    //dtor
}

bool DataWithoutTyping::set(const std::string& request)
{
    std::size_t i = 0;
    while(i < request.size() && request[i] != '/') i++;

    if (i == request.size()) {
        Logger::getInstance().log_error("Request should be in form <table_name>/<row definition>.\n Request: " + request);
        return false;
    }

    table_name = request.substr(0, i - 1);

    // settings properties and their values
    std::string rest = request.substr(i+2); /*removed "/?" */
    std::vector<std::string> properties;
    boost::split(properties, rest, boost::is_any_of("&"));

    for(std::size_t i=0; i<properties.size(); i++) {
        std::vector<std::string> strs;
        boost::split(strs, properties[i], boost::is_any_of("="));
        if (strs.size() != 2) {
            Logger::getInstance().log_error("Wrong format. Row definition in your request isn't in format ?prop1=type1[&prop2=type2]*. Your request: " + request);
            return false;
        }

        property_names.push_back(strs[0]);
        values.push_back(strs[1]);
    }

    return true;

}

std::string DataWithoutTyping::get_table_name() const
{
    return table_name;
}

std::vector<std::string> DataWithoutTyping::get_columns_names() const
{
    return property_names;

}
std::string DataWithoutTyping::get_value_for_property(const std::string& property) const
{
    for(std::size_t i=0; i<property_names.size(); i++) {
        if (property_names[i] == property) {
            return values[i];
        }
    }
    return NULL;
}