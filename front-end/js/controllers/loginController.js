angular.module("ruServer").controller("loginCtrl", function ($scope, $http, $location, config, $rootScope, $window, authenticationService) {

    // Variables declaration or attribution

    // Variables of the error alert
    $scope.showAlert = false;
    $scope.isLoginError = false;
    $scope.errorText = "";

    // Initialization of the input data to zero
    $scope.input = {
        registry: "",
        password: "",
        remember: true
    };

    // Initialization of variable showNavMenu to false in order to not show the nav menu in the login page
    $rootScope.showNavMenu = false;

    // Variable that stores the value of the token
    var _token = localStorage['ruServer'];

    // Functions declaration

    // Function to check if the user is already logged in
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
                    $location.path("/error");
                });
        }
    };

    // Function to change the color of the warning box, depending if there is an error or not
    $scope.getColor = function () {
        if ($scope.isLoginError == true) return "alert-danger";
        else return "alert-success";
    };

    // Function to check information from the input fields and create a token to the logged user
    $scope.loginUser = function () {
        var _postData = {
            registry: $scope.input.registry,
            password: $scope.input.password,
			fromApp: 0
        };
        // Post request with data of the user who is trying to login
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
            .catch(function (error) { // Error handling
                $rootScope.phpError = error.data;
                $location.path("/error");
            });
        
    };

    // Initialization

    _checkIfAlreadyLoggedIn();

});