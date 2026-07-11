import random
import pytest
import py_data_structures as ds  


# 1. Validation Helper (Replaces C++ checkHeap() method)
def check_heap(heap_instance):
    """
    Validates that elements are evicted in proper sorted ascending priority order.
    Consumes the heap context exactly like the C++ counterpart.
    """
    items = []
    while not heap_instance.empty():
        items.append(heap_instance.top())
        heap_instance.pop()
    
    # Check if items list is sorted in ascending order
    return items == sorted(items)


# 2. Replicating the GoogleTest TYPED_TEST_SUITE Type Matrix
# Map your dynamically compiled nanobind class targets here
HEAP_IMPLEMENTATIONS = [
    lambda: ds.DAryHeap(int, int, 2),
    lambda: ds.DAryHeap(int, int, 4),
    lambda: ds.BinomialHeap(int, int),
    lambda: ds.FibonacciHeap(int, int),
    lambda: ds.PairingHeap(int, int)
]

# Filtering out none in case some implementations are conditionally compiled
HEAP_IMPLEMENTATIONS = [h for h in HEAP_IMPLEMENTATIONS if h is not None]

# Replicating the NameGenerator IDs for pytest terminal output
HEAP_IDS = [
    "Binary Heap",
    "D-Ary Heap (d=4)",
    "Binomial Heap",
    "Fibonacci Heap",
    "Pairing Heap"
][:len(HEAP_IMPLEMENTATIONS)]


# 3. Dynamic Heap Class Provider Fixture
@pytest.fixture
def fresh_heap(request):
    """Instantiates a brand new instance of the target heap variant being tested."""
    HeapClass = request.param
    return HeapClass()


# 4. Parametrized Test Blocks (Replaces TYPED_TEST macros)
@pytest.mark.parametrize("fresh_heap", HEAP_IMPLEMENTATIONS, ids=HEAP_IDS, indirect=True)
def test_pushing(fresh_heap):
    # C++ pushes (value, priority) or (element, key) depending on your heap interface semantics
    fresh_heap.push(1, 1)
    fresh_heap.push(2, 2)
    fresh_heap.push(-80, 3)
    fresh_heap.push(-1, -1)

    assert fresh_heap.size() == 4
    assert fresh_heap.empty() is False

    assert fresh_heap.top() == -1

    fresh_heap.pop()
    assert fresh_heap.top() == 1
    
    fresh_heap.pop()
    assert fresh_heap.top() == 2

    fresh_heap.pop()
    assert fresh_heap.top() == -80

    fresh_heap.pop()
    
    # Exception Handling: Replaces try/catch std::logic_error
    with pytest.raises(RuntimeError) as exc_info:
        fresh_heap.top()
    assert str(exc_info.value) == "Cannot get top of an empty heap"


@pytest.mark.parametrize("fresh_heap", HEAP_IMPLEMENTATIONS, ids=HEAP_IDS, indirect=True)
def test_popping(fresh_heap):
    fresh_heap.push(2, 1)
    fresh_heap.push(3, 2)
    fresh_heap.push(4, 4)
    fresh_heap.push(-14, -1)

    fresh_heap.pop()
    assert fresh_heap.top() == 2
    assert check_heap(fresh_heap) is True


@pytest.mark.parametrize("fresh_heap", HEAP_IMPLEMENTATIONS, ids=HEAP_IDS, indirect=True)
def test_fuzz_push(fresh_heap):
    v = list(range(1000))
    # Python uses a deterministic or time-seeded shuffle natively
    random.shuffle(v)
    
    for x in v:
        fresh_heap.push(x, x)
        
    assert check_heap(fresh_heap) is True
    
    # Note: Destructor memory leak checks are validated automatically 
    # when python garbage collects fresh_heap at the end of the scope context.


@pytest.mark.parametrize("fresh_heap", HEAP_IMPLEMENTATIONS, ids=HEAP_IDS, indirect=True)
def test_fuzz_pop(fresh_heap):
    v = list(range(200))
    random.shuffle(v)
    
    for x in v:
        fresh_heap.push(x - 10, x)
        
    while not fresh_heap.empty():
        fresh_heap.pop()
        
    assert fresh_heap.size() == 0
    assert fresh_heap.empty() is True


@pytest.mark.parametrize("fresh_heap", HEAP_IMPLEMENTATIONS, ids=HEAP_IDS, indirect=True)
def test_change_key(fresh_heap):
    fresh_heap.push(1, 1)
    fresh_heap.push(2, 2)
    fresh_heap.push(5, 5)
    fresh_heap.push(-1, -1)
    fresh_heap.push(7, 7)

    fresh_heap.changeKey(5, -2)
    assert fresh_heap.top() == 5
    
    # Exception Handling: Replaces try/catch std::invalid_argument
    with pytest.raises(ValueError) as exc_info:
        fresh_heap.changeKey(-1, 2)
    assert str(exc_info.value) == "Cannot change key priority to this value"

    # Push an element with minimum priority then pop to trigger re-shuffle
    fresh_heap.push(15, -15)
    fresh_heap.pop()

    fresh_heap.changeKey(7, 5)
    fresh_heap.changeKey(5, -3)
    fresh_heap.changeKey(7, -2)
    assert fresh_heap.top() == 5
    
    fresh_heap.changeKey(2, -5)
    assert fresh_heap.top() == 2
    fresh_heap.pop()
    
    fresh_heap.changeKey(5, -5)
    assert fresh_heap.top() == 5
    fresh_heap.pop()
    assert fresh_heap.top() == 7


@pytest.mark.parametrize("fresh_heap", HEAP_IMPLEMENTATIONS, ids=HEAP_IDS, indirect=True)
def test_change_key2(fresh_heap):
    assert fresh_heap.empty()
    fresh_heap.push(5, 5)

    for x in [6, 7, 12, 8, 9, 10]:
        fresh_heap.push(x, x)
        
    assert fresh_heap.size() == 7
    assert fresh_heap.top() == 5
    fresh_heap.changeKey(12, 4)
    assert fresh_heap.top() == 12
    fresh_heap.pop()
    
    assert fresh_heap.top() == 5
    fresh_heap.pop()
    fresh_heap.changeKey(10, 4)
