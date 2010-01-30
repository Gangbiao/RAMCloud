/* Copyright (c) 2009 Stanford University
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef RAMCLOUD_SHARED_RCRPC_H
#define RAMCLOUD_SHARED_RCRPC_H

// #include <cinttypes> // this requires c++0x support because it's c99
// so we'll go ahead and use the C header
#include <inttypes.h>
#include <stdbool.h>

#define RCRPC_HEADER_LEN                        sizeof(struct rcrpc_header)
#define RCRPC_PING_REQUEST_LEN                  sizeof(struct rcrpc_ping_request)
#define RCRPC_PING_RESPONSE_LEN                 sizeof(struct rcrpc_ping_response)
#define RCRPC_READ_REQUEST_LEN                  sizeof(struct rcrpc_read_request)
#define RCRPC_READ_RESPONSE_LEN_WODATA          sizeof(struct rcrpc_read_response)
#define RCRPC_WRITE_REQUEST_LEN_WODATA          sizeof(struct rcrpc_write_request)
#define RCRPC_WRITE_RESPONSE_LEN                sizeof(struct rcrpc_write_response)
#define RCRPC_INSERT_REQUEST_LEN_WODATA         sizeof(struct rcrpc_insert_request)
#define RCRPC_INSERT_RESPONSE_LEN               sizeof(struct rcrpc_insert_response)
#define RCRPC_DELETE_REQUEST_LEN                sizeof(struct rcrpc_delete_request)
#define RCRPC_DELETE_RESPONSE_LEN               sizeof(struct rcrpc_delete_response)
#define RCRPC_CREATE_TABLE_REQUEST_LEN          sizeof(struct rcrpc_create_table_request)
#define RCRPC_CREATE_TABLE_RESPONSE_LEN         sizeof(struct rcrpc_create_table_response)
#define RCRPC_OPEN_TABLE_REQUEST_LEN            sizeof(struct rcrpc_open_table_request)
#define RCRPC_OPEN_TABLE_RESPONSE_LEN           sizeof(struct rcrpc_open_table_response)
#define RCRPC_DROP_TABLE_REQUEST_LEN            sizeof(struct rcrpc_drop_table_request)
#define RCRPC_DROP_TABLE_RESPONSE_LEN           sizeof(struct rcrpc_drop_table_response)
#define RCRPC_ERROR_RESPONSE_LEN_WODATA         sizeof(struct rcrpc_error_response)

//namespace RAMCloud {

/**
 * The type of an RPC message.
 *
 * rcrpc_header.type should be set to one of these.
 */
enum RCRPC_TYPE {
    RCRPC_PING_REQUEST,
    RCRPC_PING_RESPONSE,
    RCRPC_READ_REQUEST,
    RCRPC_READ_RESPONSE,
    RCRPC_WRITE_REQUEST,
    RCRPC_WRITE_RESPONSE,
    RCRPC_INSERT_REQUEST,
    RCRPC_INSERT_RESPONSE,
    RCRPC_DELETE_REQUEST,
    RCRPC_DELETE_RESPONSE,
    RCRPC_CREATE_TABLE_REQUEST,
    RCRPC_CREATE_TABLE_RESPONSE,
    RCRPC_OPEN_TABLE_REQUEST,
    RCRPC_OPEN_TABLE_RESPONSE,
    RCRPC_DROP_TABLE_REQUEST,
    RCRPC_DROP_TABLE_RESPONSE,
    RCRPC_ERROR_RESPONSE,
};

struct rcrpc_header {
    uint32_t type;
    uint32_t len;
};

struct rcrpc_any {
    struct rcrpc_header header;
    char opaque[0];
};

#ifdef DOXYGEN
/** \cond FALSE */
/**
 * An internal hook used to work around technical limitations in the
 * documentation system.
 */
#define DOC_HOOK(rcrpc_lower) \
    static void \
    rcrpc_lower##_RPC_doc_hook(struct rcrpc_##rcrpc_lower##_request *in, \
                               struct rcrpc_##rcrpc_lower##_response *out)
