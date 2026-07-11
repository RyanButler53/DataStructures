from . import ds_ext
import enum

class HeapCompare(enum.Enum):
    MinHeap=0
    MaxHeap=1

def _find_kd_class(data_type, dimension, clsname, *args, **kwargs):
    type_map = {
        int: "Int",
        float: "Float"
    }

    if data_type not in type_map:
        raise TypeError(f"{clsname} does not support data type: {data_type}.")
    type_suffix = type_map[data_type]
    cpp_class_name = f"{type_suffix}{clsname}{dimension}D"

    # 3. Look up the class dynamically inside the compiled binary module
    if not hasattr(ds_ext, cpp_class_name):
        raise ValueError(
            f"{clsname} dimension {dimension}D for {data_type.__name__} is not compiled in the backend. "
            f"Please verify available instantiations."
        )
    
    # Extract the class constructor object
    CppKDTreeClass = getattr(ds_ext, cpp_class_name)
    
    return CppKDTreeClass(*args, **kwargs)

def _find_interval_class(data_type, clsname):
    type_map = {
        int: "Int",
        float: "Float"
    }

    if data_type not in type_map:
        raise TypeError(f"{clsname} does not support data type: {data_type}.")
    type_suffix = type_map[data_type]
    cpp_class_name = f"{clsname}{type_suffix}"

    # 3. Look up the class dynamically inside the compiled binary module
    if not hasattr(ds_ext, cpp_class_name):
        raise ValueError(
            f"{clsname} type {data_type.__name__} is not compiled in the backend. "
            f"Please verify available instantiations."
        )
    
    # Extract the class constructor object
    IntervalTreeClass = getattr(ds_ext, cpp_class_name)
    return IntervalTreeClass



def KDTree(data_type, dimensions, *args, **kwargs):
   return _find_kd_class(data_type, dimensions, "KDTree")

def Rectangle(data_type, dimensions, *args, **kwargs):
    return _find_kd_class(data_type, dimensions, "Rectangle")

def IntervalTree(data_type, *args, **kwargs):
    Iclass = _find_interval_class(data_type, "IntervalTree")
    return Iclass(*args, **kwargs)

def Interval(data_type, low, high):
    Iclass = _find_interval_class(data_type, "Interval")
    return Iclass(low, high)

# HEAPS

def _get_heap_class(data_type, priority_type, heapType, clsname):
    type_map = {
        int: "int",
        float: "float"
    }
    compare_map = {
        HeapCompare.MinHeap: "Min",
        HeapCompare.MaxHeap: "Max"
    }

    if data_type not in type_map or priority_type not in type_map:
        raise TypeError(f"{clsname} does not support data type: {data_type} or priority type {priority_type}")
    type_suffix = type_map[data_type]
    compare_suffix = compare_map[heapType]

    cpp_class_name = f"{clsname}{type_suffix}{type_suffix}{compare_suffix}"

    # 3. Look up the class dynamically inside the compiled binary module
    if not hasattr(ds_ext, cpp_class_name):
        raise ValueError(
            f"{clsname} type {data_type.__name__} is not compiled in the backend. "
            f"Please verify available instantiations."
        )
    
    # Extract the class constructor object
    heapClass = getattr(ds_ext, cpp_class_name)
    return heapClass()

def PairingHeap(data_type, priority_type, heapType = HeapCompare.MinHeap):
    return  _get_heap_class(data_type, priority_type, heapType, "PairingHeap")

def BinomialHeap(data_type, priority_type, heapType = HeapCompare.MinHeap):
    return _get_heap_class(data_type, priority_type, heapType, "BinomialHeap")

def FibonacciHeap(data_type, priority_type, heapType = HeapCompare.MinHeap):
    return _get_heap_class(data_type, priority_type, heapType, "FibonacciHeap")

def DAryHeap(data_type, priority_type, d = 10, heapType = HeapCompare.MinHeap):
    return _get_heap_class(data_type, priority_type, heapType, f"DAryHeap{d}")



def UnrolledLinkedList(data_type, k):
    type_map = {
        int: "Int",
        float: "Float"
    }

    if data_type not in type_map:
        raise TypeError(f"{clsname} does not support data type: {data_type} or K")
    type_suffix = type_map[data_type]

    cpp_class_name = f"ull{type_suffix}{k}"

    # 3. Look up the class dynamically inside the compiled binary module
    if not hasattr(ds_ext, cpp_class_name):
        raise ValueError(
            f"ull type {data_type.__name__} ({cpp_class_name}) is not compiled in the backend. "
            f"Please verify available instantiations."
        )
    
    # Extract the class constructor object
    ullClass = getattr(ds_ext, cpp_class_name)
    return ullClass()
