<?php
$host = "127.0.0.1";
$port = 5000;

// Create socket
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if ($socket === false) {
    die("socket_create() failed: " . socket_strerror(socket_last_error()) . "\n");
}

// Connect to server (load balancer)
if (socket_connect($socket, $host, $port) === false) {
    die("socket_connect() failed: " . socket_strerror(socket_last_error($socket)) . "\n");
}

echo "Connected to $host:$port\n";

// Send message
$msg = "hello from client";
socket_write($socket, $msg, strlen($msg));
echo "Sent: $msg\n";

// Keep reading until LB closes the connection
while (true) {
    echo "Waiting for response...\n";
    $response = socket_read($socket, 1024, PHP_BINARY_READ);
    if ($response === false || $response === "") {
        echo "Connection closed by server.\n";
        break;
    }

    echo "Received: " . trim($response) . "\n";
}

// Close socket
socket_close($socket);
echo "Socket closed.\n";
