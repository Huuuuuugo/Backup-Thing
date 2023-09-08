#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <time.h>
#include <string>


std::string CurrentDateTime()
{
    using namespace std;

    time_t now = time(0);
    struct tm tstruct;
    char buf[35];

    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d (%Hh %Mmin %Ss)", &tstruct);

    return buf;
}