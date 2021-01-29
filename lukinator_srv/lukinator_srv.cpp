
#include "nitrogen/net/net.hpp"
#include <cstdlib>

net net1;

void srv_fn () {
    char b_in [80];
    if (! net1.get_line (b_in, sizeof(b_in))) {
        dlog::error ("srv_fn", "get_line");
        return;
    }

    if (! strcmp (b_in, "LUCAS ALERT")) {
        dlog::debug ("srv_fn", "received ALERT");
        if (! net1.send_str ("ALERT ACK"))
            dlog::error ("srv_fn", "send_str");
        system ("/usr/local/bin/alert");
    }
    else {
        dlog::error ("srv_fn: unknown request", b_in);
        if (! net1.send_str ("REQUEST UNKNOWN"))
            dlog::error ("srv_fn", "send_str(2)");
        return;
    }

    return;
}

int main () {
    if (! net1.server (17045, srv_fn))
        return -1;

    return 0;
} 

