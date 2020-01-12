#include <map>
#include <list>
#include <iostream>
#include <unistd.h>
#include "ring.h"
#include "ring_manager.h"

void* fake_zxw_sdk_on_location(void* arg){
    ring_manager* zxw = (ring_manager*)arg;

    location_event test_events[] = {
        {1712,1},
        {1712,2},
        {1712,3},
        {1712,4},
        {1712,5},
        {1712,7},
        {1712,9},
        {1712,5},
        {1712,10},
        {1712,5},
        {28923,1},
        {28923,2},
        {28923,32},
        {28923,41},
        {28923,5},
        {28923,74},
        {28923,9},
        {28923,53},
        {28923,10},
        {28923,52},
    };

    while(true){
        for(int i = 0;i < sizeof(test_events) / sizeof(test_events[0]);++i){
            location_event event = test_events[i];
            zxw->on_sdk_location_data(event);
        }

        sleep(1);
    }

    return nullptr;
}

int main(int argc,char* argv[]){
    ring_manager zxw;
    zxw.init();

    pthread_t thrd;
    pthread_create(&thrd,NULL,fake_zxw_sdk_on_location,&zxw);

    while(true);
}
