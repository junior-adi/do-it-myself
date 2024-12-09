#ifndef URL_PARSER_H
#define URL_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Structure pour stocker les composants de l'URL */
typedef struct {
    char *scheme;
    char *user;
    char *password;
    char *host;
    int port;
    char *path;
    char *query;
    char *fragment;
} Url;

/* Fonction pour initialiser un Url vide */
Url *url_create() {
    Url *url = (Url *)malloc(sizeof(Url));
    if (!url) return NULL;
    url->scheme = NULL;
    url->user = NULL;
    url->password = NULL;
    url->host = NULL;
    url->port = -1; // Indique qu'il n'y a pas de port
    url->path = NULL;
    url->query = NULL;
    url->fragment = NULL;
    return url;
}

/* Fonction pour libérer la mémoire associée à une structure Url */
void url_free(Url *url) {
    if (!url) return;
    free(url->scheme);
    free(url->user);
    free(url->password);
    free(url->host);
    free(url->path);
    free(url->query);
    free(url->fragment);
    free(url);
}

/* Fonction pour décoder une chaîne d'URL encodée (ex : %20 -> espace) */
char *url_decode(const char *encoded) {
    if (!encoded) return NULL;

    size_t len = strlen(encoded);
    char *decoded = (char *)malloc(len + 1);
    if (!decoded) return NULL;

    char *p = decoded;
    for (size_t i = 0; i < len; ++i) {
        if (encoded[i] == '%' && i + 2 < len && isxdigit(encoded[i + 1]) && isxdigit(encoded[i + 2])) {
            char hex[3] = {encoded[i + 1], encoded[i + 2], '\0'};
            *p++ = (char)strtol(hex, NULL, 16);
            i += 2;
        } else {
            *p++ = encoded[i];
        }
    }
    *p = '\0';
    return decoded;
}

/* Fonction principale pour parser une URL */
Url *url_parse(const char *url_string) {
    if (!url_string) return NULL;

    Url *url = url_create();
    if (!url) return NULL;

    const char *p = url_string;
    const char *scheme_end = strstr(p, "://");
    if (scheme_end) {
        size_t scheme_len = scheme_end - p;
        url->scheme = strndup(p, scheme_len);
        p = scheme_end + 3;
    }

    const char *auth_end = strchr(p, '@');
    if (auth_end) {
        const char *auth_sep = strchr(p, ':');
        if (auth_sep && auth_sep < auth_end) {
            url->user = strndup(p, auth_sep - p);
            url->password = strndup(auth_sep + 1, auth_end - auth_sep - 1);
        } else {
            url->user = strndup(p, auth_end - p);
        }
        p = auth_end + 1;
    }

    const char *path_start = strchr(p, '/');
    const char *port_start = strchr(p, ':');
    if (path_start && (!port_start || path_start < port_start)) {
        url->host = strndup(p, path_start - p);
    } else if (port_start) {
        url->host = strndup(p, port_start - p);
        url->port = atoi(port_start + 1);
    } else {
        url->host = strdup(p);
    }

    if (path_start) {
        const char *query_start = strchr(path_start, '?');
        const char *fragment_start = strchr(path_start, '#');

        if (query_start && (!fragment_start || query_start < fragment_start)) {
            url->path = strndup(path_start, query_start - path_start);
            url->query = strndup(query_start + 1, fragment_start ? fragment_start - query_start - 1 : strlen(query_start + 1));
        } else if (fragment_start) {
            url->path = strndup(path_start, fragment_start - path_start);
            url->fragment = strdup(fragment_start + 1);
        } else {
            url->path = strdup(path_start);
        }
    }

    return url;
}

// Fonction pour nettoyer et valider l'URL
char* clean_url(const char *url) {
    if (!url) return NULL;

    // Utiliser url_parse pour décomposer l'URL
    Url *parsed_url = url_parse(url);
    if (!parsed_url) return NULL;

    // Allouer de la mémoire pour la nouvelle URL nettoyée
    size_t len = 0;
    len += parsed_url->scheme ? strlen(parsed_url->scheme) + 3 : 7; // "http://"
    len += parsed_url->host ? strlen(parsed_url->host) : 0;
    len += parsed_url->port > 0 ? 6 : 0; // ":" + port (max 5 chars)
    len += parsed_url->path ? strlen(parsed_url->path) : 1; // "/"
    len += parsed_url->query ? strlen(parsed_url->query) + 1 : 0; // "?"
    len += parsed_url->fragment ? strlen(parsed_url->fragment) + 1 : 0; // "#"
    len += 1; // null terminator

    char *cleaned_url = malloc(len);
    if (!cleaned_url) {
        url_free(parsed_url);
        return NULL;
    }

    // Reconstruire l'URL nettoyée
    char *p = cleaned_url;
    if (parsed_url->scheme) {
        p += sprintf(p, "%s://", parsed_url->scheme);
    } else {
        p += sprintf(p, "http://");
    }
    if (parsed_url->host) {
        p += sprintf(p, "%s", parsed_url->host);
    }
    if (parsed_url->port > 0) {
        p += sprintf(p, ":%d", parsed_url->port);
    }
    if (parsed_url->path) {
        p += sprintf(p, "%s", parsed_url->path);
    } else {
        p += sprintf(p, "/");
    }
    if (parsed_url->query) {
        p += sprintf(p, "?%s", parsed_url->query);
    }
    if (parsed_url->fragment) {
        p += sprintf(p, "#%s", parsed_url->fragment);
    }
    *p = '\0';

    url_free(parsed_url);
    return cleaned_url;
}

/* Fonction pour afficher une URL décomposée (pour débogage) */
void url_print(const Url *url) {
    if (!url) return;
    printf("Scheme: %s\n", url->scheme ? url->scheme : "(null)");
    printf("User: %s\n", url->user ? url->user : "(null)");
    printf("Password: %s\n", url->password ? url->password : "(null)");
    printf("Host: %s\n", url->host ? url->host : "(null)");
    printf("Port: %d\n", url->port);
    printf("Path: %s\n", url->path ? url->path : "(null)");
    printf("Query: %s\n", url->query ? url->query : "(null)");
    printf("Fragment: %s\n", url->fragment ? url->fragment : "(null)");
}

#endif // URL_PARSER_H
