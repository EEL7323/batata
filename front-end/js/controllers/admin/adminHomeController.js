angular.module("ruServer").controller("adminHomeCtrl", function ($scope, $location, authenticationService, $rootScope, $http, config, $window) {

    // Variables declaration or attribution

    $scope.showAlert = false;
    $scope.isError = false;
    $scope.errorText = "";

    $scope.info = {
        subscribedStudents: 0,
        insideRU: 0,
        lastLunch: 0,
        lastDinner: 0
    };

    $scope.dataAvailable = false;

    var _token = localStorage['ruServer'];

    // Functions declaration

    var _loadHomeInfo = function () {
        $http.get(config.serverBaseUrl + "loadAdminInfo.php")
            .then(function (response) {
                $scope.info = response.data;
                if (typeof($scope.info) == "string") {
                    $scope.showAlert = true;
                    $scope.isError = true;
                    $scope.errorText = response.data;
                    $scope.dataAvailable = false;
                }
                else {
                    $scope.dataAvailable = true;
                    $scope.showAlert = false;
                    $scope.isError = false;
                    $scope.errorText = "";
                }
            })
            .catch(function (error) {
                $rootScope.phpError = error.data;
                $location.path("/error");
            });
    };

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

    $scope.refresh = function () {
        $window.location.reload();
    }

    // Initialization

    _checkAuthenticationForPage();
    _loadHomeInfo();

});