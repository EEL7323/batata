angular.module("ruServer").controller("studentChangePasswordCtrl", function ($scope, $http, $location, config, $rootScope, authenticationService) {

    // Variables declaration or attribution

    // Variables of the error alert
    $scope.showAlert = false;
    $scope.isError = false;
    $scope.errorText = "";

    // Variable that stores the value of the token
    var _token = localStorage['ruServer'];

    // Functions declaration

    // Utilization of the authentication service to check if the user is allowed to be in this page, if not load the login page
    var _checkAuthenticationForPage = function () {
        if (typeof (_token) != "undefined") {
            authenticationService.checkToken(_token)
                .then(function (response) {
                    var _allowedAccess = response.access;
                    var _accessLevel = response.accessLevel;
                    if (!_allowedAccess || _accessLevel != 1) $location.path("/login");
                })
                .catch(function (error) {
                    $rootScope.phpError = error.data;
                    $location.path("/error");
                });
        }
        else $location.path("/login");
    };

    // Function to change the color of the warning box, depending if there is an error or not
    $scope.getColor = function () {
        if ($scope.isError == true) return "alert-danger";
        else return "alert-success";
    };

    // Function to check authentication and save new password of an user in the database and update the list
    $scope.updatePassword = function (user) {
        if (user.newPassword == user.newPasswordConfirm) {
            var _sourceRegistry = _token.slice(19);
            var _postData = {
                sourceRegistryNumber: _sourceRegistry,
                oldPassword: user.oldPassword,
                newPassword: user.newPassword
            };
            $http.post(config.serverBaseUrl + "updatePassword.php", _postData)
                .then(function (response) {
                    $scope.showAlert = true;
                    $scope.errorText = response.data;
                    if (response.data.startsWith("Error -")) $scope.isError = true;
                    else $scope.isError = false;
                })
                .catch(function (error) { // Error handling
                    $rootScope.phpError = error.data;
                    $location.path("/error");
                });
        }
        else {
            $scope.showAlert = true;
            $scope.errorText = "New password and confirmation do not match. Please try again.";
            $scope.isError = true;
        }
    };

    // Initialization

    _checkAuthenticationForPage();
});