#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define BAUDRATE B4800
#define BAUDRATE2 B57600
#define MODEMDEVICE "/dev/ttyO1" 
 
//Struct to read values into
typedef struct GPS_data_t{
    float deg_longitude;
    float deg_latitude;
    float gps_altitude;
    float speed;
    float direction;
    double min_longitude;
    double min_latitude;
    float HDOP;
    int GPS_fix;
}GPS_data_t;
 
//Globals
int GPS_data_flag, GPS_file, GGA_flag, VTG_flag;
GPS_data_t GPS_data;
 
//Function definitions
float get_NMEA_field(int field, char buf[], int comma[]);
void GPS_init(int argc, char *argv[]);
int is_new_GPS_data();
void* GPS_data_watcher(void *ptr);
void read_raw_gps(char *buf);
GPS_data_t get_GPS_data();
 
int main(int argc, char *argv[]){
    GPS_data_t GPS_data; //declare structure for GPS data to do into
    GPS_init(argc, argv);//initialize the GPS
 
    while(1){
        if(is_new_GPS_data()){
            GPS_data=get_GPS_data();
            if(GPS_data.GPS_fix){
                //Print the data from the structure that was just read
                printf("lat: %2.0f°%f ", GPS_data.deg_latitude,GPS_data.min_latitude);
                printf("lon: %2.0f°%f ", GPS_data.deg_longitude, GPS_data.min_longitude);
                printf("speed: %2.2f, dir: %2.2f,",GPS_data.speed,GPS_data.direction);
                printf("alt: %2.2f,",GPS_data.gps_altitude);
                printf("HDOP: %2.2f, fix: %d",GPS_data.HDOP,GPS_data.GPS_fix);
                printf("\n");
            }
            else{
                //If no GPS fix, read raw data
                char buf[225]={0};
                read_raw_gps(buf);
                printf("No Fix, Message transmitted from Module is: %s",buf);
            }
        }
        usleep(20000);
    }
}
 
void GPS_init(int argc, char *argv[]){
    int res;
    struct termios newtio;
    char buf[255];
    GPS_file = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
    if (GPS_file < 0) { perror(MODEMDEVICE); exit(-1); }
 
    chbaud:
    bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
 
    /* BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
       CRTSCTS : output hardware flow control (only used if the cable has
                 all necessary lines. See sect. 7 of Serial-HOWTO)
       CS8     : 8n1 (8bit,no parity,1 stopbit)
       CLOCAL  : local connection, no modem contol
       CREAD   : enable receiving characters */
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    /* IGNPAR  : ignore bytes with parity errors
       otherwise make device raw (no other input processing) */
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    /* ICANON  : enable canonical input
       disable all echo functionality, and don't send signals to calling program */
    newtio.c_lflag = ICANON;
    /* now clean the modem line and activate the settings for the port */
    tcflush(GPS_file, TCIFLUSH);
    tcsetattr(GPS_file,TCSANOW,&newtio);
 
    // Initialize file descriptor sets
    fd_set read_fds, write_fds, except_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);
    FD_SET(GPS_file, &read_fds);
 
    // Set timeout to 1.0 seconds
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
 
    // Wait for input to become ready or until the time out; the first parameter is
    // 1 more than the largest file descriptor in any of the sets
    if (select(GPS_file + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1)
    {
 
        printf("Asking what the old baud rate is \n");
        write(GPS_file,"$PTNLQPT*53\r\n",13);
        usleep(100000);
        res = read(GPS_file, buf, 255);
        buf[res] = 0;
        printf("%s",buf);
        printf("\n");
        usleep(500000);
        printf("\n Changing Baud to 57600. Look for >>$PTNLRPT,A*3D<< for successful transmission\n");
        write(GPS_file,"$PTNLSPT,057600,8,N,1,4,4*12\r\n",30); //Change Baud Rade to 57600
        usleep(100000);
        res = read(GPS_file, buf, 255);
        buf[res] = 0;
        printf("%s",buf);
 
        res = read(GPS_file, buf, 255);
        buf[res] = 0;
        printf("%s",buf);
        usleep(500000);
        res = read(GPS_file, buf, 255);
        buf[res] = 0;
        printf("%s",buf);
 
    }
    else
    {
        // timeout or error
        printf("Already the correct BAUDRATE\n");
    }
 
    //Now the device is reading faster at 57600 Baud, change settings on the beagleboard
    bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
    newtio.c_cflag = BAUDRATE2 | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = ICANON;
    tcflush(GPS_file, TCIFLUSH);
    tcsetattr(GPS_file,TCSANOW,&newtio);
 
    //Check for user inputs between hot, warm, cold, or factory reset starts
    if(argc==2){
            if(!strcmp(argv[1],"hot")){
                if(write(GPS_file, "$PTNLSRT,H,1,,*20<CR><LF>\r\n", 27)==27){
                        printf("Hot Start Selected. \n\n Look for >>$PTNLRRT,A*3F<< for successful confirmation from device \n\n");
                }
                else printf("Write Error \n");
            }
            else if(!strcmp(argv[1],"warm")){
                    if(write(GPS_file, "$PTNLSRT,W,1,,*20<CR><LF>\r\n", 27)==27){
                            printf("Warm Start Selected. \n\n Look for >>$PTNLRRT,A*3F<< for successful confirmation from device \n\n");
                    }
                    else printf("Write Error \n");
            }
            else if(!strcmp(argv[1],"cold")){
                    if(write(GPS_file, "$PTNLSRT,C,1,,*20<CR><LF>\r\n", 27)==27) { //Cold Start
                            printf("Cold Start Selected. \n\n Look for >>$PTNLRRT,A*3F<< for successful confirmation from device \n\n");
                    }
                    else printf("Write Error \n");
 
                argc=1;
                goto chbaud; //On cold start, the module annoyingly goes back to default baudrate. Need to change again
            }
            else if(!strcmp(argv[1],"factory")){
                    if(write(GPS_file, "$PTNLSRT,F,1,,*10<CR><LF>\r\n", 27)==27){
                        printf("Factory Reset Selected. \n\n Look for >>$PTNLRRT,A*3F<< for successful confirmation from device \n\n");
                    }
                    else printf("Write Error \n");
                argc=1;
                goto chbaud;
            }
            else{
                if(write(GPS_file, "$PTNLSRT,C,1,,*20<CR><LF>\r\n", 27)==27) {
                    printf("Cold Start Selected by default. \n\n Look for >>$PTNLRRT,A*3F<< for successful confirmation from device \n\n");
                }
                else printf("Write Error \n");
            }
        }
 
    usleep(100000);
    res = read(GPS_file, buf, 255);
 
    buf[res] = 0;
    printf("%s", buf);
 
    write(GPS_file,"$PTNLSNM,0005,01*52\r\n",21); //NMEA message to output GGA  & VTG only
 
    //Start the GPS thread
    pthread_t gps_thread;
    pthread_create(&gps_thread, NULL, GPS_data_watcher, (void*) NULL);
    printf("GPS Thread Started\n");
}
 
