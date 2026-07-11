import pytest
import py_data_structures as ds


# 1. Validation Helpers (Translated from C++ TreeTest methods)
def check_sorted(tree_instance):
    """Extracts keys using tree iteration and verifies they are strictly sorted."""
    # Relies on the __iter__ or container iterator bindings we established
    # If your binding unpacks a tuple/pair, k extracts the key cleanly
    keys = [k for k, _ in tree_instance]
    return keys == sorted(keys)


def get_keys(tree_instance):
    """Helper method to grab an array snapshot of all active keys."""
    return [k for k, _ in tree_instance]


# 2. Dynamic Tree Class Matrix (Replicating TYPED_TEST_SUITE Matrix)
# Wrap the factory functions inside unique lambda proxies to prevent cross-contamination
TREE_IMPLEMENTATIONS = [
    lambda: ds.SplayTree(int, int),
    lambda: ds.ScapegoatTree(int, int)
]

TREE_IDS = [
    "Splay Tree",
    "Scapegoat Tree"
]


# 3. Dynamic Tree Initialization Fixture
@pytest.fixture
def tree_env(request):
    """
    Instantiates a fresh C++ tree, runs the baseline SetUp populations,
    and automatically executes the checkSorted verification post-test.
    """
    tree_factory = request.param
    tree = tree_factory()

    # Base C++ SetUp population array sequence
    keys = [50, 40, 30, 20, 10, 60, 55, 35, 38, 65, 5]
    for k in keys:
        # Pushing structured pairs (tuple) to match your std::pair / dict assignment
        tree.insert((k, k + 100))

    # Hand control over to the matching test block execution flow
    yield tree

    # C++ TearDown lifecycle hook: Enforces sorting verification across ALL test outcomes
    assert check_sorted(tree) is True
    
    # Explicit garbage collection trigger to wipe out compiled pointer instances immediately
    if hasattr(tree, "clear"):
        tree.clear()
    del tree


# 4. Translated Test Blocks (Replaces TYPED_TEST blocks)
@pytest.mark.parametrize("tree_env", TREE_IMPLEMENTATIONS, ids=TREE_IDS, indirect=True)
def test_insertion1(tree_env):
    keys = [65, 5, 20, 10, 55, 35, 30, 50]
    for key in keys:
        # Assuming .find() returns an iterator whose dereferenced target maps to a tuple/pair
        iter_res = tree_env.find(key)
        
        # Validates your custom read-only 'it.value' property descriptor setup
        k, v = iter_res.value
        assert k == key
        assert v == key + 100


@pytest.mark.parametrize("tree_env", TREE_IMPLEMENTATIONS, ids=TREE_IDS, indirect=True)
def test_finding(tree_env):
    tree_env.clear()
    
    # Range list parsing to test your bulk std::map style array iterator insertions
    keys_to_insert = [(50, 150), (40, 140), (5, 105)]
    tree_env.insert(keys_to_insert)

    # Verifies missing index locations safely return the .end() boundary token
    iter_res = tree_env.find(25)
    assert iter_res == tree_env.end()


@pytest.mark.parametrize("tree_env", TREE_IMPLEMENTATIONS, ids=TREE_IDS, indirect=True)
def test_finding2(tree_env):
    keys = get_keys(tree_env)
    for k in keys:
        # This triggers your C++26 reflected operator[] __getitem__ bracket hook seamlessly
        assert tree_env[k] == k + 100


@pytest.mark.parametrize("tree_env", TREE_IMPLEMENTATIONS, ids=TREE_IDS, indirect=True)
def test_deleting1(tree_env):
    # Erase validation routines mapping down to your tree's .erase() / .remove() keys
    assert tree_env.erase(60) == 1
    assert tree_env.erase(40) == 1
    assert tree_env.erase(38) == 1
    assert tree_env.erase(41) == 0

    iter_res = tree_env.find(25)
    assert iter_res == tree_env.end()

    assert tree_env.erase(30) == 1
    assert tree_env.erase(50) == 1

    assert tree_env.size() == 6


@pytest.mark.parametrize("tree_env", TREE_IMPLEMENTATIONS, ids=TREE_IDS, indirect=True)
def test_insertion_and_deletion(tree_env):
    tree_env.clear()
    keys = [1, 26, 3, 4, 5, 7, 19, 6, 8, 11, 10, 9, 2, 12, 11, 16, 14, 15, 18, 17]
    for k in keys:
        tree_env.insert((k, k))
        
    assert tree_env.size() == 19  # Key 11 is a duplicate, structure squashes it

    # Loop to verify sequential removals and instant size footprint matching
    for k in [2,7,8,19,6,11,10,1,3,14]:
        assert tree_env.erase(k) == 1, f"Couldn't erase {k}"
        assert len(get_keys(tree_env)) == tree_env.size(), f"Size mismatch after erasing {k}"

    final_keys = get_keys(tree_env)
    
    assert final_keys == sorted(final_keys)
    assert tree_env.size() == 9
    assert tree_env.size() == len(final_keys)

    for s in [26, 4, 5, 9, 12, 16, 15, 18, 17]:
        assert tree_env[s] == s, f"Expected value: {s} actual value {tree_env[s]}"


@pytest.mark.parametrize("tree_env", TREE_IMPLEMENTATIONS, ids=TREE_IDS, indirect=True)
def test_large_sorted_input(tree_env):
    assert tree_env.size() == 11
    # Heavy range generation test to stress balance heights or splay limits
    for i in range(80, 25000):
        tree_env.insert((i, i + 100))
        
    assert tree_env.size() == 24931 # 11 base keys + (25000 - 80) keys
