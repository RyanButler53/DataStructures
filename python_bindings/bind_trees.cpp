#include "bindStructure.hpp"
#include "tree/splay-tree.hpp"
#include "tree/scapegoat.hpp"
#include <nanobind/nanobind.h>
#include "interfaces.hpp"
#include "bindings.hpp"
#include <string>
#include <iterator>
#include <ranges>

namespace nb = nanobind;

namespace {
// Struct to do casting without copying. Needs to convert nb::handle to std::pair<K,V>
template <typename TargetType> 
class NbCastingIterator {
    nb::iterator cur_;

    public:

    struct ArrowProxy {
        TargetType obj;
        const TargetType* operator->() const { 
            return &obj; 
        }
    };
    using value_type = TargetType;
    using reference = const TargetType&;
    using pointer =  ArrowProxy;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    // Constructors wrapping nanobind's internal sequence iterator
    NbCastingIterator() = default;
    explicit NbCastingIterator(nb::iterator it) : cur_(it) {}

    value_type operator*() const {
        return nb::cast<TargetType>(*cur_);
    }

    NbCastingIterator& operator++() {
        ++cur_;
        return *this;
    }

    NbCastingIterator operator++(int) {
        NbCastingIterator temp = *this;
        ++cur_;
        return temp;
    }

    bool operator==(const NbCastingIterator& other) const {
        return other.cur_ == cur_;
    }

    bool operator!= (const NbCastingIterator& other) const {
        return !(other == *this);
    }

    pointer operator->() const {
        return ArrowProxy(**this);
    }
};


// Special binding for Tree types that have an insert() overload
template <Tree Structure>
void bindInsert(nb::class_<Structure>& cls){
    constexpr std::meta::info r = ^^Structure;
    constexpr auto ctx = std::meta::access_context::unprivileged();
    static constexpr auto members = std::define_static_array(std::meta::members_of(r, ctx));
    template for (constexpr std::meta::info m : members){
        if constexpr (std::meta::is_function_template(m)){
            using value_t = typename Structure::value_type;
            cls.def(std::meta::identifier_of(m).data(), [](Structure& self, nb::iterable seq){
                NbCastingIterator<value_t> start(seq.begin());
                NbCastingIterator<value_t> end(seq.end());
                self.insert(start, end);
            }, nb::arg("pairs"));
        }
    }
}

template <Tree Structure> 
void bindTree(nb::module_& m, std::string name){
    bindIterator<typename Structure::const_iterator>(m, std::format("{}Iterator", name).c_str());
    nb::class_<Structure> cls = bindStructureIterator<Structure>(m, name.c_str());
    bindInsert<Structure>(cls);
}
} // anonymous namespace

void bindTrees(nb::module_& m){

    static constexpr std::array data_types = {^^int, ^^float, ^^std::string};
    static constexpr std::array data_names = {"Int", "Float", "String"};
    template for (constexpr size_t ikey : std::views::iota(0UL, 3UL)){
        template for (constexpr size_t ivalue : std::views::iota(0UL, 3UL)){
        
            static constexpr std::string_view key_name = data_names[ikey];
            static constexpr std::string_view value_name = data_names[ivalue];
            using key_t = typename [: data_types[ikey] :];
            using value_t = typename [: data_types[ivalue] :];
            bindTree<SplayTree<key_t, value_t>>(m, std::format("SplayTree{}{}", key_name, value_name));
            bindTree<ScapegoatTree<key_t, value_t>>(m, std::format("ScapegoatTree{}{}", key_name, value_name));
        }
    }
}