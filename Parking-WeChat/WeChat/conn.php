
/* 连接数据库*/

<?php
           $link=mysql_connect(SAE_MYSQL_HOST_M.':'.SAE_MYSQL_PORT,SAE_MYSQL_USER,SAE_MYSQL_PASS);
           if($link)
           {
              
               $db=mysql_select_db("app_tryagain3");
             
           }
       
?>