angular.module("ruServer").controller("adminEditUserCtrl", function ($scope, $http, $location, config, $rootScope, authenticationService) {

    // Variables declaration or attribution

    $scope.showAlert = false;
    $scope.isError = false;
    $scope.errorText = "";

    $scope.user = $rootScope.selUser;
    $scope.editUserTitle = $rootScope.editUserTitle;

    $scope.isSameUser = false;

    var _token = localStorage['ruServer'];

    // Functions declaration

    var _checkAuthenticationForPage = function () {
        if (typeof (_token) != "undefined") {
            authenticationService.checkToken(_token)
                .then(function (response) {
                    var _allowedAccess = response.access;
                    var _accessLevel = response.accessLevel;
                    if (!_allowedAccess || _accessLevel != 0) $location.path("/login");
                })
                .catch(function (error) {
                    $rootScope.phpError = error.data;
                    $location.path("/error");
                });
        }
        else $location.path("/login");
    };

    $scope.isSameUser = function () {
        var _sourceAdmin = _token.slice(19);
        if (_sourceAdmin == $rootScope.selUser.registryNumber) return true;
        else return false;
    };

    $scope.getColor = function () {
        if ($scope.isError == true) return "alert-danger";
        else return "alert-success";
    };

    $scope.getMethod = function () {
        if ($rootScope.editUserTitle == "Insert user info") return "insert";
        else if ($scope.editUserTitle == "Edit user info") return "update";
    };

    $scope.saveUser = function (user) {
        var _sourceAdmin = _token.slice(19);

        if ($scope.getMethod() == 'insert') {
            var _postData = {
                tagNumber: user.tagNumber,
                sourceRegistryNumber: _sourceAdmin,
                targetRegistryNumber: user.registryNumber,
                name: user.name,
                accessLevel: user.accessLevel
            };
            $http.post(config.serverBaseUrl + "insertUser.php", _postData)
                .then(function (response) {
                    $scope.showAlert = true;
                    $scope.errorText = response.data;
                    if (response.data.startsWith("Error -")) $scope.isError = true;
                    else $scope.isError = false;
                })
                .catch(function (error) {
                    $rootScope.phpError = error.data;
                    $location.path("/error");
                });
        }
        else if ($scope.getMethod() == 'update') {

            if (typeof (user.newPassword) == "undefined" || typeof (user.oldPassword) == "undefined") {
                user.newPassword = "";
                user.oldPassword = "";
            }

            var _postData = {
                tagNumber: user.tagNumber,
                sourceRegistryNumber: _sourceAdmin,
                targetRegistryNumber: user.registryNumber,
                name: user.name,
                accessLevel: user.accessLevel,
                oldPassword: user.oldPassword,
                newPassword: user.newPassword
            };

            if (((user.newPassword == user.newPasswordConfirm) && (user.newPassword == "" && user.oldPassword == "")) || !$scope.isSameUser()) {
                $http.post(config.serverBaseUrl + "updateUser.php", _postData)
                    .then(function (response) {
                        $scope.showAlert = true;
                        $scope.errorText = response.data;
                        if (response.data.startsWith("Error -")) $scope.isError = true;
                        else $scope.isError = false;
                    })
                    .catch(function (error) {
                        $rootScope.phpError = error.data;
                        $location.path("/error");
                    });
            }
            else {
                $scope.showAlert = true;
                $scope.errorText = "New password and confirmation do not match. Please check if you typed correctly again.";
                $scope.isError = true;
            }
        }
    };

    // Initialization

    _checkAuthenticationForPage();
});