#include "http.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <URL>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *url = argv[1];

    HttpResponse *response = http_get(url);
    if (response) {
        printf("GET Response:\nStatus: %d\nHeaders:\n%s\nBody:\n%s\n",
               response->status_code,
               response->headers ? response->headers : "(null)",
               response->body ? response->body : "(null)");
        http_response_free(response);
    }

    const char *post_data = "key=value&param=123";
    response = http_post(url, post_data);
    if (response) {
        printf("POST Response:\nStatus: %d\nHeaders:\n%s\nBody:\n%s\n",
               response->status_code,
               response->headers ? response->headers : "(null)",
               response->body ? response->body : "(null)");
        http_response_free(response);
    }

    response = http_put(url, post_data);
    if (response) {
        printf("PUT Response:\nStatus: %d\nHeaders:\n%s\nBody:\n%s\n",
               response->status_code,
               response->headers ? response->headers : "(null)",
               response->body ? response->body : "(null)");
        http_response_free(response);
    }

    response = http_delete(url);
    if (response) {
        printf("DELETE Response:\nStatus: %d\nHeaders:\n%s\nBody:\n%s\n",
               response->status_code,
               response->headers ? response->headers : "(null)",
               response->body ? response->body : "(null)");
        http_response_free(response);
    }

    response = http_update(url, post_data);
    if (response) {
        printf("UPDATE Response:\nStatus: %d\nHeaders:\n%s\nBody:\n%s\n",
               response->status_code,
               response->headers ? response->headers : "(null)",
               response->body ? response->body : "(null)");
        http_response_free(response);
    }

    response = http_trace(url);
    if (response) {
        printf("TRACE Response:\nStatus: %d\nHeaders:\n%s\nBody:\n%s\n",
               response->status_code,
               response->headers ? response->headers : "(null)",
               response->body ? response->body : "(null)");
        http_response_free(response);
    }

    response = http_head(url);
    if (response) {
        printf("HEAD Response:\nStatus: %d\nHeaders:\n%s\nBody:\n%s\n",
               response->status_code,
               response->headers ? response->headers : "(null)",
               response->body ? response->body : "(null)");
        http_response_free(response);
    }

    response = http_options(url);
    if (response) {
        printf("OPTIONS Response:\nStatus: %d\nHeaders:\n%s\nBody:\n%s\n",
               response->status_code,
               response->headers ? response->headers : "(null)",
               response->body ? response->body : "(null)");
        http_response_free(response);
    }

    return EXIT_SUCCESS;
}
