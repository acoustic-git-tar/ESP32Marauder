#ifndef server_h
#define server_h

#include "WiFi.h"
#include <string.h>
#include "site.h"
#include <DNSServer.h>

namespace EvilTwinServer {
    void server_setup();
    void server_loop();    
}

#endif