<?php
$endpointURL = 'http://127.0.0.1:9090/a/testing/scriptsCgi/up.php'; 
$data = array('key' => 'value'); 

function sendPostRequest($url, $data) {
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_POST, false);
    curl_setopt($ch, 
function sendMultipleRequests($url, $data, $count) {
    for ($i = 0; $i < $count; $i++) {
        sendPostRequest($url, $data);
    }

    echo "done";
}

sendMultipleRequests($endpointURL, $data, 70000);
?>