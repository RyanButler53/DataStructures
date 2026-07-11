import pytest
import py_data_structures as ds  # Replace with your actual compiled module name


# 1. Factory Helpers to fetch the right chunk-size variant
def make_list(chunk_size):
    """
    Returns an instance of UnrolledLinkedList with a specific K parameter.
    Adjust the attribute lookup names to match your factory logic.
    """
    return ds.UnrolledLinkedList(int, chunk_size)


# 2. Translated Test Functions
def test_overload_block():
    ull = make_list(chunk_size=8)
    v = [0, 1, 2, 3, 4, 5, 6, 7, 8]
    for x in v:
        ull.push_front(x)
    assert ull.size() == 9


def test_front_operator():
    ull = make_list(chunk_size=4)
    ull.push_front(4)
    ull.push_front(3)
    assert ull.front() == 3
    assert ull.back() == 4


def test_push_front3():
    ull = make_list(chunk_size=8)
    for i in range(9):
        ull.push_front(10000 + i)
    ull.push_front(15)
    ull.push_front(16)    
    ull.push_front(17)
    ull.push_front(180000)

    comp = [180000, 17, 16, 15, 10008, 10007, 10006, 10005, 10004, 10003, 10002, 10001, 10000]
    # list(ull) relies on your __iter__ mapping to unpack values
    assert list(ull) == comp


def test_back_operator():
    ull = make_list(chunk_size=4)
    ull.push_back(4)
    ull.push_back(3)
    assert ull.front() == 4
    assert ull.back() == 3


def test_clear():
    ull = make_list(chunk_size=4)
    for i in range(16):
        ull.push_back(i)
    ull.clear()
    assert ull.size() == 0
    ull.push_front(14)
    assert ull.size() == 1
    assert ull.back() == 14


def test_insert_iterator():
    ull = make_list(chunk_size=4)
    for i in range(8):
        ull.push_back(i)
        
    it = ull.begin()
    ull.insert(it, 8)
    assert it.value == 0
    
    for _ in range(3):
        it = it.increment()
    assert it.value == 3
    
    ull.insert(it, 9)
    comp = [8, 0, 1, 2, 9, 3, 4, 5, 6, 7]
    assert list(ull) == comp
    assert it.value == 3
    assert ull.size() == 10

    ull.insert(it, 10)
    comp = [8, 0, 1, 2, 9, 10, 3, 4, 5, 6, 7]
    assert ull.size() == 11
    assert list(ull) == comp

    assert it.value == 3
    ull.insert(it, 11)
    assert ull.size() == 12
    comp = [8, 0, 1, 2, 9, 10, 11, 3, 4, 5, 6, 7]
    assert list(ull) == comp

    for _ in range(3):
        it = it.increment()
    assert it.value == 6
    
    it = it.increment()
    ull.insert(it, 12)
    assert ull.size() == 13
    
    it = it.increment()
    assert it == ull.end()

    comp = [8, 0, 1, 2, 9, 10, 11, 3, 4, 5, 6, 12, 7]
    assert list(ull) == comp

    # it is at end
    ull.insert(it, 13)
    comp = [8, 0, 1, 2, 9, 10, 11, 3, 4, 5, 6, 12, 7, 13]
    assert list(ull) == comp

    ull.insert(it, 14)
    comp = [8, 0, 1, 2, 9, 10, 11, 3, 4, 5, 6, 12, 7, 13, 14]
    assert list(ull) == comp


def test_insertion2():
    ull = make_list(chunk_size=4)
    for i in range(6):
        ull.push_back(i)
    comp = [0, 1, 2, 3, 4, 5]
    assert list(ull) == comp


def test_insert_iterator2():
    ull = make_list(chunk_size=4)
    for i in range(7):
        ull.push_back(i)
    comp = [0, 1, 2, 3, 4, 5, 6]
    assert list(ull) == comp

    it = ull.begin()
    for _ in range(3):
        it = it.increment()

    ull.insert(it, 7)
    comp = [0, 1, 2, 7, 3, 4, 5, 6]
    assert list(ull) == comp

    ull.insert(it, 8)
    comp = [0, 1, 2, 7, 8, 3, 4, 5, 6]
    assert list(ull) == comp


def test_iterator_insertion():
    ull = make_list(chunk_size=4)
    it = ull.begin()
    ull.insert(it, 1500)
    ull.insert(it, 400)


def test_pop_back():
    ull = make_list(chunk_size=4)
    for i in range(6):
        ull.push_back(i)
        
    ull.pop_back()
    comp = [0, 1, 2, 3, 4]
    assert list(ull) == comp
    
    ull.pop_back()
    comp = [0, 1, 2, 3]
    assert list(ull) == comp
    
    ull.pop_back()
    comp = [0, 1, 2]
    assert list(ull) == comp

    ull.pop_back()
    ull.pop_back()
    ull.pop_front()
    assert ull.size() == 0


def test_pop_back_and_front():
    ull = make_list(chunk_size=4)
    for i in range(6):
        ull.push_back(i)
    for _ in range(3):
        ull.pop_front()
        ull.pop_back()


def test_empty_list_exception():
    ull = make_list(chunk_size=4)
    for i in range(6):
        ull.push_back(i)
    for _ in range(3):
        ull.pop_front()
        ull.pop_back()

    # C++ std::invalid_argument maps to a Python ValueError via nanobind
    with pytest.raises(ValueError) as exc_info:
        ull.pop_back()
    assert str(exc_info.value) == "Can't call pop_back on an empty list"


def test_erase_iter1():
    ull = make_list(chunk_size=4)
    for i in range(11):
        ull.push_back(i)
        
    it = ull.begin()
    it = it.increment()
    assert it.value == 1
    
    for i in range(4):
        it = ull.erase(it)
        assert it.value == 2 * (i + 1)
        it = it.increment()
        
    assert ull.size() == 7
    comp = [0, 2, 4, 6, 8, 9, 10]
    assert list(ull) == comp

def test_iteration():
    ull = make_list(chunk_size=4)
    for i in range(11):
        ull.push_back(i)
    ull_sum = 0
    for x in ull:
        ull_sum += x
    assert ull_sum == 55

def test_insert_delete():
    ull = make_list(chunk_size=4)
    it = ull.begin()
    ull.insert(it, 10000)
    assert it == ull.end()
    
    with pytest.raises(ValueError) as exc_info:
        ull.erase(it)
    assert str(exc_info.value) == "Cannot erase from end() iterator"


def test_return_end():
    ull = make_list(chunk_size=4)
    it = ull.begin()
    ull.insert(it, 15)
    
    # Python doesn't support --it, so we assume your C++ loop bound a decrement method
    # or you can look up your custom backward iteration steps.
    if hasattr(it, "decrement"):
        it = it.decrement()
    else:
        # Fallback if decrement isn't exposed: regenerate target position
        it = ull.begin()
        
    it = ull.erase(it)
    assert it == ull.end()
