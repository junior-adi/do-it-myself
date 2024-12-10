/**
* This is the `http.c` written from scratch without libssl-dev lib
*/ 

/**
 * @file http_c.c
 * @brief Implementation of http requests functions in C.
 *
 * This file contains the implementation of http requests functions
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
#include "http.h"
#include "url_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

/* Function to connect to a host */
static int connect_to_host(const char *host, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };

    if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
        perror("Invalid host address");
        close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

/* Function to parse an HTTP response */
static HttpResponse* parse_http_response(const char *response) {
    if (!response) return NULL;

    HttpResponse *http_response = malloc(sizeof(HttpResponse));
    if (!http_response) return NULL;

    http_response->status_code = 0;
    http_response->headers = NULL;
    http_response->body = NULL;

    const char *status_line = strstr(response, "HTTP/");
    if (status_line) {
        sscanf(status_line, "HTTP/%*d.%*d %d", &http_response->status_code);
    }

    const char *headers_end = strstr(response, "\r\n\r\n");
    if (headers_end) {
        size_t headers_len = headers_end - response;
        http_response->headers = strndup(response, headers_len);
        http_response->body = strdup(headers_end + 4);
    }

    return http_response;
}

/* Function to perform an HTTP request */
static HttpResponse* http_request(const char *url, const char *method, const char *body) {
    char *cleaned_url = clean_url(url);
    if (!cleaned_url) return NULL;

    Url *parsed_url = url_parse(cleaned_url);
    free(cleaned_url);
    if (!parsed_url || !parsed_url->host || !parsed_url->scheme) {
        fprintf(stderr, "Invalid URL\n");
        url_free(parsed_url);
        return NULL;
    }

    int port = parsed_url->port > 0 ? parsed_url->port : 80;
    int sockfd = connect_to_host(parsed_url->host, port);
    if (sockfd < 0) {
        url_free(parsed_url);
        return NULL;
    }

    char request[4096];
    snprintf(request, sizeof(request),
             "%s %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n"
             "Content-Length: %zu\r\n\r\n%s",
             method,
             parsed_url->path ? parsed_url->path : "/",
             parsed_url->host,
             body ? strlen(body) : 0,
             body ? body : "");

    send(sockfd, request, strlen(request), 0);

    char *response = malloc(8192);
    if (!response) {
        perror("Memory allocation failed");
        close(sockfd);
        url_free(parsed_url);
        return NULL;
    }

    int bytes_received = recv(sockfd, response, 8192, 0);
    close(sockfd);
    url_free(parsed_url);

    HttpResponse *http_response = parse_http_response(response);
    free(response);
    return http_response;
}

HttpResponse* http_get(const char *url) {
    return http_request(url, "GET", NULL);
}

HttpResponse* http_post(const char *url, const char *body) {
    return http_request(url, "POST", body);
}

HttpResponse* http_put(const char *url, const char *body) {
    return http_request(url, "PUT", body);
}

HttpResponse* http_delete(const char *url) {
    return http_request(url, "DELETE", NULL);
}

HttpResponse* http_update(const char *url, const char *body) {
    return http_request(url, "UPDATE", body);
}

HttpResponse* http_trace(const char *url) {
    return http_request(url, "TRACE", NULL);
}

HttpResponse* http_head(const char *url) {
    return http_request(url, "HEAD", NULL);
}

HttpResponse* http_options(const char *url) {
    return http_request(url, "OPTIONS", NULL);
}

HttpResponse* ftp_request(const char *url, const char *command) {
    Url *parsed_url = url_parse(url);
    if (!parsed_url || !parsed_url->host || !parsed_url->scheme) {
        fprintf(stderr, "Invalid URL\n");
        url_free(parsed_url);
        return NULL;
    }

    int port = parsed_url->port > 0 ? parsed_url->port : 21;
    int sockfd = connect_to_host(parsed_url->host, port);
    if (sockfd < 0) {
        url_free(parsed_url);
        return NULL;
    }

    char request[1024];
    snprintf(request, sizeof(request), "%s\r\n", command);
    send(sockfd, request, strlen(request), 0);

    char *response = malloc(8192);
    if (!response) {
        perror("Memory allocation failed");
        close(sockfd);
        url_free(parsed_url);
        return NULL;
    }

    int bytes_received = recv(sockfd, response, 8192, 0);
    close(sockfd);
    url_free(parsed_url);

    HttpResponse *http_response = malloc(sizeof(HttpResponse));
    if (!http_response) {
        free(response);
        return NULL;
    }

    http_response->status_code = 0;
    http_response->headers = NULL;
    http_response->body = strndup(response, bytes_received);
    free(response);

    return http_response;
}

HttpResponse* telnet_request(const char *url, const char *command) {
    Url *parsed_url = url_parse(url);
    if (!parsed_url || !parsed_url->host || !parsed_url->scheme) {
        fprintf(stderr, "Invalid URL\n");
        url_free(parsed_url);
        return NULL;
    }

    int port = parsed_url->port > 0 ? parsed_url->port : 23;
    int sockfd = connect_to_host(parsed_url->host, port);
    if (sockfd < 0) {
        url_free(parsed_url);
        return NULL;
    }

    send(sockfd, command, strlen(command), 0);

    char *response = malloc(8192);
    if (!response) {
        perror("Memory allocation failed");
        close(sockfd);
        url_free(parsed_url);
        return NULL;
    }

    int bytes_received = recv(sockfd, response, 8192, 0);
    close(sockfd);
    url_free(parsed_url);

    HttpResponse *http_response = malloc(sizeof(HttpResponse));
    if (!http_response) {
        free(response);
        return NULL;
    }

    http_response->status_code = 0;
    http_response->headers = NULL;
    http_response->body = strndup(response, bytes_received);
    free(response);

    return http_response;
}

HttpResponse* ssh_request(const char *url, const char *command) {
    Url *parsed_url = url_parse(url);
    if (!parsed_url || !parsed_url->host || !parsed_url->scheme) {
        fprintf(stderr, "Invalid URL\n");
        url_free(parsed_url);
        return NULL;
    }

    int port = parsed_url->port > 0 ? parsed_url->port : 22;
    int sockfd = connect_to_host(parsed_url->host, port);
    if (sockfd < 0) {
        url_free(parsed_url);
        return NULL;
    }

    // Send the SSH command
    send(sockfd, command, strlen(command), 0);

    char *response = malloc(8192);
    if (!response) {
        perror("Memory allocation failed");
        close(sockfd);
        url_free(parsed_url);
        return NULL;
    }

    int bytes_received = recv(sockfd, response, 8192, 0);
    close(sockfd);
    url_free(parsed_url);

    HttpResponse *http_response = malloc(sizeof(HttpResponse));
    if (!http_response) {
        free(response);
        return NULL;
    }

    http_response->status_code = 0;
    http_response->headers = NULL;
    http_response->body = strndup(response, bytes_received);
    free(response);

    return http_response;
}

void http_response_free(HttpResponse *response) {
    if (!response) return;
    free(response->headers);
    free(response->body);
    free(response);
}
