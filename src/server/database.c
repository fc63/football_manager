#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
int print_query_results(void *NotUsed, int argc, char **argv, char **azColName){
    NotUsed = 0;
    
    // Sütun başlıklarını yazdır
    for (int i = 0; i < argc; i++) {
        printf("%-20s", azColName[i]);
    }
    printf("\n");

    // Satır verilerini yazdır
    for (int i = 0; i < argc; i++) {
        printf("%-20s", argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    
    return 0;
}
void query_and_print(sqlite3 *db, const char *sql_query) {
    char *errMsg = 0;
    if (sqlite3_exec(db, sql_query, print_query_results, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}


int initialize_db(sqlite3 **db) {
    int rc = sqlite3_open("football_manager.db", db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));
        return rc;
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }
    return 0;
}

int create_tables(sqlite3 *db) {
    char *errMsg = 0;
    const char *sql = 
        "CREATE TABLE IF NOT EXISTS PLAYER(" \
        "ID INT PRIMARY KEY NOT NULL," \
        "NAME TEXT NOT NULL," \
        "AGE INT NOT NULL," \
        "POSITION TEXT NOT NULL," \
        "RATING INT NOT NULL);" \
        "CREATE TABLE IF NOT EXISTS TEAM(" \
        "ID INT PRIMARY KEY NOT NULL," \
        "NAME TEXT NOT NULL," \
        "FOUNDED YEAR INT NOT NULL," \
        "STADIUM TEXT NOT NULL);" \
		"CREATE TABLE IF NOT EXISTS MATCH(" \
		"ID INT PRIMARY KEY NOT NULL," \
		"TEAM1_ID INT NOT NULL," \
		"TEAM2_ID INT NOT NULL," \
		"SCORE1 INT," \
		"SCORE2 INT," \
		"MATCH_DATE DATE NOT NULL);" \
		"CREATE TABLE IF NOT EXISTS STATISTICS(" \
		"PLAYER_ID INT PRIMARY KEY NOT NULL," \
		"GOALS INT NOT NULL," \
		"ASSISTS INT NOT NULL," \
		"MATCHES_PLAYED INT NOT NULL);"
	    "CREATE TABLE IF NOT EXISTS TRANSFER(" \
		"ID INT PRIMARY KEY NOT NULL," \
		"PLAYER_ID INT NOT NULL," \
		"FROM_TEAM INT," \
		"TO_TEAM INT," \
		"TRANSFER_FEE INT NOT NULL," \
		"TRANSFER_DATE DATE NOT NULL);" \
		"CREATE TABLE IF NOT EXISTS TACTICS(" \
		"ID INT PRIMARY KEY NOT NULL," \
		"TEAM_ID INT NOT NULL," \
		"FORMATION TEXT NOT NULL," \
		"STYLE TEXT NOT NULL);";
    int rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        fprintf(stdout, "Tables created successfully\n");
    }
    return rc;
}

// Oyuncu ekleme fonksiyonu
int add_player(sqlite3 *db, int id, const char *name, int age, const char *position, int rating) {
    char sql[256];
    char *errMsg = 0;

    sprintf(sql, "INSERT INTO PLAYER (ID, NAME, AGE, POSITION, RATING) VALUES (%d, '%s', %d, '%s', %d);", id, name, age, position, rating);

    int rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    } else {
        fprintf(stdout, "Player added successfully\n");
    }
    return 0;
}

// Oyuncu güncelleme fonksiyonu
int update_player(sqlite3 *db, int id, int newRating) {
    char sql[256];
    char *errMsg = 0;

    sprintf(sql, "UPDATE PLAYER set RATING = %d where ID = %d;", newRating, id);

    int rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    } else {
        fprintf(stdout, "Player updated successfully\n");
    }
    return 0;
}

// Oyuncu sorgulama fonksiyonu
int query_player(sqlite3 *db, int id) {
    char sql[256];
    sprintf(sql, "SELECT * FROM PLAYER WHERE ID = %d;", id);

    char *errMsg = 0;
    int rc = sqlite3_exec(db, sql, callback, (void*)"Query Result", &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }
    return 0;
}

void query_all_players(sqlite3 *db) {
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, "SELECT * FROM PLAYER;", -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *name = sqlite3_column_text(stmt, 1);
            int age = sqlite3_column_int(stmt, 2);
            const unsigned char *position = sqlite3_column_text(stmt, 3);
            int rating = sqlite3_column_int(stmt, 4);

            printf("ID: %d, Name: %s, Age: %d, Position: %s, Rating: %d\n", id, name, age, position, rating);
        }
    } else {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
}


// Oyuncu silme fonksiyonu
int delete_player(sqlite3 *db, int id) {
    char sql[256];
    char *errMsg = 0;

    sprintf(sql, "DELETE FROM PLAYER WHERE ID = %d;", id);

    int rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    } else {
        fprintf(stdout, "Player deleted successfully\n");
    }
    return 0;
}


int add_match(sqlite3 *db, int match_id, int team1_id, int team2_id, int score1, int score2, const char *match_date) {
    char sql[512];
    char *errMsg = 0;

    sprintf(sql, "INSERT INTO MATCH (ID, TEAM1_ID, TEAM2_ID, SCORE1, SCORE2, MATCH_DATE) VALUES (%d, %d, %d, %d, %d, '%s');", match_id, team1_id, team2_id, score1, score2, match_date);

    int rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    } else {
        fprintf(stdout, "Match added successfully\n");
    }
    return 0;
}

int update_player_statistics(sqlite3 *db, int player_id, int goals, int assists, int matches_played) {
    char sql[512];
    char *errMsg = 0;

    sprintf(sql, "UPDATE STATISTICS SET GOALS = %d, ASSISTS = %d, MATCHES_PLAYED = %d WHERE PLAYER_ID = %d;", goals, assists, matches_played, player_id);

    int rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    } else {
        fprintf(stdout, "Player statistics updated successfully\n");
    }
    return 0;
}

int add_transfer(sqlite3 *db, int id, int player_id, int from_team, int to_team, int transfer_fee, const char *transfer_date) {
    char sql[512];
    char *errMsg = 0;

    sprintf(sql, "INSERT INTO TRANSFER (ID, PLAYER_ID, FROM_TEAM, TO_TEAM, TRANSFER_FEE, TRANSFER_DATE) VALUES (%d, %d, %d, %d, %d, '%s');", id, player_id, from_team, to_team, transfer_fee, transfer_date);

    int rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    } else {
        fprintf(stdout, "Transfer added successfully\n");
    }
    return 0;
}

int set_tactics(sqlite3 *db, int id, int team_id, const char *formation, const char *style) {
    char sql[512];
    char *errMsg = 0;

    sprintf(sql, "INSERT OR REPLACE INTO TACTICS (ID, TEAM_ID, FORMATION, STYLE) VALUES (%d, %d, '%s', '%s');", id, team_id, formation, style);

    int rc = sqlite3_exec(db, sql, NULL, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    } else {
        fprintf(stdout, "Tactics set successfully\n");
    }
    return 0;
}

