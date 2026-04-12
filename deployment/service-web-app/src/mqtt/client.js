import mqtt from "mqtt";

export function createMqttClient(onMessage) {
  const client = mqtt.connect("ws://localhost:9001");

  client.subscribe("mavlink/position");

  client.on("message", (_, msg) => {
    try {
      onMessage(JSON.parse(msg.toString()));
    } catch (e) {
      console.error("MQTT error", e);
    }
  });

  return client;
}