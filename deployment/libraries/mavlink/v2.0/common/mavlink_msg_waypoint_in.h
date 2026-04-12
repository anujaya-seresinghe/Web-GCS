#pragma once
// MESSAGE WAYPOINT_IN PACKING

#define MAVLINK_MSG_ID_WAYPOINT_IN 302


typedef struct __mavlink_waypoint_in_t {
 float time_usec; /*< [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.*/
 int32_t lat; /*< [degE7] Latitude*/
 int32_t lon; /*< [degE7] Longitude*/
 uint16_t wp_id; /*<  Waypoint ID*/
} mavlink_waypoint_in_t;

#define MAVLINK_MSG_ID_WAYPOINT_IN_LEN 14
#define MAVLINK_MSG_ID_WAYPOINT_IN_MIN_LEN 14
#define MAVLINK_MSG_ID_302_LEN 14
#define MAVLINK_MSG_ID_302_MIN_LEN 14

#define MAVLINK_MSG_ID_WAYPOINT_IN_CRC 185
#define MAVLINK_MSG_ID_302_CRC 185



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_WAYPOINT_IN { \
    302, \
    "WAYPOINT_IN", \
    4, \
    {  { "time_usec", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_waypoint_in_t, time_usec) }, \
         { "wp_id", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_waypoint_in_t, wp_id) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_waypoint_in_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_waypoint_in_t, lon) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_WAYPOINT_IN { \
    "WAYPOINT_IN", \
    4, \
    {  { "time_usec", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_waypoint_in_t, time_usec) }, \
         { "wp_id", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_waypoint_in_t, wp_id) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_waypoint_in_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_waypoint_in_t, lon) }, \
         } \
}
#endif

