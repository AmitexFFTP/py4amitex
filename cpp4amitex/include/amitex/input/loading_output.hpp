#ifndef _AMITEX_LOADING_OUTPUT_HEADER_
#define _AMITEX_LOADING_OUTPUT_HEADER_

#include "amitex/input/common.hpp"
#include "amitex/input/init_load_ext.hpp"
#include "amitex/input/loading.hpp"
#include "amitex/input/output.hpp"

#include "amitex/private/input_element.hpp"

namespace amitex {

//! Definitions of all loadings and output settings
class LoadingOutput {
 public:
  static Ptr<LoadingOutput> create() { return makePtr<LoadingOutput>(LoadingOutput{}); }
  const char* xmlTag() const { return "Loading_Output"; }
  bool xmlHasBody() const { return true; }
  void xmlWriteAttributes(std::ostream& stream) const {}
  void xmlWriteInner(std::ostream& stream) const;

  //! Append a partial loading
  void add(Ptr<Loading> loading) {
    loadings.push_back(loading);
    setLastId();
  }

  //! Output specification
  Ptr<Output> output;
  //! Initialization of the temperature and the external parameters
  Ptr<InitLoadExt> initLoadExt;

 private:
  LoadingOutput() : loadings{}, output{nullptr}, initLoadExt{nullptr} {
    output = Output::create();
  };
  void setLastId() { loadings[loadings.size() - 1]->setIndex(loadings.size() - 1); }
  std::vector<Ptr<Loading>> loadings;
};

}  // namespace amitex

#endif  // _AMITEX_LOADING_OUTPUT_HEADER_