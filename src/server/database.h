#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

int initialize_db(sqlite3 **db);
int create_tables(sqlite3 *db);
// Diğer prototipler...
int add_player(sqlite3 *db, int id, const char *name, int age, const char *position, int rating);
int update_player(sqlite3 *db, int id, int newRating);
int query_player(sqlite3 *db, int id);
int delete_player(sqlite3 *db, int id);
int add_transfer(sqlite3 *db, int id, int player_id, int from_team, int to_team, int transfer_fee, const char *transfer_date);
int add_match(sqlite3 *db, int match_id, int team1_id, int team2_id, int score1, int score2, const char *match_date);
int set_tactics(sqlite3 *db, int id, int team_id, const char *formation, const char *style);
void query_all_players(sqlite3 *db);
int callback(void *data, int argc, char **argv, char **azColName);
int print_query_results(void *NotUsed, int argc, char **argv, char **azColName);
void query_and_print(sqlite3 *db, const char *sql_query);
int getRowCount(sqlite3 *db, const char *tableName);
void printsqltable(sqlite3 *db);



// Diğer veritabanı işlevlerinin prototipleri burada olacak...

#endif
