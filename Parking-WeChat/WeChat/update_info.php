
<!--修改用户 所绑定的信息-->

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">
    <link rel="icon" href="../../favicon.ico">

    <title>车主信息修改</title>

    <!-- Bootstrap core CSS -->
    <link href="http://cdn.bootcss.com/bootstrap/3.3.2/css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="signin.css" rel="stylesheet">

    <!-- Just for debugging purposes. Don't actually copy these 2 lines! -->
    <!--[if lt IE 9]><script src="../../assets/js/ie8-responsive-file-warning.js"></script><![endif]-->
    <script src="../../assets/js/ie-emulation-modes-warning.js"></script>

    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
      <script src="http://cdn.bootcss.com/html5shiv/3.7.2/html5shiv.min.js"></script>
      <script src="http://cdn.bootcss.com/respond.js/1.4.2/respond.min.js"></script>
    <![endif]-->
  </head>

  <body>

    <div class="container">

      <form class="form-signin" action="update_UI.php" method="POST">
          <center><h3 class="form-signin-heading">车主信息修改</h3></center>
       
        Name<input type="text" id="inputEmail" class="form-control" placeholder="输入你的姓名" name="username"  value="<?php if(isset($_['post'])) echo $_POST["username"];?>" required autofocus >
      
       
        Tel <input type="text" id="inputEmail" class="form-control" placeholder="输入你的手机号码" name="phone" value="<?php if(isset($_['post'])) echo $_POST["phone"];?>"  >
        No.<input type="text" id="inputEmail" class="form-control" placeholder="输入你的车牌号" name="number"  value="<?php if(isset($_['post'])) echo $_POST["number"];?>" ><br><br>
          <input type="hidden" id="wechatid" name="wechatid" value="<?php $str =$_SERVER['QUERY_STRING'];$str3 = substr($str,0);echo $str3;?>">
          
       
        <button class="btn btn-lg btn-primary btn-block" type="submit">提交</button>
      </form>

    </div> <!-- /container -->


    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <script src="../../assets/js/ie10-viewport-bug-workaround.js"></script>
  </body>
</html>
