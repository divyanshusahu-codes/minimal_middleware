#include <core/core.h>

namespace core {

Node::Node(const std::string &node_name, const int &freq_per_sec)
    : node_name_(node_name), freq_per_sec_(freq_per_sec) {
  std::cout << "Node with name: " << node_name_ << " "
            << "was initialized" << std::endl;
}
// Define the static member variable
std::map<int, std::shared_ptr<Node>> App::node_map_;

} // namespace core