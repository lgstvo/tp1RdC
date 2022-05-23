#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

int socketIpV4(char const *port_str)
{
    int domain, address_v4_size, serverfd, new_fd, opt = 1;
    struct sockaddr *address;
    struct sockaddr_in address_v4;
    int port = atoi(port_str);

    address_v4.sin_family = AF_INET;
    address_v4.sin_port = htons(port);
    address_v4.sin_addr.s_addr = htonl(INADDR_ANY);

    domain = AF_INET;
    address_v4_size = sizeof(address_v4);
    address = (struct sockaddr *)&address_v4;

    if ((serverfd = socket(domain, SOCK_STREAM, 0)) == 0) {
        printf("create socket error.\n");
        exit(-1);
    }

    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        printf("socket option error.\n");
        exit(-1);
    }

    if (bind(serverfd, address, address_v4_size) < 0) {
        printf("bind error.\n");
        exit(-1);
    }

    if (listen(serverfd, 5) < 0) {
        printf("listen error.\n");
        exit(-1);
    }

    int address_size = sizeof(address);
    if ((new_fd = accept(serverfd, (struct sockaddr *)&address, (socklen_t *)&address_size)) < 0) {
        printf("accept connection error.\n");
        exit(-1);
    }
    
    return new_fd;
}

int socketIpV6(char const *port_str) {
    int domain, address_v4_size, serverfd, new_fd, opt = 1;
    struct sockaddr *address;
    struct sockaddr_in6 address_v6;
    int port = atoi(port_str);

    address_v6.sin6_family = AF_INET6;
    address_v6.sin6_port = htons(port);
    address_v6.sin6_addr = in6addr_any;

    domain = AF_INET6;
    address_v4_size = sizeof(address_v6);
    address = (struct sockaddr *)&address_v6;


    if ((serverfd = socket(domain, SOCK_STREAM, 0)) == 0) {
        printf("create socket error.\n");
        exit(-1);
    }

    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        printf("socket option error.\n");
        exit(-1);
    }

    if (bind(serverfd, address, address_v4_size) < 0) {
        printf("bind error.\n");
        exit(-1);
    }

    if (listen(serverfd, 5) < 0) {
        printf("listen error.\n");
        exit(-1);
    }

    int address_size = sizeof(address);
    if ((new_fd = accept(serverfd, (struct sockaddr *)&address, (socklen_t *)&address_size)) < 0) {
        printf("accept connection error.\n");
        exit(-1);
    }
    return new_fd;
}

void validade_comm(int data)
{
    if (data < 0) exit(-1);
}


void add_sensors(int factory[4][4], int sensors[4], int equipment, int *current_sensors, char *reply)
{
    char added_sensors[25] = "";
    char repeated_sensors[25] = "";
    
    for (int i = 0; i < 4; i++)
    {
        if (sensors[i] == 1)
        {
            if (*current_sensors >= 15)
            {
                char aux[200] = "";
                sprintf(aux, "limit exceeded");
                strcat(reply, aux);
                return;
            }
            if (factory[equipment-1][i] != 0)
            {
                char aux[200] = "";
                sprintf(aux, "0%d ", i+1);
                strcat(repeated_sensors, aux);
            }
            else
            {
                factory[equipment-1][i] = i+1;

                char aux[200] = "";
                sprintf(aux, "0%d ", i+1);
                strcat(added_sensors, aux);

                *current_sensors += 1;
            }
        }
    }
    if (strcmp(repeated_sensors, "") != 0)
    {
        if (strcmp(added_sensors, "") != 0)
        {
            char aux[200] = "";
            sprintf(aux, "sensor %sadded and sensor %salready exists in 0%d", added_sensors, repeated_sensors, equipment);
            strcat(reply, aux);
        }
        else
        {
            char aux[200] = "";
            sprintf(aux, "sensor %salready exists in 0%d", repeated_sensors, equipment);
            strcat(reply, aux);
        }
    }
    else
    {
        char aux[200] = "";
        sprintf(aux, "sensor %sadded", added_sensors);
        strcat(reply, aux);
    }
}

void remove_sensors(int factory[4][4], int sensors[4], int equipment, int *current_sensors, char *reply)
{
    char removed_sensors[25] = "";
    char repeated_sensors[25] = "";
    for (int i = 0; i < 4; i++)
    {
        if (sensors[i] == 1) 
        {
            if (factory[equipment-1][i] == 0)
            {
                char aux[200] = "";
                sprintf(aux, "0%d ", i+1);
                strcat(repeated_sensors, aux);
            }
            else
            {
                factory[equipment-1][i] = 0;
                char aux[200] = "";
                sprintf(aux, "0%d ", i+1);
                strcat(removed_sensors, aux);
                *current_sensors -= 1;    
            }
        }
    }
    
    if (strcmp(repeated_sensors, "") != 0)
    {
        if (strcmp(removed_sensors, "") != 0)
        {
            char aux[200] = "";
            sprintf(aux, "sensor %sremoved and sensor %sdoes not exist in 0%d", removed_sensors, repeated_sensors, equipment);
            strcat(reply, aux);
        }
        else
        {
            char aux[200] = "";
            sprintf(aux, "sensor %sdoes not exist in 0%d", repeated_sensors, equipment);
            strcat(reply, aux);
        }
    }
    else
    {
        char aux[200] = "";
        sprintf(aux, "sensor %sremoved", removed_sensors);
        strcat(reply, aux);
    }
}

