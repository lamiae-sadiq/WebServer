<?php
    session_start();

    // Check if the session count is set
    if (!isset($_SESSION['count'])) {
        $_SESSION['count'] = 0;
    } else {
        $_SESSION['count']++;
    }

    // Increment session count and echo it
    echo "Session Count: " . $_SESSION['count'] . "<br>";

    // Set a cookie with the same value as session count
    $cookie_name = "count_cookie";
    $cookie_value = $_SESSION['count'];
    setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/"); // Cookie set to expire in 30 days (86400 seconds per day)

    // Echo the value of the cookie if it's set
    if(isset($_COOKIE[$cookie_name])) {
        echo "Cookie Value: " . $_COOKIE[$cookie_name];
    } else {
        echo "Cookie not set.";
    }
?>
