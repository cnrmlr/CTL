# CTL (Custom Template Library)

CTL is a custom template library designed to provide a set of utilities and data structures for C++ projects.

## Features

- **Hypergraph**: A graph data structure that implements directed edges of `1...n` specialized nodes.

## Getting Started

### Prerequisites

- C++20 or later
- CMake 3.13 or later
- Docker
- VSCode

### Building the Project

To build the project, follow these steps:

1. Clone the repository and open in VSCode:

   ```sh
   git clone https://github.com/cnrmlr/CTL.git
   cd CTL
   code .
   ```

2. Reopen in DevContainer when prompted

3. Init the google test submodule

   ```sh
   git submodule update --init --recursive
   ```

4. Run the build script

   ```sh
   ./build.sh Release
   ./build.sh Debug
   ```

### Running Examples

The `examples` directory contains example programs demonstrating how to use the library. To run an example:

1. Build the project as described above.
2. Run the `ctl-examples` executable

   ```sh
   ./build/examples/ctl-examples
   ```

### Running Tests

The project uses GoogleTest for unit testing. To run the tests:

1. Build the project as described above.
2. Run the `ctl-tests` executable

   ```sh
   ./build/test/ctl-tests
   ```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For questions or suggestions, please open an issue or contact the repository owner.
