# CompressionService
I found this test task in stored files on my PC.. and decided to do it :D
see _res/subject.pdf for more info and requirements to this project.

## **Target spec.**
**OS:** Windows 10
**Language:** C++17 under g++ 8.1.0 (x86_64-posix-seh-rev0, Built by MinGW-W64 project)

## **Dependencies.**
Boost library 1.73

## **Project description**
So, actually the project is only Server(server/) and shared lib for it(protocol/) with msg description(header, payload, etc.).
Client(client/) - just for tests.
Purpose of this project is to compress data(ASCI text) received from the cliend over TCP socket, and send data back.

Compress algo:
- a -> a
- aa -> aa
- aaa -> 3a
- abc -> abc
- ab2 -> error, contains digits.
- aBc -> error, contains uppercase.
