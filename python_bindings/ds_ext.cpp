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

#include "bindings.hpp"

namespace nb = nanobind;


NB_MODULE(ds_ext, m) {
    m.doc() = "Bindings for Various Data Structures";

    bindTrees(m);
    bindSpatial(m);
    bindHeaps(m);
    bindUnrolledLinkedList(m);
    bindCuckooHash(m);
    
}
