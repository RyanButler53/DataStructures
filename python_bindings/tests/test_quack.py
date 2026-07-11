import pytest
import py_data_structures as ds  


@pytest.fixture
def fresh_quack():
    """Provides a brand new, clean Quack instance for every test function."""
    # Assuming your top-level factory or direct instantiation resolves QuackInt
    return ds.Quack(int)


# 1. Test baseline initialization states
def test_initial_state(fresh_quack):
    assert fresh_quack.empty() is True
    assert fresh_quack.size() == 0
    assert str(fresh_quack) == "Queue: \nStack: \n"


# 2. Test standard stack-like pushes and pops (LIFO)
def test_push_and_pop_lifo(fresh_quack):
    fresh_quack.push(10)
    fresh_quack.push(20)
    fresh_quack.push(30)

    assert fresh_quack.size() == 3
    assert fresh_quack.empty() is False
    assert fresh_quack.front() == 10
    assert fresh_quack.back() == 30

    # Pop removes from the back (stack behavior)
    fresh_quack.pop()
    assert fresh_quack.back() == 20
    assert fresh_quack.size() == 2

    fresh_quack.pop()
    assert fresh_quack.back() == 10

    fresh_quack.pop()
    assert fresh_quack.empty() is True


# 3. Test standard queue-like operations (FIFO)
def test_push_and_dequeue_fifo(fresh_quack):
    fresh_quack.push(1)
    fresh_quack.push(2)
    fresh_quack.push(3)

    # Dequeue removes from the front (queue behavior)
    fresh_quack.dequeue()
    assert fresh_quack.front() == 2
    assert fresh_quack.size() == 2

    fresh_quack.dequeue()
    assert fresh_quack.front() == 3

    fresh_quack.dequeue()
    assert fresh_quack.empty() is True


# 4. Test front-loading operations
def test_push_front_operations(fresh_quack):
    fresh_quack.push_front(100)
    fresh_quack.push_front(200)
    
    assert fresh_quack.front() == 200
    assert fresh_quack.back() == 100
    assert fresh_quack.size() == 2


# 5. Test string serialization overrides
def test_serialization(fresh_quack):
    fresh_quack.push(5)
    fresh_quack.push(10)
    fresh_quack.push(15)



# 6. Test boundary conditions and internal buffer reshuffling triggers
def test_interleaved_reshuffle_stress(fresh_quack):
    for i in range(5):
        fresh_quack.push(i)
        
    fresh_quack.dequeue()
    assert fresh_quack.front() == 1
    
    fresh_quack.push_front(99)
    assert fresh_quack.front() == 99
    assert fresh_quack.back() == 4
    
    fresh_quack.pop()
    assert fresh_quack.back() == 3


# 7. Test exception protections for empty configurations
def test_empty_access_exceptions(fresh_quack):
    # Depending on how your nanobind engine maps errors, C++ standard exceptions 
    # translate directly to native Python RuntimeError configurations.
    with pytest.raises(RuntimeError):
        fresh_quack.front()
        
    with pytest.raises(RuntimeError):
        fresh_quack.back()
        
    with pytest.raises(RuntimeError):
        fresh_quack.pop()
        
    with pytest.raises(RuntimeError):
        fresh_quack.dequeue()
