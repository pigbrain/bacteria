<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Microscope</title>

<style type="text/css">
div:HOVER {
	cursor: w-resize;
}
</style>

</head>
<body style="width: 100%; height: 100%; position: absolute; margin: 0px; padding: 0px; overflow: hidden;">
	<section style="position: absolute; width: 20%; height: 100%; background-color: rgb(60, 63, 65);"></section>
	<section style="position: absolute; left: 20%; width: 40%; height: 100%; background-color: rgb(60, 63, 65);">
		<div style="position: relative; left: -2px; width: 2px; height: 100%; background-color: black;"></div>
	</section>
	<section style="position: absolute; left: 60%; width: 40%; height: 100%; background-color: rgb(60, 63, 65);">
		<div style="position: relative; left: -2px; width: 2px; height: 100%; background-color: black;"></div>
	</section>
	<section style="position: absolute; width: 100%; height: 30px; background-color: rgb(60, 63, 65); border-bottom: solid rgb(0, 0, 0)"></section>
</body>
</html>