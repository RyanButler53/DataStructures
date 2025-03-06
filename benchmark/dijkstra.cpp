// Benchmark heaps with Dijkstra's algorithm

template <typename T>
concept Heap = requires(T &heap,
                        T::value_type value) {
    heap();
    heap.empty();
    heap.size();
    heap.pop();
    heap.push(value);
    heap.top();
    heap.decreaseKey();
};

class Graph{
};

template <Heap heap_t>
int dijkstra(Graph g){
    return 5;
}

int main(){
    return 0;
}
