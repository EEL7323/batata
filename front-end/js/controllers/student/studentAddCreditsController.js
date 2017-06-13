angular.module("ruServer").controller("studentAddCreditsCtrl", function ($scope, $http, $location, config, $rootScope, authenticationService) {

    // Variables declaration or attribution

    $scope.showAlert = false;
    $scope.isError = false;
    $scope.errorText = "";

    $scope.input = {
        phoneIncrement: 0,
        cardIncrement: 0
    };

    var _token = localStorage['ruServer'];

    // Functions declaration

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

    $scope.getColor = function () {
        if ($scope.isError == true) return "alert-danger";
        else return "alert-success";
    };

    $scope.addCredits = function (input) {
        var _sourceRegistry = _token.slice(19);
        var _postData = {
            sourceRegistryNumber: _sourceRegistry,
            diffCreditCellphone: input.phoneIncrement,
            diffCreditTag: input.cardIncrement,
            creditCardNumber: input.creditCardNumber,
            creditCardSecurityCode: input.creditCardSecurityCode,
            creditCardExpirationDate: input.creditCardExpirationDate
        };

        $http.post(config.serverBaseUrl + "updateCreditsbyCreditCard.php", _postData)
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
    };

    // Initialization
    _checkAuthenticationForPage();
});