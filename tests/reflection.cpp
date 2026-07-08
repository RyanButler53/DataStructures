#include <meta>
#include <print>
#include "spatial/kdtree.hpp"

// Goal is to print member functions, member vars (public only), operators and the types of the function arguments
template <std::meta::info m>
constexpr void printFunctionArgs(){
    // assume that m is a reflection of a function
    static constexpr auto params = std::define_static_array(std::meta::parameters_of(m));
    template for (constexpr std::meta::info p : params){
        if constexpr (std::meta::has_identifier(p)){
            std::println("  Arg: {}", std::meta::identifier_of(p));
        } else {
            std::println("  Argument without identifier");
        }
    }

}

template <typename T>
void printMembers(){
    constexpr std::meta::info r = ^^T;
    constexpr auto ctx = std::meta::access_context::unprivileged();
    static constexpr auto members = std::define_static_array(std::meta::members_of(r, ctx));
    template for (constexpr std::meta::info m : members){
        if constexpr (std::meta::is_nonstatic_data_member(m)){
            std::println("Data Memeber: {}",  std::meta::identifier_of(m));
        } else if constexpr (std::meta::is_operator_function(m)){
            constexpr std::meta::operators op = std::meta::operator_of(m);
            std::println("Operator: {}", std::meta::symbol_of(op));
        }  else if constexpr (std::meta::is_constructor(m)){
            std::println("Constructor: {}", std::meta::display_string_of(m));
            // Get the parameter names, pybind doesn't need the types!
            printFunctionArgs<m>();
        } else if constexpr (std::meta::is_destructor(m)){
            std::println("Destructor: {}", std::meta::display_string_of(m));
        } else if constexpr (std::meta::is_class_member(m) && std::meta::is_function(m)){
            std::println("Member Function: {}", std::meta::identifier_of(m));
            printFunctionArgs<m>();
        } else {
            std::println("Something else: {}", std::meta::display_string_of(m));
        }
    }
}

int main(){
    printMembers<Tree3D<long>>();
    // std::println();
    // printMembers<Rectangle<int, 5>>();
}