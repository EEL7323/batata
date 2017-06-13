angular.module("ruServer").controller("studentLogCtrl", function ($scope, $http, $location, config, $window, $rootScope, authenticationService) {

    // Variables declaration or attribution

    $scope.showAlert = false;
    $scope.isError = false;
    $scope.errorText = "";

    $scope.orderCriteria = 'time';
    $scope.orderDirection = false;

    $scope.bigScreen = $window.innerWidth > 700 ? true : false;

    $scope.logs = [{
        id: 0,
        sourceId: "",
        targetId: "",
        type: "",
        diffCredCellphone: 0,
        diffCredTag: 0,
        toresolve: false,
        time: ""
    }];

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

    var _rotateArray = function (array) {
        var _size = array.length;
        var _newArray = [];
        for (i = 0; i < _size; i++) {
            _newArray[i] = array[_size - i - 1];
        }
        return _newArray;
    };

    var _loadLogs = function () {
        _targetRegistry = _token.slice(19);
        var _postData = {
            targetRegistry: _targetRegistry
        };

        $http.post(config.serverBaseUrl + "loadLogs.php", _postData)
            .then(function (response) {
                if (typeof (response.data) == "array") {
                    response.data.forEach(function (index) {
                        index.time = new Date(index.time.slice(0, 4), index.time.slice(5, 7) - 1, index.time.slice(8, 10),
                            index.time.slice(11, 13), index.time.slice(14, 16), index.time.slice(17, 19), 0);
                    });
                }
                $scope.logs = response.data;
                if (typeof ($scope.logs) == "string") {
                    $scope.showAlert = true;
                    $scope.isError = true;
                    $scope.errorText = $scope.logs;
                    $scope.logs = [];
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
            case 'sourceId':
                if ($scope.orderDirection) $scope.sourceIdClass = "glyphicon glyphicon-triangle-top";
                else $scope.sourceIdClass = "glyphicon glyphicon-triangle-bottom";
                $scope.targetIdClass = "";
                $scope.typeClass = "";
                $scope.timeClass = "";
                break;
            case 'type':
                if ($scope.orderDirection) $scope.typeClass = "glyphicon glyphicon-triangle-top";
                else $scope.typeClass = "glyphicon glyphicon-triangle-bottom";
                $scope.targetIdClass = "";
                $scope.sourceIdClass = "";
                $scope.timeClass = "";
                break;
            case 'time':
                if ($scope.orderDirection) $scope.timeClass = "glyphicon glyphicon-triangle-top";
                else $scope.timeClass = "glyphicon glyphicon-triangle-bottom";
                $scope.targetIdClass = "";
                $scope.typeClass = "";
                $scope.sourceIdClass = "";
                break;
            default:
                $scope.sourceIdClass = "";
                $scope.targetIdClass = "";
                $scope.typeClass = "";
                $scope.timeClass = "";
        }
    };

    $scope.getColor = function () {
        if ($scope.isError == true) return "alert-danger";
        else return "alert-success";
    };

    // Initalization

    _checkAuthenticationForPage();
    _loadLogs();

});