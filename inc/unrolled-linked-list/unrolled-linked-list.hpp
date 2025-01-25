// Unrolled linked list needs to have the interface of an std::list

template <typename T>
class UnrolledLinkedList
{
private:
    /* data */
public:
    UnrolledLinkedList<T>();
    
    // UnrolledLinkedList<T>(size_t maxElements);
    
    // template <class InputIt>
    // UnrolledLinkedList<T>(InputIt begin, InputIt end);
    
    ~UnrolledLinkedList<T>();

    // // assignment operator operator=
    // T& front();
    // const T &front() const;
    
    // T &back();
    // const T &back() const;

    // void clear();

    // void insert();
    
    // // template<container-compatible-range<T> rg>
    // // insert_range()
    // template <class iter>
    // void insert_range(iter begin, iter end);

    // void erase(iterator pos);

    // class iterator{
    // };
    
};
