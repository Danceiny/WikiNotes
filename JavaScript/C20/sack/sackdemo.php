<?php
ob_start();
	print_r($_POST);
$postdata = ob_get_clean();

ob_start();
	print_r($_GET);
$getdata = ob_get_clean();

ob_start();
	print_r($_SERVER);
$serverdata = ob_get_clean();

$string = "<p>���µ����ݱ�����:</p>\n<table id=\"demo\">\n<thead><tr><th>POST ����</th><th>GET ����</th></tr></thead>\n<tr><td>$postdata</td><td>$getdata</td></tr>\n</table>\n";
echo $string;
?>