#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
//#include <winsock2.h>
#include "database.h"
#include "sqlite3.h"
#include <openssl/sha.h> 

#define PORT 8017

int main() {
    int server_fd, new_socket,a;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    sqlite3 *db;
    initialize_db(&db);
    create_tables(db);
    char command[50];
    char buffer[1024] = {0};


    // Soket oluşturma
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Adresi sokete bağlama
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Bağlantıları dinleme
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // İstemci bağlantısını kabul etme
    do{
        // Yeni istemci bağlantısını kabul etme
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue;
        }
//sql içindekileri gösterme
	 const char *sqltables = "SELECT name FROM sqlite_master WHERE type='table'";
	 const char *sql2 = "SELECT * FROM TRANSFER;";
	 const char *sql3 = "SELECT * FROM STATISTICS;";
	 const char *sql4 = "SELECT * FROM TACTICS;";
    // SQL sorgusunu çalıştır
	printf("SQL Tabloları: \n");
	for(int i=0;i<)
    query_and_print(db, sqltables);
	query_and_print(db, sql2);
	query_and_print(db, sql3);
	query_and_print(db, sql4);
    char *message = "Merhaba Futbol Menajeri!\n";
    send(new_socket, message, strlen(message), 0);
    printf("Hoş geldin mesajı gönderildi\n");

        read(new_socket, buffer, 1024); // İstemciden komut okuma

    // Komutları işleme (örnek: "add_match 1 2 3 2 1 2024-01-12")
    sscanf(buffer, "%s", command);
    if (strcmp(command, "add_match") == 0) {
        int match_id, team1_id, team2_id, score1, score2;
        char match_date[50];
        sscanf(buffer, "%*s %d %d %d %d %d %s", &match_id, &team1_id, &team2_id, &score1, &score2, match_date);
        add_match(db, match_id, team1_id, team2_id, score1, score2, match_date);
    }
	if (strcmp(command, "add_transfer") == 0) {
		int id, player_id, from_team, to_team, transfer_fee;
		char transfer_date[50];
		sscanf(buffer, "%*s %d %d %d %d %d %s", &id, &player_id, &from_team, &to_team, &transfer_fee, transfer_date);
		add_transfer(db, id, player_id, from_team, to_team, transfer_fee, transfer_date);
	}
	if (strcmp(command, "set_tactics") == 0) {
		int id, team_id;
		char formation[50], style[50];
		sscanf(buffer, "%*s %d %d %s %s", &id, &team_id, formation, style);
		set_tactics(db, id, team_id, formation, style);
	}
        // Diğer komutlar ve işlevler...

        close(new_socket); // İstemci bağlantısını kapat
    }while(a!=33);



    // Soketleri kapatma
    close(server_fd);
    sqlite3_close(db);

    return 0;
}