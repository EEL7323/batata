angular.module("ruServer").controller("studentHomeCtrl", function ($scope, $location, authenticationService) {

    $scope.tokenError = false;
    $scope.tokenErrorText = "";

    var _token = localStorage['ruServerToken'];
    var _valid = false;

    authenticationService.checkToken(_token)
        .then(function (response) {
            $scope.name = response.name;
            var _allowedAccess = response.access;
            $scope.registry = response.registry;
            $scope.tag = response.tag;

            if (_allowedAccess) {

                if (_token.length == 16) {
                    _slice = _token.slice(7, 8);

                    if (_slice == 0) {
                        $scope.buttonBarlink = "view/admin/admin-top-menu.html";
                        $scope.pageLink = "view/admin/admin-home.html";
                        $scope.tokenError = false;
                    }
                    else if (_slice == 1) {
                        $scope.buttonBarlink = "view/admin/student-top-menu.html";
                        $scope.pageLink = "view/admin/student-home.html";
                        $scope.tokenError = false;
                    }
                    else {
                        $scope.tokenError = true;
                        $scope.tokenErrorText = "Your session has expired. Please, login again.";
                        $location.path("/login");
                    }

                }
                else {
                    $scope.tokenError = true;
                    $scope.tokenErrorText = "Your session has expired. Please, login again.";
                }

            }

            else {
                $scope.tokenError = true;
                $scope.tokenErrorText = "Your session has expired. Please, login again.";
            }

        });

});