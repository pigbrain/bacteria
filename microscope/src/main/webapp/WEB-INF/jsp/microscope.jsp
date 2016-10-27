<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" isELIgnored="false" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>

<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Microscope</title>

</head>
<body style="width: 100%; height: 100%; position: absolute; margin: 0px; padding: 0px; overflow: hidden;">
	<section style="position: absolute; width:25%; height: 100%; background-color: rgb(60, 63, 65);">
		<h1><font color="white">Exception Logs</font></h1>
		<hr>
		<c:forEach var="exceptionLog" items="${exceptionLogList}">
			<c:set var="logInfo" value="${fn:split(exceptionLog.fileName, '_')}" />
			<section><a href="/${exceptionLog.key}"><font color="white">${logInfo[0]} ${logInfo[1]}</font></a></section><br>
		</c:forEach> 
	</section>
	<section style="position: absolute; left: 25%; width: 100%; height: 100%; background-color: rgb(60, 63, 65);">
		<!-- <div style="position: relative; left: 10px; width: 2px; height: 100%; background-color: black;" /> -->
		<section>
			<c:if test="${not empty exceptionLog}">
				<div><font color="white">${exceptionLog}</font></div>
			</c:if>
			<c:if test="${empty exceptionLog}">
				<font color="white"><p>choice log</p></font>
			</c:if>
		</section>
	</section>
</body>
</html>