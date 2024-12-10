/**
 * @file http.h
 * @brief Implementation of http requests functions header in C.
 *
 * This file contains the implementation of http requests functions header written with the use external libssl-dev
 *
 * @author Junior ADI
 * @date December 10th, 2024
 * @location Yamoussoukro, Côte d'Ivoire, West Africa
 *
 * @details
 * The URL parser is designed to be portable, robust, and modular. It includes
 * functions to handle the following:
 * - Initializing an empty URL structure.
 * - Freeing memory associated with a URL structure.
 * - Decoding an encoded URL string.
 * - Parsing a URL string into its components.
 * - Printing a parsed URL for debugging purposes.
 *
 * The implementation is self-contained within a single header file, making it
 * easy to integrate into any C project.
 *
 * @license
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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
