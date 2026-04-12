import L from "leaflet";

export const yellowDotIcon = L.divIcon({
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

export const uavIcon = L.divIcon({
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