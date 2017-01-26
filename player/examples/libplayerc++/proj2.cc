#include <stdio.h>
#include <libplayerc++/playerc++.h>

int main(int argc, char *argv[])
{
    using namespace PlayerCc;
    PlayerClient robot("localhost");

    Position2dProxy p2dProxy(&robot,0);

    while (true) {
      /* code */
    }
    return 0;
}
