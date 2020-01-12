#pragma once

#include <map>
#include <pthread.h>
#include "ring.h"

class ring_manager{
public:
    ring_manager();
    ~ring_manager();

public:
    bool init();
    void on_sdk_location_data(location_event& l_event);
    static void* upload_to_platform(void* arg);

private:
    std::map<ring_id_t,ring*> m_ring_table;
    pthread_mutex_t m_mutex_ring_table;
};
