#include <iostream>
#include "ring.h"

ring::ring(ring_id_t ring_id):
    m_ring_id(ring_id),
    m_last_area_id(invalid_area_id)
{
}

ring::~ring(){
}

void ring::get_area_events_type(area_id_t current_area_id,area_event_type a_events_type_array[],int& a_events_type_count){
    if(current_area_id == invalid_area_id){
        a_events_type_count = 0;
        return;
    }

    if(current_area_id == m_last_area_id){
        a_events_type_count = 0;
        return;
    }

    if(m_last_area_id == invalid_area_id){
        //first event
        a_events_type_array[0] = area_event_in;
        a_events_type_count = 1;
        return;
    }

    //make sure out-area-event must be before in-area-event
    a_events_type_array[0] = area_event_out;
    a_events_type_array[1] = area_event_in;
    a_events_type_count = 2;
}

void ring::get_area_events(location_event& l_event,area_event* a_events_array[],int& a_events_count){
    area_event_type a_events_type_array[2];
    int a_events_type_count = 0;
    get_area_events_type(l_event.area_id,a_events_type_array,a_events_type_count);

    a_events_count = a_events_type_count;

    for(int i = 0;i < a_events_count;++i){
        area_id_t area_id;

        area_event_type a_event_type = a_events_type_array[i];

        if(a_event_type == area_event_in){
            area_id = l_event.area_id;
        }

        if(a_event_type == area_event_out){
            area_id = m_last_area_id;
        }

        a_events_array[i] = new area_event({area_id,a_event_type});
    }

    m_last_area_id = l_event.area_id;
}

bool ring::on_recv_location_event(location_event& l_event){
    if(l_event.ring_id != m_ring_id){
        return false;
    }

    area_event* a_event_array[2];
    int a_event_count = 0;
    get_area_events(l_event,a_event_array,a_event_count);

    for(int i = 0;i < a_event_count;++i){
        m_event_list.push_back(a_event_array[i]);
    }

    return true;
}

bool ring::on_send_area_event(){
    std::list<area_event*>::iterator itor = m_event_list.begin();
    for(;itor != m_event_list.end();){
        area_event* a_event = (area_event*)*itor;
        std::cout << "ring_id:" << m_ring_id
                  << " area_id:" << a_event->area_id
                  << " area_event_type:" << a_event->event_type
                  << std::endl;
        delete *itor;
        itor = m_event_list.erase(itor);
    }
    return true;
}
