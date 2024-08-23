#include "chestnut.h"
#include <fwoop_log.h>
#include <iostream>
#include <signal.h>

int main(int argc, char *argv[])
{
    signal(SIGPIPE, SIG_IGN);
    return chestnut::Chestnut().start();
}
