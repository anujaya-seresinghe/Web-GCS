import { useEffect, useState, useRef } from "react";

export function useWaypoints() {
  const [waypoints, setWaypoints] = useState([]);
  const wpCounter = useRef(0);

  useEffect(() => {
    const saved = localStorage.getItem("waypoints");
    if (saved) setWaypoints(JSON.parse(saved));

    const counter = localStorage.getItem("wp_counter");
    if (counter) wpCounter.current = parseInt(counter);
  }, []);

  useEffect(() => {
    localStorage.setItem("waypoints", JSON.stringify(waypoints));
    localStorage.setItem("wp_counter", wpCounter.current);
  }, [waypoints]);

  return { waypoints, setWaypoints, wpCounter };
}