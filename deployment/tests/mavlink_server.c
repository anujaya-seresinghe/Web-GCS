#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "mavlink/v2.0/common/mavlink.h"

#define SERVER_PORT 18590
#define BUFFER_SIZE 2048

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    uint8_t buffer[BUFFER_SIZE];

    // -----------------------------
    // Create UDP server socket
    // -----------------------------
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Allow reuse (good practice for servers)
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // -----------------------------
    // Bind to port 18590 (SERVER)
    // -----------------------------
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("MAVLink UDP SERVER listening on port %d\n", SERVER_PORT);

    // -----------------------------
    // MAVLink parser
    // -----------------------------
    mavlink_message_t msg;
    mavlink_status_t status;

    while (1) {
        ssize_t len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                               (struct sockaddr *)&client_addr, &client_len);

        if (len < 0) {
            perror("recvfrom failed");
            continue;
        }

        for (ssize_t i = 0; i < len; i++) {
            if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status)) {

                printf("MSG ID: %d | SYS: %d | COMP: %d\n",
                       msg.msgid, msg.sysid, msg.compid);
                
            switch (msg.msgid) {
                case MAVLINK_MSG_ID_WAYPOINT_IN: {
                    mavlink_waypoint_in_t wp_in;
                    mavlink_msg_waypoint_in_decode(&msg, &wp_in);
                    printf("[WAYPOINT_IN] system id=%d waypoint id=%u lat=%.7f lon=%.7f\n", msg.sysid, wp_in.wp_id, wp_in.lat/1e7, wp_in.lon/1e7);
                    break;
                }

                case MAVLINK_MSG_ID_WAYPOINT_OUT: {
                    mavlink_waypoint_out_t wp_out;
                    mavlink_msg_waypoint_out_decode(&msg, &wp_out);
                    printf("[WAYPOINT_OUT] system id=%d waypoint id=%u lat=%.7f lon=%.7f\n", msg.sysid, wp_out.wp_id, wp_out.lat/1e7, wp_out.lon/1e7);
                    break;
                }

                default:
                    // optionally print unknown messages
                    // printf("Unknown msgid: %d\n", msg.msgid);
                    break;
                }

            
            }
        }
    }

    close(sockfd);
    return 0;
}
