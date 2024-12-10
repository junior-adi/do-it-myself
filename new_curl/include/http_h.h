#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>

/**
 * Represents an HTTP response.
 */
typedef struct {
    int status_code;      // HTTP status code (e.g., 200, 404)
    char *headers;        // Response headers
    char *body;           // Response body
} HttpResponse;

/**
 * Frees an HttpResponse structure.
 * @param response The HTTP response to free.
 */
void http_response_free(HttpResponse *response);

/**
 * Performs an HTTP GET request.
 * @param url The target URL.
 * @return An HttpResponse or NULL on failure.
 */
HttpResponse* http_get(const char *url);

/**
 * Performs an HTTP POST request.
 * @param url The target URL.
 * @param body The request body (can be NULL).
 * @return An HttpResponse or NULL on failure.
 */
HttpResponse* http_post(const char *url, const char *body);

/**
 * Performs an HTTP PUT request.
 * @param url The target URL.
 * @param body The request body (can be NULL).
 * @return An HttpResponse or NULL on failure.
 */
HttpResponse* http_put(const char *url, const char *body);

/**
 * Performs an HTTP DELETE request.
 * @param url The target URL.
 * @return An HttpResponse or NULL on failure.
 */
HttpResponse* http_delete(const char *url);

/**
 * Performs an HTTP UPDATE request.
 * @param url The target URL.
 * @param body The request body (can be NULL).
 * @return An HttpResponse or NULL on failure.
 */
HttpResponse* http_update(const char *url, const char *body);

/**
 * Performs an HTTP TRACE request.
 * @param url The target URL.
 * @return An HttpResponse or NULL on failure.
 */
HttpResponse* http_trace(const char *url);

/**
 * Performs an HTTP HEAD request.
 * @param url The target URL.
 * @return An HttpResponse or NULL on failure.
 */
HttpResponse* http_head(const char *url);

/**
 * Performs an HTTP OPTIONS request.
 * @param url The target URL.
 * @return An HttpResponse or NULL on failure.
 */
HttpResponse* http_options(const char *url);

/**
 * Performs an FTP request.
 * @param url The target URL.
 * @param command The FTP command to execute.
 * @return An HttpResponse or NULL on failure.
 */
HttpResponse* ftp_request(const char *url, const char *command);

/**
 * Performs a TELNET request.
 * @param url The target URL.
 * @param command The TELNET command to execute.
 * @return An HttpResponse or NULL on failure.
 */
HttpResponse* telnet_request(const char *url, const char *command);

/**
 * Performs an SSH request.
 * @param url The target URL.
 * @param command The SSH command to execute.
 * @return An HttpResponse or NULL on failure.
 */
HttpResponse* ssh_request(const char *url, const char *command);

#endif // HTTP_H
