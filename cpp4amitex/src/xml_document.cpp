#include "amitex/private/xml_document.hpp"

#include <deque>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace amitex {
namespace xml {

using Entry = std::pair<std::string, std::string>;

static std::string& getValueRef(std::vector<std::pair<std::string, std::string>>& keyvals,
                                std::string_view key) {
  std::string* valRef = nullptr;
  for (auto& keyval : keyvals) {
    if (std::get<0>(keyval) == key) {
      valRef = &std::get<1>(keyval);
    }
  }
  if (!valRef) {
    keyvals.push_back({std::string{key}, std::string{}});
    valRef = &std::get<1>(keyvals.back());
  }
  return *valRef;
}

void Document::Node::setAttribute(std::string_view name, const char* value) {
  getValueRef(data().attrs, name) = value;
}

void Document::Node::setAttribute(std::string_view name, const std::string& value) {
  getValueRef(data().attrs, name) = value;
}

void Document::Node::setAttribute(std::string_view name, std::string&& value) {
  getValueRef(data().attrs, name) = std::move(value);
}

void Document::Node::setAttribute(std::string_view name, bool value) {
  getValueRef(data().attrs, name) = value ? "True" : "False";
}

void Document::Node::setAttribute(std::string_view name, size_t value) {
  getValueRef(data().attrs, name) = std::to_string(value);
}

void Document::Node::setAttribute(std::string_view name, int value) {
  getValueRef(data().attrs, name) = std::to_string(value);
}

void Document::Node::setAttribute(std::string_view name, double value) {
  // stringstream to reproduce same output as with streams
  std::stringstream ss;
  ss << std::setprecision(12) << value;
  getValueRef(data().attrs, name) = ss.str();
}

Document::Node Document::createText(std::string_view text) {
  Node::Data data = {.type = Node::Type::Text, .nameOrText = std::string{text}};
  nodes_.push_back(std::move(data));
  return Node{&nodes_, nodes_.size() - 1};
}

Document::Node Document::createElement(std::string_view tagname) {
  Node::Data data = {.type = Node::Type::Element, .nameOrText = std::string{tagname}};
  nodes_.push_back(std::move(data));
  return Node{&nodes_, nodes_.size() - 1};
}

void Document::writeNodeBegin(std::ostream& stream, Node node) {
  switch (node.type()) {
    case Node::Type::Element:
      stream << '<' << node.data().nameOrText;
      for (const auto& [key, val] : node.data().attrs) {
        stream << ' ' << key << '=' << '"' << val << '"';
      }
      if (node.data().children.empty())
        stream << "/>";
      else
        stream << '>';
      break;
    case Node::Type::Text:
      stream << node.data().nameOrText;
      break;
    default:
      break;
  }
}

void Document::writeNodeEnd(std::ostream& stream, Node node) {
  switch (node.type()) {
    case Node::Type::Element:
      if (!node.data().children.empty()) stream << "</" << node.data().nameOrText << '>';
      break;
    default:
      break;
  }
}

void Document::write(std::ostream& stream, Node root) const {
  writeNodeBegin(stream, root);

  std::deque<std::pair<Node, size_t>> stack{{root, 0}};
  while (!stack.empty()) {
    auto& [parent, index] = stack.back();
    if (index == parent.data().children.size()) {
      writeNodeEnd(stream, parent);
      stack.pop_back();
    } else {
      Node current = parent.data().children[index];
      writeNodeBegin(stream, current);
      index++;
      if (current.data().children.size() > 0) {
        stack.push_back({current, 0});
      }
    }
  }
}

void Document::write(std::ostream& stream) const { write(stream, root_); }

void Document::writeFile(const std::filesystem::path& path) const {
  std::ofstream stream{path};
  if (stream) {
    stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    write(stream);
    stream << '\n';
  } else {
    throw std::runtime_error(std::string{"Could not open file"} + path.string());
  }
}

}  // namespace xml

}  // namespace amitex
