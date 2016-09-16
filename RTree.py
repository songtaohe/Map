import ctypes
import time
import code

class RTree(object):
	def __init__(self,SONAME):
		self.lib = ctypes.CDLL(SONAME)
		self.lib.PythonInsert.argtypes = [ctypes.c_double,ctypes.c_double,ctypes.c_double,ctypes.c_double, ctypes.c_int]
		self.lib.PythonQuery.argtypes = [ctypes.c_double,ctypes.c_double,ctypes.c_double,ctypes.c_double]
		self.lib.PythonQuery.restype = ctypes.c_int
		self.lib.PythonReadKey.argtypes = [] 
		self.n = 0

	def Insert(self,minx,miny,maxx,maxy,key):
		self.lib.PythonInsert(minx,miny,maxx,maxy,key)
#		time.sleep(1)

	def Query(self,minx,miny,maxx,maxy):
		self.n = self.lib.PythonQuery(minx,miny,maxx,maxy)
		return self.ReadKey()

	def ReadKey(self):
		keys = []
		for i in range(self.n):
			keys.append(self.lib.PythonReadKey())
			#print(self.lib.PythonReadKey())

		return keys


myRTree = RTree('RTree.so')

myRTree.Insert(10,10,20,20,1)
myRTree.Insert(11,11,50,50,2)
myRTree.Insert(15,15,16,16,3)

myRTree.Insert(1,1,2,2,4)
myRTree.Insert(4,4,5,5,5)
myRTree.Insert(0,0,100,100,6)

#print(myRTree.Query(0.5,0.5,8,8))
#print(myRTree.ReadKey())

print(myRTree.Query(-100,-100,200,200))

#code.interact(local=locals())
