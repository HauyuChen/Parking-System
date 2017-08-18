/*

微信基本交互 功能代码

*/

<?php
//装载模板文件
session_start();
include_once("wx_tpl.php");
include_once("conn.php");

//获取微信发送数据
$postStr = $GLOBALS["HTTP_RAW_POST_DATA"];

  //返回回复数据
if (!empty($postStr))
    
   {

    	//解析数据
          $postObj = simplexml_load_string($postStr, 'SimpleXMLElement', LIBXML_NOCDATA);
    	//发送消息方ID
          $fromUsername = $postObj->FromUserName;
    	//接收消息方ID
          $toUsername = $postObj->ToUserName;
   	 //消息类型
          $form_MsgType = $postObj->MsgType;
          $form_Event=$postObj->Event;
          $the_time=date('y-m-d');   //如果时间添加有分钟就更好了


//地理位置,本地天气
          if($form_MsgType=="location")
          {
            //获取地理消息信息，经纬度，地图缩放比例，地址
             $from_Location_X=$postObj->Location_X;
             $from_Location_Y=$postObj->Location_Y;
       
            //建立抓取对象
              $f = new SaeFetchurl();
            //抓取百度地址解析
              $geocoder = $f->fetch($map_api_url.$map_coord_type."&location=".$from_Location_X.",".$from_Location_Y);
              $msgType = "text";
              //          其中的ak是利用百度api 中 浏览器端的ak 密匙！！！
              // $x="http://api.map.baidu.com/telematics/v3/distance?waypoints=118.77147503233,32.054128923368;116.3521416286,39.965780080447;116.28215586757,39.965780080447&ak=gscbdbptoWIduB6nSPfOMXdi";
              // 查找附近最近的 停车场
       
              $url="http://api.map.baidu.com/telematics/v3/distance?callback=myCallback&ak=gscbdbptoWIduB6nSPfOMXdi&waypoints=".$from_Location_Y.",".$from_Location_X.";114.415766,23.031780";
              $apistr=file_get_contents($url);
              $apiobj=simplexml_load_string($apistr);
              $distanceobj=$apiobj->results->distance;
              $juli=intval($distanceobj);
     
              $out="停车场距离你有 ".$juli." 米远,点击即可导航到停车场";
    
              $x="http://2.tryagain3.sinaapp.com/car_tip.php?x=".$from_Location_X."&y=".$from_Location_Y;
             
               $resultStr="<xml>\n
              <ToUserName><![CDATA[".$fromUsername."]]></ToUserName>\n
              <FromUserName><![CDATA[".$toUsername."]]></FromUserName>\n
              <CreateTime>".time()."</CreateTime>\n
              <MsgType><![CDATA[news]]></MsgType>\n
              <ArticleCount>1</ArticleCount>\n
              <Articles>\n
              <item>\n
              <Title><![CDATA[点击导航到停车场]]></Title> \n
              <Description><![CDATA[$out]]></Description>\n
              <PicUrl><![CDATA[http://dreamwhat-img.stor.sinaapp.com/u=3116266380,2489126012&fm=21&gp=0.jpg]]></PicUrl>\n
              <Url><![CDATA[$x]]></Url>\n
              </item>\n

              </Articles>\n
              <FuncFlag>0</FuncFlag>\n
              </xml>";
              echo $resultStr;
              exit;  
           
             
          }

        
  	//文字消息

          if($form_MsgType=="text" )
          {

           //获取用户发送的文字内容
            $form_Content = trim($postObj->Content);

	      //如果发送内容则根据关键字恢复相应信息
 	        if(!empty($form_Content))
            {
            if($form_Content=='车位' || $form_Content=='5' )
              {
                   
                $fp = fsockopen("tcp://119.146.68.41", 5000, $errno, $errstr);
              
                if (!$fp)
                {
                    $out= "ERROR: $errno - $errstr\n";
                } else
                {
                   fwrite($fp,"WeChat"."search\n".$fromUsername);

                       $out= fread($fp, 8192);
                       $kk= fread($fp, 8192);
                    fclose($fp);
                      
                }   
    
              $out=mb_convert_encoding($out,"utf-8", "GBK"); 
              $kk=mb_convert_encoding($kk,"utf-8", "GBK"); 
              $resultStr="<xml>\n
              <ToUserName><![CDATA[".$fromUsername."]]></ToUserName>\n
              <FromUserName><![CDATA[".$toUsername."]]></FromUserName>\n
              <CreateTime>".time()."</CreateTime>\n
              <MsgType><![CDATA[news]]></MsgType>\n
              <ArticleCount>1</ArticleCount>\n
              <Articles>\n
              <item>\n
              <Title><![CDATA[点击进行占用车位]]></Title> \n
              <Description><![CDATA[车位情况：$out$kk]]></Description>\n
              <PicUrl><![CDATA[http://dreamwhat-img.stor.sinaapp.com/u=3116266380,2489126012&fm=21&gp=0.jpg]]></PicUrl>\n
              <Url><![CDATA[http://2.tryagain3.sinaapp.com/position.php?x=$kk&y=$fromUsername]]></Url>\n
              </item>\n
              </Articles>\n
              <FuncFlag>0</FuncFlag>\n
              </xml>";   
              echo $resultStr;    
              exit;
 		     }
            
                //当用户输入 查询或3的时候，返回用户的停车信息 
            
              if($form_Content=='查询' || $form_Content=='3' )
              {
                   
                $fp = fsockopen("tcp://119.146.68.41", 5000, $errno, $errstr);
                 if (!$fp)
                 {
                    $out= "ERROR: $errno - $errstr\n";
                 }
                 else
                {
                     
                   fwrite($fp,"WeChat"."look\n".$fromUsername);
                   $out= fread($fp, 1024);
                   fclose($fp);
                      
                }   
 
                $out=mb_convert_encoding($out,"utf-8", "GBK"); 
                $msgType = "text";
                $resultStr = sprintf($textTpl, $fromUsername, $toUsername, $time, $msgType, $out."[玫瑰]");
                echo $resultStr;
                exit;
   
 		   }
                  
                
              
                // 利用缓存 保留用户上下文信息，引导管理员登陆管理模式
			 if($form_Content=='admin' ||  $form_Content=='xxx' || $form_Content=='7' || $form_Content=='管理')
			{
				 $mmc=memcache_init();//开启缓存
	 
                if($form_Content=='admin'  )
                {
                    memcache_set($mmc,$fromUsername."key",$fromUsername.$form_Content,$flag=0,$expire=15);
                    $contentStr="请输入管理员密码";
                    $msgType = "text";
                    $resultStr = sprintf($textTpl, $fromUsername, $toUsername, $time, $msgType,  $contentStr."[玫瑰]");
                    echo $resultStr; 
                    exit;
                }
               else
                    
                {
               
                    $struser= memcache_get($mmc,$fromUsername."key");
                    if($struser==$fromUsername."admin" )
                    {
                       
                        if($form_Content=='xxx')
                        {
                      
                            //弹出页面请，加入一个查询车位动作，将车位数量保存到数据库中
                            
                            $fp = fsockopen("tcp://119.146.68.41", 5000, $errno, $errstr);
              
                if (!$fp)
                {
                    $out= "ERROR: $errno - $errstr\n";
                } else
                {
                   fwrite($fp,"WeChat"."search\n".$fromUsername);

                       $out= fread($fp, 8192);
                       $kk= fread($fp, 8192);
                    fclose($fp);
                      
                }   
    
               $out=mb_convert_encoding($out,"utf-8", "GBK"); 
               $kk=mb_convert_encoding($kk,"utf-8", "GBK");  //获得车位信息
                            
                            //对所获得信息进行处理              
               $string=$kk;
               $an=0;
               $bn=0;
                for($k=0;$k<strlen($string);$k=$k+5)
                        if($string[$k]=="A")
                                  $an++;      
                    $sqla="update car_num set  a_num='{$an}' where openid='admin'";
                            mysql_query($sqla);  //将A类空车位好保存到数据库中
                   for($k=0;$k<strlen($string);$k=$k+5)
                        if($string[$k]=="B")
                                  $bn++;      
                    $sqlb="update car_num set  b_num='{$bn}' where openid='admin'";
                            mysql_query($sqlb);  //将B类空车位好保存到数据库中
                 $all=$an+$bn;
                  $sqlab="update car_num set  total='{$all}' where openid='admin'";
                 mysql_query($sqlab);  //统计空车总数量
                                     
                            $resultStr="<xml>\n
              <ToUserName><![CDATA[".$fromUsername."]]></ToUserName>\n
              <FromUserName><![CDATA[".$toUsername."]]></FromUserName>\n
              <CreateTime>".time()."</CreateTime>\n
              <MsgType><![CDATA[news]]></MsgType>\n
              <ArticleCount>1</ArticleCount>\n
              <Articles>\n
              <item>\n
              <Title><![CDATA[欢迎进入管理员界面]]></Title> \n
              <Description><![CDATA[]]></Description>\n
              <PicUrl><![CDATA[http://tryagain3-photo.stor.sinaapp.com/administrator.jpg]]></PicUrl>\n
              <Url><![CDATA[http://2.tryagain3.sinaapp.com/admin.php]]></Url>\n
              </item>\n

              </Articles>\n
              <FuncFlag>0</FuncFlag>\n
              </xml>";
              echo $resultStr;
                            
              exit; 
                            
                        } 
                        
                  }
               
                    else
                    {
                        $contentStr="请先输入管理员账号";
                          $msgType = "text";
                  $resultStr = sprintf($textTpl, $fromUsername, $toUsername, $time, $msgType,  $contentStr."[玫瑰]");
                  echo $resultStr; 
                  exit; 
                    }
               
                           
                  $msgType = "text";
                  $resultStr = sprintf($textTpl, $fromUsername, $toUsername, $time, $msgType,  $contentStr."[玫瑰]");
                  echo $resultStr; 
                  exit; 
                } 
              }
                
               
                //当用户输入绑定或2 ，弹出绑定界面,当用户重复绑定，提示消息
            
               if($form_Content=='绑定' || $form_Content=='2' )
              {
   
             
               $sql="select * from wechat where openid='{$fromUsername}'";
               $query=mysql_query($sql);
               $rs=mysql_fetch_array($query);
               // $weiyi=$rs['openid'];
                   $id=$rs['id'];//如果为1 说明用户之前绑定成功！
               if($id==1)
               {
                   $tip="你已经绑定过了，不能重复绑定！";
                   $msgType = "text";
                   $resultStr = sprintf($textTpl, $fromUsername, $toUsername, $time, $msgType, $tip."[玫瑰]");
                   echo $resultStr;
                   exit;
               }
               else
               { 
                   $sql2="insert  into wechat (openid) values ('{$fromUsername}')";
                   mysql_query($sql2);

              $resultStr="<xml>\n
              <ToUserName><![CDATA[".$fromUsername."]]></ToUserName>\n
              <FromUserName><![CDATA[".$toUsername."]]></FromUserName>\n
              <CreateTime>".time()."</CreateTime>\n
              <MsgType><![CDATA[news]]></MsgType>\n
              <ArticleCount>1</ArticleCount>\n
              <Articles>\n
              <item>\n
              <Title><![CDATA[点击绑定车主信息]]></Title> \n
              <Description><![CDATA[快戳我，戳！戳！戳！]]></Description>\n
              <PicUrl><![CDATA[http://dreamwhat-img.stor.sinaapp.com/ovDrpjmGFPmHQtgqB9CrY6e_bHDk20150401201959.jpg]]></PicUrl>\n
              <Url><![CDATA[http://2.tryagain3.sinaapp.com/car_user.php?x=$fromUsername]]></Url>\n
              </item>\n
              </Articles>\n
              <FuncFlag>0</FuncFlag>\n
              </xml>";
              echo $resultStr;
              exit;
               }
          
             }
              
                           if($form_Content=='车位' || $form_Content=='1')
               {
               $fp = fsockopen("tcp://119.146.68.41", 5000, $errno, $errstr);
              
                if (!$fp)
                {
                    $out= "ERROR: $errno - $errstr\n";
                } else
                {
                   fwrite($fp,"WeChat"."search\n".$fromUsername);

                       $out= fread($fp, 8192);
                       $kk= fread($fp, 8192);
                    fclose($fp);
                      
                }   
    
              $out=mb_convert_encoding($out,"utf-8", "GBK"); 
              $kk=mb_convert_encoding($kk,"utf-8", "GBK"); 
                                 $string=$kk;
               $an=0;
               $bn=0;
                for($k=0;$k<strlen($string);$k=$k+5)
                        if($string[$k]=="A")
                                  $an++;      
                
                   for($k=0;$k<strlen($string);$k=$k+5)
                        if($string[$k]=="B")
                                  $bn++;      
                 
                 $all=$an+$bn;
              
                                     
              $msgType = "text";
                               $resultStr = sprintf($textTpl, $fromUsername, $toUsername, $time, $msgType, "尊敬的用户[玫瑰]，目前剩余空闲车位数量如下：\n总空闲车位有".$all."个\nA区空闲车位有".$an."个\nB区空闲车位有".$bn."个\n"."[愉快]");  
                echo $resultStr;
                exit;  
 
             }
                
           
             
            if($form_Content=='解除' || $form_Content=='6')
                {
                    
                $fp = fsockopen("tcp://119.146.68.41", 5000, $errno, $errstr);
                
                if (!$fp)
                {
                    $out= "ERROR: $errno - $errstr\n";
                } else
                {
                  
                   
                   fwrite($fp,"WeChat"."free\n".$fromUsername);
                   $out= fread($fp, 1024);
                   fclose($fp);
                      
                }   
              
                $out=mb_convert_encoding($out,"utf-8", "GBK"); 
                $msgType = "text";
                $resultStr = sprintf($textTpl, $fromUsername, $toUsername, $time, $msgType, $out."[愉快]");  
                echo $resultStr;
                exit;  
 
               }
            
               $huida=(explode('+', $form_Content, 4));  //对用户输出的内容分割，然后分别存储！！！
               $huida[1]=mb_convert_encoding($huida[1], "GBK","utf-8"); 
            
               if($form_Content=='修改' || $form_Content=='4')
               {
                 
              $resultStr="<xml>\n
              <ToUserName><![CDATA[".$fromUsername."]]></ToUserName>\n
              <FromUserName><![CDATA[".$toUsername."]]></FromUserName>\n
              <CreateTime>".time()."</CreateTime>\n
              <MsgType><![CDATA[news]]></MsgType>\n
              <ArticleCount>1</ArticleCount>\n
              <Articles>\n
              <item>\n
              <Title><![CDATA[点击修改车主信息]]></Title> \n
              <Description><![CDATA[快戳我，戳！戳！戳！]]></Description>\n
              <PicUrl><![CDATA[http://dreamwhat-img.stor.sinaapp.com/u=3116266380,2489126012&fm=21&gp=0.jpg]]></PicUrl>\n
              <Url><![CDATA[http://2.tryagain3.sinaapp.com/update.info.php?$fromUsername]]></Url>\n
              </item>\n

              </Articles>\n
              <FuncFlag>0</FuncFlag>\n
              </xml>";
              echo $resultStr;
              exit; 
         
        }
            
             $wo=iconv('utf-8','gbk', $form_Content);
             $word=substr($wo,0,3);
             if($word=='use')
               {
                                
                 $fp = fsockopen("tcp://119.146.68.41", 5000, $errno, $errstr);
                 if (!$fp)
                 {
                    $out= "ERROR: $errno - $errstr\n";
                 }
                 else
                {
                     
                   fwrite($fp,"WeChat"."use\n".substr($form_Content,3)."\n".$fromUsername);
                   $out= fread($fp, 1024);
                   fclose($fp);
                      
                }   
 
                $out=mb_convert_encoding($out,"utf-8", "GBK"); 
                $msgType = "text";
                $resultStr = sprintf($textTpl, $fromUsername, $toUsername, $time, $msgType, $out."[玫瑰]");
                echo $resultStr;
                exit;
   
             }
                      
   
               
                 $resultStr="<xml>\n
              <ToUserName><![CDATA[".$fromUsername."]]></ToUserName>\n
              <FromUserName><![CDATA[".$toUsername."]]></FromUserName>\n
              <CreateTime>".time()."</CreateTime>\n
              <MsgType><![CDATA[news]]></MsgType>\n
              <ArticleCount>1</ArticleCount>\n
              <Articles>\n
              <item>\n
              <Title><![CDATA[欢迎使用@ 鹅城智能停车系统 ]]></Title> 
              <Description><![CDATA[0--->发送 地理位置信息 导航到停车场\n1--->发送【1】或【车位】查看剩余车位\n2--->发送【2】或【绑定】即可绑定车主信息\n3--->发送【3】或【查询】即可查询所绑定的车主信息\n4--->发送【4】或【修改】即可修改车主信息\n5--->发送【5】或【防盗】查看和选择空闲车位并开启防盗模式\n6--->发送【6】或【解除】即可关闭防盗模式\n7--->发送【7】或【管理】即可进入管理员模式\n]]></Description>\n
              <PicUrl><![CDATA[http://tryagain3-photo.stor.sinaapp.com/u=2104178393,2703306304&fm=21&gp=0.jpg]]></PicUrl>\n
            
              </item>\n
              </Articles>\n
              <FuncFlag>0</FuncFlag>\n
              </xml>";
              echo $resultStr;
              exit;
            }      
          
          }   
    
    if( $form_Event=="subscribe")
    {
     
                $resultStr="<xml>\n
              <ToUserName><![CDATA[".$fromUsername."]]></ToUserName>\n
              <FromUserName><![CDATA[".$toUsername."]]></FromUserName>\n
              <CreateTime>".time()."</CreateTime>\n
              <MsgType><![CDATA[news]]></MsgType>\n
              <ArticleCount>1</ArticleCount>\n
              <Articles>\n
              <item>\n
              <Title><![CDATA[欢迎使用@ 鹅城智能停车系统 ]]></Title> 
              <Description><![CDATA[0--->发送 地理位置信息 导航到停车场\n1--->发送【1】或【车位】查看剩余车位\n2--->发送【2】或【绑定】即可绑定车主信息\n3--->发送【3】或【查询】即可查询所绑定的车主信息\n4--->发送【4】或【修改】即可修改车主信息\n5--->发送【5】或【防盗】查看和选择空闲车位并开启防盗模式\n6--->发送【6】或【解除】即可关闭防盗模式\n7--->发送【7】或【管理】即可进入管理员模式\n]]></Description>\n
              <PicUrl><![CDATA[http://tryagain3-photo.stor.sinaapp.com/u=2104178393,2703306304&fm=21&gp=0.jpg]]></PicUrl>\n
            
              </item>\n
              </Articles>\n
              <FuncFlag>0</FuncFlag>\n
              </xml>";
              echo $resultStr;
              exit;
    }
        

  }

?>