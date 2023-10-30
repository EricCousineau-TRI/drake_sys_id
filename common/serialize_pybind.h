#pragma once

#include <string>

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <pybind11/pybind11.h>

#include "drake/bindings/pydrake/common/serialize_pybind.h"
#include "drake/bindings/pydrake/pydrake_pybind.h"
#include "drake/common/yaml/yaml_io.h"

namespace anzu {

/**
Adds a standard set of bindings to a given class. Requires the class to be
"schema-like" - i.e.:
  - can be default initialized
  - can be param initialized
  - has a Serialize method
  - can be trivially copied and deepcopied
A matching test function lives in `anzu_pybind_test_utilities.py` for
testing most aspects of this - aspects like a deepcopy being deep are not
tested.
*/
template <typename PyClass>
void DefSchemaUsingSerialize(PyClass* ppy_class) {
  using CxxClass = typename PyClass::type;
  namespace py = pybind11;
  auto& cls = *ppy_class;
  cls  // BR
      .def(py::init<>())
      .def(drake::pydrake::ParamInit<CxxClass>())
      .def("SaveYamlString", [](const CxxClass& self) {
        return drake::yaml::SaveYamlString(self);
      })
      .def_static("LoadYamlString", [](const std::string& data) {
        return drake::yaml::LoadYamlString<CxxClass>(data);
      });
  drake::pydrake::DefAttributesUsingSerialize(&cls);
  drake::pydrake::DefCopyAndDeepCopy(&cls);
  // TODO(#9399): This doesn't do a bitwise copy and it probably should.
  cls.def(py::pickle(
      [](const CxxClass& self) { return drake::yaml::SaveYamlString(self); },
      [](const std::string& data) {
        return drake::yaml::LoadYamlString<CxxClass>(data);
      }));
}

/**
Compact version of DefSchemaUsingSerialize.
*/
template <typename Class>
auto BindSchema(pybind11::object scope, const std::string& name) {
  pybind11::class_<Class> cls(scope, name.c_str());
  DefSchemaUsingSerialize(&cls);
  return cls;
}

}  // namespace anzu
