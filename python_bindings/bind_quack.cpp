#include "bindStructure.hpp"
#include "bindings.hpp"
#include <nanobind/nanobind.h>
#include "quack/quack.hpp"
#include <ranges>

namespace nb = nanobind;

void bindQuack(nb::module_& m){
    static constexpr std::array data_types = {^^int, ^^float, ^^std::string};
    static constexpr std::array data_names = {"Int", "Float", "String"};
    template for (constexpr size_t ikey : std::views::iota(0UL, 3UL)){
        using T = typename [: data_types[ikey] :];
        static constexpr std::string_view key_name = data_names[ikey];
        bindStructure<Quack<T>>(m, std::format("Quack{}", key_name));
    } 
}