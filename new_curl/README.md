### `README.md`

```markdown
# new_curl

`new_curl` is a simple implementation of an HTTP client in C, inspired by `cURL`. This project allows you to perform basic HTTP requests (GET, POST, PUT, DELETE, etc.) and handle HTTP responses. It also includes basic support for FTP, TELNET, and SSH requests.

## Project Structure

```raw
new_curl/
|____include
|         |____ url_parser.h
|         |____ http.h
|____ src
          |____ http.c
          |____ main.c
|____ Makefile
|____ README.md
|____ LICENSE
```

## Prerequisites

- A C compiler (e.g., `gcc`).

## Installation

1. **Clone the repository** :
   ```sh
   git clone https://github.com/your-username/new_curl.git
   cd new_curl
   ```

2. **Install dependencies** :
   - On Debian/Ubuntu:
     ```sh
     sudo apt-get install libssl-dev
     ```

## Compilation

To compile the project, use the following command:

```sh
make
```

This will generate an executable named `my_curl`.

## Usage

To use `my_curl`, run the following command with a URL:

```sh
./my_curl <URL>
```

For example:

```sh
./my_curl http://example.com
```

This will display the HTTP responses for various methods (GET, POST, PUT, DELETE, etc.).

## Cleanup

To clean up the generated files (object files and executable), use the following command:

```sh
make clean
```

## Files

- **`include/url_parser.h`** : Declarations for the URL parser.
- **`include/http.h`** : Declarations for the HTTP functions.
- **`src/http.c`** : Implementation of the HTTP functions.
- **`src/main.c`** : Entry point of the program.
- **`Makefile`** : Makefile to compile the project.
- **`README.md`** : This file.
- **`LICENSE`** : License file (GNU GPL v3).

## Contribution

Contributions are welcome! To contribute, follow these steps:

1. Fork the repository.
2. Create a branch for your feature (`git checkout -b feature/my-feature`).
3. Commit your changes (`git commit -am 'Add new feature'`).
4. Push the branch (`git push origin feature/my-feature`).
5. Open a Pull Request.

## License

This project is licensed under the GNU GPL v3. See the [LICENSE](LICENSE) file for details.

## Contact

For any questions or suggestions, please open an issue.
```

### Explications

1. **Introduction**: A brief introduction to the `new_curl` project, explaining that it is a simple implementation of an HTTP client in C, inspired by `cURL`.
2. **Project Structure**: A description of the file and directory structure of the project.
3. **Prerequisites**: The prerequisites necessary to compile and run the project, including a C compiler.
4. **Installation**: The steps to clone the repository and install the necessary dependencies.
5. **Compilation**: The commands to compile the project and generate the executable.
6. **Usage**: The commands to use the generated executable and perform HTTP requests.
7. **Cleanup**: The command to clean up generated files.
8. **Files**: A description of each file in the project.
9. **Contribution**: The steps to contribute to the project.
10. **License**: Information about the project's license (GNU GPL v3).
11. **Contact**: Contact information for questions or suggestions.
