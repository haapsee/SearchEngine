#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the database connection. Returns 0 on success.
int db_manager_init(
    const char* db_host,
    const char* db_port,
    const char* db_name,
    const char* db_user,
    const char* db_password);

// Close the database connection.
void db_manager_close();

// Insert a crawled document into the database. Returns the document ID or -1 on error.
int db_manager_insert_site(const char *url, const char *content);

int db_manager_count_sites();

int db_manager_site_exists(const char *url);

#ifdef __cplusplus
}
#endif

#endif // DB_MANAGER_H
