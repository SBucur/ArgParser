#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <algorithm>

/**
 * @brief The ArgumentParser class
 *
 * This class is used to parse command line arguments.
 *
 * @author 
 * @date 
 */
class ArgumentParser
{
    public:
    enum class arg_type
    {
        arg_string = 0x01,
        arg_int = 0x02,
        arg_double = 0x03,
        arg_bool = 0x04,
        arg_path = 0x05
    };
    typedef std::variant<
        std::string,
        int,
        double,
        bool,
        std::filesystem::path
    > arg_value;
    
    class arg_entry
    {
        public:
            arg_entry(std::string name, std::string flag, std::string help, arg_type type = arg_type::arg_bool):
                name(name),
                help(help),
                type(type)
            {
                if(isValidArgName(flag))
                {
                    this->flag = flag;
                }
                else
                {
                    throw std::invalid_argument("arg_entry: Invalid default value for argument def ->" + flag);
                }
            }

            std::string name;
            std::string flag;
            arg_type type;
            std::string help;
            
        private:
            bool isValidArgName(std::string name)
            {
                if(name.length() == 0 || !std::isalpha(name[0]))
                    return false;
                for(int i = 0; i < name.length(); i++)
                {
                    if(!std::isalnum(name[i]))
                        return false;
                }
                return true;
            }
    };
    ArgumentParser(int argc, char *argv[]);
    ~ArgumentParser();

    void addArgEntry(arg_entry arg);
    void addArgEntry(std::string name, std::string flag, std::string help, arg_type type = arg_type::arg_bool);
    void parse(int argc, char** argv);

    protected:
    std::vector<arg_entry> args;
    std::vector<std::pair<std::string, arg_value>> parsed_args;

};