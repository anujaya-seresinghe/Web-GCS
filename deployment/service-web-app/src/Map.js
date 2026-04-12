import {
  MapContainer,
  TileLayer,
  GeoJSON,
  Marker,
  Polyline,
  useMapEvents
} from "react-leaflet";

import { useEffect, useState, useRef } from "react";
import L from "leaflet";
import mqtt from "mqtt";
import "leaflet/dist/leaflet.css";
import "./leafletFix";

/* =========================
   ICONS
========================= */

const yellowDotIcon = L.divIcon({
  className: "",
  html: `<div style="
    width: 10px;
    height: 10px;
    background: yellow;
    border: 2px solid black;
    border-radius: 50%;
  "></div>`,
  iconSize: [10, 10],
  iconAnchor: [5, 5],
});

const uavIcon = L.divIcon({
  className: "",
  html: `<div style="
    width: 0;
    height: 0;
    border-left: 10px solid transparent;
    border-right: 10px solid transparent;
    border-bottom: 18px solid red;
    transform: rotate(180deg);
  "></div>`,
  iconSize: [20, 20],
  iconAnchor: [10, 10],
});

/* =========================
   SPLINE
========================= */

function catmullRomSpline(points, segments = 25) {
  if (points.length < 2) return points;

  const result = [];

  for (let i = 0; i < points.length - 1; i++) {
    const p0 = points[i - 1] || points[i];
    const p1 = points[i];
    const p2 = points[i + 1];
    const p3 = points[i + 2] || p2;

    for (let t = 0; t <= 1; t += 1 / segments) {
      const t2 = t * t;
      const t3 = t2 * t;

      const lat =
        0.5 *
        (2 * p1[0] +
          (-p0[0] + p2[0]) * t +
          (2 * p0[0] - 5 * p1[0] + 4 * p2[0] - p3[0]) * t2 +
          (-p0[0] + 3 * p1[0] - 3 * p2[0] + p3[0]) * t3);

      const lng =
        0.5 *
        (2 * p1[1] +
          (-p0[1] + p2[1]) * t +
          (2 * p0[1] - 5 * p1[1] + 4 * p2[1] - p3[1]) * t2 +
          (-p0[1] + 3 * p1[1] - 3 * p2[1] + p3[1]) * t3);

      result.push([lat, lng]);
    }
  }

  return result;
}

/* =========================
   CLICK HANDLER
========================= */

function ClickHandler({ setWaypoints, clientRef, wpCounter }) {
  useMapEvents({
    click(e) {

      setWaypoints((prev) => {

        const wp_id = wpCounter.current++;

        const waypoint = {
          id: Date.now(),
          wp_id,
          lat: e.latlng.lat,
          lng: e.latlng.lng
        };

        const payload = {
          sys_id: 0,
          wp_id,
          lat: waypoint.lat,
          lon: waypoint.lng
        };

        // PRINT EVERY MQTT MESSAGE
        console.log("MQTT PUBLISH (WP_IN):", payload);

        if (clientRef.current?.connected) {
          clientRef.current.publish(
            "mavlink/wp_in",
            JSON.stringify(payload)
          );
        }

        return [...prev, waypoint];
      });

    },
  });

  return null;
}

/* =========================
   MAIN MAP
========================= */

export default function Map() {
  const [data, setData] = useState(null);
  const [waypoints, setWaypoints] = useState([]);
  const [showPopup, setShowPopup] = useState(false);
  const [uavs, setUavs] = useState({});

  const clientRef = useRef(null);
  const wpCounter = useRef(0);

  /* =========================
     LOAD DATA
  ========================= */

  useEffect(() => {
    fetch("/ismaning.geojson")
      .then((res) => res.json())
      .then(setData);

    const savedWaypoints = localStorage.getItem("waypoints");
    if (savedWaypoints) setWaypoints(JSON.parse(savedWaypoints));

    const savedCounter = localStorage.getItem("wp_counter");
    if (savedCounter) wpCounter.current = parseInt(savedCounter);
  }, []);

  /* =========================
     SAVE STATE
  ========================= */

  useEffect(() => {
    localStorage.setItem("waypoints", JSON.stringify(waypoints));
    localStorage.setItem("wp_counter", wpCounter.current);
  }, [waypoints]);

  /* =========================
     MQTT
  ========================= */

  useEffect(() => {
    const client = mqtt.connect("ws://localhost:9001");
    clientRef.current = client;

    client.subscribe("mavlink/position");

    client.on("message", (_, msg) => {
      try {
        const data = JSON.parse(msg.toString());

        setUavs((prev) => ({
          ...prev,
          [data.sysid]: {
            lat: Number(data.lat),
            lon: Number(data.lon),
          }
        }));

      } catch (e) {
        console.error("MQTT error", e);
      }
    });

    return () => client.end();
  }, []);

  /* =========================
     REMOVE WAYPOINT (WP_OUT)
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
      action: "delete"
    };

    // PRINT EVERY MQTT MESSAGE
    console.log("MQTT PUBLISH (WP_OUT):", payload);

    if (clientRef.current?.connected) {
      clientRef.current.publish(
        "mavlink/wp_out",
        JSON.stringify(payload)
      );
    }
  };

  /* =========================
     SPLINE PATH
  ========================= */

  const rawPoints = waypoints.map((wp) => [wp.lat, wp.lng]);
  const smoothPath = catmullRomSpline(rawPoints, 25);

  /* =========================
     STYLE
  ========================= */

  const style = (feature) => {
    if (feature.properties?.building) return { color: "gray" };
    if (feature.properties?.highway) return { color: "blue" };
    if (feature.properties?.leisure) return { color: "green" };
    return { color: "black", weight: 1 };
  };

  return (
    <div style={{ position: "relative" }}>

      <div style={{
        position: "absolute",
        top: 10,
        right: 10,
        zIndex: 1000
      }}>
        <button
          onClick={() => setShowPopup(true)}
          style={{
            padding: "8px 12px",
            background: "black",
            color: "white",
            borderRadius: "6px",
            border: "none"
          }}
        >
          Upload
        </button>
      </div>

      {showPopup && (
        <div style={{
          position: "absolute",
          top: "50%",
          left: "50%",
          transform: "translate(-50%, -50%)",
          background: "white",
          padding: "20px",
          zIndex: 2000
        }}>
          <h3>Waypoints</h3>

          {waypoints.map((wp) => (
            <div key={wp.wp_id}>
              wp_id: {wp.wp_id} <br />
              lat: {wp.lat.toFixed(6)} <br />
              lng: {wp.lng.toFixed(6)}
              <hr />
            </div>
          ))}

          <button onClick={() => setShowPopup(false)}>
            Close
          </button>
        </div>
      )}

      <MapContainer
        center={[48.233, 11.683]}
        zoom={13}
        style={{ height: "100vh", width: "100%" }}
      >

        <TileLayer url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png" />

        {data && <GeoJSON data={data} style={style} />}

        <ClickHandler
          setWaypoints={setWaypoints}
          clientRef={clientRef}
          wpCounter={wpCounter}
        />

        {smoothPath.length > 1 && (
          <Polyline positions={smoothPath} color="red" weight={4} />
        )}

        {waypoints.map((wp) => (
          <Marker
            key={wp.wp_id}
            position={[wp.lat, wp.lng]}
            icon={yellowDotIcon}
            eventHandlers={{
              contextmenu: () => removeWaypoint(wp)
            }}
          />
        ))}

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