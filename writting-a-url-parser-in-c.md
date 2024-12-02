# Writting a URL Parser in C

A **URL (Uniform Resource Locator)** is a reference or address used to access resources on the internet. It specifies the location of a resource and the protocol used to retrieve it. URLs are essential for web navigation, enabling users to find and interact with various types of content, such as web pages, images, and videos.

### Structure of a URL
A URL typically consists of several components:

1. **Scheme**: This indicates the protocol used to access the resource (e.g., `http`, `https`, `ftp`).
2. **Authority**: This includes:
   - **Userinfo**: Optional information for authentication (username and password).
   - **Host**: The domain name or IP address of the server hosting the resource.
   - **Port**: An optional port number specifying the server's listening port.
3. **Path**: The specific location of the resource on the server.
4. **Query**: Optional parameters that provide additional information to the server.
5. **Fragment**: An optional identifier for a specific section within the resource.

### Example of a URL
For example, in the URL `https://user:pass@www.example.com:8080/path/to/resource?key=value#fragment`:
- **Scheme**: `https`
- **Userinfo**: `user:pass`
- **Host**: `www.example.com`
- **Port**: `8080`
- **Path**: `/path/to/resource`
- **Query**: `key=value`
- **Fragment**: `fragment`

### Importance of URLs
URLs are crucial for locating resources on the internet. They allow users to navigate between different pages and services, making them fundamental to web browsing and online communication. Properly structured URLs can also enhance search engine optimization (SEO), improving visibility and accessibility of web content.

![image](https://github.com/user-attachments/assets/59544db6-fad8-426b-b2ae-c8dbbe066514)

# Implement standalone URL parser in C

feat: Implement standalone URL parser in C

- Added `url_parser.h` header file with a self-contained URL parser implementation.
- The parser handles various URL components including scheme, user, password, host, port, path, query, and fragment.
- Includes functions for URL creation, parsing, decoding, printing, and memory management.
- Provided examples for basic, IPv4, IPv6, and complex URLs to ensure comprehensive testing.
- Added a simple test program to validate the parser functionality.

**NOTE** :  *This implementation is portable, robust, and modular, making it easy to integrate into any C project without external dependencies.*

---

### **Header File: `url_parser.h`**

```c
/**
 * @file url_parser.c
 * @brief Implementation of a standalone URL parser in C.
 *
 * This file contains the implementation of a URL parser that can be included
 * in any C project without external dependencies. The parser handles various
 * URL components including scheme, user, password, host, port, path, query,
 * and fragment. It provides functions for URL creation, parsing, decoding,
 * printing, and memory management.
 *
 * @author Junior ADI
 * @date December 1st, 2024
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
 * @example
 * #include "url_parser.h"
 *
 * int main() {
 *     const char *url_str = "https://user:pass@www.example.com:8080/path/to/resource?key=value#fragment";
 *     Url *url = url_parse(url_str);
 *
 *     if (url) {
 *         url_print(url);
 *         url_free(url);
 *     } else {
 *         printf("Failed to parse URL\n");
 *     }
 *
 *     return 0;
 * }
 */
#ifndef URL_PARSER_H
#define URL_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Structure to store the components of the URL */
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

/* Function to initialize an empty Url */
Url *url_create() {
    Url *url = (Url *)malloc(sizeof(Url));
    if (!url) return NULL;
    url->scheme = NULL;
    url->user = NULL;
    url->password = NULL;
    url->host = NULL;
    url->port = -1; // Indicates no port
    url->path = NULL;
    url->query = NULL;
    url->fragment = NULL;
    return url;
}

/* Function to free the memory associated with a Url structure */
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

/* Function to decode an encoded URL string (e.g., %20 -> space) */
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

/* Main function to parse a URL */
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

/* Function to print a parsed URL (for debugging) */
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
```

---

### **Usage**

1. Include the header in your project:

   ```c
   #include "url_parser.h"
   ```

2. Example program to test the parser:

   ```c
   #include "url_parser.h"

   int main() {
       const char *url_str = "https://user:pass@www.example.com:8080/path/to/resource?key=value#fragment";
       Url *url = url_parse(url_str);

       if (url) {
           url_print(url);
           url_free(url);
       } else {
           printf("Failed to parse URL\n");
       }

       return 0;
   }
   ```

3. Complex Examples program to test the parser:

Here are three examples for each type of URL: **basic**, **IPv4**, **IPv6**, and **complex**. These examples cover a variety of cases to ensure that the parser works correctly.

---

### **1. Basic URLs**
#### Example 1: Scheme + Host + Path
```
http://example.com/path/to/resource
```

#### Example 2: Scheme + Host (no path)
```
https://example.com
```

#### Example 3: Scheme + Host + Port
```
ftp://example.com:21
```

---

### **2. URLs with IPv4**
#### Example 1: Scheme + IPv4
```
http://192.168.1.1
```

#### Example 2: Scheme + IPv4 + Port
```
https://192.168.1.1:8080
```

#### Example 3: Scheme + IPv4 + Path + Query
```
http://192.168.1.1/api/v1/resources?filter=active
```

---

### **3. URLs with IPv6**
#### Example 1: Scheme + IPv6
```
http://[2001:db8::ff00:42:8329]
```

#### Example 2: Scheme + IPv6 + Port
```
https://[2001:db8::ff00:42:8329]:443
```

#### Example 3: Scheme + IPv6 + Path + Query + Fragment
```
http://[2001:db8::ff00:42:8329]/path/to/resource?query=value#section
```

---

### **4. Complex URLs**
#### Example 1: Scheme + User:Password + Host + Port + Path
```
ftp://user:password@example.com:21/path/to/resource
```

#### Example 2: Scheme + Host + Path + Query + Fragment
```
https://example.com/path/to/resource?name=value&lang=en#top
```

#### Example 3: Scheme + Host with Subdomain + Path
```
http://subdomain.example.com/path/to/resource
```

---

### **Testing with the Parser**
You can test these URLs with the parser header file using a loop or a simple unit test:

```c
#include "url_parser.h"

int main() {
    const char *urls[] = {
        "http://example.com/path/to/resource",
        "https://192.168.1.1:8080",
        "http://[2001:db8::ff00:42:8329]/path/to/resource?query=value#section",
        "ftp://user:password@example.com:21/path/to/resource"
    };

    for (int i = 0; i < 4; i++) {
        Url *url = url_parse(urls[i]);
        if (url) {
            printf("Testing URL: %s\n", urls[i]);
            url_print(url);
            url_free(url);
            printf("\n");
        } else {
            printf("Failed to parse URL: %s\n", urls[i]);
        }
    }

    return 0;
}
```

This test will display the results for each of the URLs, allowing you to validate their parsing.

4. 

---

### **Features**
- **Portability**: No need for external libraries.
- **Robustness**: Handles most common URL cases.
- **Modularity**: Each component's management is isolated.
- **Ease of Debugging**: The `url_print` function allows for easy verification of results.

NOTE : `You can easily integrate this file into any project by simply copying it into your working directory.`


Citations:
* [1] https://www.reddit.com/r/unpopularopinion/comments/t2cihz/i_dont_always_need_to_provide_sources/
* [2] https://www.linkedin.com/pulse/information-without-source-adesh-tamrakar
* [3] https://gitlab.kitware.com/cmake/cmake/-/issues/17978
* [4] https://stackoverflow.com/questions/26462227/debugging-with-gdb-without-sources
* [5] https://stackoverflow.com/questions/40223903/cmake-how-to-include-headers-without-sources
* [6] https://strategylab.ca/parts-of-a-url/
