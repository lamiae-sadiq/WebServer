#!/usr/bin/php
<?php

declare(ticks = 1); // Enable signal handling

// Function to handle timeout
function timeout_handler($signum) {
    global $filename;
    if (file_exists($filename)) {
        unlink($filename);
    }
    exit(0);
}

pcntl_signal(SIGALRM, "timeout_handler"); // Register signal handler for timeout

header("Content-Type: text/html");
header("Status: 201 Created");

// Set timeout (in seconds)
pcntl_alarm(6);

$upload_dir = './cgi-bin'; // Directory to store uploaded files
if (!file_exists($upload_dir)) {
    mkdir($upload_dir, 0777, true);
}

// Generate a random filename
$random = substr(md5(uniqid(rand(), true)), 0, 8);
$filename = $upload_dir . '/' . $random . '_uploaded_file' . getenv('EXTENSION_');

$chunk_size = 2048; // Chunk size for reading from input

// Open file for writing
$file = fopen($filename, 'wb');
if (!$file) {
    echo "Failed to open file for writing.";
    exit(1);
}

// Read from input and write to file
$input_stream = fopen('../vd.mp4', 'rb');
while (!feof($input_stream)) {
    $data_chunk = fread($input_stream, $chunk_size);
    fwrite($file, $data_chunk);
}

fclose($file);
fclose($input_stream);

// Cancel timeout
pcntl_alarm(0);

echo "File uploaded successfully.\n";
?>
