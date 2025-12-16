// this file is the core of the application. It contains the interface which the
// nodes will use to interact with the application.
// TODO: MOVE IMPLEMENATAIONS TO CPP
#ifndef CORE_H
#define CORE_H

#include <any>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
// #include <ordered_map>
#include <functional>
#include <string>
#include <thread>
#include <typeindex>
#include <unordered_map>
namespace core {
#define ADD_INPUT(name, type, default_value)                                   \
  addInput(name, std::type_index(typeid(type)), default_value)
#define ADD_OUTPUT(name, type, default_value)                                  \
  addOutput(name, std::type_index(typeid(type)), default_value)

class Node {
public:
  friend class App;
  explicit Node(const std::string &node_name, const int &freq_per_sec);
  ~Node() = default;

  virtual void init() = 0;
  virtual void run() = 0;
  virtual void stop() = 0;
  virtual void destroy() = 0;

  // I need a strategy for making this node to share things with other nodes
  // for now lets assume a parent child structure with no check on the DAG.
  // For this to work, we need to ask users to "register" their inputs and
  // outputs. Maybe in the future we can make this easier.

  template <typename T>
  void addInput(const std::string &input_name, std::type_index type_info,
                const T &default_value) {
    input_default_values_[input_name] = default_value;
    input_type_map_.emplace(input_name, type_info);

    // Print debug info
    std::cout << "ADD_INPUT called: name='" << input_name << "', type='"
              << type_info.name() << "', default_value=" << default_value
              << std::endl;
  }

  template <typename T>
  void addOutput(const std::string &output_name, std::type_index type_info,
                 const T &default_value) {
    output_default_values_[output_name] = default_value;
    output_type_map_.emplace(output_name, type_info);
  }

  // a node should have its own pubs and subs and it should be a part of the
  // node. can real time nodes do this? keep it simple for now... assume that it
  // can.. we can add layers to make sure it doesnt later the intention here to
  // have an "on event" way of delering things which can be non-DAC. But this
  // will be non-realtime.. coz of dyanmic mem alloc.. for realtime thread, we
  // can make this work by making a queue between pubs and subs

  // for publsher.. we need a way to get the data? keep it simple, we first just
  // get info... like what type its publishing

  template <typename T>
  void addPublisher(const std::string &publisher_name,
                    std::type_index type_info, const T &default_values) {
    pub_default_values_[publisher_name] = default_values;
    pub_type_map_.emplace(publisher_name, type_info);
  }

  // for the sub, we need a callback function which we can call when we get the
  // data. For the sub, we ned to fix the signature and make it a standard.
  // smthing ike function<void(T*)>
  // template <typename T>
  // void addSubcriber(const std::string &publisher_name,
  //                   std::type_index type_info,
  //                   std::function<void(T *)> callback) {
  //   pub_default_values_[publisher_name] = default_values;
  //   pub_type_map_.emplace(publisher_name, type_info);
  // }

  std::string getNodeName() const { return node_name_; };

private:
  std::string node_name_;
  int freq_per_sec_;

  // GRAPH
  std::unordered_map<std::string, std::any> input_default_values_;
  std::unordered_map<std::string, std::type_index> input_type_map_;
  std::unordered_map<std::string, std::any> output_default_values_;
  std::unordered_map<std::string, std::type_index> output_type_map_;
  // GRAPH

  //  pubs and subs use the same strategy
  std::unordered_map<std::string, std::any> pub_default_values_;
  std::unordered_map<std::string, std::type_index> pub_type_map_;
  std::unordered_map<std::string, std::type_index> sub_type_map_;
  // std::unordered_map<std::string, std::function> sub_type_map_; //NEED A
  // CONTAIER TO STORE THE CALLBACK.. HOW?

  // pubds and subs
};

// now will need to make an app that actually can register and run things
// for now it will be simple... they will register and the flow is only from one
// node to other this class will have to be a singleton.. and will have to be
// the manager

class App {
public:
  // we keep it simple for now. Let them register nodes and run them in
  // sequence. They will pass a pointer to the Node created? we need a better
  // way?

  static void registerNode(std::shared_ptr<Node> reg_node,
                           const int &priority) {
    std::cout << "REGISTERING THE NODE WITH NAME: " << reg_node->getNodeName()
              << std::endl;

    // add it to the map for now
    // TODO: Multiple nodes at the same priority?
    node_map_.emplace(priority, reg_node);
  }

  static void runGraph() {
    // loop through the graph and run the nodes in priority
    // lets do iterators?
    // for now lets run it only 10 times and then end.. i know this is bad code
    int num_iters = 10;
    int curr_iters = 0;
    std::cout << "initilalizing all nodes" << std::endl;

    for (auto it = node_map_.begin(); it != node_map_.end(); it++) {
      // run the ordered nodes
      it->second->init();
    };

    std::cout << "stepping through all nodes" << std::endl;

    while (curr_iters < num_iters) {
      for (auto it = node_map_.begin(); it != node_map_.end(); it++) {
        // run the ordered nodes
        it->second->run();
      };
      curr_iters++;
    };

    // ideally this should be done on catching sigint or smthin
    std::cout << "STOPPING all nodes" << std::endl;

    // STOP all nodes
    for (auto it = node_map_.begin(); it != node_map_.end(); it++) {
      // run the ordered nodes
      it->second->stop();
    };

    std::cout << "DESTROYING all nodes" << std::endl;

    // STOP all nodes
    for (auto it = node_map_.begin(); it != node_map_.end(); it++) {
      // run the ordered nodes
      it->second->destroy();
    };
  };

private:
  App() = default;

  ~App(){};

  // copy and move constuctors
  //   App(const & App) = delete;
  App(App &&) = delete;

  // store the nodes and their sequence.. this is not inline need to make a
  // defination or smthing
  static std::map<int, std::shared_ptr<Node>> node_map_;
};
} // namespace core

#endif