
<!--将车主所绑定的信息提交到服务器，同时记录绑定状态，保存到云数据库-->

<?php
include_once("conn.php");
header("Content-type: text/html; charset=utf8"); 

    $na=$_POST["username"];
    $na=mb_convert_encoding($na, "GBK","utf-8"); 
    $ph=$_POST["phone"];
    $no=$_POST["number"];
    $no=mb_convert_encoding($no, "GBK","utf-8"); 
    $wid=$_POST['who'];
// $fromUsername=$_POST['who'];

 $fp = fsockopen("tcp://119.146.68.41", 5000, $errno, $errstr);
              
                if (!$fp)
                {
                    $out= "ERROR: $errno - $errstr\n";
                } else
                {
                  
             
                    fwrite($fp,"WeChat"."insert"."\n".$na."\n".$ph."\n".$no."\n".$wid);
                    $out= fread($fp, 1024);
                    $kk= fread($fp, 1024);
                    fclose($fp);
                      
                }   
               
              
                $out=mb_convert_encoding($out,"utf-8", "GBK"); 
if($out=="绑定成功")
         {
    $sql="update  wechat set id=1 where openid='{$wid}'";
             mysql_query($sql);
         }
                $kk=mb_convert_encoding($kk,"utf-8", "GBK"); 

echo "<center><br><br><br><h2><font color='green'>".$out.$kk."<br>点击左上角关闭界面。</font></h2></center>";
                
?>
<meta name="viewport" content="width=device-width,height=device-height,inital-scale=1.0,maximum-scale=1.0,user-scalable=no;">
