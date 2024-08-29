#ifndef _AMITEX_LOADING_OUTPUT_HEADER_
#define _AMITEX_LOADING_OUTPUT_HEADER_

#include "amitex/input/init_load_ext.hpp"
#include "amitex/input/loading.hpp"
#include "amitex/input/output.hpp"

#include "amitex/private/input_element.hpp"

namespace amitex {

//! Definitions of all loadings and output settings
class LoadingOutput {
 public:
  LoadingOutput() = default;

  template <typename TDoc>
  TDoc toXMLDocument() const {
    TDoc doc{"Loading_Output"};
    auto root = doc.getRoot();
    root.appendChild(output.toXMLElement(doc));
    if (initLoadExt) root.appendChild(initLoadExt.value().toXMLElement(doc));
    for (const auto& loading : loadings) {
      root.appendChild(loading.toXMLElement(doc));
    }
    return doc;
  }

  //! Append a partial loading
  void add(const Loading& loading) {
    loadings.push_back(loading);
    setLastId();
  }
  void add(Loading&& loading) {
    loadings.push_back(std::move(loading));
    setLastId();
  }

  //! Output specification
  Output output;
  //! Initialization of the temperature and the external parameters
  std::optional<InitLoadExt> initLoadExt;

 private:
  void setLastId() { loadings[loadings.size() - 1].setIndex(loadings.size() - 1); }
  std::vector<Loading> loadings;
};

}  // namespace amitex

#endif  // _AMITEX_LOADING_OUTPUT_HEADER_