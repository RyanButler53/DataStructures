import math
import pytest
import py_data_structures as ds


# 1. Helper Functions (Translated from C++ Test2D methods)
def dist(p1, p2):
    """Calculates Euclidean distance between two points."""
    dx = p1[0] - p2[0]
    dy = p1[1] - p2[1]
    return math.sqrt((dx * dx) + (dy * dy))


def points_in_radius(pts, q, r):
    """Returns points within radius r from query point q."""
    return [p for p in pts if dist(p, q) <= r]


# 2. Pytest Fixture (Replaces C++ Test2D SetUp/Member Data)
class TreeContext:
    """Holds data vectors and populated trees mimicking the C++ fixture."""
    def __init__(self):
        # Python lists of tuples seamlessly match your std::array / std::vector bindings
        self.v1 = [(35, 40), (5, 45), (25, 35), (50, 10), (90, 5), (60, 75), (80, 65), (85, 15)]
        self.v2 = [(35, 60), (20, 45), (60, 80), (80, 40), (90, 60), (50, 30), (70, 20), (75, 10), (10, 35), (20, 20)]
        self.v3 = [(10, 20), (11, 5), (5, 10), (5, 8), (15, 2), (20, 1)]
        self.v4 = [(8, 10), (12, 7), (8, 6), (11, 6), (10, 2)]

        # Assuming IntTree2D is exposed via your nanobind reflection engine
        self.t1 = ds.IntTree2D()
        self.t2 = ds.IntTree2D()
        self.t3 = ds.IntTree2D()
        self.t4 = ds.IntTree2D()

        # Populate trees mimicking SetUp()
        for k in self.v1: self.t1.insert(k)
        for k in self.v2: self.t2.insert(k)
        for k in self.v3: self.t3.insert(k)
        for k in self.v4: self.t4.insert(k)

    # 3. Translated Validation Helpers
    def assert_range_query(self, tree, points, q, r):
        """Verifies rangeQuery results match the math model."""
        expected = points_in_radius(points, q, r)
        # Assuming DistanceFunction enum is exported to python as well
        actual = tree.rangeQuery(q, r, ds.DistanceFunction.Euclidean)
        
        # Unordered elements match can be checked via sets of tuples
        assert set(actual) == set(expected)

    def assert_nearest_neighbor(self, tree, points, q):
        """Verifies nearestNeighbor matches a sorted distance scan."""
        # Sort points by their distance from query point q
        actual_nn = min(points, key=lambda pt: dist(pt, q))
        tree_nn = tree.nearestNeighbor(q, ds.DistanceFunction.Euclidean)
        assert tree_nn == actual_nn


@pytest.fixture
def env():
    """Provides a fresh, populated test environment for every test function."""
    return TreeContext()


# 4. Translated Test Blocks
def test_find_min(env):
    xmin = env.t1.findMin(0)
    ymin = env.t1.findMin(1)

    assert xmin == 5
    assert ymin == 5

    # Test Exception Handling (Replaces C++ try/catch block)
    with pytest.raises(RuntimeError) as exc_info:
        env.t1.findMin(2)
    assert str(exc_info.value) == "Invalid Dimension"

    # Insert minimum in both dimensions
    p = (-1, -1)
    env.t1.insert(p)

    assert env.t1.findMin(0) == -1
    assert env.t1.findMin(1) == -1


def test_size(env):
    assert env.t1.size() == 8
    assert env.t2.size() == 10
    assert env.t3.size() == 6
    assert env.t4.size() == 5


def test_contains(env):
    assert env.t1.contains((60, 75)) is True
    env.t1.insert((16, 27))
    assert env.t1.contains((16, 27)) is True
    assert env.t1.contains((90, 5)) is True
    assert env.t1.size() == 9


def test_delete2(env):
    env.t3.remove((10, 20))
    assert env.t3.findMin(0) == 5
    assert env.t3.findMin(1) == 1
    assert env.t3.size() == 5


