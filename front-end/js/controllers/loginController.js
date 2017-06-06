angular.module("ruServer").controller("loginCtrl", function ($scope, $http, $location, config, $rootScope, $window, authenticationService) {

    // Initialize variables

    $scope.input = {
        registry: "",
        password: "",
        remember: true
    };

    $scope.showAlert = false;
    $scope.isLoginError = false;
    $scope.errorText = "";

    $rootScope.showNavMenu = false;

    // Define functions

    $scope.getColor = function () {
        if ($scope.isLoginError == true) return "alert-danger";
        else return "alert-success";
    }

    $scope.loginUser = function () {
        var _postData = {
            registry: $scope.input.registry,
            password: $scope.input.password
        };

        $http.post(config.serverBaseUrl + "login.php", _postData)
            .then(function (response) {
                console.log(response.data);
                if (response.data.startsWith("Authentication error")) {
                    $scope.isLoginError = true;
                    $scope.errorText = "Authentication error. Please verify if you typed your registry number and your password correctly and try again. If the error persists, contact RU administration.";
                    localStorage.setItem("ruServer", "invalid");
                    $rootScope.showNavMenu = false;
                }
                else {
                    $scope.isLoginError = false;
                    $scope.errorText = "Login successful. You will be redirected to homepage.";
                    localStorage.setItem("ruServer", response.data);
                    var _accessLevel = response.data.slice(7, 8);
                    if (_accessLevel == 0) {
                        $location.path("/admin");
                        $rootScope.showNavMenu = true;
                        $window.location.reload();
                    }
                    else if (_acessLevel == 1) {
                        $location.path("/student");
                        $rootScope.showNavMenu = true;
                        $window.location.reload();
                    }
                    else {
                        $location.path("/login")
                        $rootScope.showNavMenu = false;
                    }
                }
                $scope.showAlert = true;
            })
            .catch(function (error) {
                $scope.isLoginError = true;
                $scope.errorText = "Error connecting to the database. Please verify your Internet connection and try again. If the error persists, contact RU administration.";
                localStorage.setItem("ruServerToken", "invalid");
                $rootScope.showNavMenu = false;
            });
        
    };

    // Initialization
    
    var _token = localStorage['ruServerToken'];
    authenticationService.checkToken(_token)
        .then(function (response) {
            if (response.access) $window.location.reload();
        });
});