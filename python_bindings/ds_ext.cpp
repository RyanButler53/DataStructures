#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/string.h>
#include <nanobind/operators.h>
#include <sstream>
#include <string>
#include "spatial/kdtree.hpp"
namespace nb = nanobind;

using namespace nb::literals;

nb::class_<Tree2D<int>> bindKdTree(nb::module_& m, std::string name){

    nb::class_<Tree2D<int>> cls(m, name.c_str());
    cls.def(nb::init<>());
    cls.def("size", &Tree2D<int>::size, "gets the size of the tree");
    cls.def("insert", &Tree2D<int>::insert, "key"_a, "insert");
    cls.def("remove", &Tree2D<int>::remove, "key"_a, "remove");
    cls.def("contains", &Tree2D<int>::contains, "key"_a, "contains");

    return cls;
}



NB_MODULE(ds_ext, m) {
    m.doc() = "Bindings for Various Data Structures";
    bindKdTree(m, "IntTree2D");

    // nb::class_<KDTree<int, 2>>(m, "IntTree2D", "2 dimension with ints")
    // .def(nb::init<>())
    // .def("size", &KDTree<int,2>::size, "gets the size of the tree");


    // m.def("addInts", [](int a, int b){
    //     return a + b;
    // }, "a"_a, "b"_a ,"adds two numbers");
}
    // // toplines 
    // nb::class_<Fraction>(m, "Fraction", "Class Representing a Fraction")
    // .def(nb::init<long long, long long>(), "Initializes from integer numerator and denominator") // No function body needed. Forwards to the constructor
    // .def(nb::init<long long>(), "Initializes from integer numerator only")
    // .def(nb::init<std::string>(), "Converts string to fractional type. Must not have a decimal")
    // .def("__repr__", [](const Fraction &f){ 
    //     std::stringstream s; 
    //     s << f; 
    //     return s.str();
    //  })
    // .def("simplify", &Fraction::simplify, "Simplifies a fraction through GCD")
    // .def("to_float", &Fraction::toFloat, "Converts to a floating point")
    // .def_rw("num", &Fraction::num_, "numerator")
    // .def_rw("den", &Fraction::den_, "denominator")
    // .def(nb::self + nb::self)
    // .def(nb::self - nb::self)
    // .def(nb::self * nb::self)
    // .def(nb::self / nb::self)
    // .def(nb::self == nb::self, nb::is_operator(), "Check equality with cross multiply");


// }