/**
 * @brief Pack a waypoint_in message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_usec [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.
 * @param wp_id  Waypoint ID
 * @param lat [degE7] Latitude
 * @param lon [degE7] Longitude
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_in_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float time_usec, uint16_t wp_id, int32_t lat, int32_t lon)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_IN_LEN];
    _mav_put_float(buf, 0, time_usec);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_uint16_t(buf, 12, wp_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WAYPOINT_IN_LEN);
#else
    mavlink_waypoint_in_t packet;
    packet.time_usec = time_usec;
    packet.lat = lat;
    packet.lon = lon;
    packet.wp_id = wp_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WAYPOINT_IN_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WAYPOINT_IN;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_WAYPOINT_IN_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_CRC);
}

/**
 * @brief Pack a waypoint_in message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_usec [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.
 * @param wp_id  Waypoint ID
 * @param lat [degE7] Latitude
 * @param lon [degE7] Longitude
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_in_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               float time_usec, uint16_t wp_id, int32_t lat, int32_t lon)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_IN_LEN];
    _mav_put_float(buf, 0, time_usec);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_uint16_t(buf, 12, wp_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WAYPOINT_IN_LEN);
#else
    mavlink_waypoint_in_t packet;
    packet.time_usec = time_usec;
    packet.lat = lat;
    packet.lon = lon;
    packet.wp_id = wp_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WAYPOINT_IN_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WAYPOINT_IN;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_WAYPOINT_IN_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_WAYPOINT_IN_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_LEN);
#endif
}

/**
 * @brief Pack a waypoint_in message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_usec [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.
 * @param wp_id  Waypoint ID
 * @param lat [degE7] Latitude
 * @param lon [degE7] Longitude
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_waypoint_in_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float time_usec,uint16_t wp_id,int32_t lat,int32_t lon)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_IN_LEN];
    _mav_put_float(buf, 0, time_usec);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_uint16_t(buf, 12, wp_id);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_WAYPOINT_IN_LEN);
#else
    mavlink_waypoint_in_t packet;
    packet.time_usec = time_usec;
    packet.lat = lat;
    packet.lon = lon;
    packet.wp_id = wp_id;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_WAYPOINT_IN_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_WAYPOINT_IN;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_WAYPOINT_IN_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_CRC);
}

/**
 * @brief Encode a waypoint_in struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param waypoint_in C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_waypoint_in_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_waypoint_in_t* waypoint_in)
{
    return mavlink_msg_waypoint_in_pack(system_id, component_id, msg, waypoint_in->time_usec, waypoint_in->wp_id, waypoint_in->lat, waypoint_in->lon);
}

/**
 * @brief Encode a waypoint_in struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param waypoint_in C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_waypoint_in_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_waypoint_in_t* waypoint_in)
{
    return mavlink_msg_waypoint_in_pack_chan(system_id, component_id, chan, msg, waypoint_in->time_usec, waypoint_in->wp_id, waypoint_in->lat, waypoint_in->lon);
}

/**
 * @brief Encode a waypoint_in struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param waypoint_in C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_waypoint_in_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_waypoint_in_t* waypoint_in)
{
    return mavlink_msg_waypoint_in_pack_status(system_id, component_id, _status, msg,  waypoint_in->time_usec, waypoint_in->wp_id, waypoint_in->lat, waypoint_in->lon);
}

/**
 * @brief Send a waypoint_in message
 * @param chan MAVLink channel to send the message
 *
 * @param time_usec [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.
 * @param wp_id  Waypoint ID
 * @param lat [degE7] Latitude
 * @param lon [degE7] Longitude
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_waypoint_in_send(mavlink_channel_t chan, float time_usec, uint16_t wp_id, int32_t lat, int32_t lon)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_WAYPOINT_IN_LEN];
    _mav_put_float(buf, 0, time_usec);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_uint16_t(buf, 12, wp_id);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_IN, buf, MAVLINK_MSG_ID_WAYPOINT_IN_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_CRC);
#else
    mavlink_waypoint_in_t packet;
    packet.time_usec = time_usec;
    packet.lat = lat;
    packet.lon = lon;
    packet.wp_id = wp_id;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_IN, (const char *)&packet, MAVLINK_MSG_ID_WAYPOINT_IN_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_CRC);
#endif
}

/**
 * @brief Send a waypoint_in message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_waypoint_in_send_struct(mavlink_channel_t chan, const mavlink_waypoint_in_t* waypoint_in)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_waypoint_in_send(chan, waypoint_in->time_usec, waypoint_in->wp_id, waypoint_in->lat, waypoint_in->lon);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_IN, (const char *)waypoint_in, MAVLINK_MSG_ID_WAYPOINT_IN_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_CRC);
#endif
}

#if MAVLINK_MSG_ID_WAYPOINT_IN_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_waypoint_in_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float time_usec, uint16_t wp_id, int32_t lat, int32_t lon)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, time_usec);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_uint16_t(buf, 12, wp_id);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_IN, buf, MAVLINK_MSG_ID_WAYPOINT_IN_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_CRC);
#else
    mavlink_waypoint_in_t *packet = (mavlink_waypoint_in_t *)msgbuf;
    packet->time_usec = time_usec;
    packet->lat = lat;
    packet->lon = lon;
    packet->wp_id = wp_id;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_WAYPOINT_IN, (const char *)packet, MAVLINK_MSG_ID_WAYPOINT_IN_MIN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_LEN, MAVLINK_MSG_ID_WAYPOINT_IN_CRC);
#endif
}
#endif

#endif

// MESSAGE WAYPOINT_IN UNPACKING


/**
 * @brief Get field time_usec from waypoint_in message
 *
 * @return [us] Timestamp (UNIX Epoch time or time since system boot). The receiving end can infer timestamp format (since 1.1.1970 or since system boot) by checking for the magnitude of the number.
 */
static inline float mavlink_msg_waypoint_in_get_time_usec(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field wp_id from waypoint_in message
 *
 * @return  Waypoint ID
 */
static inline uint16_t mavlink_msg_waypoint_in_get_wp_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  12);
}

/**
 * @brief Get field lat from waypoint_in message
 *
 * @return [degE7] Latitude
 */
static inline int32_t mavlink_msg_waypoint_in_get_lat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field lon from waypoint_in message
 *
 * @return [degE7] Longitude
 */
static inline int32_t mavlink_msg_waypoint_in_get_lon(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Decode a waypoint_in message into a struct
 *
 * @param msg The message to decode
 * @param waypoint_in C-struct to decode the message contents into
 */
static inline void mavlink_msg_waypoint_in_decode(const mavlink_message_t* msg, mavlink_waypoint_in_t* waypoint_in)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    waypoint_in->time_usec = mavlink_msg_waypoint_in_get_time_usec(msg);
    waypoint_in->lat = mavlink_msg_waypoint_in_get_lat(msg);
    waypoint_in->lon = mavlink_msg_waypoint_in_get_lon(msg);
    waypoint_in->wp_id = mavlink_msg_waypoint_in_get_wp_id(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_WAYPOINT_IN_LEN? msg->len : MAVLINK_MSG_ID_WAYPOINT_IN_LEN;
        memset(waypoint_in, 0, MAVLINK_MSG_ID_WAYPOINT_IN_LEN);
    memcpy(waypoint_in, _MAV_PAYLOAD(msg), len);
#endif
}
