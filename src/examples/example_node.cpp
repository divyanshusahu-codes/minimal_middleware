#include <core/core.h>

class ExampleNode : public core::Node {
public:
  ExampleNode() : core::Node("my_node1", 30) { ADD_INPUT("hello", int, 0); }

  void init() {
    std::cout << "NODE 1 INIT" << std::endl;

    std::this_thread::sleep_for(
        std::chrono::duration(std::chrono::milliseconds(500)));
  };
  void run() {
    std::cout << "NODE 1 RUN" << std::endl;
    std::this_thread::sleep_for(
        std::chrono::duration(std::chrono::milliseconds(100)));
  };
  void stop() { std::cout << "NODE 1 STOP" << std::endl; };
  void destroy() { std::cout << "NODE 1 DESTROY" << std::endl; };
};

class ExampleNode2 : public core::Node {
public:
  ExampleNode2() : core::Node("my_node2", 30) { ADD_INPUT("hello", int, 0); }

  void init() {
    std::cout << "NODE 2 INIT" << std::endl;
    std::this_thread::sleep_for(
        std::chrono::duration(std::chrono::milliseconds(500)));
  };
  void run() {
    std::cout << "NODE 2 RUN" << std::endl;
    std::this_thread::sleep_for(
        std::chrono::duration(std::chrono::milliseconds(100)));
  };
  void stop() { std::cout << "NODE 2 STOP" << std::endl; };
  void destroy() { std::cout << "NODE 2 DESTROY" << std::endl; };
};

int main() {

  // need to make a pointer here and add it to the application

  std::shared_ptr<core::Node> node1 = std::make_shared<ExampleNode>();
  std::shared_ptr<core::Node> node2 = std::make_shared<ExampleNode2>();

  core::App::registerNode(node1, 1);
  core::App::registerNode(node2, 2);

  core::App::runGraph();
};