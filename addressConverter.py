from geopy.geocoders import GoogleV3

fname_in = "/home/jgreen/LocationSorter/addresses.txt"
fname_out = "/home/jgreen/LocationSorter/addressesOut.txt"
outFile = open(fname_out,"w")
with open(fname_in) as f:
    content = f.readlines()
for address in content:
    geolocator =  GoogleV3(api_key = "AIzaSyBirONocHuXh5un_iDZVLfE_pg6c6vv4EQ")
    location = geolocator.geocode(address)
    if location != None: 
        latitude = location.latitude
        longitude = location.longitude
	outFile.write(str(latitude) + "," + str(longitude) + "\n");
        print latitude
        print longitude

outFile.close();
