import py_data_structures as ds
# print(py_data_structures.addInts(5,4))
k = ds.IntTree2D()
print(k.size())
k.insert([5, 4])
print(k.size())
print(k.contains([5,4]))
k.remove([5,4])
print(f"Post Removal", k.size())
# 