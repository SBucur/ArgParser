#include "ArgumentParser.hpp"

ArgumentParser::ArgumentParser(int argc, char *argv[])
{
    this->addArgEntry("Help", "help", "Prints this help menu", arg_type::arg_bool);
    this->parse(argc, argv);
}

ArgumentParser::~ArgumentParser()
{
}

/**
 * @brief addArgEntry
 * 
 * @param arg arg_entry
 */
void ArgumentParser::addArgEntry(arg_entry arg)
{
    args.push_back(arg);
}

/**
 * @brief addArgEntry
 * 
 * @param name Name of the argument
 * @param desc Description of the argument
 * @param flag Flag to call the argument
 * @param help Help text for the argument: displays when calling -help
 * @param type Arguemnt type
 */
void ArgumentParser::addArgEntry(std::string name, std::string flag, std::string help, arg_type type)
{
    arg_entry arg(name, flag, help, type);
    args.push_back(arg);
}

/**
 * @brief Parses the arguments and stores them in parsed_args
 * 
 * @param argc 
 * @param argv 
 */
void ArgumentParser::parse(int argc, char** argv)
{
    for(int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if(arg[0] == '-')
        {
            // "-flag" --> "flag" in map
            std::string flag = arg.substr(1);
            bool found = false;
            for(arg_entry& entry : ArgumentParser::args)
            {
                if( !entry.flag.compare(flag) )
                {
                    found = true;
                    switch( entry.type )
                    {
                        case arg_type::arg_string:
                            // if type is string, get next arg
                            if(i+1 < argc && argv[i+1][0] != '-')
                            {
                                std::string val = argv[i+1];
                                parsed_args.push_back(std::make_pair(entry.name, val));
                            }
                            else throw std::invalid_argument("ArgumentParser: Missing value for argument " + entry.name);
                            break;

                        case arg_type::arg_bool:
                            // if type is bool, set to true
                            parsed_args.push_back(std::make_pair(entry.name, true));
                            break;

                        case arg_type::arg_path:
                            // if type is path, get next arg
                            if(i+1 < argc && argv[i+1][0] != '-')
                            {
                                if(std::filesystem::exists(argv[i+1]))
                                {
                                    std::string val = argv[i+1];
                                    parsed_args.push_back(std::make_pair(entry.name, val));
                                }
                                else throw std::invalid_argument("ArgumentParser: Invalid path for argument " + entry.name);
                            }
                            else throw std::invalid_argument("ArgumentParser: Missing value for argument " + entry.name);
                            break;
                        case arg_type::arg_int:
                            // if type is int, get next arg
                            if(i+1 < argc)
                            {
                                std::string val = argv[i+1];
                                int int_val;
                                try { int_val = std::stoi(val); }
                                catch(std::invalid_argument& e)
                                {
                                    throw std::invalid_argument("ArgumentParser: Invalid value for argument, expected int for " + entry.name);
                                }
                                parsed_args.push_back(std::make_pair(entry.name, int_val));
                            }
                            else throw std::invalid_argument("ArgumentParser: Missing value for argument " + entry.name);
                            break;

                        case arg_type::arg_double:
                            // if type is double, get next arg
                            if(i+1 < argc)
                            {
                                std::string val = argv[i+1];
                                double double_val;
                                try { double_val = std::stod(val); }
                                catch(std::invalid_argument& e)
                                {
                                    throw std::invalid_argument("ArgumentParser: Invalid value for argument, expected double for " + entry.name);
                                }
                                parsed_args.push_back(std::make_pair(entry.name, double_val));
                            }
                            else throw std::invalid_argument("ArgumentParser: Missing value for argument " + entry.name);
                            break;

                        default:
                            throw std::invalid_argument("ArgumentParser: Internal Error: Invalid type for argument " + entry.name);
                            break;
                    }
                }
            }
            if(!found) throw std::invalid_argument("ArgumentParser: Invalid argument " + arg);
        }
        else throw std::invalid_argument("ArgumentParser: argument flags must start with \"-\", ex: -" + std::string(argv[i]));
    }               
}

