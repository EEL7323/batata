angular.module("ruServer").controller("mainCtrl", function ($scope, authenticationService, $location, $rootScope) {

    // Variables declaration or attribution

    var _token = localStorage['ruServer'];

    // Functions declaration

    var _mainRedirect = function () {
        if (typeof (_token) != "undefined") {
            authenticationService.checkToken(_token)
                .then(function (response) {
                    $scope.name = response.name;
                    var _allowedAccess = response.access;
                    var _accessLevel = response.accessLevel;
                    $scope.registry = response.registry;
                    $scope.tag = response.tag;
                    $rootScope.showNavMenu = false;
                    if (_allowedAccess) {
                        if (_accessLevel == 0) {
                            $scope.titleButton1 = "Home";
                            $scope.linkButton1 = "/admin";
                            $scope.titleButton2 = "Manage Credits";
                            $scope.linkButton2 = "/admin/manage-credits";
                            $scope.titleButton3 = "Manage Users";
                            $scope.linkButton3 = "/admin/manage-users";
                            $scope.titleButton4 = "View log";
                            $scope.linkButton4 = "/admin/log";
                            if ($rootScope.loadAgain) $location.path("/admin");
                            $rootScope.showNavMenu = true;
                        }
                        else if (_accessLevel == 1) {
                            $scope.titleButton1 = "Home";
                            $scope.linkButton1 = "/student";
                            $scope.titleButton2 = "Add Credits";
                            $scope.linkButton2 = "/student/add-credits";
                            $scope.titleButton3 = "Change Password";
                            $scope.linkButton3 = "/student/change-password";
                            $scope.titleButton4 = "View log";
                            $scope.linkButton4 = "/student/log";
                            if ($rootScope.loadAgain) $location.path("/student");
                            $rootScope.showNavMenu = true;
                        }
                        else {
                            $scope.titleButton1 = "";
                            $scope.linkButton1 = "";
                            $scope.titleButton2 = "";
                            $scope.linkButton2 = "";
                            $scope.titleButton3 = "";
                            $scope.linkButton3 = "";
                            $scope.titleButton4 = "";
                            $scope.linkButton4 = "";
                            $rootScope.showNavMenu = false;
                            $location.path("/login");

                        }

                    }
                    else {
                        $scope.titleButton1 = "";
                        $scope.linkButton1 = "";
                        $scope.titleButton2 = "";
                        $scope.linkButton2 = "";
                        $scope.titleButton3 = "";
                        $scope.linkButton3 = "";
                        $scope.titleButton4 = "";
                        $scope.linkButton4 = "";
                        $rootScope.showNavMenu = false;
                        $location.path("/login");
                    }

                })
                .catch(function (error) {
                    $rootScope.phpError = error.data;
                    $location.path("/error");
                });
        }
        else {
            $scope.titleButton1 = "";
            $scope.linkButton1 = "";
            $scope.titleButton2 = "";
            $scope.linkButton2 = "";
            $scope.titleButton3 = "";
            $scope.linkButton3 = "";
            $scope.titleButton4 = "";
            $scope.linkButton4 = "";
            $rootScope.showNavMenu = false;
            $location.path("/login");
        }
    };

    $scope.logout = function () {
        localStorage.removeItem("ruServer");
        $location.path("/login");
    };

    // Initialization

    _mainRedirect();

});