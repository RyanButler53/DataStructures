/**
 * @file bindings.hpp
 * @author Ryan Butler
 * @brief Header file for function signatures for binding functions
 * @version 0.1
 * @date 2026-07-11
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once

#include <nanobind/nanobind.h>
namespace nb = nanobind;

void bindCuckooHash(nb::module_& m);
void bindUnrolledLinkedList(nb::module_& m);
void bindHeaps(nb::module_& m);
void bindSpatial(nb::module_& m);
void bindTrees(nb::module_& m);
// void bindQuack(nb::module_& m);
