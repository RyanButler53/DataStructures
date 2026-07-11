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
    }

    // Cuckoo Hash Table
    bindIterator<typename CuckooHashMap<std::string, int>::const_iterator>(m, "CuckooHashMapStringIntIterator");
    bindStructureIterator<CuckooHashMap<std::string, int>>(m, "CuckooHashMapStringInt");
    bindIterator<typename CuckooHashSet<std::string>::const_iterator>(m, "CuckooHashSetIntIntIterator");
    bindStructureIterator<CuckooHashSet<std::string>>(m, "CuckooHashSetString");
}
