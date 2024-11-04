#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PACKETS 100000

// Функция для преобразования IP адреса в целое число
unsigned int ip_to_int(const char *ip) 
{
    unsigned int result = 0;
    unsigned int octet;
    char *token;
    char *ip_copy = strdup(ip); // Создаем копию строки 
    token = strtok(ip_copy, ".");
    
    while (token != NULL) 
    {
        octet = atoi(token);
        result = (result << 8) | (octet & 0xFF); // Сдвигаем на 8 бит и добавляем текущий октет
        token = strtok(NULL, ".");
    }
    
    free(ip_copy);
    return result;
}

// Функция для проверки принадлежности IP адреса к подсети
int is_in_subnet(unsigned int ip, unsigned int gateway, unsigned int netmask) 
{
    return (ip & netmask) == (gateway & netmask);
}

// Функция для генерации случайного IP адреса
void generate_random_ip(char *ip)
{
    sprintf(ip, "%d.%d.%d.%d", rand() % 256, rand() % 256, rand() % 256, rand() % 256);
    printf("ip - %s \n", ip);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Use: %s <gateway_ip> <subnet_mask> <number_of_packets>\n", argv[0]);
        return 1;
    }

    char *gateway_ip = argv[1];
    char *subnet_mask = argv[2];
    int num_packets = atoi(argv[3]);

    if (num_packets <= 0 || num_packets > MAX_PACKETS) {// условие по колву 
        printf("The number of packets should be in the range from 1 to %d.\n", MAX_PACKETS);
        return 1;
    }

    srand(time(NULL)); // Инициализация генератора случайных чисел

    unsigned int gateway_int = ip_to_int(gateway_ip);
    unsigned int mask_int = ip_to_int(subnet_mask);

    int own_subnet_count = 0;
    int other_subnet_count = 0;

    for (int i = 0; i < num_packets; i++) 
    {
        char dest_ip[16];
        generate_random_ip(dest_ip);
        unsigned int dest_ip_int = ip_to_int(dest_ip);

        if (is_in_subnet(dest_ip_int, gateway_int, mask_int)) 
        {
            own_subnet_count++;
        } 
        else 
        {
            other_subnet_count++;
        }
    }

    int total_packets = own_subnet_count + other_subnet_count;
    float own_subnet_percentage = (float)own_subnet_count / total_packets * 100;
    float other_subnet_percentage = (float)other_subnet_count / total_packets * 100;

    printf("Packets in their subnet: %d (%.2f%%)\n", own_subnet_count, own_subnet_percentage);
    printf("Packages on other networks: %d (%.2f%%)\n", other_subnet_count, other_subnet_percentage);

    return 0;
}
