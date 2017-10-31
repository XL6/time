
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lmdb.h"


#define BUF_SIZE 1024
#define CACHE_SIZE 1UL * 1024UL * 1024UL * 1024UL

#define E(expr) CHECK((rc = (expr)) == MDB_SUCCESS, #expr)
#define CHECK(test, msg) ((test) ? (void)0 : ((void)fprintf(stderr, \
    "%s:%d: %s: %s\n", __FILE__, __LINE__, msg, mdb_strerror(rc)), abort()))

/* Must create somedb folder first */
#define DBDIR "./somedb"


/* Put the key to db */
int putdb(char *, char *);

/* Put and get the key from db */
int getandputdb(char *, char *);

/* Get the key from db */
int getdb(char *);


int main(int argc, char * argv[])
{
//    putdb("key", "value");
    getdb("key");

    /* Must to be atomic */
//    getandputdb("KeyToGet", "ValueToBePutAfterGet");

    return 0;
}

/* Put to db */
int putdb(char *keyvalue, char *value) {

    int rc;
    MDB_env *env;
    MDB_dbi dbi;
    MDB_txn *txn;
    MDB_val key, data, rdata;
    MDB_cursor *cursor;
    char sval[32];

    //printf("put %s %s\n",keyvalue, value);

    E(mdb_env_create(&env));

    /* Set the cache size */
    E(mdb_env_set_mapsize(env, CACHE_SIZE));
    E(mdb_env_open(env, DBDIR, 0, 0664));

    /* Put some data */
    E(mdb_txn_begin(env, NULL, 0, &txn));
    E(mdb_dbi_open(txn, NULL, 0, &dbi));

    key.mv_data = keyvalue;
    key.mv_size = strlen(keyvalue);
    data.mv_data = value;
    data.mv_size = strlen(value);

    E(mdb_put(txn, dbi, &key, &data, 0));

    E(mdb_txn_commit(txn));

    mdb_dbi_close(env, dbi);
    mdb_env_close(env);

    return 0;
}

/* Get from lmdb */
int getdb(char *thekey) {

    int rc;
    MDB_env *env;
    MDB_dbi dbi;
    MDB_txn *txn;
    MDB_val key, data, rdata;
    MDB_cursor *cursor;

    //printf("get %s\n", thekey);

    E(mdb_env_create(&env));

    char* uu = mdb_version(0,0,0);
    printf("version %s\n", uu);

    /* Set the cache size */
    E(mdb_env_set_mapsize(env, CACHE_SIZE));
    E(mdb_env_open(env, DBDIR, 0, 0664));

    /* Get some data */
    E(mdb_txn_begin(env, NULL, 0, &txn));
    E(mdb_dbi_open(txn, NULL, 0, &dbi));

    key.mv_data = thekey;
    key.mv_size = strlen(thekey);

    rc = mdb_get(txn, dbi, &key, &data);
    if (!rc) {
        char *valuen = (char *) malloc(data.mv_size + 1);
        memcpy(valuen, data.mv_data, data.mv_size);
        valuen[data.mv_size] = 0;

        printf("%s %s\n", (char *) key.mv_data, valuen);

        free(valuen);
    } else {
        printf("No such key\n");
    }

//    printf("%s %d, %s %d\n", (char *) key.mv_data, key.mv_size, (char *)data.mv_data, data.mv_size);

    E(mdb_txn_commit(txn));


    mdb_dbi_close(env, dbi);
    mdb_env_close(env);

    return 0;
}

/* Put and get from db */
int getandputdb(char *keyvalue, char *value) {

    int rc;
    MDB_env *env;
    MDB_dbi dbi;
    MDB_txn *txn;
    MDB_val key, data, rdata;
    MDB_cursor *cursor;
    char sval[32];

    /*
     * Atomic operation, how to do this?
     * Get the key value and then put the value with some changes back in the atomic operation.
     */



    return 0;

}

