#include <pybind11/pybind11.h>

#include "drake/bindings/pydrake/pydrake_pybind.h"
#include "common/serialize_pybind.h"
#include "sys_id/sys_id_param.h"

namespace py = pybind11;

namespace anzu {

void BindSysId(py::module m) {
  py::module::import("pydrake.multibody.plant");
  BindSchema<JointDryFriction>(m, "JointDryFriction");
  BindSchema<JointViscousFriction>(m, "JointViscousFriction");
  BindSchema<JointFriction>(m, "JointFriction");
  BindSchema<JointParam>(m, "JointParam");
  BindSchema<InertiaParam>(m, "InertiaParam");
  BindSchema<BodyParam>(m, "BodyParam");
  BindSchema<ModelParam>(m, "ModelParam");

  m.def(
      "ApplyJointParam", &ApplyJointParam,
      py::arg("param"), py::arg("context"), py::arg("joint"),
      py::arg("include_damping") = false);
  m.def(
      "ExtractJointParam", &ExtractJointParam,
      py::arg("context"), py::arg("joint"),
      py::arg("include_damping") = false);
  m.def(
      "ApplyBodyParam", &ApplyBodyParam,
      py::arg("param"), py::arg("context"), py::arg("body"));
  m.def(
      "ExtractBodyParam", &ExtractBodyParam,
      py::arg("context"), py::arg("body"));
}

PYBIND11_MODULE(cc, m) {
  BindSysId(m);
}

}  // namespace anzu
