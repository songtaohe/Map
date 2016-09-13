import xml.etree.ElementTree
import numpy as np
import matplotlib.pyplot as plt

roadForMotorDict = {'motorway','trunk','primary','secondary','tertiary','unclassified','residential','service'}

roadForMotorBlackSet = {'None', 'pedestrian','footway','bridleway','steps','path','sidewalk','cycleway','proposed','construction','bus_stop','crossing','elevator','emergency_access_point','escape','give_way'}

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

for away in ways:
	nds = away.findall('nd')
	highway = 'None'
	for atag in away.findall('tag'):
		if atag.get('k') == 'highway':
			highway = atag.get('v')
		
	#if nds[0].get('ref') != nds[-1].get('ref') :
	if highway not in roadForMotorBlackSet: 
		waydict.update({away.get('id'):away})

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


#Read Route Trace

value = open('trace').read().split()

gpsx = []
gpsy = []

for i in range(len(value)) :
	if i % 3 == 1 : gpsx.append(float(value[i]))
	if i % 3 == 2 : gpsy.append(float(value[i]))

plt.plot(gpsy,gpsx,'ro')
plt.plot(gpsy,gpsx,'r')




plt.show()







