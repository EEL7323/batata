angular.module("ruServer").controller("adminManageCreditsCtrl", function ($scope, $http, $location, config, $rootScope, authenticationService) {

    // Variables declaration or attribution

    // Variables of the error alert
    $scope.showAlert = false;
    $scope.isError = false;
    $scope.errorText = "";

    // Variables of the filter
    $scope.orderCriteria = 'name';
    $scope.orderDirection = false;

    // Initialization of variable students, used to feed the table showed in the screen
    $scope.students = [{
        name: "",
        registryNumber: "",
        cellphoneCredit: 0,
        cardCredit: 0,
        checked: false
    }];

    // Initialization of the input data to zero
    $scope.input = {
        phoneIncrement: 0,
        cardIncrement: 0
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
                    if (!_allowedAccess || _accessLevel != 0) $location.path("/login");
                })
                .catch(function (error) {
                    $rootScope.phpError = error.data;
                    $location.path("/error");
                });
        }
        else $location.path("/login");
    };

    // Function to order data showed in the table
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

    // Function to uncheck all checkboxes and select only the last checked (it means that the admin user can only change credits of one student at a time)
    $scope.selectOne = function (students, studentToCheck) {
        students.forEach(function (student) {
            if (student == studentToCheck) student.checked = student.checked;
            else student.checked = false;
        });
    };

    // Function to change the color of the warning box, depending if there is an error or not
    $scope.getColor = function () {
        if ($scope.isError == true) return "alert-danger";
        else return "alert-success";
    };

    // Count the number of rows where the checkbox is checked
    $scope.countChecked = function (students) {
        var _acum = 0;
        students.forEach(function (student) {
            if (student.checked) _acum++;
        });
        return _acum;
    };

    // Function load students declaration
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
            .catch(function (error) { // Error handling
                $rootScope.phpError = error.data;
                $location.path("/error");
            });
    };

    // Function to create the event of adding the credits in the database
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
                // Post request with data of the user who is creating the event (the admin) and the user who is affected by the event
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
        });
    };

    // Initialization
    _checkAuthenticationForPage();
    _loadStudents();
});