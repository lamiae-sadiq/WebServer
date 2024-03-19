
<?php
   session_start();
   session_regenerate_id(false);
   if(isset( $_SESSION['counter'])) {
      $_SESSION['counter'] += 1;
   }else {

      $_SESSION['counter'] = 1;
   }
   $my_Msg = "counter = ".  $_SESSION['counter'];
?>
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Centered Text</title>
  <link rel="stylesheet" href="styles.css">
</head>
<body>
   <div class="centered-text">
      <p><?php echo $my_Msg; ?><button onclick="redirectToLocation()">increment</button>
         <script>
            function redirectToLocation() {
               window.location.href = "CgiScript.php";
            }
</script></p>  

      
         </div>
</body>
</html>