# Setting Up

## To build and run the Docker containers, go to deployment and run:
```
bash startGCS.sh
```
## To access the web app: 
http://172.20.0.13:3000/

## To shut down docker containers, go to deployment and run:
```
bash stopGCS.sh
```

# Waypoints
To create a waypoint, left click anywhere on the map. To remove it, right click on that waypoint. Waypoints can be dragged and dropped. 

# Testing the MAVLink Interface
The web app communicates with outside by using MAVLink. To listen to incoming MAVLink packets and to send MAVLink packets, two programs are available in the service-test container. mavlink_server listens to incoming MAVLink packets coming from the web app. When a waypoint is created, for example, a MAVLink packet is sent and this is then processed by mavlink_server. 

The web app also displays the position of UAVs and this is communicated by the MAVLink message GLOBAL_POSITION_INT. The Python script send_coords.py in the service-test container sends global position messages and the web app would then visualize the position of the UAV.  

## MAVLink interceptor
Access the docker container service-test. While inside the container, run:
```
./mavlink_server
```

## To send UAV positions:
Run the Python script send_coords.py in the service-test container:
```
python3 send_coords.py
```

## Issues
If running send_coords.py does not display an icon of an UAV, restart the container mavlink-bridge




-----------------------------------------------------------------------------------------------------
For a more complete description, refer to docs


