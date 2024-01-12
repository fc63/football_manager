#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8018
int show_menu();

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char command[1024];

    // Soket oluşturma
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Sunucu adresine bağlanma
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Sunucudan gelen mesajı okuma
    //read(sock, buffer, 1024);
    //printf("%s\n", buffer);
    while (1) {
    	show_menu();
        printf("Komut girin (örneğin, 'add_match 1 2 3 2 1 2024-01-12'): ");
        fgets(command, 1024, stdin);

        send(sock, command, strlen(command), 0); // Komutu sunucuya gönder

        // Sunucudan yanıtı al ve yazdır
        char buffer[1024] = {0};
        read(sock, buffer, 1024);
        printf("Sunucudan gelen yanıt: %s\n", buffer);

        if (strncmp(command, "exit", 4) == 0) {
            break;
        }
    }
char username[50];
printf("Kullanıcı adınızı girin: ");
scanf("%s", username);
send(sock, username, strlen(username), 0); // Kullanıcı adını sunucuya gönder

printf("Transfer komutunu girin (örneğin, 'add_transfer 1 101 1 2 5000000 2024-01-01'): ");
fgets(command, 1024, stdin);
send(sock, command, strlen(command), 0);
printf("Taktik ayar komutunu girin (örneğin, 'set_tactics 1 1 4-4-2 Offensive'): ");
fgets(command, 1024, stdin);
send(sock, command, strlen(command), 0);


    // Soketi kapatma
    close(sock);

    return 0;
}

int show_menu() {
    int choice;
    printf("\n--- Futbol Menajerlik Oyunu Menüsü ---\n");
    printf("1. Transferler\n");
    printf("2. Taktikler\n");
    printf("3. Maçlar\n");
    printf("4. İstatistikler\n");
    printf("5. Çıkış\n");
    printf("Seçiminizi yapın: ");
    scanf("%d", &choice);
    return choice;
}
