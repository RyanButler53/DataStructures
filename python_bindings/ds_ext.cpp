#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/operators.h>
#include <sstream>
#include <string>
#include <format>
#include <array>
#include <utility>
#include <tuple>
#include <print>
#include <meta>

// Data structures
#include "spatial/kdtree.hpp"
#include "spatial/intervalTree.hpp"
#include "heap/pairing.hpp"
#include "heap/d-ary.hpp"
#include "heap/fibonacci.hpp"
#include "heap/binomial.hpp"
namespace nb = nanobind;

using namespace nb::literals;


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

// Helper for binding enumes
void bindHeaps(nb::module_ m){

    static constexpr std::array data_types = {^^int, ^^float};

    template for (constexpr std::meta::info data_token : data_types){
        template for (constexpr std::meta::info priority_token : data_types){
            // Make an array for min/max
            static constexpr std::string_view data_name = std::meta::display_string_of(data_token);
            static constexpr std::string_view priority_name = std::meta::display_string_of(priority_token);
            using data_t = typename [: data_token :];
            using priority_t = typename [: priority_token :];
            static constexpr std::array comparators = { ^^std::less<priority_t>, ^^std::greater<priority_t>};
            template for (constexpr std::meta::info comp_token : comparators) {
            
                using comp_t = typename [: comp_token :];

                // Generate a clean name suffix for Min vs Max heaps
                static constexpr bool is_less = (comp_token == ^^std::less<priority_t>);
                static constexpr std::string_view heap_kind = is_less ? "Min" : "Max";

                bindStructure<PairingHeap<data_t, priority_t, comp_t>>(m, std::format("PairingHeap{}{}{}", data_name, priority_name, heap_kind));
                bindStructure<DAryHeap<data_t, priority_t, 2, comp_t>>(m, std::format("DAryHeap2{}{}{}", data_name, priority_name, heap_kind));
                bindStructure<DAryHeap<data_t, priority_t, 4, comp_t>>(m, std::format("DAryHeap4{}{}{}", data_name, priority_name, heap_kind));
                bindStructure<DAryHeap<data_t, priority_t, 10, comp_t>>(m, std::format("DAryHeap10{}{}{}", data_name, priority_name, heap_kind));
                bindStructure<BinomialHeap<data_t, priority_t, comp_t>>(m, std::format("BinomialHeap{}{}{}", data_name, priority_name, heap_kind));
                bindStructure<FibonacciHeap<data_t, priority_t, comp_t>>(m, std::format("FibonacciHeap{}{}{}", data_name, priority_name, heap_kind));

            }
    }    }
}

NB_MODULE(ds_ext, m) {
    m.doc() = "Bindings for Various Data Structures";

    // KD Tree
    bindEnum<DistanceFunction>(m, "DistanceFunction");
    template for (constexpr size_t dim : std::views::iota(2UL, 3UL)){
        bindStructure<KDTree<int, dim>>(m, std::format("IntKDTree{}D", dim));
        bindStructure<KDTree<double, dim>>(m, std::format("FloatKDTree{}D", dim));
        bindStructure<Rectangle<int, dim>>(m, std::format("IntRectangle{}D", dim));
        bindStructure<Rectangle<double, dim>>(m, std::format("FloatRectangle{}D", dim));
    }

    // Interval Tree
    bindStructure<SimpleInterval<int>>(m, "IntervalInt");
    bindStructure<IntervalTree<SimpleInterval<int>>>(m, "IntervalTreeInt");
    bindStructure<SimpleInterval<float>>(m, "IntervalFloat");
    bindStructure<IntervalTree<SimpleInterval<float>>>(m, "IntervalTreeFloat");

    // Heaps
    bindHeaps(m);
    
}
