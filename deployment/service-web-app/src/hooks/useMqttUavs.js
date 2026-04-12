import { useEffect, useRef, useState } from "react";
import { createMqttClient } from "../mqtt/client";

export function useMqttUavs() {
  const [uavs, setUavs] = useState({});
  const clientRef = useRef(null);

  useEffect(() => {
    clientRef.current = createMqttClient((data) => {
      setUavs((prev) => ({
        ...prev,
        [data.sysid]: {
          lat: Number(data.lat),
          lon: Number(data.lon),
        },
      }));
    });

    return () => clientRef.current?.end();
  }, []);

  return { uavs, clientRef };
}