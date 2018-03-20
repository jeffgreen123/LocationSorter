from geopy.geocoders import GoogleV3

fname_adresses = ["addresses.txt","stops.txt","starts.txt"]
fname_adressesOut = ["addressesOut.txt","stopsOut.txt","startsOut.txt"]
for idx, val in enumerate(fname_adresses):
    outFile = open(fname_adressesOut[idx],"w")
    with open(fname_adresses[idx]) as f:
        content = f.readlines()
    for address in content:
	print address;
	if address == "\n":
            outFile.write("\n");
	else:
            geolocator =  GoogleV3(api_key = "AIzaSyBirONocHuXh5un_iDZVLfE_pg6c6vv4EQ", timeout = None)
            location = geolocator.geocode(address + ", Canada")
            if location != None: 
                latitude = location.latitude
                longitude = location.longitude
	        address =  str(location.raw['address_components'][0]['short_name']) + " " + str(location.raw['address_components'][1]['short_name']);
	        outFile.write(str(latitude) + "," + str(longitude) + "=" + address + "\n");
                print latitude
                print longitude

    outFile.close();
