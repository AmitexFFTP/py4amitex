# Developping

## Dependencies

- Python >= 3.8
- CMake
- C++-17 compiler
- numpy, vtk
- pybind11 (only for developping with cmake)

With pip:

    pip install numpy vtk pybind11

## Build with CMake

With `build` and `_install` as build directory and install directory respectively:

    cmake -S. -Bbuild -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=Debug -Dpybind11_DIR=`python -m pybind11 --cmakedir`
    cmake --build build
    cmake --install

One can then add `_install/lib` to `PYTHONPATH`.

Deactivate python support with variable `AMITEX_PYTHON`:

    cmake -S. -Bbuild -DCMAKE_INSTALL_PREFIX=_install -DCMAKE_BUILD_TYPE=Debug -DAMITEX_PYTHON=OFF

## Tests

To launch python tests:

    pytest

To launch c++ tests:

    cmake --test-dir build --output-on-failure

## Documentation

It uses [Sphinx](https://www.sphinx-doc.org)

The python module(s) need to be compiled and in (python) path. In `docs`

    make html

(use `make` to see available targets).

### C++ documentation

in `cpp4amitex/docs`

    doxygen

## C++ to Python

A script is at disposal to generate to C++ code pybind11 binding. The script needs [litgen](https://pthom.github.io/litgen).
In `cpp4amitex` run:

    python3 scripts/gen_python_binding.py <path to an header file>

The code is printed to standard output and can be inserted to the relevant bindings source file (in `bindings/python/src`).
In particular new input classes are to be added to `bindings/python/src/input_general.cpp`.

## Input nodes

Suppose we want to wrap AMITEX input XML element:
```XML
<Some_Parameter nb_iterations="10" tolerance="1e-4">
<Increments>1 2 3</Increments>
</Some_Parameter>
```

On can implement it in C++ with the following code:
```cpp
#include "amitex/input/common.hpp"
#include "amitex/input/private/list_element.hpp"

namespace amitex {

//! Some parameter
class SomeParameter {
  public:
   // This is the meat of XML generation. Each class transformable
   // to an DOM element needs to implement this template.
   // 'TDoc' is basically a simplified DOM API.
   // The actual generation of XML output is done by 'TDOC'.
   // See amitex::xml::Document for an implementation of 'TDoc'.
    template <typename TDoc>
    typename TDoc::Node toXMLElement(TDoc& doc) const {
      // Create the element with the correct tag
      auto ret = doc.createElement("Some_Parameter");
      // set attributes (strings, integers, floating-point, boolean are supported)
      ret.setAttribute("nb_iterations", this->nbIterations);
      ret.setAttribute("tolerance", this->tolerance);
      // add a child element. List<> is itself an element
      ret.appendChild(this->increments.toXMLElement(doc))
    }
    
    //! number of iterations
    int nbIterations;
    //! tolerance
    double tolerance;
    
    // The 'Increments' element is a list of integers. We have a template
    // class to handle this case.
    
    //! increments. For ex `increments.add(1)` for adding an element
    List<int> increments;

    // Constructing a pointer matches better Python semantics
    static Ptr<SomeParameter> create(int nbIterations, double tolerance) {
        return makePtr<SomeParameter>(SomeParameter{nbIterations, tolerance});
    }
    
    private:
        SomeParameter(int nbIterations, double tolerance): 
            bIterations{nbIterations}, tolerance{tolerance}, increments{} {};
};

}
```

To be produced, an instance of this class needs to be in a parent element 
and appended as child accordingly, or be a root element.

The root elements (AlgorithmParameters, Materials, LoadingOutput) need to implement `toXMLDocument`.
See below an example fragment:

```cpp
class Materials {
 public:
  template <typename TDoc>
  TDoc toXMLDocument() const {
    TDoc doc{"Materials"};
    // ...
    return doc;
  }
  //...
};
```