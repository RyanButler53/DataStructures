#include "bindStructure.hpp"
#include "unrolled-linked-list/unrolled-linked-list.hpp"
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

}
