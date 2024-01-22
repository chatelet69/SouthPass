<?php
use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;
use PHPMailer\PHPMailer\SMTP;

require 'PHPMailer/src/Exception.php';
require 'PHPMailer/src/PHPMailer.php';
require 'PHPMailer/src/SMTP.php';

include("./config.php");
$mailSource = $confMail;

function sendMail($mailSource, $mailPass, $email, $emailBody, $emailObject, $emailAltBody)
{
    $mail = new PHPMailer(true);

    try {
        //Server settings
        $mail->SMTPDebug = SMTP::DEBUG_SERVER;
        $mail->isSMTP(); 
        $mail->Host = 'smtp.gmail.com'; 
        $mail->SMTPAuth = true;
        $mail->Username = $mailSource;
        $mail->Password = $mailPass;
        $mail->Port = 465;
        $mail->SMTPSecure = "ssl";
        $mail->CharSet = 'UTF-8';

        //Recipients
        $mail->setFrom($mailSource);
        $mail->FromName = "SouthPass"; 
        $mail->addAddress($email); 

        $endBody = "<div style='text-align:center'><h3 style='color:#5bc77a;'>Support SouthPass</h3></div>";
        $emailBody = $emailBody. "<br>". $endBody;
        $mail->isHTML(true); 
        $mail->Subject = $emailObject;
        $mail->Body = $emailBody;
        $mail->AltBody = $emailAltBody;

        $mail->send();

        return true;
    } catch (Exception $e) {
        echo "Message could not be sent. Mailer Error: {$mail->ErrorInfo}";
        return false;
    }
}

if ($argc < 3) {
    echo "Need email and code: php sendmail.php <email> <code>\n";
    exit(1);
}

$emailToSend = $argv[1];
$emailCode = $argv[2];
$emailBody = "<div style='text-align:center;'><h1 style='font-weight:bold;font-size:30px;'>"
 ."Bonjour, vous avez reçu un code de vérification</h1><p>Code : ".$emailCode."<br>Bonne journée</div>";
sendMail($mailSource, $mailPass, $emailToSend, $emailBody, "SouthPass", "Alt");

?>