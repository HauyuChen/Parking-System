
<!--调用百度api，实现驾车导航-->

<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
	<style type="text/css">
		body, html {width: 100%;height: 100%; margin:0;font-family:"微软雅黑";}
		#l-map{height:300px;width:100%;}
		#r-result,#r-result table{width:100%;}
	</style>
	<script type="text/javascript" src="http://api.map.baidu.com/api?v=2.0&ak=BgclRn6ghHh36oLX3NVwaiIT"></script>
	<title>驾车导航</title>
</head>
<body>
	<div id="l-map"></div>
	<div id="r-result"></div>
    <input type="hidden" id="jindu" value="<?php $str =$_SERVER['QUERY_STRING'];$str2 = substr($str,2,8);echo $str2;?>">
    <input type="hidden" id="weidu" value="<?php $str =$_SERVER['QUERY_STRING'];$str3 = substr($str,14,18);echo $str3;?>">
    
  
</body>
</html>

<script type="text/javascript">
	// 百度地图API功能
	var map = new BMap.Map("l-map");
    map.centerAndZoom(new BMap.Point(114.41, 23.04), 12);// 固定目的地，及停车场的经纬度
    var jin=document.getElementById("jindu").value;
    var wei=document.getElementById("weidu").value;
	 var p1 = new BMap.Point(wei,jin);
	var p2 = new BMap.Point(114.415764,23.031794);

	var driving = new BMap.DrivingRoute(map, {renderOptions: {map: map, panel: "r-result", autoViewport: true}});
	driving.search(p1,p2);
</script>