void consult_equipment(int factory[4][4], int equipment, char *reply)
{
    int sensor_pos = 0;
    char sensor_str[100] = "";
    for (int i = 0; i < 4; i++){
        if (factory[equipment-1][i] != 0) 
        {
            char aux[200] = "";
            sprintf(aux, "0%d ", factory[equipment-1][i]);
            strcat(sensor_str, aux);
            sensor_pos++;
        }
    }
    if (sensor_pos == 0)
    {
        strcat(reply, "none");
    }
    else
    {
        strcat(reply, sensor_str);
    }
}

void consult_process(int factory[4][4], int sensors[4], int equipment, char *reply)
{
    char tmp_string[25] = "";
    char sensored_not_installed[25] = "";
    for (int i = 0; i < 4; i++)
    {
        if (sensors[i] == 1)
        {
            if (factory[equipment-1][i] != 0) 
            {
                char aux[200] = "";
                sprintf(aux, "%d.%d%d ", rand()%9, rand()%9, rand()%9);
                strcat(tmp_string, aux);
            }
            else
            {
                char aux[200] = "";
                sprintf(aux, "0%d ", i+1);
                strcat(sensored_not_installed, aux);
            }
        }
    }
    if (strcmp(sensored_not_installed, "") != 0)
    {
        if (strcmp(tmp_string, "") != 0)
        {
            char aux[200] = "";
            sprintf(aux, "%sand sensor(s) %snot installed", tmp_string, sensored_not_installed);
            strcat(reply, aux);
        }
        else
        {
            char aux[200] = "";
            sprintf(aux, "sensor(s) %snot installed", sensored_not_installed);
            strcat(reply, aux);
        }
    }
    else if (strcmp(tmp_string, "") != 0)
        strcat(reply, tmp_string);
}

void kill_server()
{
    exit(0);
}

void parse_add(char *command_string, int factory[4][4], int *current_sensors, char *reply)
{
    char *controller = "";
    int sensors[4] = {0, 0, 0, 0};
    controller = strtok(NULL, " ");
    while(strcmp(controller, "in") != 0){
        if (controller[0] == '\0') break;
        sensors[atoi(controller)-1] = 1;
        controller = strtok(NULL, " ");
    }
    int equipment = atoi(strtok(NULL, " "));

    add_sensors(factory, sensors, equipment, current_sensors, reply);
}

void parse_remove(char *command_string, int factory[4][4], int *current_sensors, char *reply)
{
    char *controller = "";
    int sensors[4] = {0, 0, 0, 0};
    controller = strtok(NULL, " ");
    while(strcmp(controller, "in") != 0){
        if (controller[0] == '\0') break;
        sensors[atoi(controller)-1] = 1;
        controller = strtok(NULL, " ");
    }
    int equipment = atoi(strtok(NULL, " "));

    remove_sensors(factory, sensors, equipment, current_sensors, reply);
}

void parse_list(char *command_string, int factory[4][4], char *reply)
{
    char *controller = "";
    controller = strtok(NULL, " ");
    while(strcmp(controller, "in") != 0){
        if (controller[0] == '\0') break;
        controller = strtok(NULL, " ");
    }
    int equipment = atoi(strtok(NULL, " "));

    consult_equipment(factory, equipment, reply);
}

void parse_read(int factory[4][4], char *reply)
{
    char *controller = "";
    int sensors[4] = {0, 0, 0, 0};
    controller = strtok(NULL, " ");
    while(strcmp(controller, "in") != 0){
        if (controller[0] == '\0') break;
        sensors[atoi(controller)-1] = 1;
        controller = strtok(NULL, " ");
    }
    int equipment = atoi(strtok(NULL, " "));

    consult_process(factory, sensors, equipment, reply);
}

void parse_commands(char *commanad_string, int factory[4][4], int *current_sensors, char *reply)
{
    char *option = strtok(commanad_string, " ");

    if (strcmp(option, "add") == 0){
        parse_add(strtok(NULL, " "), factory, current_sensors, reply);
    }
    else if (strcmp(option, "remove") == 0){
        parse_remove(strtok(NULL, " "), factory, current_sensors, reply);
    }
    else if (strcmp(option, "list") == 0){
        parse_list(strtok(NULL, " "), factory, reply);
    }
    else if (strcmp(option, "read") == 0){
        parse_read(factory, reply);
    }
    else if (strcmp(option, "kill") == 0){
        kill_server();
    }
}

int main(int argc, char const *argv[])
{
    srand(time(0));
    if (argc != 3) {
        printf("Expected 3 inputs.\n");
        exit(-1);
    }

    int sock = strcmp(argv[1], "v6") == 0 ? socketIpV6(argv[2]) : socketIpV4(argv[2]);

    char buffer[500];  
    char reply[500];

    int current_sensors = 0;
    int factory[4][4] =
    {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };
    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        memset(reply, 0, sizeof(*reply));
        int data = read(sock, buffer, sizeof(buffer));
        validade_comm(data);
        parse_commands(strdup(buffer), factory, &current_sensors, reply);
        printf("%s\n", buffer);
        data = write(sock, reply, strlen(reply));
        validade_comm(data);
    }
}