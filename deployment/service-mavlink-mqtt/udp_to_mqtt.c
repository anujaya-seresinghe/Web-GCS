#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <mosquitto.h>
#include <fcntl.h>
#include "mavlink/v2.0/common/mavlink.h"

#define UDP_PORT 18550
#define MQTT_HOST "mosquitto"
#define MQTT_TOPIC "mavlink/position"

int sock;
struct mosquitto *mosq;

int main()
{
    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq)
    {
        printf("Failed to create mosquitto instance\n");
        return 1;
    }

    // Auto-reconnect settings
    mosquitto_reconnect_delay_set(mosq, 2, 10, true);

    if (mosquitto_connect(mosq, MQTT_HOST, 1883, 60) != MOSQ_ERR_SUCCESS)
    {
        printf("MQTT connect failed\n");
        return 1;
    }

    // ⭐ CRITICAL: start MQTT background thread
    mosquitto_loop_start(mosq);

    // UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(UDP_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Bind failed");
        return 1;
    }

    // Optional: make socket non-blocking (safer)
    fcntl(sock, F_SETFL, O_NONBLOCK);

    uint8_t buffer[2048];
    mavlink_message_t msg;
    mavlink_status_t status;

    printf("[UDP→MQTT] running\n");

    while (1)
    {
        ssize_t len = recv(sock, buffer, sizeof(buffer), 0);

        if (len <= 0)
        {
            usleep(1000);  // prevent CPU spin
            continue;
        }

        // Debug (optional)
        // printf("Received %ld bytes\n", len);

        for (int i = 0; i < len; i++)
        {
            if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status))
            {
                if (msg.msgid == MAVLINK_MSG_ID_GLOBAL_POSITION_INT)
                {
                    mavlink_global_position_int_t pos;
                    mavlink_msg_global_position_int_decode(&msg, &pos);

                    char payload[128];
                    snprintf(payload, sizeof(payload),
                             "{\"sysid\":%d,\"lat\":%.6f,\"lon\":%.6f}",
                             msg.sysid,
                             pos.lat / 1e7,
                             pos.lon / 1e7);

                    int ret = mosquitto_publish(
                        mosq,
                        NULL,
                        MQTT_TOPIC,
                        strlen(payload),
                        payload,
                        0,
                        false);

                    if (ret != MOSQ_ERR_SUCCESS)
                    {
                        printf("Publish error: %s\n", mosquitto_strerror(ret));
                    }

                    printf("[PUBLISH] %s\n", payload);
                }
            }
        }
    }

    // Cleanup (never reached, but good practice)
    mosquitto_loop_stop(mosq, true);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}