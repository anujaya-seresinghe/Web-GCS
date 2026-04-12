export const geoStyle = (feature) => {
  if (feature.properties?.building) return { color: "gray" };
  if (feature.properties?.highway) return { color: "blue" };
  if (feature.properties?.leisure) return { color: "green" };
  return { color: "black", weight: 1 };
};