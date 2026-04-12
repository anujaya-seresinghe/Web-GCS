#!/bin/bash

echo "Starting UDP→MQTT..."
./udp_to_mqtt &

echo "Starting MQTT→UDP..."
./mqtt_to_udp &

wait
