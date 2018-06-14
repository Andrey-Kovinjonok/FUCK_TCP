The only right way to read packets from a TCP socket.

```
char tmp[];
Buffer buffer;
// Network Loop：data comes continuously
while(1){
    // read data from socket, may be less than you expected
    tcp.read(tmp);
    // append newly received data to previously received
    buffer.append(tmp);
    // Protocol Loop：parse buffer in a loop, there may be more than 1 packet in buffer
    while(1){
        // try to parse
        msg = parse(buffer);
        if(!msg){
            // packet not ready, keep reading from network
            break;
        }
        // remove parsed data from buffer
        buffer.remove(msg.length);
        // business processing
        process(msg);
    }
}
```

You can improve the code by:

* read data into buffer directly, not tmp
* increment parsing
