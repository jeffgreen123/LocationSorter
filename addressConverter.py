from geopy.geocoders import GoogleV3

fname_in = "addresses.txt"
fname_out = "addressesOut.txt"
outFile = open(fname_out,"w")
with open(fname_in) as f:
    content = f.readlines()
for address in content:
    geolocator =  GoogleV3(api_key = "AIzaSyBirONocHuXh5un_iDZVLfE_pg6c6vv4EQ")
    location = geolocator.geocode(address + ", Canada")
    if location != None: 
        latitude = location.latitude
        longitude = location.longitude
	address =  str(location.raw['address_components'][0]['short_name']) + " " + str(location.raw['address_components'][1]['short_name']);
	outFile.write(str(latitude) + "," + str(longitude) + "=" + address + "\n");
        print latitude
        print longitude

outFile.close();
