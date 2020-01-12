#pragma once

#include <list>

typedef uint32_t ring_id_t;
typedef uint32_t area_id_t;
const area_id_t invalid_area_id = 0xFFFFFFFF;

struct location_event{
  ring_id_t ring_id;
  area_id_t area_id;
};

enum area_event_type{
  //出区事件发生
  area_event_out,
  //入区事件发生
  area_event_in,
  //没有区域事件发生
  area_event_none
};

struct area_event{
  area_id_t area_id;
  area_event_type event_type;
};

class ring{
public:
  ring(ring_id_t ring_id);
  ~ring();

public:
  bool on_recv_location_event(location_event& event);
  bool on_send_area_event();

private:
  void get_area_events_type(area_id_t current_area_id,area_event_type a_events_type_array[],int& _events_type_count);
  void get_area_events(location_event& l_event,area_event* a_events_array[],int& a_events_count);

private:
  ring_id_t m_ring_id;
  area_id_t m_last_area_id;
  std::list<area_event*> m_event_list;
};
