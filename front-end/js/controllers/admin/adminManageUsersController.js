angular.module("ruServer").controller("adminManageUsersCtrl", function ($scope, $http, $location, config, $rootScope, authenticationService) {

    // Variables declaration or attribution

    $scope.showAlert = false;
    $scope.isError = false;
    $scope.errorText = "";

    $scope.orderCriteria = 'name';
    $scope.orderDirection = false;

    $scope.users = [{
        name: "",
        registryNumber: "",
        tagNumber: 0,
        cellPhoneCredit: 0,
        cardCredit: 0,
        accessLevel: 1,
        checked: false
    }];

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

    var _loadUsers = function () {
		var _postData = {
			registryNumber: "all"
		};
        $http.post(config.serverBaseUrl + "loadUsers.php", _postData)
            .then(function (response) {
                $scope.users = response.data;
                if (typeof ($scope.users) == "string") {
                    $scope.showAlert = true;
                    $scope.isError = true;
                    $scope.errorText = $scope.users;
                    $scope.users = [];
                }
            })
            .catch(function (error) {
                $rootScope.phpError = error.data;
                $location.path("/error");
            });
    };

    var _rotateArray = function (array) {
        var _size = array.length;
        var _newArray = [];
        for (i = 0; i < _size; i++) {
            _newArray[i] = array[_size - i - 1];
        }
        return _newArray;
    };

    $scope.orderBy = function (field) {
        $scope.orderCriteria = field;
        $scope.orderDirection = !$scope.orderDirection;

        switch (field) {
            case 'name':
                if ($scope.orderDirection) $scope.nameClass = "glyphicon glyphicon-triangle-top";
                else $scope.nameClass = "glyphicon glyphicon-triangle-bottom";
                $scope.registryNumberClass = "";
                $scope.tagNumberClass = "";
                break;
            case 'registryNumber':
                if ($scope.orderDirection) $scope.registryNumberClass = "glyphicon glyphicon-triangle-top";
                else $scope.registryNumberClass = "glyphicon glyphicon-triangle-bottom";
                $scope.nameClass = "";
                $scope.tagNumberClass = "";
                break;
            case 'tagNumber':
                if ($scope.orderDirection) $scope.tagNumberClass = "glyphicon glyphicon-triangle-top";
                else $scope.tagNumberClass = "glyphicon glyphicon-triangle-bottom";
                $scope.nameClass = "";
                $scope.registryNumberClass = "";
                break;
            default:
                $scope.nameClass = "";
                $scope.registryNumber = "";
                $scope.tagNumberClass = "";
        }
    };

    $scope.getColor = function () {
        if ($scope.isError == true) return "alert-danger";
        else return "alert-success";
    };

    $scope.countChecked = function (users) {
        _acum = 0;
        if (typeof(users) == "object") {
            users.forEach(function (user) {
                if (user.checked) _acum++;
            });
        }
        return _acum;
    };

    $scope.toggleAll = function (toggle, users) {
        if (typeof(users) == "object") {
            users = users.filter(function (user) {
                var _searchCriteriaUpperCase = typeof ($scope.searchCriteria) == "undefined" ? "" : $scope.searchCriteria.toUpperCase();
                if (user.name.toUpperCase().indexOf(_searchCriteriaUpperCase) != -1 || user.registryNumber.toUpperCase().indexOf(_searchCriteriaUpperCase) != -1 || user.tagNumber.toUpperCase().indexOf(_searchCriteriaUpperCase) != -1) return user;
            });

            if (toggle) {
                users.forEach(function (user) {
                    user.checked = true;
                });
            }
            else {
                users.forEach(function (user) {
                    user.checked = false;
                });
            }
        }
    };

    $scope.insertUser = function () {
        $location.path("/admin/manage-users/edit-user");
        $rootScope.selUser = {
            name: "",
            registryNumber: "",
            tagNumber: "",
            accessLevel: ""
        };
        $rootScope.editUserMethod = "Insert user info";
    };

    $scope.editUser = function (users) {
        $location.path("/admin/manage-users/edit-user");
        users.forEach(function (user) {
            if (user.checked) {
                $rootScope.selUser = {
                    name: user.name,
                    registryNumber: user.registryNumber,
                    tagNumber: (user.tagNumber * 1),
                    accessLevel: user.accessLevel
                };
                $rootScope.editUserMethod = "Edit user info";
            }
        });
    };

    $scope.deleteUsers = function (users) {
        var _accessLevel = _token.slice(7, 8);
        var _sourceAdmin = _token.slice(19);
        var _postData = {
            usersToDelete: [],
            sourceAdmin: _sourceAdmin
        };
        var _indexesToDelete = [];
        users.forEach(function (user) {
            if (user.checked) {
                _postData.usersToDelete.push(user.registryNumber);
                _indexesToDelete.push(users.indexOf(user));
            }
        });
		console.log(_postData);
        $http.post(config.serverBaseUrl + "deleteUsers.php", _postData)
            .then(function (response) {
                $scope.showAlert = true;
                $scope.errorText = response.data;
                if (response.data.startsWith("Error -")) $scope.isError = true;
                else {
                    $scope.isError = false;
                    _indexesToDelete = _rotateArray(_indexesToDelete);
                    _indexesToDelete.forEach(function (index) {
                        $scope.users.splice(index, 1);
                    });
                }
            })
            .catch(function (error) {
                $rootScope.phpError = error.data;
                $location.path("/error");
            });

    };

    // Initialization

    _checkAuthenticationForPage();
    _loadUsers();

});