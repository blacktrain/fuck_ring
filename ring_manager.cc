#include <unistd.h>
#include "ring_manager.h"

ring_manager::ring_manager(){

}

ring_manager::~ring_manager(){

}

bool ring_manager::init(){
    pthread_mutex_init(&m_mutex_ring_table,nullptr);
    pthread_t thrd;
    pthread_create(&thrd,nullptr,ring_manager::upload_to_platform,this);
    return true;
}

void ring_manager::on_sdk_location_data(location_event& l_event){
    ring* ring_obj = nullptr;
    std::map<ring_id_t,ring*>::iterator itor = m_ring_table.find(l_event.ring_id);
    if(itor == m_ring_table.end()){
        ring_obj = new ring(l_event.ring_id);
        pthread_mutex_lock(&m_mutex_ring_table);
        m_ring_table[l_event.ring_id] = ring_obj;
        pthread_mutex_unlock(&m_mutex_ring_table);
    }
    else{
        ring_obj = itor->second;
    }

    ring_obj->on_recv_location_event(l_event);
}

void* ring_manager::upload_to_platform(void* arg){
    ring_manager* manager = (ring_manager*)arg;

    ring* ring_obj = nullptr;
	while(true){
        std::map<ring_id_t,ring*>::iterator itor = manager->m_ring_table.begin();
        pthread_mutex_lock(&manager->m_mutex_ring_table);
        for(;itor != manager->m_ring_table.end();++itor){
            ring_obj = itor->second;
            pthread_mutex_unlock(&manager->m_mutex_ring_table);
            ring_obj->on_send_area_event();
            pthread_mutex_lock(&manager->m_mutex_ring_table);
        }
        pthread_mutex_unlock(&manager->m_mutex_ring_table);
        usleep(1000);
	}
}
