<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>User Info Card</title>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            background-color: #f4f4f4;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }

        .card {
            background-color: #fff;
            border-radius: 8px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            padding: 20px;
            width: 300px;
            text-align: center;
        }

        .card img {
            width: 100%;
            height: auto; /* Maintain aspect ratio of the image */
            border-radius: 50%;
            margin-bottom: 15px;
        }

        .card h2 {
            color: #333;
        }

        .card p {
            color: #666;
        }
    </style>
</head>
<body>

<?php
// Handle form submission
header("Content-Type: text/html");
header("Status: 200 ok");

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // POST user input
    $name = isset($_POST['name']) ? htmlspecialchars($_POST['name']) : '';
    $email = isset($_POST['email']) ? htmlspecialchars($_POST['email']) : '';
    $phone = isset($_POST['phone']) ? htmlspecialchars($_POST['phone']) : '';

    // Display user info in a card
    echo '<div class="card">';
    echo '<img src="placeholder.jpg" alt="Profile Picture">';
    echo '<h2>' . $name . '</h2>';
    echo '<p>Email: ' . $email . '</p>';
    echo '<p>Phone: ' . $phone . '</p>';
    echo '</div>';
}
?>

<!-- HTML form for user input -->
<form method="POST" action="">
    <label for="name">Name:</label>
    <input type="text" id="name" name="name" required>

    <label for="email">Email:</label>
    <input type="email" id="email" name="email" required>

    <label for="phone">Phone:</label>
    <input type="tel" id="phone" name="phone" required>

    <button type="submit">Generate Card</button>
</form>

</body>
</html>