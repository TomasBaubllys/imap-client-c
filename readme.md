# IMAP Client

This is a terminal-based IMAP client developed as part of the Vilnius University Computer Science course "Networking."

## Features
- Connects to an IMAP server via the command line.
- Sends and receives IMAP commands.
- Parses server responses.

## Usage
```sh
./imap <server>
```
Replace `<server>` with the address of the IMAP server you want to connect to.

## Compilation
To build the program, run:
```sh
make
```

To clean the build files, use:
```sh
make clean
```

To remove object files only, run:
```sh
make clean_objs
```

## Requirements
- A C compiler (e.g., `gcc`)
- A working internet connection to communicate with the IMAP server

## Notes
- Ensure you have the correct server address before running the client.
- This program is for educational purposes and may not include all IMAP features.

## License
This project was developed for educational use and does not include a specific license.

