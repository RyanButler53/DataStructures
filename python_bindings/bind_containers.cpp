#include "bindStructure.hpp"
#include "unrolled-linked-list/unrolled-linked-list.hpp"
#include "cuckoo-hash/cuckoo-hash.hpp"
#include <meta>
#include <format>
#include <nanobind/nanobind.h>
namespace nb = nanobind;

void bindContainers(nb::module_ m){
        // Unrolled Linked List
    template for(constexpr size_t k : {4, 8, 16, 64, 512}){
        bindIterator<typename UnrolledLinkedList<int, k>::iterator>(m, std::format("ullIntiterator{}", k));
        bindStructureIterator<UnrolledLinkedList<int, k>>(m, std::format("ullInt{}",k));
        bindIterator<typename UnrolledLinkedList<double, k>::iterator>(m, std::format("ullFloatiterator{}",k));
        bindStructureIterator<UnrolledLinkedList<double, k>>(m, std::format("ullFloat{}", k));
        bindIterator<typename UnrolledLinkedList<std::string, k>::iterator>(m, std::format("ullStringiterator{}",k));
        bindStructureIterator<UnrolledLinkedList<std::string, k>>(m, std::format("ullString{}", k));
    }

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
