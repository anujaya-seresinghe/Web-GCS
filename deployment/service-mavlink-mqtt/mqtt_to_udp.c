#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mosquitto.h>

#include "mavlink/v2.0/common/mavlink.h"

#define MQTT_HOST "mosquitto"
#define MQTT_PORT 1883

#define TOPIC_WP_IN  "mavlink/wp_in"
#define TOPIC_WP_OUT "mavlink/wp_out"

#define COMPONENT_ID 34

/* UDP socket + destination */
int sock;
struct sockaddr_in addr_out;

/* =========================
   SEND WP_IN
========================= */
void send_mavlink_wp_in(uint8_t sys_id, uint16_t wp_id, double lat, double lon)
{
    mavlink_message_t msg;
    uint8_t buffer[300];

    int32_t lat_i = lat * 1e7;
    int32_t lon_i = lon * 1e7;

    mavlink_msg_waypoint_in_pack(sys_id, COMPONENT_ID, &msg,
                                 0, wp_id, lat_i, lon_i);

    int len = mavlink_msg_to_send_buffer(buffer, &msg);

    sendto(sock, buffer, len, 0,
           (struct sockaddr *)&addr_out,
           sizeof(addr_out));

    printf("[WP_IN SENT] sys=%u wp=%u lat=%.6f lon=%.6f\n",
           sys_id, wp_id, lat, lon);
}

/* =========================
   SEND WP_OUT
========================= */
void send_mavlink_wp_out(uint8_t sys_id, uint16_t wp_id, double lat, double lon)
{
    mavlink_message_t msg;
    uint8_t buffer[300];

    int32_t lat_i = lat * 1e7;
    int32_t lon_i = lon * 1e7;

    mavlink_msg_waypoint_out_pack(sys_id, COMPONENT_ID, &msg,
                                  0, wp_id, lat_i, lon_i);

    int len = mavlink_msg_to_send_buffer(buffer, &msg);

    sendto(sock, buffer, len, 0,
           (struct sockaddr *)&addr_out,
           sizeof(addr_out));

    printf("[WP_OUT SENT] sys=%u wp=%u lat=%.6f lon=%.6f\n",
           sys_id, wp_id, lat, lon);
}

/* =========================
   MQTT CALLBACK
========================= */
void on_message(struct mosquitto *mosq, void *ud,
                const struct mosquitto_message *msg)
{
    printf("[MQTT RX] topic=%s payload=%s\n",
           msg->topic, (char*)msg->payload);

    uint8_t sys_id;
    uint16_t wp_id;
    double lat, lon;

    int ok = sscanf((char*)msg->payload,
        "{\"sys_id\":%hhu,\"wp_id\":%hu,\"lat\":%lf,\"lon\":%lf}",
        &sys_id, &wp_id, &lat, &lon);

    if (ok != 4) {
        printf("[ERROR] JSON parse failed\n");
        return;
    }

    if (strcmp(msg->topic, TOPIC_WP_IN) == 0)
        send_mavlink_wp_in(sys_id, wp_id, lat, lon);

    else if (strcmp(msg->topic, TOPIC_WP_OUT) == 0)
        send_mavlink_wp_out(sys_id, wp_id, lat, lon);
}

/* =========================
   MAIN
========================= */
int main()
{
    /* -------------------------
       UDP INIT (NO CONNECT)
    ------------------------- */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket failed");
        return 1;
    }

    memset(&addr_out, 0, sizeof(addr_out));
    addr_out.sin_family = AF_INET;
    addr_out.sin_port = htons(18560);

    if (inet_aton("172.20.0.10", &addr_out.sin_addr) == 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    /* -------------------------
       MQTT INIT
    ------------------------- */
    mosquitto_lib_init();

    struct mosquitto *mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        printf("mosquitto init failed\n");
        return 1;
    }

    mosquitto_message_callback_set(mosq, on_message);

    if (mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60) != MOSQ_ERR_SUCCESS) {
        printf("MQTT connect failed\n");
        return 1;
    }

    mosquitto_subscribe(mosq, NULL, TOPIC_WP_IN, 0);
    mosquitto_subscribe(mosq, NULL, TOPIC_WP_OUT, 0);

    mosquitto_loop_start(mosq);



    while (1) {
        sleep(1);
    }

    return 0;
}