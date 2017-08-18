 <!--管理员模式板块，实现停车场空闲车位查看，灯光控制-->

 <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
 <meta name="viewport" content="width=device-width, initial-scale=1">


<?php

 $fp = fsockopen("tcp://119.146.68.41", 5000, $errno, $errstr);
              
                if (!$fp)
                {
                    $out= "ERROR: $errno - $errstr\n";
                } else
                {
                   fwrite($fp,"WeChat"."admin"."\n"."xxx");

                       $out= fread($fp,8192 );
                        $kk= fread($fp, 8192);
                     $dd= fread($fp, 8192);
                    fclose($fp);
                    // echo "发送成功！";
                    $out=mb_convert_encoding($out,"utf-8", "GBK"); 
                     $kk=mb_convert_encoding($kk,"utf-8", "GBK"); 
                      $dd=mb_convert_encoding($dd,"utf-8", "GBK"); 
                    //  echo "收到的信息为:".$out.$kk.$dd;

                }

                
//$out.$kk="LIGHT00006LX,Status:1";

$data=$out.$kk.$dd;
//$data="A001101010B010101110110101LIGHT00006LX,Status:1";
$pos= strpos($data,'L');
$new=substr($data,$pos,21);
$liangdu= substr($new,5,5);
$t=substr($new,20,1);
if($t=='1')
$two='开启';
else
$two='关闭';

               
?>
<head>        
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1">
     <script>
         function aa(osel)
         {
             if(osel.options[osel.selectedIndex].value=='1')
              document.getElementById("co").innerHTML="<button id='u'>开灯</button> <button id='u'>关灯</button>";
            
            
             if(osel.options[osel.selectedIndex].value=='0')
             { document.getElementById("co").innerHTML="<button id='u'><a href='light.php?x=LIGHTON'><span id='s'>开灯</span></a></button> <button  id='u'><a href='light.php?x=LIGHTOFF'><span id='s'>关灯</span></a></button>";
              }
              
                 
         }
  </script>
   
<style>
    #u{
        color:#C0C0C0;
        height:30px;
        width:60px;
        
        
    }
  
     #s{
        color:blue;   
        
    }
    #ui
    {color:white;
    }
  
    body{
background-color:#87CEFA;
}
    </style>
    
</head>
<body>
<div id='ui'>
<center>
<center><h2>欢迎使用管理员模式！</h2></center>
    <hr />
<h3>灯光控制模式选择:<h3>
    <form method="POST" >
    <select name="xuan" onchange="aa(this)">
    <option  selected="selected" value="1">自动模式</option>
    <option  value="0">手动模式</option>
    </select>
    </form>
    
    <div id='co'><button id='u'>开灯</button> <button id='u'>关灯</button></div>
  
   <br>
  

<span>照明灯状态：</span><input style="width:70px" type="text" value="<?php echo $two;?>"><br>
    <span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;亮度值：</span><input style="width:70px" type="text" value="<?php echo $liangdu.' LX';?>">
    <hr />
    <h3>空闲车位状况</h3>
    
   &nbsp;&nbsp;&nbsp;总空闲车位数: <input type="text"  style="width:60px" value="<?php include('conn.php');$all='select * from car_num ';
$query=mysql_query($all);
$row=mysql_fetch_array($query);
echo $row['total'];
?>"><br>
    A区空闲车位数: <input type="text" style="width:60px" value="<?php  include('conn.php'); $all='select * from car_num ';
$query=mysql_query($all);
$row=mysql_fetch_array($query);
echo $row['a_num']; ?>"><br>
    B区空闲车位数: <input type="text" style="width:60px" value="<?php  include('conn.php'); $all='select * from car_num ';
$query=mysql_query($all);
$row=mysql_fetch_array($query);
echo $row['b_num']; ?>"><br>
    </hr />    
    </center>
    </div>
    </body>

