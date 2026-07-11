from . import ds_ext
import enum


class HeapCompare(enum.Enum):
    MinHeap = 0
    MaxHeap = 1

# Central type name translation maps
_TYPE_MAP = {int: "Int", float: "Float", str: "String", None: ""}
_HEAP_VAL_MAP = {int: "int", float: "float"}
_COMP_MAP = {HeapCompare.MinHeap: "Min", HeapCompare.MaxHeap: "Max"}

def _resolve_and_instantiate(cpp_class_name, clsname, *args, **kwargs):
    """Unified dynamic loader and error injector for the compiled backend."""
    if not hasattr(ds_ext, cpp_class_name):
        raise ValueError(
            f"{clsname} variant ({cpp_class_name}) is not compiled in the backend. "
            f"Please verify available instantiations."
        )
    return getattr(ds_ext, cpp_class_name)(*args, **kwargs)

def _validate_types(clsname, *types):
    """Enforces common primitive support checks."""
    for t in types:
        if t not in _TYPE_MAP and t is not None:
            raise TypeError(f"{clsname} does not support data type: {getattr(t, '__name__', str(t))}.")

# --- Spatial & Interval Trees ---

def KDTree(data_type, dimensions):
    _validate_types("KDTree", data_type)
    return _resolve_and_instantiate(f"{_TYPE_MAP[data_type]}KDTree{dimensions}D", "KDTree")

def Rectangle(data_type, dimensions):
    _validate_types("Rectangle", data_type)
    return _resolve_and_instantiate(f"{_TYPE_MAP[data_type]}Rectangle{dimensions}D", "Rectangle")

def IntervalTree(data_type):
    _validate_types("IntervalTree", data_type)
    return _resolve_and_instantiate(f"IntervalTree{_TYPE_MAP[data_type]}", "IntervalTree")

def Interval(data_type, low, high):
    _validate_types("Interval", data_type)
    return _resolve_and_instantiate(f"Interval{_TYPE_MAP[data_type]}", "Interval", low, high)

# --- Prioritized Heaps ---

def _build_heap(data_type, priority_type, heapType, clsname):
    if data_type not in _HEAP_VAL_MAP or priority_type not in _HEAP_VAL_MAP:
        raise TypeError(f"{clsname} does not support data type: {data_type} or priority type {priority_type}")
    suffix = f"{_HEAP_VAL_MAP[data_type]}{_HEAP_VAL_MAP[priority_type]}{_COMP_MAP[heapType]}"
    return _resolve_and_instantiate(f"{clsname}{suffix}", clsname)

def PairingHeap(data_type, priority_type, heapType=HeapCompare.MinHeap):
    return _build_heap(data_type, priority_type, heapType, "PairingHeap")

def BinomialHeap(data_type, priority_type, heapType=HeapCompare.MinHeap):
    return _build_heap(data_type, priority_type, heapType, "BinomialHeap")

def FibonacciHeap(data_type, priority_type, heapType=HeapCompare.MinHeap):
    return _build_heap(data_type, priority_type, heapType, "FibonacciHeap")

def DAryHeap(data_type, priority_type, d=16, heapType=HeapCompare.MinHeap):
    return _build_heap(data_type, priority_type, heapType, f"DAryHeap{d}")

# --- Linear Containers & Search Trees ---

def UnrolledLinkedList(data_type, k):
    _validate_types("UnrolledLinkedList", data_type)
    return _resolve_and_instantiate(f"ull{_TYPE_MAP[data_type]}{k}", "UnrolledLinkedList")

def SplayTree(key_type, value_type):
    _validate_types("SplayTree", key_type, value_type)
    return _resolve_and_instantiate(f"SplayTree{_TYPE_MAP[key_type]}{_TYPE_MAP[value_type]}", "SplayTree")

def ScapegoatTree(key_type, value_type, alpha: float = 2/3):
    _validate_types("ScapegoatTree", key_type, value_type)
    # Preservation notice: C++ signature doesn't take alpha, mirror original setup
    return _resolve_and_instantiate(f"ScapegoatTree{_TYPE_MAP[key_type]}{_TYPE_MAP[value_type]}", "ScapegoatTree")

def CuckooHashMap(key_type, value_type, epsilon=0.4, downsizeThresh=0.2):
    _validate_types("CuckooHashMap", key_type, value_type)
    name = f"CuckooHashMap{_TYPE_MAP[key_type]}{_TYPE_MAP[value_type]}"
    return _resolve_and_instantiate(name, "CuckooHashMap", epsilon, downsizeThresh)

def CuckooHashSet(key_type, epsilon=0.4, downsizeThresh=0.2):
    _validate_types("CuckooHashSet", key_type)
    return _resolve_and_instantiate(f"CuckooHashSet{_TYPE_MAP[key_type]}", "CuckooHashSet", epsilon, downsizeThresh)

def Quack(data_type):
    _validate_types("Quack", data_type)
    return _resolve_and_instantiate(f"Quack{_TYPE_MAP[data_type]}", "Quack")
