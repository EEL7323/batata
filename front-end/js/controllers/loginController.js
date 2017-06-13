angular.module("ruServer").controller("loginCtrl", function ($scope, $http, $location, config, $rootScope, $window, authenticationService) {

    // Variables declaration or attribution

    $scope.input = {
        registry: "",
        password: "",
        remember: true
    };

    $scope.showAlert = false;
    $scope.isLoginError = false;
    $scope.errorText = "";
    $rootScope.showNavMenu = false;

    var _token = localStorage['ruServer'];

    // Functions declaration

    var _checkIfAlreadyLoggedIn = function () {
        if (typeof (_token) != "undefined") {
            authenticationService.checkToken(_token)
                .then(function (response) {
                    if (typeof(response.data == "object")) {
                        if (response.access) {
                            $window.location.reload();
                        }
                    }
                    else {
                        $scope.showAlert = true;
                        $scope.isLoginError = true;
                        $scope.errorText = "Authentication error. Please verify if you typed your registry number and your password correctly and try again. If the error persists, contact RU administration.";
                        $rootScope.showNavMenu = false;
                    }
                })
                .catch(function (error) {
                    $rootScope.phpError = error.data;
                    console.log("é aqui meu amigo!");
                    $location.path("/error");
                });
        }
    };

    $scope.getColor = function () {
        if ($scope.isLoginError == true) return "alert-danger";
        else return "alert-success";
    };

    $scope.loginUser = function () {
        var _postData = {
            registry: $scope.input.registry,
            password: $scope.input.password
        };

        $http.post(config.serverBaseUrl + "login.php", _postData)
            .then(function (response) {
                if (response.data.startsWith("Authentication error")) {
                    $scope.isLoginError = true;
                    $scope.errorText = "Authentication error. Please verify if you typed your registry number and your password correctly and try again. If the error persists, contact RU administration.";
                    localStorage.setItem("ruServer", "invalid");
                    $rootScope.loading = false;
                    $rootScope.showNavMenu = false;
                }
                else {
                    $scope.isLoginError = false;
                    $scope.errorText = "Login successful. You will be redirected to homepage.";
                    localStorage.setItem("ruServer", response.data);
                    var _accessLevel = response.data.slice(7, 8);
                    if (_accessLevel == 0) {
                        $rootScope.showNavMenu = true;
                        $rootScope.loading = false;
                        $location.path("/admin");
                        $window.location.reload();
                    }
                    else if (_accessLevel == 1) {
                        $rootScope.showNavMenu = true;
                        $rootScope.loading = false;
                        $location.path("/student");
                        $window.location.reload();
                    }
                    else {
                        $location.path("/login")
                        $rootScope.loading = false;
                        $rootScope.showNavMenu = false;
                    }
                }
                $scope.showAlert = true;
            })
            .catch(function (error) {
                $rootScope.phpError = error.data;
                //$location.path("/error");
                console.log(error.data);
            });
        
    };

    // Initialization

    _checkIfAlreadyLoggedIn();

});