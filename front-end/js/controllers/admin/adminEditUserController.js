angular.module("ruServer").controller("adminEditUserCtrl", function ($scope, $http, $location, config, $rootScope, authenticationService) {

    // Variables declaration or attribution

    // Variables of the error alert
    $scope.showAlert = false;
    $scope.isError = false;
    $scope.errorText = "";

    // Load the information of the selected user passed through a rootScope variable
    $scope.user = $rootScope.selUser;

    // Initialize the variable isSameUser to false (this variable checks if the user beeing edited is the same user who is editing to enable the change password fields)
    $scope.isSameUser = false;

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
                    if (!_allowedAccess || _accessLevel != 0) $location.path("/login");
                })
                .catch(function (error) {
                    $rootScope.phpError = error.data;
                    $location.path("/error");
                });
        }
        else $location.path("/login");
    };

    // Function to check if the user editing his own user info
    $scope.isSameUser = function () {
        var _sourceAdmin = _token.slice(19);
        if (typeof($rootScope.selUser) != "undefined") {
            if (_sourceAdmin == $rootScope.selUser.registryNumber) return true;
            else return false;
        }
        else return false;
    };

    // Function to change the color of the warning box, depending if there is an error or not
    $scope.getColor = function () {
        if ($scope.isError == true) return "alert-danger";
        else return "alert-success";
    };

    // Function to get what method is beeing used (insert or edit), in order to disable the editing of some fields (e.g. Registry Number)
    $scope.getMethod = function () {
        if ($rootScope.editUserMethod == "Insert user info") return "insert";
        else if ($rootScope.editUserMethod == "Edit user info") return "update";
    };

    // Function to save new information of an user in the database and update the list
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
            // Post request to update users table with information of the new user
            $http.post(config.serverBaseUrl + "insertUser.php", _postData)
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
        else if ($scope.getMethod() == 'update') {

            if (!$scope.changePassword) {
                user.oldPassword = "";
                user.newPassword = "";
                user.newPasswordConfirm = "";
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

            if (user.newPassword == user.newPasswordConfirm) {
                // Post request to update users table with information of the new user
                $http.post(config.serverBaseUrl + "updateUser.php", _postData)
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
                $scope.errorText = "New password and confirmation do not match. Please check if you typed correctly again.";
                $scope.isError = true;
            }
        }
        else {
            $scope.showAlert = true;
            $scope.errorText = "An error ocurred. Go back to the Manage Users tab and try again.";
            $scope.isError = true;
        }
    };

    // Initialization

    _checkAuthenticationForPage();
});