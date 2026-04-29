#include "../include/db_manager.h"
#include "../include/logger.h"
#include <stdio.h>
#include <string.h>
#include <libpq-fe.h>
#include <unistd.h>
#include <stdlib.h>

PGconn *conn = NULL;

int run_create_table(char *query) {
    PGresult *result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
        log_error("Failed to execute query!\n%s\n%s", query, PQerrorMessage(conn));
        PQclear(result);
        return 1;
    } else {
        log_info("Table created successfully!");
        PQclear(result);
        return 0;
    }
}

int create_tables() {
    char *query_sites =
        "CREATE TABLE IF NOT EXISTS sites ("
        "id SERIAL PRIMARY KEY, "
        "url TEXT UNIQUE NOT NULL, "
        "content TEXT, "
        "search_idx tsvector GENERATED ALWAYS AS ("
        "setweight(to_tsvector('english', url), 'A') || "
        "setweight(to_tsvector('english', coalesce(content, '')), 'B')"
        ") STORED"
        ");"
        "CREATE INDEX idx_sites_search ON sites USING GIN(search_idx);";
   log_debug("Creating table sites");
   if (run_create_table(query_sites)!=0) {
       log_error("Failed to create table sites");
       return 1;
   }
   return 0;
}

int db_manager_init(
    const char* db_host,
    const char* db_port,
    const char* db_name,
    const char* db_user,
    const char* db_password
) {
    char *connection_string = "host=%s port=%s dbname=%s user=%s password=%s";
    int len = strlen(connection_string);
    len += strlen(db_host) + strlen(db_port) + strlen(db_name) + strlen(db_user) + strlen(db_password);
    char connection_info[len];
    sprintf(
        connection_info,
        "host=%s port=%s dbname=%s user=%s password=%s",
        db_host, db_port, db_name, db_user, db_password
    );

    conn = PQconnectdb(connection_info);
    int err = 0;
    for (int i = 0; i < 30; i++) {
        switch (PQstatus(conn)) {
            case CONNECTION_OK:
                log_info("Connection created success fully at %s:%s", db_host, db_port);
                err = create_tables();
                i = 30;
                break;
            case CONNECTION_BAD:
                log_error("Failed to connect to the database");
                return 1;
            default:
                log_debug("Forming connection to the  database...");
                sleep(1);
        }
    }
    if (err == 1) {
        log_error("Failed to create tables");
        return 1;
    }
    PGresult *result = PQprepare(
        conn,
        "insert_site",
        "INSERT INTO sites (url, content) VALUES ($1, $2)",
        2,
        NULL
    );
    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Prepare failed: %s", PQerrorMessage(conn));
    }
    PQclear(result);
    return 1;
}

void db_manager_close() {
    if (conn != NULL) {
        log_info("Closing database connection");
        PQfinish(conn);
    }
}

int db_manager_insert_site(const char *url, const char *content) {
    const char* values[2];
    values[0] = url;
    values[1] = content;

    PGresult* result = PQexecPrepared(conn, "insert_site", 2, values, NULL, NULL, 0);

    int err = 0;
    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
        log_error("Update failed: %s", PQerrorMessage(conn));
        err = 1;
    }
    PQclear(result);
    return err;
}

int db_manager_count_sites() {
    PGresult *result = PQexec(conn, "select count(url) c from sites");
    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        int res = atoi(PQgetvalue(result, 0, 0));
        PQclear(result);
        return res;
    } else {
        PQclear(result);
        return -1;
    }
}
