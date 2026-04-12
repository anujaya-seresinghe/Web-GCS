import time
import socket
import math
from pymavlink.dialects.v20 import common as mavlink2

# =========================
# UDP CONFIG
# =========================
UDP_IP = "172.20.0.10"
UDP_PORT = 18560

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# =========================
# START POSITION
# =========================
START_LAT = 48.223066
START_LON = 11.676839

# movement radius (meters)
RADIUS_M = 30

# earth constants
METERS_PER_DEG_LAT = 111111.0
METERS_PER_DEG_LON = 111111.0 * math.cos(math.radians(START_LAT))


# =========================
# MAVLINK PACK
# =========================
def build_mavlink(lat, lon, alt=100):
    mav = mavlink2.MAVLink(None)

    msg = mavlink2.MAVLink_global_position_int_message(
        time_boot_ms=int(time.time() * 1000) % 4294967295,
        lat=int(lat * 1e7),
        lon=int(lon * 1e7),
        alt=int(alt * 1000),
        relative_alt=0,
        vx=0,
        vy=0,
        vz=0,
        hdg=0
    )

    return msg.pack(mav)


# =========================
# SEND FUNCTION
# =========================
def send(lat, lon):
    packet = build_mavlink(lat, lon)
    sock.sendto(packet, (UDP_IP, UDP_PORT))
    print(f"Sent -> lat={lat:.7f}, lon={lon:.7f}")


# =========================
# MAIN LOOP (CIRCULAR MOTION)
# =========================
if __name__ == "__main__":
    t = 0

    while True:
        # circular motion
        dx = RADIUS_M * math.cos(t)
        dy = RADIUS_M * math.sin(t)

        lat = START_LAT + (dy / METERS_PER_DEG_LAT)
        lon = START_LON + (dx / METERS_PER_DEG_LON)

        send(lat, lon)

        t += 0.1
        time.sleep(0.5)
