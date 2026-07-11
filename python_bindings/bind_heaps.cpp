#include "bindStructure.hpp"
#include "heap/pairing.hpp"
#include "heap/d-ary.hpp"
#include "heap/fibonacci.hpp"
#include "heap/binomial.hpp"

#include <nanobind/nanobind.h>
namespace nb = nanobind;

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
        }
    }    
}