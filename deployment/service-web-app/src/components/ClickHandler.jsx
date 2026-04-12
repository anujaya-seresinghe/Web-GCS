import { useMapEvents } from "react-leaflet";

export default function ClickHandler({ setWaypoints, clientRef, wpCounter }) {
  useMapEvents({
    click(e) {
      setWaypoints((prev) => {
        const wp_id = wpCounter.current++;

        const waypoint = {
          id: Date.now(),
          wp_id,
          lat: e.latlng.lat,
          lng: e.latlng.lng,
        };

        const payload = {
          sys_id: 0,
          wp_id,
          lat: waypoint.lat,
          lon: waypoint.lng,
        };

        console.log("MQTT PUBLISH (WP_IN):", payload);

        clientRef.current?.connected &&
          clientRef.current.publish(
            "mavlink/wp_in",
            JSON.stringify(payload)
          );

        return [...prev, waypoint];
      });
    },
  });

  return null;
}