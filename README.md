# Wep App

## Build and run the Docker containers with:
```
bash deployment/startGCS.sh
```
To access the web app: 
http://172.20.0.13:3000/

# Testing the MAVLink interface
## MAVLink interceptor
Access the docker container service-test. While inside the container, run 
```
./mavlink_server
```

## To send UAV positions:
Run the Python script send_coords.py
```
python3 send_coords.py
```

# Issues
If running send_coords.py does not display an icon of an UAV, restart the container mavlink-bridge

# TO DO
Save waypoints in a database 


-----------------------------------------------------------------------------------------------------
For a more complete description, refer to docs


