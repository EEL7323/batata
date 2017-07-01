angular.module("ruServer").controller("adminManageCreditsCtrl", function ($scope, $http, $location, config, $rootScope, authenticationService) {

    // Variables declaration or attribution

    $scope.showAlert = false;
    $scope.isError = false;
    $scope.errorText = "";

    $scope.orderCriteria = 'name';
    $scope.orderDirection = false;

    $scope.students = [{
        name: "",
        registryNumber: "",
        cellphoneCredit: 0,
        cardCredit: 0,
        checked: false
    }];

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
                    if (!_allowedAccess || _accessLevel != 0) $location.path("/login");
                })
                .catch(function (error) {
                    $rootScope.phpError = error.data;
                    $location.path("/error");
                });
        }
        else $location.path("/login");
    };

    var _loadStudents = function () {
        $http.get(config.serverBaseUrl + "loadStudents.php")
            .then(function (response) {
                $scope.students = response.data;
                if (typeof ($scope.students) == "string") {
                    $scope.showAlert = true;
                    $scope.isError = true;
                    $scope.errorText = $scope.students;
                    $scope.students = [];
                }
            })
            .catch(function (error) {
                $rootScope.phpError = error.data;
                $location.path("/error");
            });
    };

    $scope.orderBy = function (field) {
        $scope.orderCriteria = field;
        $scope.orderDirection = !$scope.orderDirection;

        switch (field) {
            case 'name':
                if ($scope.orderDirection) $scope.nameClass = "glyphicon glyphicon-triangle-top";
                else $scope.nameClass = "glyphicon glyphicon-triangle-bottom";
                $scope.registryNumberClass = "";
                break;
            case 'registryNumber':
                if ($scope.orderDirection) $scope.registryNumberClass = "glyphicon glyphicon-triangle-top";
                else $scope.registryNumberClass = "glyphicon glyphicon-triangle-bottom";
                $scope.nameClass = "";
                break;
            default:
                $scope.nameClass = "";
                $scope.registryNumber = "";
        }
    };

    $scope.selectOne = function (students, studentToCheck) {
        students.forEach(function (student) {
            if (student == studentToCheck) student.checked = student.checked;
            else student.checked = false;
        });
    };

    $scope.getColor = function () {
        if ($scope.isError == true) return "alert-danger";
        else return "alert-success";
    };

    $scope.countChecked = function (students) {
        var _acum = 0;
        students.forEach(function (student) {
            if (student.checked) _acum++;
        });
        return _acum;
    };

    $scope.addCredits = function (input) {
        $scope.students.forEach(function (student) {
            if (student.checked) {
                var _sourceAdmin = _token.slice(19);
                var _postData = {
                    sourceRegistryNumber: _sourceAdmin,
                    targetRegistryNumber: student.registryNumber,
                    diffCreditCellphone: input.phoneIncrement,
                    diffCreditTag: input.cardIncrement
                };
                $http.post(config.serverBaseUrl + "createCreditEvents.php", _postData)
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
        });
    };

    // Initialization
    _checkAuthenticationForPage();
    _loadStudents();
});