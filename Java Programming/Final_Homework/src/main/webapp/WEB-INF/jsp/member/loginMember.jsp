<%@ page contentType="text/html; charset=utf-8" %>
<html>
<head>
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css"/>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
    <script type="text/javascript">
        function loginMemeber() {
            document.newMember.action = '/member/processAddMember.do';
            $("#mail").val($("#mail1").val() + "@" + $("#mail2").val());
            document.newMember.submit();
        }

    </script>
    <title>Login</title>
</head>
<body onload="init()">
<jsp:include page="../menu.jsp"/>
<div class="jumbotron">
    <div class="container">
        <h1 class="display-3">로그인</h1>
    </div>
</div>
<div class="container" align="center">
    <div class="col-md-4 col-md-offset-4">
        <h3 class="form-signin-heading">Please sign in</h3>
        <div class='alert alert-danger' id="error" style="display:none">
            "아이디와 비밀번호를 확인해 주세요"
        </div>
        <form name="member" class="form-signin" action="/member/processLoginMember.do" method="post">
            <div class="form-group">
                <label for="inputUserName" class="sr-only">User Name</label>
                <input type="text" class="form-control" placeholder="ID" name="id" required autofocus>
            </div>
            <div class="form-group">
                <label for="pw" class="sr-only">Password</label>
                <input type="password" class="form-control" placeholder="Password" name="pw" required>
            </div>
            <button class="btn btn btn-lg btn-success btn-block" type="submit" onclick="loginMemeber()">로그인</button>
        </form>
    </div>
</div>
</body>

<script type="text/javascript">
    function init() {
        if ("${msg}" !== '')
            $("#error").show();
    }
</script>
</html>