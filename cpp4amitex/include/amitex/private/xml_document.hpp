#ifndef _AMITEX_XML_HEADER_
#define _AMITEX_XML_HEADER_

#include <filesystem>
#include <map>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace amitex {

namespace xml {

class Document;

class Document {
 public:
  friend class Node;
  class Node {
   public:
    Node() : data_{nullptr}, index_{0} {}
    enum class Type { Element, Text };
    friend class Document;
    Type type() const { return data().type; }

    void appendChild(Node child) { data().children.push_back(child); }
    void setAttribute(std::string_view name, const char* value);
    void setAttribute(std::string_view name, const std::string& value);
    void setAttribute(std::string_view name, std::string&& value);
    void setAttribute(std::string_view name, bool value);
    void setAttribute(std::string_view name, size_t value);
    void setAttribute(std::string_view name, int value);
    void setAttribute(std::string_view name, double value);
    size_t numberChildren() const { return data().children.size(); }

   private:
    struct Data {
      Node::Type type;
      std::vector<Node> children;
      std::vector<std::pair<std::string, std::string>> attrs;
      std::string nameOrText;
    };
    Node(std::vector<Data>* data, size_t index) : data_{data}, index_{index} {};
    Data& data() { return (*data_)[index_]; }
    const Data& data() const { return (*data_)[index_]; }

    std::vector<Data>* data_;
    size_t index_;
  };
  Document(std::string_view qname) : root_{nullptr, 0} { root_ = createElement(qname); }
  Node createText(std::string_view text);
  Node createElement(std::string_view tagname);
  Node getRoot() const { return root_; }
  void writeFile(const std::filesystem::path& path) const;
  void write(std::ostream& stream, Node root) const;
  void write(std::ostream& stream) const;

 private:
  std::vector<Node::Data> nodes_;
  Node root_;

  static void writeNodeBegin(std::ostream& stream, Node node);
  static void writeNodeEnd(std::ostream& stream, Node node);
};

}  // namespace xml

}  // namespace amitex

#endif  // _AMITEX_XML_HEADER_
