<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" isELIgnored="false" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>

<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Microscope</title>


</head>
<body>
 	
	<c:forEach var="exceptionLog" items="${exceptionLogList}">
		<div><a href="/${exceptionLog.key}">${exceptionLog.filePath} (${exceptionLog.key})</a></div>	
	</c:forEach> 
	
	<hr>
	
	<c:if test="${not empty exceptionLog}">
		<div>${exceptionLog}</div>
	</c:if>
</body>
</html>