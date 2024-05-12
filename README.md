#Sync_server_boost
A sync server, synchronous server, consists of two threads, the first one accept and adds new connections, the second one handles existing clients. Each client has a name, the server can provide a list of connected clients, if the list has been changed, also disconnects a client if the time interval between its requests is more than 5 seconds.
