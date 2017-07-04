angular.module("ruServer").controller("studentHomeCtrl", function ($scope, $location, authenticationService, $rootScope, $http, config, $window) {

    // Variables declaration or attribution

    // Variables of the error alert
    $scope.showAlert = false;
    $scope.isError = false;
    $scope.errorText = "";

    // Initialization of variable info, used to show information of RU to the student user
    $scope.info = {
        cardCredits: 0,
        cellphoneCredits: 0,
        insideRU: 0,
        numberOfEntries: 0
    };

    // Initialization of variable dataAvailable, used to show the data only if it has been already loaded
    $scope.dataAvailable = false;

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

    // Function to load basic status of RU to the student user
    var _loadHomeInfo = function () {
        var _userRegistry = _token.slice(19);
        var _postData = {
            registryNumber: _userRegistry
        };
        // Post request to load data returned from the page loadStudentInfo.php with information of the user passed as parameter in postData
        $http.post(config.serverBaseUrl + "loadStudentInfo.php", _postData)
            .then(function (response) {
                $scope.info = response.data;
                if (typeof ($scope.info) == "string") {
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
            .catch(function (error) { // Error handling
                $rootScope.phpError = error.data;
                $location.path("/error");
            });
    };

    // Function to refresh the page and load new information
    $scope.refresh = function () {
        $window.location.reload();
    }

    // Initialization

    _checkAuthenticationForPage();
    _loadHomeInfo();

});