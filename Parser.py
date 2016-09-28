import xml.etree.ElementTree
import numpy as np
import matplotlib.pyplot as plt
import code
from MyRTree import RTree

roadForMotorDict = {'motorway','trunk','primary','secondary','tertiary','unclassified','residential','service'}

roadForMotorBlackList = {'None', 'pedestrian','footway','bridleway','steps','path','sidewalk','cycleway','proposed','construction','bus_stop','crossing','elevator','emergency_access_point','escape','give_way'}

#import map from XML  
mapxml = xml.etree.ElementTree.parse('map').getroot()

nodes = mapxml.findall('node')
ways = mapxml.findall('way')
relations = mapxml.findall('relation')

nodedict = {}
waydict = {}
roadlist = [] 
roaddict = {}

minlat = float(mapxml.find('bounds').get('minlat'))
maxlat = float(mapxml.find('bounds').get('maxlat'))
minlon = float(mapxml.find('bounds').get('minlon'))
maxlon = float(mapxml.find('bounds').get('maxlon'))


#
#for arelation in relations:
#	for atag in arelation.findall('tag'):
#		if atag.get('k') == 'type' and atag.get('v') == 'route' :
#			for amember in arelation.findall('member'):
#				if amember.get('type') == 'way':
#					roadlist.append(amember.get('ref'))
#


for anode in nodes:
	nodedict.update({anode.get('id'):anode})

# Load RTree from shared library
myRTree = RTree('RTree.so')
RTreeWays = []



total = len(ways)

for away in ways:
	nds = away.findall('nd')
	highway = 'None'
	for atag in away.findall('tag'):
		if atag.get('k') == 'highway':
			highway = atag.get('v')
		
	if highway not in roadForMotorBlackList: 
		waydict.update({away.get('id'):away})
		mlat = []
		mlon = []
		for anode in away.findall('nd'):
			refid = anode.get('ref')
			mlat.append(float(nodedict[refid].get('lat')))
			mlon.append(float(nodedict[refid].get('lon')))

		wayid = len(RTreeWays)
		print(wayid,total)
		RTreeWays.append(away)
		myRTree.Insert(min(mlon),min(mlat),max(mlon),max(mlat),wayid) 


for key, value in waydict.iteritems():
	mlat = []
	mlon = []
	for anode in value.findall('nd'):
		refid = anode.get('ref')
		mlat.append(float(nodedict[refid].get('lat')))
		mlon.append(float(nodedict[refid].get('lon')))

	plt.plot(mlon,mlat)


plt.axes().set_aspect('equal','datalim')
#plt.xlim([minlon,maxlon])
#plt.ylim([minlat,maxlat])


#Read GPS Trace
value = open('trace').read().split()

gpsx = []
gpsy = []

for i in range(len(value)) :
	if i % 3 == 1 : gpsx.append(float(value[i]))
	if i % 3 == 2 : gpsy.append(float(value[i]))


plt.plot(gpsy,gpsx,'ro')
plt.plot(gpsy,gpsx,'r')



# Test the RTree
# Query all the roads in a rectangle
# Highlight the result
DrawDict = {}
print(myRTree.Query(-71.104-0.01,42.37-0.01,-71.104+0.01,42.37+0.01))

for i in myRTree.Query(-71.104-0.01,42.37-0.01,-71.104+0.01,42.37+0.01):
	v = RTreeWays[i]
	DrawDict.update({v.get('id'):v})


for key, value in DrawDict.iteritems():
	mlat = []
	mlon = []
	for anode in value.findall('nd'):
		refid = anode.get('ref')
		mlat.append(float(nodedict[refid].get('lat')))
		mlon.append(float(nodedict[refid].get('lon')))

	plt.plot(mlon,mlat,linewidth = 2.0)





plt.show()


code.interact(local=locals())




