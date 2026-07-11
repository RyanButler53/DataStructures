#pragma once

// Nanobind
#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/operators.h>
#include <nanobind/make_iterator.h>

// Stl
#include <sstream>
#include <string>
#include <format>
#include <array>
#include <utility>
#include <tuple>
#include <print>
#include <meta>

namespace nb = nanobind;

template <typename Enum>
nb::enum_<Enum> bindEnum(nb::module_& m, std::string name){
    nb::enum_<Enum> enu(m, name.c_str());
    constexpr std::meta::info r = ^^Enum;
    static constexpr auto values = std::define_static_array(std::meta::enumerators_of(r));
    template for (constexpr std::meta::info v : values){
        enu.value(std::meta::identifier_of(v).data(), [:v:]);
    }
    return enu;
}

// Simpler function to bind iterators, no reflection needed
template <typename Iterator>
nb::class_<Iterator> bindIterator(nb::module_& m, std::string name){
    nb::class_<Iterator> cls(m, name.c_str());
    cls.def(nb::self == nb::self);
    cls.def(nb::self != nb::self);
    cls.def_prop_ro("value", [](const Iterator& it){return *it;});
    cls.def("increment", &Iterator::operator++);
    cls.def("decrement", &Iterator::operator--);
    return cls;
}
// Iterators are a TYPE, that get exposed. The struct with an iterator must overload __iter__ 

template <typename Structure>
nb::class_<Structure> bindStructure(nb::module_& m, std::string name){
    nb::class_<Structure> cls(m, name.c_str());
    constexpr std::meta::info r = ^^Structure;
    constexpr auto ctx = std::meta::access_context::unprivileged();
    static constexpr auto members = std::define_static_array(std::meta::members_of(r, ctx));
    template for (constexpr std::meta::info m : members){
        if constexpr (std::meta::is_nonstatic_data_member(m)){
            cls.def_ro(std::meta::identifier_of(m), [:m:], "");
        } else if constexpr (std::meta::is_operator_function(m)){
            constexpr std::meta::operators op = std::meta::operator_of(m);
            // std::println("Operator: {}", std::meta::symbol_of(op));
            // No operators yet...
        }  else if constexpr (std::meta::is_constructor(m)){
            static constexpr auto params = std::define_static_array(std::meta::parameters_of(m));
            [&]<size_t... Indexes>(std::index_sequence<Indexes...>){
                cls.def(nb::init<typename [: std::meta::type_of(params[Indexes]) :]...>());
            }(std::make_index_sequence<params.size()>{});
        } else if constexpr(std::meta::is_destructor(m)){
            // Pass through 
        } else if constexpr (std::meta::is_class_member(m) && std::meta::is_function(m)){
            static constexpr auto name = std::meta::identifier_of(m);
            static constexpr auto params = std::define_static_array(std::meta::parameters_of(m));
                using FuncType = decltype(&[:m:]);
                [&]<std::meta::info M, size_t... Indexes>(std::index_sequence<Indexes...>){
                    cls.def(name.data(), static_cast<FuncType>(&[:M:]), nb::arg(std::meta::identifier_of(params[Indexes]).data())...);
                }.template operator()<m>(std::make_index_sequence<params.size()>{});
        }
    }
    return cls;

}

// Binds a structure that exposes a public iterator. 
template <typename Structure>
nb::class_<Structure> bindStructureIterator(nb::module_& m, std::string name){
    nb::class_<Structure> cls = bindStructure<Structure>(m, name);
    
    // Add on an iterator
    cls.def("__iter__", [](Structure& s){
        return nb::make_iterator(nb::type<Structure>(), "iterator",
        s.begin(), s.end());
    }, nb::keep_alive<0, 1>());
    return cls;
}
