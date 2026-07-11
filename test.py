import py_data_structures as ds

st = ds.SplayTree(int, int)
st.insert((15, 5))
st.insert((20, 6))
print(st)

print(st[15])
st[25] = 12
st[15] = 14
print(st)