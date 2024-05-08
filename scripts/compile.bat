gcc -o build/no_threaded_server src/no_threaded_server.c -lws2_32
gcc -o build/threaded_server src/threaded_server.c -lws2_32