//this file is the core of the application. It contains the interface which the nodes will use to interact with the application.

#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <any>
#include <typeindex>

namespace core {

class Node {
    public: 
    explicit Node(const std::string& node_name, const int& freq_per_sec);
    ~Node()=default;

    virtual void init()=0;
    virtual void run()=0;
    virtual void stop()=0;
    virtual void destroy()=0;

    //I need a strategy for making this node to share things with other nodes
    //for now lets assume a parent child structure with no check on the DAG.
    //For this to work, we need to ask users to "register" their inputs and outputs. 
    //Maybe in the future we can make this easier.

    template<typename T>
    void addInput(const T& input_default, const std::string& input_name)
    {
        input_default_values_[input_name] = input_default;
        input_type_map_[input_name] = std::type_index(typeid(T));
    }

    template<typename T>
    void addOutput(const T& output_default, const std::string& output_name)
    {
        output_default_values_[output_name] = output_default;
        output_type_map_[output_name] = std::type_index(typeid(T));
    }

    private:

    std::string node_name_;
    int freq_per_sec_;
    std::unordered_map<std::string, std::any> input_default_values_;
    std::unordered_map<std::string, std::type_index> input_type_map_;
    std::unordered_map<std::string, std::any> output_default_values_;
    std::unordered_map<std::string, std::type_index> output_type_map_;

};
}

#endif