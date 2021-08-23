<%@ page contentType="text/html; charset=utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ taglib prefix="sql" uri="http://java.sun.com/jsp/jstl/sql"%>
<html>
<head>
<link rel="stylesheet" href="../resource/res/css/bootstrap.min.css" />
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>

<%
	String sessionId = (String) session.getAttribute("sessionId");
%>

<title>회원 수정</title>
</head>
<body onload="init()">
	<jsp:include page="../menu.jsp" />
	<div class="jumbotron">
		<div class="container">
			<h1 class="display-3">회원 수정</h1>
		</div>
	</div>
	
	<div class="container">
		<form name="newMember" class="form-horizontal"
			action="/member/processUpdateMember.do" method="post"
			onsubmit="return checkForm()">
			<div class="form-group  row">
				<label class="col-sm-2 ">아이디</label>
				<div class="col-sm-3">
					<input name="id" type="text" class="form-control" placeholder="id" value="<c:out value=''/>" >
				</div>
			</div>
			<div class="form-group  row">
				<label class="col-sm-2">비밀번호</label>
				<div class="col-sm-3">
					<input name="pw" type="text" class="form-control" placeholder="pw" value="<c:out value=''/>" >
				</div>
			</div>
			<div class="form-group  row">
				<label class="col-sm-2">비밀번호확인</label>
				<div class="col-sm-3">
					<input name="password_confirm" type="text" class="form-control" placeholder="password_confirm" >
				</div>
			</div>
			<div class="form-group  row">
				<label class="col-sm-2">닉네임</label>
				<div class="col-sm-3">
					<input name="nickname" type="text" class="form-control" placeholder="nickname" value="<c:out value=''/>" >
				</div>
			</div>
			<div class="form-group  row">
				<label class="col-sm-2">영어 이름</label>
				<div class="col-sm-3">
					<input name="name" type="text" class="form-control" placeholder="english name" value="<c:out value=''/>" >
				</div>
			</div>
			<div class="form-group  row ">
				<label class="col-sm-2">이메일</label>
				<div class="col-sm-10">
					<input type="hidden" name="mail" id="mail">
					<input type="text" name="mail1" id="mail1" maxlength="50" value="">@
					<select name="mail2" id="mail2">
						<option>naver.com</option>
						<option>daum.net</option>
						<option>gmail.com</option>
						<option>nate.com</option>
					</select>
				</div>
			</div>
			<div class="form-group  row">
				<label class="col-sm-2">가입인사</label>
				<div class="col-sm-3">
					<input name="hello" type="text" class="form-control" placeholder="phone" value="<c:out value=''/>">
				</div>
			</div>
			<div class="form-group  row">
				<div class="col-sm-offset-2 col-sm-10 ">
					<a href="javascript:fn_updateMember()" class="btn btn-primary">회원수정</a>
					<a href="/member/deleteMember.do" class="btn btn-primary">회원탈퇴</a>
				</div>
			</div>
		</form>	
	</div>
</body>
</html>
<script type="text/javascript">
	function init() {
		var mail = "";
		$("#mail1").val(mail.split('@')[0]);
		$("#mail2").val(mail.split('@')[1]);
	}

	
	function checkForm() {
		if (!document.newMember.id.value) {
			alert("아이디를 입력하세요.");
			return false;
		}
		if (!document.newMember.password.value) {
			alert("비밀번호를 입력하세요.");
			return false;
		}
		if (document.newMember.password.value !== document.newMember.password_confirm.value) {
			alert("비밀번호를 동일하게 입력하세요.");
			return false;
		}
	}

	function fn_updateMember(){
		document.newMember.action = '/member/processUpdateMember.do';
		$("#mail").val($("#mail1").val()+"@"+$("#mail2").val());
		document.newMember.submit();
	}
</script>