/** \endcond */
#else
#define DOC_HOOK(rcrpc_lower)
#endif

/**
 * Verify network connectivity.
 *
 * \limit This function declaration is only a hook for documentation. The
 * function does not exist and should not be called.
 */
DOC_HOOK(ping);

struct rcrpc_ping_request {
    struct rcrpc_header header;
};

struct rcrpc_ping_response {
    struct rcrpc_header header;
};

#ifdef __cplusplus
/**
 * A reserved version number representing no particular version.
 */
#define RCRPC_VERSION_ANY (static_cast<uint64_t>(-1))
#else
#define RCRPC_VERSION_ANY ((uint64_t)(-1ULL))
#endif

/**
 * Read an object from a %RAMCloud.
 *
 * \li Let \c o be the object identified by \c in.table, \c in.key.
 * \li If \c o exists:
 *      <ul>
 *      <li> \c out.version is set to \c o's version.
 *      <li> If <tt>in.version == #RCRPC_VERSION_ANY || in.version == o's
 *      version</tt>:
 *          <ul>
 *          <li> \c out.buf of size \c out.buf_len is set to \c o's opaque
 *          blob.
 *          </ul>
 *      <li> Else:
 *          <ul>
 *          <li> \c out.buf_len is \c 0 and \c out.buf is empty.
 *          </ul>
 *      </ul>
 * \li Else:
 *      <ul>
 *      <li> \c out.version is set to #RCRPC_VERSION_ANY
 *      <li> \c out.buf_len is \c 0 and \c out.buf is empty.
 *      </ul>
 *
 * \warning The caller can not distinguish the cases based on \c out.buf_len.
 * Use \c in.version and \c out.version.
 *
 * \limit This function declaration is only a hook for documentation. The
 * function does not exist and should not be called.
 */
DOC_HOOK(read);

struct rcrpc_read_request {
    struct rcrpc_header header;
    uint64_t table;
    uint64_t key;
    uint64_t version;
};

struct rcrpc_read_response {
    struct rcrpc_header header;
    uint64_t version;
    uint64_t buf_len;
    char buf[0];                        /* Variable length (see buf_len) */
};

/**
 * Update or create an object at a given key.
 *
 * \li Let \c o be the object identified by \c in.table, \c in.key.
 * \li If \c o exists:
 *      <ul>
 *      <li> If <tt>in.version == #RCRPC_VERSION_ANY ||
 *                  in.version == o's version</tt>:
 *          <ul>
 *          <li> \c o's opaque blob is set to \c in.buf of size \c in.buf_len.
 *          <li> \c o's version is increased.
 *          <li> \c o is sent to the backups and their ack is received.
 *          <li> \c out.version is set to \c o's new version.
 *          </ul>
 *      <li> Else:
 *          <ul>
 *          <li> \c out.version is set to \c o's existing version.
 *          </ul>
 *      </ul>
 * \li Else:
 *      <ul>
 *      <li> \c o is created.
 *      <li> \c o's opaque blob is set to \c in.buf of size \c in.buf_len.
 *      <li> \c o's version is set to a value guaranteed to be greater than
 *          that of any previous object that resided at \c in.table, \c in.key.
 *      <li> \c o is sent to the backups and their ack is received.
 *      <li> \c out.version is set to \c o's new version.
 *      </ul>
 *
 * \TODO Should \c o be created if \c o did not exist and \c in.version is not
 *      #RCRPC_VERSION_ANY?
 *
 * \limit This function declaration is only a hook for documentation. The
 * function does not exist and should not be called.
 */
DOC_HOOK(write);

struct rcrpc_write_request {
    struct rcrpc_header header;
    uint64_t table;
    uint64_t key;
    uint64_t version;
    uint64_t buf_len;
    char buf[0];                        /* Variable length (see buf_len) */
};

struct rcrpc_write_response {
    struct rcrpc_header header;
    uint64_t version;
};