void* GPS_data_watcher(void *ptr){
    char buf[255];
    int res, comma[15];
    char Deg_Lat_buf[12],Min_Lat_buf[12],Deg_Lon_buf[12],Min_Lon_buf[12];
 
    while(1){
        memset(buf,0,sizeof(buf));
        res = read(GPS_file, buf, 255);
        buf[res] = 0;
    //  printf("%s\n",buf);
 
        //Clear all the buffers before reading any data
        memset(Deg_Lat_buf,0,sizeof(Deg_Lat_buf)); memset(Min_Lat_buf,0,sizeof(Min_Lat_buf));
        memset(Deg_Lon_buf,0,sizeof(Deg_Lon_buf)); memset(Min_Lon_buf,0,sizeof(Min_Lon_buf));
        memset(comma,0,sizeof(comma));
        int count=0, i=0;
 
        if(buf[3] == 'G') if(buf[4]=='G') if(buf[5]=='A'){
            while(count<13){
                if(buf[i]==',') {
                    comma[count]=i;
                    count++;
                }
                i++;
            }
 
            if((buf[comma[5]+1]=='1') || (buf[comma[5]+1]=='2')){
                GPS_data.GPS_fix=1;
            }
            else{GPS_data.GPS_fix=0;
            }
 
            //Parse Latitude
            for(i=comma[1];i<=comma[2]-2;i++){
                if(i<=comma[1]+1){
                    Deg_Lat_buf[i-comma[1]]=buf[i+1];
                }
                else{
                    Min_Lat_buf[i-comma[1]-2]=buf[i+1];
                }
            }
            //Parse Longitude
            for(i=comma[3];i<=comma[4]-2;i++){
                if(i<=comma[3]+2){
                    Deg_Lon_buf[i-comma[3]]=buf[i+1];
                }
                else{
                    Min_Lon_buf[i-comma[3]-3]=buf[i+1];
                }
            }
 
            //Parse Horizontal Dilution of Presision, accuracy of measurements
            GPS_data.HDOP=get_NMEA_field(8, buf, comma);
 
            //Get altitude
            GPS_data.gps_altitude=get_NMEA_field(9, buf, comma);
 
            //Convert Strings to floats for use
            GPS_data.deg_latitude=strtod(Deg_Lat_buf,NULL);
            GPS_data.deg_longitude=strtod(Deg_Lon_buf,NULL);
            GPS_data.min_latitude=strtod(Min_Lat_buf,NULL);
            GPS_data.min_longitude=strtod(Min_Lon_buf,NULL);
            GGA_flag=1;
 
        }
        if(buf[3] == 'V') if(buf[4]=='T') if(buf[5]=='G'){
 
            while(count<13){
                if(buf[i]==',') {
                    comma[count]=i;
                    count++;
                }
                i++;
            }
            //Get Speed from NMEA Message
            GPS_data.speed=get_NMEA_field(6, buf, comma);
 
            //Get Direction from NMEA Message
            GPS_data.direction=get_NMEA_field(1, buf, comma);
 
            VTG_flag=1;
        }
        //printf("GGA: %d VTG %d Data ready: %d\n\n",GGA_flag,VTG_flag,GPS_data_flag);
        if(GGA_flag==1 && VTG_flag==1) {GPS_data_flag=1;} //Flag to note that GPS data is ready
        usleep(100000); //GPS only updates at 1 Hz, but need to process 2 messages
    }
    return NULL;
}
 
int is_new_GPS_data(){
    return GPS_data_flag;
}
 
GPS_data_t get_GPS_data(){
    GPS_data_flag=0;
    VTG_flag=0; GGA_flag=0;
    return GPS_data;
}
 
float get_NMEA_field(int field, char buf[], int comma[]){
    int i;
    char value_buf[10];
    memset(value_buf,0,sizeof(value_buf));
    for(i=comma[field-1];i<=comma[field]-2;i++){
        value_buf[i-comma[field-1]]=buf[i+1];
    }
    return strtod(value_buf,NULL);
}
 
void read_raw_gps(char *buf){
    int res;
    res = read(GPS_file, buf, 255);
    buf[res] = 0;
}