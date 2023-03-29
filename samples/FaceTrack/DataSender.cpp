#include "DataSender.hpp"

#include <Winsock2.h>
#include <Ws2tcpip.h>

#include <math.h>

#pragma comment(lib, "Ws2_32.lib")

DataSender::DataSender()
{
    // set server address
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(4242);
    server_address.sin_family = AF_INET;

    // create datagram socket
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    // connect to server
    connect(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address));
}

DataSender::~DataSender()
{
    closesocket(socket_fd);
}

void DataSender::send_data(const double translation_x, const double translation_y, const double translation_z,
    const double rotation_x, const double rotation_y, const double totation_z, const double rotation_w)
{
    double packet[6] = { 0 };

    // process translation data
    packet[0] = translation_x - 50; // initial head horizontal position should be in the center of the camera (full range 0-100)
    packet[1] = translation_y - 30; // while we sit still, initial head height should be at 30
    packet[2] = translation_z;

    // convert rotation data from quaternion to euler angle
    double roll = atan2(2 * rotation_y * rotation_w - 2 * rotation_x * totation_z, 1 - 2 * rotation_y * rotation_y - 2 * totation_z * totation_z);
    double pitch = atan2(2 * rotation_x * rotation_w - 2 * rotation_y * totation_z, 1 - 2 * rotation_x * rotation_x - 2 * totation_z * totation_z);
    double yaw = asin(2 * rotation_x * rotation_y + 2 * totation_z * rotation_w);

    // normalize rotation angle to a smaller range, in order to have better responsiveness
    packet[3] = -normalize(roll, -45, 45);
    packet[4] = -normalize(pitch, -45, 45);
    packet[5] = -normalize(yaw, -45, 45);

    //printf("%f\t%f\t%f\t%f\t%f\t%f\n", packet[0], packet[1], packet[2], packet[3], packet[4], packet[5]);

    // send packet
    sendto(socket_fd, (const char*)packet, sizeof(packet), 0, 0, sizeof(server_address));
};

inline double DataSender::normalize(const double in, const int min, const int max)
{
    return ((max - min) * ((in - (-1)) / (1 - (-1))) + min);
}
