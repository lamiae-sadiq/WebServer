#!/usr/bin/php-cgi
<?php
// Print Content-Type header
// header("Status: 200 OK");
header("Content-Type: text/html");
// header("Content-Length: 500");

// Check if username is provided in the query string
if (isset($_GET["username"])) {
    // Retrieve username from query string
    $username = $_GET["username"];
    
    // Display welcome message
    echo "<html><body>";
    echo "<h1>Welcome, $username!</h1>";
    echo "</body></html>";
} else {
    // Display form to input username
    echo "<html><body>";
    echo "<h1>Enter your username</h1>";
    echo "<form method='get'>";
    echo "Username: <input type='text' name='username'><br>";
    echo "<input type='submit' value='Submit'>";
    echo "</form>";
    echo "</body></html>";
}
?>
