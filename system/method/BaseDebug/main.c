#include <unistd.h>
main()
{
char resolved_path[80];
realpath("FW_room_pad_2g_7inch_0.1.0.zip",resolved_path);
printf("resolved_path: %s\n", resolved_path);
}
