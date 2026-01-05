

#ifndef APP_H
#define APP_H

#include <iostream>
#include <map>
#include <thread>
#include <memory>

namespace core {

// Forward declaration to avoid circular dependency
class Node;

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
                           const int &priority);

  static void runGraph();

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