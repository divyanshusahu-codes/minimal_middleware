#include <core/app.h>
#include <core/core.h>

namespace core {

void App::registerNode(std::shared_ptr<Node> reg_node,
                       const int &priority) {
  std::cout << "REGISTERING THE NODE WITH NAME: " << reg_node->getNodeName()
            << std::endl;

  // add it to the map for now
  // TODO: Multiple nodes at the same priority?
  node_map_.emplace(priority, reg_node);
}

void App::runGraph() {
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
  }
}

} // namespace core