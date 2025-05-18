#pragma once

#include <array>
#include <optional>
#include <utility>
#include <numeric>

/**
 * @class Rectangle class. Rectangle class that supports n dimensional rectangles
 * A wrapper around an std::array of pairs representing uper and lower bounds for a given dimension. 
 * All entries in this bounds array are optional so dimensions can be left unspecified. 
 * Unspecified bounds are treated as (-inf, inf)
 * @tparam T type to store bounds of the rectangle
 * @tparam K dimension of the rectangle. 
 */
template <typename T, size_t K>
class Rectangle {

    /**
     * @brief  Stores the lower and upper boundaries for each dimension. 
     * Values are optionals to leave the bounds for that dimension unspecified
     */
    std::array<std::optional<std::pair<T, T>>, K> bounds_;

    public: 

    Rectangle() {clear(); }
    Rectangle(std::array<std::pair<T,T>,K> bounds):bounds_{bounds}{}

    public:
    /**
     * @brief Inserts or overrides a bound in the rectangle. 
     * @throw Throws an exception if the dimension is out of bounds. 
     */
    void insert(size_t dim, T low, T high){
        if (dim > K){throw std::invalid_argument("Invalid Dimension");}
        bounds_[dim] = std::make_pair(low, high);
    }

    /**
     * @brief Checks if a dimension has a bound specified. 
     */
    bool dimensionDefined (size_t dim) const {return bounds_[dim] != std::nullopt; } 

    /**
     * Checks if the key (point) is in the bounding box
     */
    bool contains(const std::array<T,K>& key) const {
        for (size_t dim = 0; dim < K; ++dim){
            if(!dimensionDefined(dim)){
                // dimension is not defined, therefore defaults to -inf to +inf
                continue;
            }
            T lower = bounds_[dim]->first;
            T upper = bounds_[dim]->second; 
            if (key[dim] < lower or key[dim] > upper){
                return false;
            }
        }
        // All dimensions are valid!
        return true;
    }

    /**
     * Clears the bounding box 
     */
    void clear(){
        for (size_t dim = 0; dim < K; ++dim){
            bounds_[dim] = std::nullopt;
        }
    }

    /**
     * @brief Accesses the bounds at the specified dimension
     * @throw Will throw an exception if the dimension is not specified
     */
    std::pair<T,T> operator[](size_t dim){
        if (!dimensionDefined(dim)){
            throw std::invalid_argument("Dimension not specified");
        }
        return *bounds_[dim];
    }
};