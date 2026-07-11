import pytest
import py_data_structures as ds  

# 1. Hardware limits mapping for standard 32-bit signed integers
INT32_MIN = -2147483648       # std::numeric_limits<int>::min()
INT32_MAX = 2147483647        # std::numeric_limits<int>::max()


def sort_intervals(intervals):
    """Sorts intervals primarily by lower bound, then by upper bound to match C++ ordering."""
    # Assuming Interval instances expose attributes or list indices like p[0], p[1]
    # Adjust lambda properties (e.g., pt.low, pt.high) to match your custom reflection bindings
    intervals.sort(key=lambda pt: (pt.low(), pt.high()))

def to_tuple(interval):
    return (interval.low(), interval.high())
# 2. Pytest Fixture (Replaces C++ IntervalTreeTest Setup & Member variables)
class IntervalTreeContext:
    def __init__(self):
        self.min_ = INT32_MIN
        self.max_ = INT32_MAX
        
        # Instantiate your native C++ classes bound via nanobind
        self.itree_ = ds.IntervalTree(int)

        lower_bounds = [10, 15, 18, 12, 21, 9, 17, 30, 19, 12, self.min_]
        upper_bounds = [20, 25, 22, 22, 23, 15, 20, 40, 20, 13, self.max_]

        for i in range(11):
            # Using Python tuples directly to match standard nanobind mapping
            # Alternatively, if you explicitly bound the class 'Interval':
            bounds = ds.Interval(int, lower_bounds[i], upper_bounds[i])
            # bounds = (lower_bounds[i], upper_bounds[i])
            self.itree_.insert(bounds)


@pytest.fixture
def env():
    """Provides a fresh, populated interval tree context for every test function."""
    return IntervalTreeContext()


# 3. Translated Test Blocks
def test_query(env):
    overlaps = env.itree_.findOverlaps(13)
    assert len(overlaps) == 5
    
    # Asserting individual structural elements returned by nanobind
    assert to_tuple(overlaps[0]) == (12, 13)
    assert to_tuple(overlaps[1]) == (12, 22)
    assert to_tuple(overlaps[2]) == (9, 15)
    assert to_tuple(overlaps[3]) == (10, 20)
    assert to_tuple(overlaps[4]) == (env.min_, env.max_)


def test_supersets(env):
    supersets = env.itree_.findSupersets(17, 21)
    assert len(supersets) == 3
    assert to_tuple(supersets[0]) == (12, 22)
    assert to_tuple(supersets[1]) == (15, 25)
    assert to_tuple(supersets[2]) == (env.min_, env.max_)


def test_supersets_max(env):
    supersets = env.itree_.findSupersets(env.min_, env.max_)
    assert len(supersets) == 1
    assert to_tuple(supersets[0]) == (env.min_, env.max_)


def test_range_query(env):
    env.itree_.clear()
    env.itree_.insert((1874, 1951))
    env.itree_.insert((1779, 1828))
    env.itree_.insert((1585, 1672))
    env.itree_.insert((1843, 1907))
    env.itree_.insert((1888, 1971))
    env.itree_.insert((1756, 1791))

    # Overload variant accepting an interval/tuple bounding region
    intervals = env.itree_.findOverlaps((1755, 1830))
    assert len(intervals) == 2
    assert to_tuple(intervals[0]) == (1779, 1828)
    assert to_tuple(intervals[1]) == (1756, 1791)


def test_range_query2(env):
    intervals = env.itree_.findOverlaps((17, 21))
    sort_intervals(intervals)
    
    assert len(intervals) == 8
    lower = [env.min_, 10, 12, 15, 17, 18, 19, 21]
    upper = [env.max_, 20, 22, 25, 20, 22, 20, 23]
    
    for i in range(8):
        assert to_tuple(intervals[i]) == (lower[i], upper[i])
