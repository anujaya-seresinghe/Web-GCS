import {
  MapContainer,
  TileLayer,
  GeoJSON,
  Marker,
  Polyline,
} from "react-leaflet";

import { useEffect, useState } from "react";

import ClickHandler from "./ClickHandler";
import WaypointPopup from "./WaypointPopup";

import { yellowDotIcon, uavIcon } from "../map/icons";
import { catmullRomSpline } from "../map/spline";
import { geoStyle } from "../map/geoStyle";

import { useWaypoints } from "../hooks/useWaypoints";
import { useMqttUavs } from "../hooks/useMqttUavs";

import "leaflet/dist/leaflet.css";
import "../leafletFix";

export default function MapView() {
  const [data, setData] = useState(null);
  const [showPopup, setShowPopup] = useState(false);

  const { waypoints, setWaypoints, wpCounter } = useWaypoints();
  const { uavs, clientRef } = useMqttUavs();

  /* =========================
     LOAD GEOJSON
  ========================= */
  useEffect(() => {
    fetch("/ismaning.geojson")
      .then((res) => res.json())
      .then(setData);
  }, []);

  /* =========================
     REMOVE WAYPOINT
  ========================= */
  const removeWaypoint = (wp) => {
    setWaypoints((prev) =>
      prev.filter((p) => p.wp_id !== wp.wp_id)
    );

    const payload = {
      sys_id: 0,
      wp_id: wp.wp_id,
      lat: wp.lat,
      lon: wp.lng,
      action: "delete",
    };

    console.log("MQTT PUBLISH (WP_OUT):", payload);

    if (clientRef.current?.connected) {
      clientRef.current.publish(
        "mavlink/wp_out",
        JSON.stringify(payload)
      );
    }
  };

  /* =========================
     RESET WAYPOINTS
  ========================= */
  const resetWaypoints = () => {
    setWaypoints([]);
    wpCounter.current = 0;

    localStorage.removeItem("waypoints");
    localStorage.setItem("wp_counter", "0");

    console.log("Waypoints reset");
  };

  /* =========================
     SPLINE PATH
  ========================= */
  const smoothPath = catmullRomSpline(
    waypoints.map((wp) => [wp.lat, wp.lng]),
    25
  );

  return (
    <div style={{ position: "relative" }}>
      
      {/* =========================
         TOP BUTTONS
      ========================= */}
      <div
        style={{
          position: "absolute",
          top: 10,
          right: 10,
          zIndex: 1000,
          display: "flex",
          gap: "8px",
        }}
      >
        <button
          onClick={() => setShowPopup(true)}
          style={{
            padding: "8px 12px",
            background: "black",
            color: "white",
            borderRadius: "6px",
            border: "none",
          }}
        >
          Upload
        </button>

        <button
          onClick={resetWaypoints}
          style={{
            padding: "8px 12px",
            background: "red",
            color: "white",
            borderRadius: "6px",
            border: "none",
          }}
        >
          Reset
        </button>
      </div>

      {/* =========================
         WAYPOINT POPUP
      ========================= */}
      {showPopup && (
        <WaypointPopup
          waypoints={waypoints}
          onClose={() => setShowPopup(false)}
        />
      )}

      {/* =========================
         MAP
      ========================= */}
      <MapContainer
        center={[48.233, 11.683]}
        zoom={16}
        style={{ height: "100vh", width: "100%" }}
      >
       
        <TileLayer url="/tiles/{z}/{x}/{y}.png" />

        {data && <GeoJSON data={data} style={geoStyle} />}

        <ClickHandler
          setWaypoints={setWaypoints}
          clientRef={clientRef}
          wpCounter={wpCounter}
        />

        {/* =========================
           SPLINE PATH
        ========================= */}
        {smoothPath.length > 1 && (
          <Polyline
            positions={smoothPath}
            color="red"
            weight={4}
          />
        )}

        {/* =========================
           DRAGGABLE WAYPOINT MARKERS
        ========================= */}
        {waypoints.map((wp) => (
          <Marker
            key={wp.wp_id}
            position={[wp.lat, wp.lng]}
            icon={yellowDotIcon}
            draggable={true}
            eventHandlers={{
              contextmenu: () => removeWaypoint(wp),

              dragend: (e) => {
                const marker = e.target;
                const newPos = marker.getLatLng();

               
                setWaypoints((prev) =>
                  prev.map((p) =>
                    p.wp_id === wp.wp_id
                      ? { ...p, lat: newPos.lat, lng: newPos.lng }
                      : p
                  )
                );

           
                const payload = {
                  sys_id: 0,
                  wp_id: wp.wp_id,
                  lat: newPos.lat,
                  lon: newPos.lng,
                  action: "update",
                };

                console.log("MQTT PUBLISH (WP_UPDATE):", payload);

              
                if (clientRef.current?.connected) {
                  clientRef.current.publish(
                    "mavlink/wp_in",
                    JSON.stringify(payload)
                  );
                }
              },
            }}
          />
        ))}

        {/* =========================
           UAV MARKERS
        ========================= */}
        {Object.entries(uavs).map(([id, uav]) => (
          <Marker
            key={`uav-${id}`}
            position={[uav.lat, uav.lon]}
            icon={uavIcon}
          />
        ))}

      </MapContainer>
    </div>
  );
}