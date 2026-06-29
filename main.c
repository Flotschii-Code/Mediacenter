#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

void write_json_file(sqlite3 *db);      //updates the services.json with the new configuration of enabled services

int main(int argc, char *argv[]){
    sqlite3 *db;
    int reco, rc;
    char *err_msg = NULL;

    reco = sqlite3_open("media.db", &db);
        if(reco != SQLITE_OK){
            fprintf(stderr, "Fehler: kann DB nicht öffnen: %s\n", sqlite3_errmsg(db));      //opening the sqlite file
            return 1;
        }

    const char *sql_create =
        "CREATE TABLE IF NOT EXISTS services("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"         //creating table if it doesn't allready exist
        "name TEXT NOT NULL UNIQUE,"
        "url TEXT NOT NULL,"
        "logo_path TEXT,"
        "is_active INTEGER DEFAULT 0);";

        rc = sqlite3_exec(db, sql_create, 0, 0, &err_msg);

    if(argc == 2 && strcmp(argv[1], "--services") == 0){        //list enabled services
        printf("Active Services:\n");
        
        sqlite3_stmt *stmt;

        const char *sql_search = "SELECT name FROM services WHERE is_active = 1;";      //selecting enabled services

        if(sqlite3_prepare_v2(db, sql_search, -1, &stmt, 0) != SQLITE_OK){
            fprintf(stderr, "error while ennabling: %s\n", sqlite3_errmsg(db));
            return 1;
        }

        int counter = 0;

        while(sqlite3_step(stmt) == SQLITE_ROW){
            counter++;

            const unsigned char *name = sqlite3_column_text(stmt, 0);

            printf("%s\n", name);
        }

        if(counter == 0){
            printf("Use ./mediacenter --enable [Name] to enable services!\n");
        }   

        sqlite3_finalize(stmt);

    } else if(argc == 2 && strcmp(argv[1], "--update") == 0){

        write_json_file(db);

    } else if(argc == 2 && strcmp(argv[1], "--start") == 0){
        
        write_json_file(db);

        printf("starting the server on http://localhost:8080 ...\n");
        printf("to end the server press ctrl + c\n\n");
    
        system("python3 -m http.server 8080 -d \"$PWD\" &");

    } else if (argc == 2 && strcmp(argv[1], "--compile") == 0){

        system("gcc main.c -o mediacenter -lsqlite3");
        system("sudo gcc main.c -o /usr/local/bin/mediacenter -lsqlite3");

    } else if(argc == 2 && strcmp(argv[1], "--setup") == 0){

        printf("starting automatic setup...\n");
        printf("please enter your password if asked!\n\n");

        system("sudo apt update && sudo apt install -y build-essential sqlite3 libsqlite3-dev python3 python3-full");   //installs everything needed to use the programm

        printf("setup is completed!");

    }else if(argc == 3 && strcmp(argv[1], "--enable") == 0){       //enable service from databank
        sqlite3_stmt *stmt;

        const char *sql_enable = "UPDATE services SET is_active = 1 WHERE name = ?;";

        if(sqlite3_prepare_v2(db, sql_enable, -1, &stmt, 0) != SQLITE_OK){
            fprintf(stderr, "error while enabling: %s\n", sqlite3_errmsg(db));
            return 1;
        }

        sqlite3_bind_text(stmt, 1, argv[2], -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if(rc != SQLITE_DONE){
            fprintf(stderr, "an error occurred while searching: %s\n", sqlite3_errmsg(db));
        } else {
            printf("enabled service: %s\n", argv[2]);
        }

        sqlite3_finalize(stmt);
        write_json_file(db);

    } else if(argc == 5 && strcmp(argv[1], "--add") == 0){      //add service
        sqlite3_stmt *stmt;

        const char *sql_insert = "INSERT INTO services (name ,url ,logo_path) VALUES (?, ?, ?);";

        if(sqlite3_prepare_v2(db, sql_insert, -1, &stmt, 0) != SQLITE_OK){
            fprintf(stderr, "error while insterting: %s\n", sqlite3_errmsg(db));
            return 1;
        }

        char formatted_url[512];

        if(strncmp(argv[3], "https://", 7) || strncmp(argv[3], "https://", 8)){
            snprintf(formatted_url, sizeof(formatted_url), "%s", argv[4]);
        } else {
            snprintf(formatted_url, sizeof(formatted_url), "https://%s", argv[4]);
        }

        sqlite3_bind_text(stmt, 1, argv[2], -1, SQLITE_STATIC); //Name
        sqlite3_bind_text(stmt, 2, argv[3], -1, SQLITE_STATIC); //URL
        sqlite3_bind_text(stmt, 3, argv[4], -1, SQLITE_STATIC); //Logo-Path

        rc = sqlite3_step(stmt);
        if(rc == SQLITE_DONE){
            printf("%s was saved to the database\n", argv[2]);
        } else {
            fprintf(stderr, "an error occurred while saving: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);

    } else{
        printf("--Command not found--\n\n");
        printf("mediacenter --add [Name] [URL] [Logo]   //create and use a new service of your own\n");
        printf("mediacenter --enable [Name] //use a service from the databank\n");
        printf("mediacenter --services  //view your used services\n");
        printf("mediacenter --update    //update the services.json\n");
        printf("mediacenter --start //start the localhost mediacenter server with port 8080 and update the services.json\n");

        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}

void write_json_file(sqlite3 *db){
    FILE *json_file = fopen("services.json", "w");

    int first = 1;
    int rc;

    if(json_file == NULL){
        fprintf(stderr, "Error opening or creating services.json!\n");
        return;
    }

    sqlite3_stmt *stmt;

    const char *sql_search_for_json = "SELECT name, url, logo_path FROM services WHERE is_active = 1;";

    if(sqlite3_prepare_v2(db, sql_search_for_json, -1, &stmt, 0) != SQLITE_OK){
        fclose(json_file);
        return;
    }

    fprintf(json_file, "[\n");

    while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if(!first){
            fprintf(json_file, ",\n");
        }
        first = 0;

        const unsigned char *name = sqlite3_column_text(stmt, 0);
        const unsigned char *url = sqlite3_column_text(stmt, 1);
        const unsigned char *logo_path = sqlite3_column_text(stmt, 2);

        const char *logo_str = (logo_path != NULL) ? (const char *)logo_path: "";

        fprintf(json_file, "  {\n");
        fprintf(json_file, "    \"name\": \"%s\",\n", name);
        fprintf(json_file, "    \"url\": \"%s\",\n", url);
        fprintf(json_file, "    \"logo\": \"%s\"\n", logo_str);
        fprintf(json_file, "  }");
    }

    fprintf(json_file, "\n]\n");

    sqlite3_finalize(stmt);
    fclose(json_file);

    printf("services.json has been updated!");
}