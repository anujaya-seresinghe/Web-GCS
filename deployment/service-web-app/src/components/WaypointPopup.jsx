export default function WaypointPopup({ waypoints, onClose }) {
  return (
    <div
      style={{
        position: "absolute",
        top: "50%",
        left: "50%",
        transform: "translate(-50%, -50%)",
        background: "white",
        width: "400px",
        height: "500px",          // ✅ fixed height
        zIndex: 2000,
        borderRadius: "10px",
        padding: "16px",
        display: "flex",
        flexDirection: "column",
        boxShadow: "0 10px 30px rgba(0,0,0,0.3)",
      }}
    >
      {/* HEADER */}
      <h3 style={{ marginBottom: "10px" }}>Waypoints</h3>

      {/* SCROLLABLE CONTENT */}
      <div
        style={{
          flex: 1,                 // takes remaining space
          overflowY: "auto",      // ✅ scroll enabled
          border: "1px solid #ddd",
          padding: "8px",
          borderRadius: "6px",
        }}
      >
        {waypoints.length === 0 && <p>No waypoints yet</p>}

        {waypoints.map((wp) => (
          <div key={wp.wp_id} style={{ marginBottom: "10px" }}>
            <b>wp_id:</b> {wp.wp_id} <br />
            <b>lat:</b> {wp.lat.toFixed(6)} <br />
            <b>lng:</b> {wp.lng.toFixed(6)}
            <hr />
          </div>
        ))}
      </div>

      {/* FOOTER BUTTON */}
      <button
        onClick={onClose}
        style={{
          marginTop: "10px",
          padding: "8px",
          border: "none",
          background: "black",
          color: "white",
          borderRadius: "6px",
          cursor: "pointer",
        }}
      >
        Close
      </button>
    </div>
  );
}