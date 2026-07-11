#include "bindStructure.hpp"
#include "cuckoo-hash/cuckoo-hash.hpp"
#include <meta>
#include <format>
#include <ranges>
#include <nanobind/nanobind.h>
#include "bindings.hpp"

namespace nb = nanobind;

void bindCuckooHash(nb::module_& m){

    // Cuckoo Hash Table
    static constexpr std::array data_types = {^^int, ^^float, ^^std::string};
    static constexpr std::array data_names = {"Int", "Float", "String"};
    template for (constexpr size_t ikey : std::views::iota(0UL, 3UL)){
        using key_t = typename [: data_types[ikey] :];
        static constexpr std::string_view key_name = data_names[ikey];

        template for (constexpr size_t ivalue : std::views::iota(0UL, 3UL)){
            static constexpr std::string_view value_name = data_names[ivalue];
            using value_t = typename [: data_types[ivalue] :];
            bindIterator<typename CuckooHashMap<key_t, value_t>::const_iterator>(m, std::format("CuckooHashMap{}{}_iterator", key_name, value_name));
            bindStructure<CuckooHashMap<key_t, value_t>>(m, std::format("CuckooHashMap{}{}", key_name, value_name));
        }
        bindIterator<typename CuckooHashSet<key_t>::const_iterator>(m, std::format("CuckooHashSet{}_iterator", key_name));
        bindStructure<CuckooHashSet<key_t>>(m, std::format("CuckooHashSet{}", key_name));

    } 
}
