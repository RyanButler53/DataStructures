#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/operators.h>
#include <sstream>
#include <string>
#include <format>
#include <array>
#include <utility>
#include <tuple>
#include <print>
#include <meta>
#include "spatial/kdtree.hpp"
namespace nb = nanobind;

using namespace nb::literals;

nb::class_<Tree2D<int>> bindKdTree(nb::module_& m, std::string name){

    nb::class_<Tree2D<int>> cls(m, name.c_str());
    cls.def(nb::init<>());
    cls.def("size", &Tree2D<int>::size, "gets the size of the tree");
    cls.def("insert", &Tree2D<int>::insert, "key"_a, "insert");
    cls.def("remove", &Tree2D<int>::remove, "key"_a, "remove");
    cls.def("contains", &Tree2D<int>::contains, nb::arg("key"), "contains");

    return cls;
}


template <typename Structure>
nb::class_<Structure> bindStructure(nb::module_& m, std::string name){
    nb::class_<Structure> cls(m, name.c_str());
    constexpr std::meta::info r = ^^Structure;
    constexpr auto ctx = std::meta::access_context::unprivileged();
    static constexpr auto members = std::define_static_array(std::meta::members_of(r, ctx));
    template for (constexpr std::meta::info m : members){
        if constexpr (std::meta::is_nonstatic_data_member(m)){
            std::println("Data Memeber: {}",  std::meta::identifier_of(m));
            cls.def_ro(std::meta::identifier_of(m), [:m:], "");
        } else if constexpr (std::meta::is_operator_function(m)){
            constexpr std::meta::operators op = std::meta::operator_of(m);
            // std::println("Operator: {}", std::meta::symbol_of(op));
            // No operators yet...
        }  else if constexpr (std::meta::is_constructor(m)){
            std::println("Constructor: {}", std::meta::display_string_of(m));
            // Get the parameter names, pybind doesn't need the types!
            // static constexpr auto params = std::define_static_array(std::meta::parameters_of(m));
            nb::init<>(); // Default ctor only
        } else if constexpr (std::meta::is_destructor(m)){
            // std::println("Destructor: {}", std::meta::display_string_of(m));
        } else if constexpr (std::meta::is_class_member(m) && std::meta::is_function(m)){
            std::println("Member Function: {}", std::meta::identifier_of(m));
            // printFunctionArgs<m>();
            static constexpr auto name = std::meta::identifier_of(m);
            static constexpr auto params = std::define_static_array(std::meta::parameters_of(m));

            [&]<std::meta::info M, size_t... Indexes>(std::index_sequence<Indexes...>){
                    cls.def(name.data(), &[:M:], nb::arg(std::meta::identifier_of(params[Indexes]).data())...);
                }.template operator()<m>(std::make_index_sequence<params.size()>{});


        } else {
            std::println("Something else: {}", std::meta::display_string_of(m));
        }
    }
    return cls;

}
int add_ints(int a, int b) {
    return a + b;
}


NB_MODULE(ds_ext, m) {
    m.doc() = "Bindings for Various Data Structures";
    // bindKdTree(m, "IntTree2D");
    bindStructure<KDTree<int, 2>>(m, "IntTree2D");
    // nb::class_<KDTree<int, 2>>(m, "IntTree2D", "2 dimension with ints")
    // .def(nb::init<>())
    // .def("size", &KDTree<int,2>::size, "gets the size of the tree");

    // m.def("addInts", &add_ints, args);
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
