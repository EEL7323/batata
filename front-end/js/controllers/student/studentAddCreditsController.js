angular.module("ruServer").controller("studentAddCreditsCtrl", function ($scope, $http, $location, config, $rootScope, authenticationService) {

    // Variables declaration or attribution

    // Variables of the error alert
    $scope.showAlert = false;
    $scope.isError = false;
    $scope.errorText = "";

    // Initialization of the input data to zero
    $scope.input = {
        phoneIncrement: 0,
        cardIncrement: 0,
        creditCardNumber: 0,
        creditCardExpirationDate: 0,
        creditCardSecurityCode:0
    };

    // Variable created to simulate the input of a credit card (the user must use this number to approve the transaction)
    var _simulatedCard = {
        creditCardNumber: 1111222233334444,
        creditCardExpirationDate: new Date(2017, 10, 1, 0, 0, 0, 0),
        creditCardSecurityCode: 123
    };

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

    // Function to create the event of adding the credits in the database
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
            // Post request with data of the user who is creating the event (the user accessing the server) and the user who is affected by the event (himself)
            $http.post(config.serverBaseUrl + "createCreditEvents.php", _postData)
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
            $scope.errorText = "Credit card not found.";
            $scope.isError = true;
        }
    };

    // Initialization
    _checkAuthenticationForPage();
});