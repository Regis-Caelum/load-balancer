<?php
$host = "127.0.0.1";
$port = 5001;

// Create TCP/IP socket
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if ($socket === false) {
    die("socket_create() failed: " . socket_strerror(socket_last_error()) . "\n");
}

// Bind socket
if (socket_bind($socket, $host, $port) === false) {
    die("socket_bind() failed: " . socket_strerror(socket_last_error($socket)) . "\n");
}

// Listen for connections
if (socket_listen($socket, 5) === false) {
    die("socket_listen() failed: " . socket_strerror(socket_last_error($socket)) . "\n");
}

echo "Server listening on $host:$port\n";

while (true) {
    $client = socket_accept($socket);
    if ($client === false) {
        echo "socket_accept() failed: " . socket_strerror(socket_last_error($socket)) . "\n";
        continue;
    }

    echo "New connection accepted.\n";

    // Step 1: Read message from LB
    $data = '';
    while (true) {
        $chunk = socket_read($client, 1024, PHP_BINARY_READ);
        if ($chunk === false) {
            echo "Read failed: " . socket_strerror(socket_last_error($client)) . "\n";
            break;
        }
        if ($chunk === 'EOF') {
            // EOF reached (LB closed)
            echo "Connection closed by LB\n";
            break;
        }
        $data .= $chunk;
        echo "Received chunk: '$chunk'\n";
    }
    echo "Out of reading loop.\n";

    if ($data !== '') {
        echo "Received from LB: $data\n";
    }

    // Step 2: Send response
    $message = "hello from 1";
    socket_write($client, $message, strlen($message));
    echo "Sent to LB: $message\n";

    // Step 3: Wait for LB to close connection
    while ($chunk = socket_read($client, 1024)) {
        echo "LB sent (before closing): $chunk\n";
    }

    echo "LB closed connection.\n";
    socket_close($client);
}
