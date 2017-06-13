angular.module("ruServer").controller("studentAddCreditsCtrl", function ($scope, $http, $location, config, $rootScope, authenticationService) {

    // Variables declaration or attribution

    $scope.showAlert = false;
    $scope.isError = false;
    $scope.errorText = "";

    $scope.input = {
        phoneIncrement: 0,
        cardIncrement: 0,
        creditCardNumber: 0,
        creditCardExpirationDate: 0,
        creditCardSecurityCode:0
    };

    var _simulatedCard = {
        creditCardNumber: 1111222233334444,
        creditCardExpirationDate: new Date(2017, 11, 1, 0, 0, 0, 0),
        creditCardSecurityCode: 123
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
            targetRegistryNumber: _sourceRegistry,
            diffCreditCellphone: input.phoneIncrement,
            diffCreditTag: input.cardIncrement,
        };

        var _sameCreditCardNumber = (_simulatedCard.creditCardNumber == input.creditCardNumber);
        var _sameExpirationDate = (_simulatedCard.creditCardExpirationDate.toString() == input.creditCardExpirationDate.toString());
        var _sameSecurityCode = (_simulatedCard.creditCardSecurityCode == input.creditCardSecurityCode);

        if (_sameCreditCardNumber && _sameExpirationDate && _sameSecurityCode) {

            $http.post(config.serverBaseUrl + "updateCredits.php", _postData)
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
            $scope.errorText = "Credit card not found.";
            $scope.isError = true;
        }
    };

    // Initialization
    _checkAuthenticationForPage();
});