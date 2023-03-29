#pragma once

#include <Winsock2.h>

class DataSender {
private:
    DataSender();
    DataSender(const DataSender&) = delete;

    ~DataSender();
    
    DataSender& operator=(const DataSender&) = delete;

    sockaddr_in server_address = { 0 };
    int socket_fd = 0;

    inline double normalize(const double in, const int min, const int max);

public:
    static DataSender& get_instance() {
        static DataSender instance;
        return instance;
    }

    void send_data(const double translation_x, const double translation_y, const double translation_z,
        const double rotation_x, const double rotation_y, const double totation_z, const double rotation_w);
};
