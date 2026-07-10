from . import ds_ext

def _find_class(data_type, dimension, clsname, *args, **kwargs):
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

def KDTree(data_type, dimensions, *args, **kwargs):
   return _find_class(data_type, dimensions, "KDTree")

def Rectangle(data_type, dimensions, *args, **kwargs):
    return _find_class(data_type, dimensions, "Rectangle")