/**
 * Create an object at an assigned key.
 *
 * \li Let \c o be a new object inside \c in.table.
 * \li \c o is assigned a key based on the table's key allocation strategy.
 * \li \c o's version is set to a value guaranteed to be greater than that of
 *      any previous object that resided at \c in.table with \c o's key.
 * \li \c o's opaque blob is set to \c in.buf of size \c in.buf_len.
 * \li \c o is sent to the backups and their ack is received.
 * \li \c out.key is set to \c o's key.
 * \li \c out.version is set to \c o's version.
 *
 * \TODO Define the table's key allocation strategy.
 *
 * \limit This function declaration is only a hook for documentation. The
 * function does not exist and should not be called.
 */
DOC_HOOK(insert);

struct rcrpc_insert_request {
    struct rcrpc_header header;
    uint64_t table;
    uint64_t buf_len;
    char buf[0];                        /* Variable length (see buf_len) */
};

struct rcrpc_insert_response {
    struct rcrpc_header header;
    uint64_t key;
    uint64_t version;
};

/**
 * Delete an object.
 *
 * \li Let \c o be the object identified by \c in.table, \c in.key.
 * \li If \c o exists:
 *      <ul>
 *      <li> \c out.version is set to \c o's existing version.
 *      <li> If <tt>in.version == #RCRPC_VERSION_ANY || in.version == o's
 *      version</tt>:
 *           <ul>
 *           <li> \c o is removed from the table.
 *           <li> \c o's deletion is sent to the backups and their ack is
 *           received.
 *           </ul>
 *      </ul>
 * \li Else:
 *      <ul>
 *      <li> \c out.version is set to #RCRPC_VERSION_ANY
 *      </ul>
 *
 * \limit This function declaration is only a hook for documentation. The
 * function does not exist and should not be called.
 */
DOC_HOOK(delete);

struct rcrpc_delete_request {
    struct rcrpc_header header;
    uint64_t table;
    uint64_t key;
    uint64_t version;
};

struct rcrpc_delete_response {
    struct rcrpc_header header;
    uint64_t version;
};

/**
 * Create a table.
 *
 * \li Let \c t be a new table identified by \c in.name.
 * \li If \c t exists: an rcrpc_error_response is sent instead.
 * \li If there system is out of space for tables: an rcrpc_error_response is
 *      sent instead.
 * \li A table identified by \c in.name is created.
 *
 * \TODO Don't use rcrpc_error_response for application errors.
 *
 * \limit This function declaration is only a hook for documentation. The
 * function does not exist and should not be called.
 */
DOC_HOOK(create_table);

struct rcrpc_create_table_request {
    struct rcrpc_header header;
    char name[64];
};

struct rcrpc_create_table_response {
    struct rcrpc_header header;
};

/**
 * Open a table.
 *
 * \li Let \c t be the table identified by name.
 * \li If \c t does not exist: an rcrpc_error_response is sent instead.
 * \li \c out.handle is set to a handle to \c t.
 *
 * \TODO Don't use rcrpc_error_response for application errors.
 *
 * \limit This function declaration is only a hook for documentation. The
 * function does not exist and should not be called.
 */
DOC_HOOK(open_table);

struct rcrpc_open_table_request {
    struct rcrpc_header header;
    char name[64];
};

struct rcrpc_open_table_response {
    struct rcrpc_header header;
    uint64_t handle;
};

/**
 * Delete a table.
 *
 * \li Let \c t be the table identified by \c name.
 * \li If \c t does not exist: an rcrpc_error_response is sent instead.
 * \li \c t is deleted.
 *
 * \TODO Don't use rcrpc_error_response for application errors.
 *
 * \limit This function declaration is only a hook for documentation. The
 * function does not exist and should not be called.
 */
DOC_HOOK(drop_table);

struct rcrpc_drop_table_request {
    struct rcrpc_header header;
    char name[64];
};

struct rcrpc_drop_table_response {
    struct rcrpc_header header;
};

struct rcrpc_error_response {
    struct rcrpc_header header;
    char message[0];                    /* Variable length */
};

//} // namespace RAMCloud

#undef DOC_HOOK

#endif

