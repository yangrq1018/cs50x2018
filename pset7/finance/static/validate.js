String.prototype.isMatch = function(s){
    return this.match(s)!==null 
}
$(document).ready(function() {

    $(".form-check #username").on('input', function() {
        var input = $(this);
        var username = input.val();
        var available;
        $.getJSON("/username_is_available", {
            username: username
        }, function(data) {
            available = data['response'];
            console.log(available);
            if (available) {
                input.addClass("valid").removeClass("invalid");
                // show check-info
                $("#username-info").css("display", "none");
            } else {
                input.addClass("invalid").removeClass("valid");
                $("#username-info").css("display", "block");
            }
        })
    });

    $(".form-check").on('submit', function(event) {
        var input = $(".form-check #password");
        var password = input.val();

        var passwordOk = (password.length >=8 && password.isMatch(/[A-z]/) && password.isMatch(/[A-Z]/) &&  password.isMatch(/\d/));
        if (passwordOk) {
            input.addClass("valid").removeClass("invalid");
            $("#password-info").css("display", "none");
        } else {
            input.addClass("invalid").removeClass("valid");
            $("#password-info").css("display", "block");
        }
        
        var confirm_password = $(".form-check #confirmation");
        if (password != confirm_password.val()) {
            confirm_password.addClass("invalid").removeClass("valid");
            $("#confirmation-info").css("display", "block");
        } else {
            confirm_password.addClass("valid").removeClass("invalid");
            $("#confirmation-info").css("display", "none");
        }
        
    
        $(".input-check").each(function(i) {
            if ($(this).hasClass("invalid")) {
                event.preventDefault();
            }
        });
    
    });
});