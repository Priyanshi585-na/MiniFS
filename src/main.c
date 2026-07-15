#include "disk.h"

#include <stdio.h>

int main(){
    if(create_disk("disk_img") == 0){
        printf("Disk created.\n");
    }

    if(open_disk("disk.img") == 0){
        printf("Disk opened. \n");
    }

    close_disk();

    return 0;
}