def test_delete1(env):
    env.t2.remove((35, 60))
    assert env.t2.contains((35, 60)) is False
    assert env.t2.findMin(1) == 10

    env.t2.remove((10, 35))
    assert env.t2.contains((10, 35)) is False
    assert env.t2.findMin(0) == 20
    assert env.t2.size() == 8

    env.t3.remove((10, 20))
    assert env.t3.contains((10, 20)) is False
    assert env.t3.size() == 5


def test_range_query(env):
    # Testing default distance fallback variant
    q = env.t4.rangeQuery((10, 7), 4.0, ds.DistanceFunction.Euclidean)
    assert len(q) == 4
    
    expected = [(8, 10), (12, 7), (8, 6), (11, 6)]
    # Use sets to check unordered array equality cleanly
    assert set(tuple(p) for p in q) == set(expected)


def test_range_query2(env):
    q = (20, 35)
    r = 15.0
    env.assert_range_query(env.t2, env.v2, q, r)


def test_range_query3(env):
    q = (11, 5)
    r = 4.0
    env.assert_range_query(env.t2, env.v2, q, r)


def test_nearest_neighbor1(env):
    nn = env.t2.nearestNeighbor((50, 60))
    assert nn == (35, 60)

    nn = env.t2.nearestNeighbor((35, 30))
    assert nn == (50, 30)

    nn = env.t2.nearestNeighbor((100, 100))
    assert nn == (90, 60)


def test_nearest_neighbor2(env):
    env.assert_nearest_neighbor(env.t3, env.v3, (12, 6))
    env.assert_nearest_neighbor(env.t4, env.v4, (11, 11))


def test_k_nearest_neighbor1(env):
    query = (35, 30)
    expected = [(50, 30), (20, 20), (20, 45)]
    # Assuming your k-NN overload accepts the 'k' parameter seamlessly
    nn = env.t2.nearestNeighbor(query, 3)
    assert set(tuple(p) for p in nn) == set(expected)


def test_rectangle_bounding_box():
    # Instantiating your bound Rectangle class
    bounds = ds.IntRectangle2D()
    
    # Python arguments map down to (dimension, min, max)
    bounds.insert(0, 59, 95)
    bounds.insert(1, 39, 81)
    
    assert bounds.contains((60, 80)) is True
    assert bounds.contains((80, 40)) is True
 
    # Reset/clear operations testing
    bounds.clear()
    assert bounds.contains((20, 60)) is True

    # One-dimensional bounding constraint
    bounds.insert(0, 59, 95)
    assert bounds.contains((60, 150)) is True
    assert bounds.contains((80, -150)) is True
    assert bounds.contains((30, 75)) is False


def test_rectangle_range(env):
    bounds = ds.IntRectangle2D()
    bounds.insert(0, 59, 95)
    bounds.insert(1, 39, 81)
    
    points = env.t2.rangeQuery(bounds)
    expected = [(60, 80), (80, 40), (90, 60)]
    assert set(tuple(p) for p in points) == set(expected)

    bounds2 = ds.IntRectangle2D()
    bounds2.insert(0, 33, 65)
    
    points = env.t2.rangeQuery(bounds2)
    expected = [(60, 80), (35, 60), (50, 30)]
    assert set(tuple(p) for p in points) == set(expected)


def test_clear(env):
    env.t1.clear()
    assert env.t1.size() == 0
    
    env.t1.insert((15, 20))
    assert env.t1.findMin(0) == 15
    assert env.t1.size() == 1
    
    env.t1.remove((15, 20))
    assert env.t1.size() == 0

# Extra tests leveraging the range/nn helpers you wrote on the C++ side
# def test_range_query_validation(env):
#     env.assert_range_query(env.t1, env.v1, (30, 35), 15.0)


# def test_nearest_neighbor_validation(env):
#     env.assert_nearest_neighbor(env.t2, env.v2, (55, 55))
